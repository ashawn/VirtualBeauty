package com.cvte.virtualbeauty.executor;

import io.reactivex.Scheduler;

/**
 * 异步线程
 * @author laizhenqi
 * @since 16/8/17
 */
public interface PostThreadExecutor {
    Scheduler getSchedule();
}
