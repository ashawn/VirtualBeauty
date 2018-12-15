/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.component;

import android.content.Context;

import com.cvte.virtualbeauty.App;
import com.cvte.virtualbeauty.executor.PostThreadExecutor;
import com.cvte.virtualbeauty.executor.ThreadExecutor;
import com.cvte.virtualbeauty.internal.di.module.AppModule;

import javax.inject.Singleton;

import dagger.Component;

/**
 * App类component
 *
 * @author laizhenqi
 * @since 2017/1/21
 */
@Singleton
@Component(modules = AppModule.class)
public interface AppComponent {

    void inject(App app);

    Context getContext();

    ThreadExecutor threadExecutor();

    PostThreadExecutor postThreadExecutor();
}
