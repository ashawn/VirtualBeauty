/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.capture;

import com.cvte.virtualbeauty.data.render.RenderSchemeRepository;
import com.cvte.virtualbeauty.executor.PostThreadExecutor;
import com.cvte.virtualbeauty.executor.ThreadExecutor;

import javax.inject.Inject;

import io.reactivex.schedulers.Schedulers;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
public class CapturePresenterImpl implements CapturePresenter {

    CaptureView mView;

    RenderSchemeRepository mRenderSchemeRepository;

    ThreadExecutor mThreadExecutor;
    PostThreadExecutor mPostThreadExecutor;

    @Inject
    public CapturePresenterImpl(CaptureView view, RenderSchemeRepository renderSchemeRepository, ThreadExecutor threadExecutor, PostThreadExecutor postThreadExecutor) {
        this.mView = view;
        this.mRenderSchemeRepository = renderSchemeRepository;
        this.mThreadExecutor = threadExecutor;
        this.mPostThreadExecutor = postThreadExecutor;
    }

    @Inject
    void init() {
        mView.setPresenter(this);
    }

    @Override
    public void start() {

    }

    @Override
    public void resume() {
        mRenderSchemeRepository.getRenderSchemeList()
                .subscribeOn(Schedulers.from(mThreadExecutor))
                .observeOn(mPostThreadExecutor.getSchedule())
                .subscribe(list -> mView.updateRenderSchemeList(list));
    }

    @Override
    public void pause() {
    }

    @Override
    public void stop() {

    }
}
