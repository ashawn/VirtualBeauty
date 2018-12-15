package com.meitu.core.facedetect;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import com.meitu.core.types.FaceData;
import com.meitu.core.types.NativeBitmap;

/**
 * 反编译自美图apk
 *
 * @author laizhenqi
 * @since 2017/3/10
 */
public class FaceDetector {

    private final static String TAG = FaceDetector.class.getSimpleName();

    static {
        System.loadLibrary("mttypes");
        while (true) {
            try {
                System.loadLibrary("MeituFDFA");
                System.loadLibrary("mtfacedetect");
            } catch (Throwable t) {
                t.printStackTrace();
                continue;
            }
            break;
        }
    }

    static class ApplicationConfigureHolder {
        static FaceDetector sharedApplicationConfigure = new FaceDetector();
    }

    public static FaceDetector instance() {
        return ApplicationConfigureHolder.sharedApplicationConfigure;
    }

    private static native boolean nativeFaceDetectBitmap(final long p0, final Bitmap p1);

    private static native boolean nativeFaceDetectBitmapWithFace(final long p0, final Bitmap p1, final int[] p2);

    private static native boolean nativeFaceDetectI420(final long p0, final byte[] p1, final int p2, final int p3);

    private static native boolean nativeFaceDetectInit(final Context p0);

    private static native boolean nativeFaceDetectMaxFaceCount(final int p0);

    private static native boolean nativeFaceDetectNV21(final long p0, final byte[] p1, final int p2, final int p3, final int p4, final boolean p5, final boolean p6, final boolean p7, final boolean p8, final boolean p9);

    private static native boolean nativeFaceDetectNativeBitmap(final long p0, final long p1);

    private static native boolean nativeFaceDetectNativeBitmapWithFace(final long p0, final long p1, final int[] p2);

    private static native void nativeFaceDetectStopTracking();

    public synchronized FaceData faceDetect_Bitmap(final Bitmap bitmap) {
        FaceData faceData = null;
        if (bitmap == null) {
            return faceData;
        }
        try {
            faceData = new FaceData();
            nativeFaceDetectBitmap(faceData.nativeInstance(), bitmap);
            return faceData;
        } catch (Exception e) {
            return null;
        }
    }

    public synchronized FaceData faceDetect_Bitmap_WithFace(final Bitmap bitmap, final int[] array) {
        FaceData faceData2;
        final FaceData faceData = faceData2 = null;
        if (bitmap == null) {
            return faceData2;
        }
        faceData2 = faceData;
        if (array == null) {
            return faceData2;
        }
        try {
            faceData2 = new FaceData();
            nativeFaceDetectBitmapWithFace(faceData2.nativeInstance(), bitmap, array);
            return faceData2;
        } catch (Exception e) {
            return null;
        } finally {
        }
    }

    public synchronized FaceData faceDetect_I420(final byte[] array, int length, final int n) {
        while (true) {
            try {
                if (length * n * 3 / 2 == array.length) {
                    final Object append = new FaceData();
                    nativeFaceDetectI420(((FaceData) append).nativeInstance(), array, length, n);
                    return (FaceData) append;
                }
                final Object append = new StringBuilder().append("please check the input parameters,if yuv data is null . make sure yuv data length is ").append(length * n * 3 / 2).append(" current length is ");
                if (array == null) {
                    length = 0;
                    throw new IllegalArgumentException(((StringBuilder) append).append(length).toString());
                }
            } catch (Exception e) {
                Log.e(TAG, "faceDetect_I420");
                return null;
            }
        }
    }

    public synchronized FaceData faceDetect_NV21(final byte[] array, int length, final int n, final int n2, final boolean b) {
        while (true) {
            try {
                if (length * n * 3 / 2 == array.length) {
                    final Object append = new FaceData();
                    nativeFaceDetectNV21(((FaceData) append).nativeInstance(), array, length, n, n2, b, false, false, false, false);
                    return (FaceData) append;
                }
                final Object append = new StringBuilder().append("please check the input parameters,if yuv data is null . make sure yuv data length is ").append(length * n * 3 / 2).append(" current length is ");
                if (array == null) {
                    length = 0;
                    throw new IllegalArgumentException(((StringBuilder) append).append(length).toString());
                }
            } catch (Exception e) {
                Log.e(TAG, "faceDetect_NV21");
                return null;
            }
        }
    }

    public synchronized FaceData faceDetect_NV21(final byte[] array, int length, final int n, final int n2, final boolean b, final boolean b2, final boolean b3, final boolean b4, final boolean b5) {
        while (true) {
            try {
                if (length * n * 3 / 2 == array.length) {
                    final Object append = new FaceData();
                    nativeFaceDetectNV21(((FaceData) append).nativeInstance(), array, length, n, n2, b, b2, b3, b4, b5);
                    return (FaceData) append;
                }
                final Object append = new StringBuilder().append("please check the input parameters,if yuv data is null . make sure yuv data length is ").append(length * n * 3 / 2).append(" current length is ");
                if (array == null) {
                    length = 0;
                    throw new IllegalArgumentException(((StringBuilder) append).append(length).toString());
                }
            } catch (Exception e) {
                Log.e(TAG, "faceDetect_NV21");
                return null;
            }
        }
    }

    public synchronized FaceData faceDetect_NativeBitmap(final NativeBitmap nativeBitmap) {
        // monitorenter(this)
        FaceData faceData = null;
        if (nativeBitmap == null) {
            return faceData;
        }
        try {
            faceData = new FaceData();
            nativeFaceDetectNativeBitmap(faceData.nativeInstance(), nativeBitmap.nativeInstance());
            return faceData;
        } finally {
        }
        // monitorexit(this)
    }

    public FaceData faceDetect_NativeBitmap_WithFace(final NativeBitmap nativeBitmap, final int[] array) {
        // monitorenter(this)
        FaceData faceData2;
        final FaceData faceData = faceData2 = null;
        if (nativeBitmap == null) {
            return faceData2;
        }
        faceData2 = faceData;
        if (array == null) {
            return faceData2;
        }
        try {
            faceData2 = new FaceData();
            nativeFaceDetectNativeBitmapWithFace(faceData2.nativeInstance(), nativeBitmap.nativeInstance(), array);
            return faceData2;
        } finally {
        }
        // monitorexit(this)
    }

    public void faceDetect_StopTracking() {
        synchronized (this) {
            nativeFaceDetectStopTracking();
        }
    }

    public boolean faceDetect_init(final Context context) {
        // monitorenter(this)
        boolean nativeFaceDetectInit = false;
        if (context == null) {
            return nativeFaceDetectInit;
        }
        try {
            nativeFaceDetectInit = nativeFaceDetectInit(context);
            return nativeFaceDetectInit;
        } finally {
        }
        // monitorexit(this)
    }

    public boolean faceDetect_setMaxFaceCount(final int n) {
        synchronized (this) {
            return nativeFaceDetectMaxFaceCount(n);
        }
    }
}
