package com.meitu.core.types;

import android.graphics.PointF;
import android.graphics.RectF;

import java.util.ArrayList;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/3/10
 */

public class FaceData {
    public static final int LANDMARK_TYPE_39 = 0;
    public static final int LANDMARK_TYPE_83 = 1;
    public static final int LANDMARK_TYPE_EXT = 2;
    public static final int LANDMARK_TYPE_EXT_3D = 3;

    protected final long nativeInstance;

    static {
        System.loadLibrary("mttypes");
    }

    public FaceData() {
        this.nativeInstance = nativeCreate();
    }

    private FaceData(final FaceData faceData) {
        this.nativeInstance = nativeCopy(faceData.nativeInstance());
    }

    private static native void finalizer(final long p0);

    private static native void nativeClear(final long p0);

    private static native long nativeCopy(final long p0);

    private static native long nativeCreate();

    private static native int nativeGetAvgBrightness(final long p0);

    private static native int nativeGetDetectHeight(final long p0);

    private static native int nativeGetDetectWidth(final long p0);

    private static native int nativeGetFaceCount(final long p0);

    private static native float[] nativeGetFaceRect(final long p0, final int p1);

    private static native float[] nativeGetLandmark(final long p0, final int p1, final int p2);

    private static native boolean nativeScale(final long p0, final float p1);

    private static native boolean nativeSetLandmark(final long p0, final int p1, final int p2, final float[] p3);

    public void clear() {
        nativeClear(this.nativeInstance);
    }

    public FaceData copy() {
        return new FaceData(this);
    }

    @Override
    protected void finalize() {
        finalizer(this.nativeInstance);
        try {
            super.finalize();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
    }

    public int getAvgBright() {
        return nativeGetAvgBrightness(this.nativeInstance);
    }

    public int getDetectHeight() {
        return nativeGetDetectHeight(this.nativeInstance);
    }

    public int getDetectWidth() {
        return nativeGetDetectWidth(this.nativeInstance);
    }

    public int getFaceCount() {
        return nativeGetFaceCount(this.nativeInstance);
    }

    public ArrayList<PointF> getFaceLandmark(final int n, final int n2) {
        return this.getFaceLandmark(n, n2, this.getDetectWidth(), this.getDetectHeight());
    }

    public ArrayList<PointF> getFaceLandmark(int i, final int n, final int n2, final int n3) {
        final float[] nativeGetLandmark = nativeGetLandmark(this.nativeInstance, n, i);
        ArrayList<PointF> list2;
        final ArrayList<PointF> list = list2 = null;
        if (nativeGetLandmark != null) {
            list2 = list;
            if (nativeGetLandmark.length > 0) {
                list2 = new ArrayList<PointF>();
                for (i = 0; i < nativeGetLandmark.length / 2; ++i) {
                    list2.add(new PointF(nativeGetLandmark[i * 2] * n2, nativeGetLandmark[i * 2 + 1] * n3));
                }
            }
        }
        return list2;
    }

    public RectF getFaceRect(final int n) {
        return this.getFaceRect(n, this.getDetectWidth(), this.getDetectHeight());
    }

    public RectF getFaceRect(final int n, final int n2, final int n3) {
        final RectF rectF = null;
        final float[] nativeGetFaceRect = nativeGetFaceRect(this.nativeInstance, n);
        RectF rectF2 = rectF;
        if (nativeGetFaceRect != null) {
            rectF2 = rectF;
            if (nativeGetFaceRect.length == 4) {
                rectF2 = new RectF(nativeGetFaceRect[0] * n2, nativeGetFaceRect[1] * n3, nativeGetFaceRect[2] * n2, nativeGetFaceRect[3] * n3);
            }
        }
        return rectF2;
    }

    public RectF getFaceRectFrom0To1(final int n) {
        final RectF rectF = null;
        final float[] nativeGetFaceRect = nativeGetFaceRect(this.nativeInstance, n);
        RectF rectF2 = rectF;
        if (nativeGetFaceRect != null) {
            rectF2 = rectF;
            if (nativeGetFaceRect.length == 4) {
                rectF2 = new RectF(nativeGetFaceRect[0], nativeGetFaceRect[1], nativeGetFaceRect[2], nativeGetFaceRect[3]);
            }
        }
        return rectF2;
    }

    public ArrayList<RectF> getFaceRectList() {
        final int faceCount = this.getFaceCount();
        ArrayList<RectF> list = null;
        if (faceCount > 0) {
            final ArrayList<RectF> list2 = new ArrayList<RectF>();
            int n = 0;
            while (true) {
                list = list2;
                if (n >= faceCount) {
                    break;
                }
                list2.add(this.getFaceRect(n));
                ++n;
            }
        }
        return list;
    }

    public long nativeInstance() {
        return this.nativeInstance;
    }

    public boolean scale(final float n) {
        return nativeScale(this.nativeInstance, n);
    }

    public boolean setFaceLandmark(final ArrayList<PointF> list, final int n, final int n2) {
        boolean nativeSetLandmark = false;
        if (list != null) {
            nativeSetLandmark = nativeSetLandmark;
            if (list.size() > 0) {
                final float[] array = new float[list.size() * 2];
                for (int i = 0; i < list.size(); ++i) {
                    array[i * 2] = list.get(i).x;
                    array[i * 2 + 1] = list.get(i).y;
                }
                nativeSetLandmark = nativeSetLandmark(this.nativeInstance, n2, n, array);
            }
        }
        return nativeSetLandmark;
    }
}
