package com.cvte.virtualbeauty.ui.store;

import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.view.View;

import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.base.BaseAppCompatActivity;
import com.cvte.virtualbeauty.common.utils.LogUtil;

import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/4/11
 */

public class StoreActivity extends BaseAppCompatActivity {

    private final static String TAG = StoreActivity.class.getSimpleName();

    @BindView(R.id.view_test1)
    View testView1;

    @BindView(R.id.view_test2)
    View testView2;

    private Handler mHandler = new Handler();

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_store);
        ButterKnife.bind(this);

        mHandler.postDelayed(() -> {
            Rect rect = new Rect();
            int[] loca = new int[2];
            testView1.getLocalVisibleRect(rect);
            LogUtil.d(TAG,"testView1 getLocalVisibleRect:"+rect);
            testView1.getGlobalVisibleRect(rect);
            LogUtil.d(TAG,"testView1 getGlobalVisibleRect:"+rect);
            testView1.getLocationOnScreen(loca);
            LogUtil.d(TAG,"testView1 getLocationOnScreen:"+loca[0]+" "+loca[1]);
            testView2.getLocalVisibleRect(rect);
            LogUtil.d(TAG,"testView2 getLocalVisibleRect:"+rect);
            testView2.getGlobalVisibleRect(rect);
            LogUtil.d(TAG,"testView2 getGlobalVisibleRect:"+rect);
            testView2.getLocationOnScreen(loca);
            LogUtil.d(TAG,"testView2 getLocationOnScreen:"+loca[0]+" "+loca[1]);
        },1000);
    }

}
