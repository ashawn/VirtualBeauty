/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.executor;

import javax.inject.Inject;

import io.reactivex.Scheduler;
import io.reactivex.android.schedulers.AndroidSchedulers;

/**
 * ui线程
 * @author laizhenqi
 * @since  2016/9/30
 */
public class UIThread implements PostThreadExecutor {

    @Inject
    public UIThread() {
    }

    @Override
    public Scheduler getSchedule() {
        return AndroidSchedulers.mainThread();
    }
}
