package com.cvte.virtualbeauty.dlib;

import android.graphics.Bitmap;
import android.support.annotation.Keep;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.WorkerThread;

import com.cvte.virtualbeauty.common.utils.LogUtil;

import org.opencv.core.Mat;

import java.util.Arrays;
import java.util.List;

/**
 * Created by houzhi on 16-10-20.
 * Modified by cvte on 16-11-15
 */
public class FaceDet {
    private static final String TAG = "dlib";

    // accessed by native methods
    @SuppressWarnings("unused")
    private long mNativeFaceDetContext;

    static {
        try {
            System.loadLibrary("android_dlib");
            jniNativeClassInit();
            LogUtil.d(TAG, "jniNativeClassInit success");
        } catch (UnsatisfiedLinkError e) {
            LogUtil.e(TAG, "library not found");
        }
    }

    public FaceDet(String detectModelPath,String landMarkPath) {
        jniInit(detectModelPath,landMarkPath);
    }

    @WorkerThread
    public void setMode(int mode) {
        jniSetMode(mode);
    }

    @Nullable
    @WorkerThread
    public List<VisionDetRet> detect(@NonNull String path) {
        VisionDetRet[] detRets = jniDetect(path);
        return Arrays.asList(detRets);
    }

    @Nullable
    @WorkerThread
    public List<VisionDetRet> detect(@NonNull Bitmap bitmap) {
        VisionDetRet[] detRets = jniBitmapDetect(bitmap);
        return Arrays.asList(detRets);
    }

    @Nullable
    @WorkerThread
    public DetectorRet detectWithResult(@NonNull Bitmap bitmap) {
        return jniDetectBitmapWithResult(bitmap);
    }

    @Nullable
    @WorkerThread
    public DetectorRet detectWithResult(@NonNull Mat mat) {
        return jniDetectMatWithResult(mat.getNativeObjAddr());
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        release();
    }

    public void release() {
        jniDeInit();
    }

    @Keep
    private native static void jniNativeClassInit();

    @Keep
    private synchronized native int jniInit(String detectModelPath, String landmarkModelPath);

    @Keep
    private synchronized native int jniDeInit();

    @Keep
    private synchronized native VisionDetRet[] jniBitmapDetect(Bitmap bitmap);

    @Keep
    private synchronized native VisionDetRet[] jniDetect(String path);

    @Keep
    private synchronized native DetectorRet jniDetectBitmapWithResult(Bitmap bitmap);

    @Keep
    private synchronized native DetectorRet jniDetectMatWithResult(long mat);

    @Keep
    public synchronized native void naiveSetDetectRegion(int left, int top, int right, int bottom) ;

    @Keep
    private synchronized native void jniSetMode(int mode) ;
}