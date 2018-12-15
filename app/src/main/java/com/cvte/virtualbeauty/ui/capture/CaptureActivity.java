/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.capture;

import android.app.Fragment;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.View;

import com.cvte.virtualbeauty.App;
import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.base.BaseAppCompatActivity;
import com.cvte.virtualbeauty.common.util.ActivityUtils;
import com.cvte.virtualbeauty.common.util.PermissionUtils;
import com.cvte.virtualbeauty.common.utils.LogUtil;
import com.cvte.virtualbeauty.internal.di.component.DaggerCaptureComponent;
import com.cvte.virtualbeauty.internal.di.module.CaptureModule;

import javax.inject.Inject;

/**
 * 预览activity，通过glsurfaceview预览，然后传到opengl进行渲染，人脸识别部分通过识别sdk进行识别
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
public class CaptureActivity extends BaseAppCompatActivity {

    private static final String TAG = CaptureActivity.class.getSimpleName();

    @Inject
    CapturePresenter mPresenter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LogUtil.w(TAG,"start activity:"+getIntent().getComponent().getClassName());
        setContentView(R.layout.activity_capture);
        setWindowDisplayMode();
        PermissionUtils.requestPermission(this,PermissionUtils.CODE_CAMERA,mPermissionGrant);
    }

    private void setWindowDisplayMode() {
        setSystemUiVisibilityMode();
        getWindow().getDecorView().setOnSystemUiVisibilityChangeListener(
                visibility -> setSystemUiVisibilityMode());
    }

    private void setSystemUiVisibilityMode() {
        int options = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        getWindow().getDecorView().setSystemUiVisibility(options);
    }

    private void loadCaptureFragment() {
        CaptureFragment fragment = (CaptureFragment) getFragmentManager().findFragmentById(R.id.capture_container);
        if (fragment == null) {
            fragment = (CaptureFragment) Fragment.instantiate(this, CaptureFragment.class.getName());
            ActivityUtils.replaceFragment(getFragmentManager(), fragment, R.id.capture_container);
        }

        DaggerCaptureComponent.builder()
                .appComponent(App.getInstance().getComponent())
                .captureModule(new CaptureModule(fragment))
                .build()
                .inject(this);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        PermissionUtils.requestPermissionsResult(this,requestCode,permissions,grantResults,mPermissionGrant);
    }

    private final PermissionUtils.PermissionGrant mPermissionGrant = requestCode -> {
        switch (requestCode) {
            case PermissionUtils.CODE_CAMERA: {
                loadCaptureFragment();
                break;
            }
        }
    };
}
