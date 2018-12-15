package com.cvte.virtualbeauty.dlib;

import android.graphics.Bitmap;
import android.graphics.Rect;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;

import org.opencv.core.Mat;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/3/17
 */

public class DetectorHelper {

    public interface Callback {
        void done(DetectorRet result);
    }

    private static final String TAG = DetectorHelper.class.getSimpleName();

    private HandlerThread mThread;
    private Handler mHandler;

    private FaceDet mFaceDet;

    public void start() {
        if (mThread != null) {
            stop();
        }
        mThread = new HandlerThread(TAG);
        mThread.start();
        mHandler = new Handler(mThread.getLooper());
        if (mFaceDet != null) mFaceDet.release();
        mFaceDet = new FaceDet(Environment.getExternalStorageDirectory() + "/seeta_fd_frontal_v1.0.bin",
                Environment.getExternalStorageDirectory() + "/shape_predictor_68_face_landmarks.dat");
        mFaceDet.setMode(1);
    }

    public void stop() {
        if (mFaceDet != null) {
            mFaceDet.release();
            mFaceDet = null;
        }
        if (mThread != null) {
            mHandler.removeCallbacksAndMessages(null);
            mThread.quitSafely();
            try {
                mThread.join();
                mThread = null;
                mHandler = null;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void setDetectRegion(Rect region) {
        if(mFaceDet!=null) {
            mFaceDet.naiveSetDetectRegion(region.left,region.top,region.right,region.bottom);
        }
    }

    public boolean detect(final Mat mat, final Callback callback) {
        if (mHandler != null) {
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (mFaceDet == null) callback.done(null);
                    else callback.done(mFaceDet.detectWithResult(mat));
                }
            });
            return true;
        }
        return false;
    }

    public boolean detect(final Bitmap bitmap, final Callback callback) {
        if (mHandler != null) {
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (mFaceDet == null) callback.done(null);
                    else callback.done(mFaceDet.detectWithResult(bitmap));
                }
            });
            return true;
        }
        return false;
    }
}
