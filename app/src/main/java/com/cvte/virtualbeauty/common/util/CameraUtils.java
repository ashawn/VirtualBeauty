/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.common.util;

import android.support.annotation.NonNull;
import android.util.Size;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * 相机相关工具
 *
 * @author laizhenqi
 * @since 2017/1/22
 */
public class CameraUtils {


    /**
     * @param choices
     * @return
     */
    @NonNull
    public static Size chooseVideoSize(Size[] choices) {
        if (choices == null) {
            return null;
        }
        for (Size size : choices) {
            if (size.getWidth() == size.getHeight() * 4 / 3 && size.getWidth() <= 1080) {
                return size;
            }
        }
        //return last size
        return choices[choices.length - 1];
    }

    @NonNull
    public static Size chooseOptimalSize(Size[] choices, int width, int height, Size aspectRatio) {
        List<Size> bigEnough = new ArrayList<>();
        int w = aspectRatio.getWidth();
        int h = aspectRatio.getHeight();
        for (Size option : choices) {
            if (option.getHeight() == option.getWidth() * h / w && option.getWidth() <= width && option.getHeight() <= height && option.getWidth() * option.getHeight() > 150000) {
                bigEnough.add(option);
            }
        }

        if (bigEnough.size() > 0) {
            return Collections.min(bigEnough,
                    (a, b) -> Long.signum((long) a.getWidth() * a.getHeight() - (long) b.getWidth() * b.getHeight())
            );
        } else {
            return choices[0];
        }
    }
}
