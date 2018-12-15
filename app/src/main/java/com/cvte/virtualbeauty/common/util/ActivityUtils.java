/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.common.util;

import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.support.annotation.IdRes;

/**
 * activity相关工具方法
 * @author laizhenqi
 * @since 2017/1/22
 */
public final class ActivityUtils {

    public static void replaceFragment(FragmentManager fm, Fragment fragment,@IdRes int resId) {
        FragmentTransaction ft = fm.beginTransaction();
        ft.replace(resId,fragment);
        ft.commit();
    }

}
