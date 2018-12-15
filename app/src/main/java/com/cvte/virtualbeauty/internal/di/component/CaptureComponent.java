/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.component;

import com.cvte.virtualbeauty.internal.di.ActivityScope;
import com.cvte.virtualbeauty.internal.di.module.CaptureModule;
import com.cvte.virtualbeauty.ui.capture.CaptureActivity;

import dagger.Component;

/**
 * @author laizhenqi
 * @since 2017/2/6
 */
@ActivityScope
@Component(dependencies = AppComponent.class,modules = CaptureModule.class)
public interface CaptureComponent {

    void inject(CaptureActivity activity);

}
