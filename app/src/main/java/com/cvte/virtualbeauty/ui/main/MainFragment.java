/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.main;

import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.common.util.ToastUtils;
import com.cvte.virtualbeauty.ui.capture.CaptureActivity;
import com.cvte.virtualbeauty.ui.store.StoreActivity;

import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * 主页fragment，包含主要界面布局
 *
 * @author laizhenqi
 * @since 2017/1/21
 */
public class MainFragment extends Fragment implements MainView {

    private static final String TAG = MainFragment.class.getSimpleName();

    MainPresenter mPresenter;

    @BindView(R.id.textView_exit_main_fragment)
    TextView mBtnExit;

    @BindView(R.id.button_beauty_video_main_fragment)
    Button mBtnVideo;

    @BindView(R.id.button_beauty_camera_main_fragment)
    Button mBtnCamera;

    @BindView(R.id.button_beauty_store_main_fragment)
    Button mBtnStore;

    @Override
    public void setPresenter(MainPresenter presenter) {
        mPresenter = presenter;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_main, container, false);
        ButterKnife.bind(this, root);

        initViews();
        return root;
    }

    @Override
    public void onStart() {
        super.onStart();
        mPresenter.start();
    }

    @Override
    public void onResume() {
        super.onResume();
        mPresenter.resume();
    }

    @Override
    public void onPause() {
        mPresenter.pause();
        super.onPause();
    }

    @Override
    public void onStop() {
        mPresenter.stop();
        super.onStop();
    }

    private void initViews() {
        mBtnExit.setOnClickListener(v -> getActivity().finish());
        mBtnVideo.setOnClickListener(v -> {
            //// TODO: 2017/2/6 打开视频
            ToastUtils.getInstance().showToast(getActivity(), "打开视频");
        });
        mBtnCamera.setOnClickListener(v -> {
            Intent intent = new Intent(getActivity(), CaptureActivity.class);
            startActivity(intent);
        });
        mBtnStore.setOnClickListener(v -> {
            Intent intent = new Intent(getActivity(), StoreActivity.class);
            startActivity(intent);
        });
    }
}
