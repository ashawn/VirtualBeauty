package com.cvte.virtualbeauty.internal.di;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import javax.inject.Scope;

/**
 * 使用该注解使对象仅作用于activity域
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
@Scope
@Retention(RetentionPolicy.RUNTIME)
public @interface ActivityScope {
}
