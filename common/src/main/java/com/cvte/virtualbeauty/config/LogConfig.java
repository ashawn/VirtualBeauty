package com.cvte.virtualbeauty.config;

import android.content.Context;
import android.content.res.Resources;
import android.os.Bundle;

import com.cvte.virtualbeauty.common.R;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/4/13
 */

public class LogConfig {

    private static final Bundle mValues = new Bundle();

    public static void init(Context context) {
        Resources res = context.getResources();
        mValues.putBoolean(ConfigValues.CONFIG_TEST_ENABLED, res.getBoolean(R.bool.testEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_DEBUG_LOG_ENABLED,res.getBoolean(R.bool.debugEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_INFO_LOG_ENABLED,res.getBoolean(R.bool.infoEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_WARNING_LOG_ENABLED,res.getBoolean(R.bool.warningEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_ERROR_LOG_ENABLED,res.getBoolean(R.bool.errorEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_VERBOSE_LOG_ENABLED,res.getBoolean(R.bool.verboseEnabled));
        mValues.putBoolean(ConfigValues.CONFIG_STACK_LOG_ENABLED,res.getBoolean(R.bool.stackEnabled));
    }

    public static boolean isTestEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_TEST_ENABLED,false);
    }

    public static boolean isDebugEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_DEBUG_LOG_ENABLED,false);
    }

    public static boolean isInfoEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_INFO_LOG_ENABLED,false);
    }

    public static boolean isWarningEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_WARNING_LOG_ENABLED,false);
    }

    public static boolean isErrorEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_ERROR_LOG_ENABLED,false);
    }

    public static boolean isVerboseEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_VERBOSE_LOG_ENABLED,false);
    }

    public static boolean isStackEnabled() {
        return mValues.getBoolean(ConfigValues.CONFIG_STACK_LOG_ENABLED,false);
    }

}
