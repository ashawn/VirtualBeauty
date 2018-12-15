/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty;

import android.app.Application;
import android.util.Log;

import com.cvte.virtualbeauty.common.utils.LogUtil;
import com.cvte.virtualbeauty.config.LogConfig;
import com.cvte.virtualbeauty.gles.GlUtil;
import com.cvte.virtualbeauty.internal.di.component.AppComponent;
import com.cvte.virtualbeauty.internal.di.component.DaggerAppComponent;
import com.cvte.virtualbeauty.internal.di.module.AppModule;

/**
 * 主app类，应用启动入口
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
public class App extends Application {

    private static final String TAG = App.class.getSimpleName();

    static {
        try {
            System.loadLibrary("glnative");
        } catch (Exception e) {
            LogUtil.e(TAG, "library not found");
        }
    }

    private static App sInstance;

    public static App getInstance() {
        return sInstance;
    }

    AppComponent mComponent;

    @Override
    public void onCreate() {
        super.onCreate();

        sInstance = this;

        mComponent = DaggerAppComponent.builder()
                .appModule(new AppModule(getApplicationContext()))
                .build();
        mComponent.inject(this);

        LogConfig.init(this);
        GlUtil.init(this);
    }

    public AppComponent getComponent() {
        return mComponent;
    }
}
