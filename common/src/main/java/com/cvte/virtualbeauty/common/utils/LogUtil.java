package com.cvte.virtualbeauty.common.utils;

import android.util.Log;

import com.cvte.virtualbeauty.config.LogConfig;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/4/13
 */

public class LogUtil {

    private static final String TAG_PREFIX = "CVTE";

    private static final boolean DEBUG_OPEN = LogConfig.isDebugEnabled();
    private static final boolean INFO_OPEN = LogConfig.isInfoEnabled();
    private static final boolean WARNING_OPEN = LogConfig.isWarningEnabled();
    private static final boolean ERROR_OPEN = LogConfig.isErrorEnabled();
    private static final boolean VERBOSE_OPEN = LogConfig.isVerboseEnabled();
    private static final boolean STACK_OPEN = LogConfig.isStackEnabled();

    public static void d(String tag,String msg) {
        if(DEBUG_OPEN) {
            Log.d(tag,msg);
        }
    }

    public static void i(String tag,String msg) {
        if(INFO_OPEN) {
            Log.i(tag,msg);
        }
    }

    public static void v(String tag,String msg) {
        if(VERBOSE_OPEN) {
            Log.v(tag,msg);
        }
    }

    public static void w(String tag,String msg) {
        if(WARNING_OPEN) {
            Log.w(tag,msg);
        }
    }

    public static void e(String tag,String msg) {
        if(ERROR_OPEN) {
            Log.e(tag, msg);
        }
    }

    public static void e(String tag,String msg,Throwable e) {
        if(ERROR_OPEN) {
            Log.e(tag, msg, e);
        }
    }

    public static void printStackTracesString(String msg) {
        if(STACK_OPEN) {
            Log.d(TAG_PREFIX,msg+"\n"+Log.getStackTraceString(new Throwable()));
        }
    }
}
