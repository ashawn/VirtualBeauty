/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.module;

import android.content.Context;

import com.cvte.virtualbeauty.executor.JobExecutor;
import com.cvte.virtualbeauty.executor.PostThreadExecutor;
import com.cvte.virtualbeauty.executor.ThreadExecutor;
import com.cvte.virtualbeauty.executor.UIThread;

import javax.inject.Singleton;

import dagger.Module;
import dagger.Provides;

/**
 * app类module
 *
 * @author laizhenqi
 * @since 2017/1/21
 */
@Module
public class AppModule {

    Context mContext;

    public AppModule(Context context) {
        mContext = context;
    }

    @Singleton
    @Provides
    Context provideContext() {
        return this.mContext;
    }

    @Singleton
    @Provides
    ThreadExecutor provideThreadExecutor(JobExecutor executor) {
        return executor;
    }

    @Singleton
    @Provides
    PostThreadExecutor providePostThreadExecutor(UIThread thread) {
        return thread;
    }
}
