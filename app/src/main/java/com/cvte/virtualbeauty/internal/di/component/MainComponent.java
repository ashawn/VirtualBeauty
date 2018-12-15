/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.internal.di.component;

import com.cvte.virtualbeauty.internal.di.ActivityScope;
import com.cvte.virtualbeauty.internal.di.module.MainModule;
import com.cvte.virtualbeauty.ui.main.MainActivity;

import dagger.Component;

/**
 * @author laizhenqi
 * @since 2017/2/6
 */
@ActivityScope
@Component(dependencies = AppComponent.class, modules = MainModule.class)
public interface MainComponent {

    void inject(MainActivity activity);

}
