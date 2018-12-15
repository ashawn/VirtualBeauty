/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.module;

import com.cvte.virtualbeauty.data.render.RenderSchemeRepository;
import com.cvte.virtualbeauty.data.render.RenderSchemeRepositoryImpl;
import com.cvte.virtualbeauty.internal.di.ActivityScope;
import com.cvte.virtualbeauty.ui.capture.CapturePresenter;
import com.cvte.virtualbeauty.ui.capture.CapturePresenterImpl;
import com.cvte.virtualbeauty.ui.capture.CaptureView;

import dagger.Module;
import dagger.Provides;

/**
 * @author laizhenqi
 * @since 2017/2/6
 */
@Module
public class CaptureModule {

    private CaptureView mView;

    public CaptureModule(CaptureView view) {
        mView = view;
    }

    @ActivityScope
    @Provides
    CaptureView provideCaptureView() {
        return this.mView;
    }

    @ActivityScope
    @Provides
    CapturePresenter provideCapturePresenter(CapturePresenterImpl presenter) {
        return presenter;
    }

    @ActivityScope
    @Provides
    RenderSchemeRepository provideRenderSchemeRepository(RenderSchemeRepositoryImpl repository) {
        return repository;
    }

}
