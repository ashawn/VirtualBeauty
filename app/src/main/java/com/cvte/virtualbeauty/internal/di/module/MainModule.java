/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.module;

import com.cvte.virtualbeauty.internal.di.ActivityScope;
import com.cvte.virtualbeauty.ui.main.MainPresenter;
import com.cvte.virtualbeauty.ui.main.MainPresenterImpl;
import com.cvte.virtualbeauty.ui.main.MainView;

import dagger.Module;
import dagger.Provides;

/**
 * @author laizhenqi
 * @since 2017/2/6
 */
@Module
public class MainModule {

    MainView mView;

    public MainModule(MainView view) {
        this.mView = view;
    }

    @ActivityScope
    @Provides
    MainView provideMainView() {
        return this.mView;
    }

    @ActivityScope
    @Provides
    MainPresenter provideMainPresenter(MainPresenterImpl presenter) {
        return presenter;
    }

}
