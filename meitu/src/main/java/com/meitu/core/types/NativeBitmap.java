package com.meitu.core.types;

import android.graphics.Bitmap;
import android.util.Log;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/3/10
 */

public class NativeBitmap
{
    private final static String TAG = NativeBitmap.class.getSimpleName();

    public static final int COLOR_TYPE_ABGR = 3;
    public static final int COLOR_TYPE_ARGB = 2;
    public static final int COLOR_TYPE_BGRA = 1;
    public static final int COLOR_TYPE_RGBA = 0;
    public static final int MAX_SIZE_DEFAULT = 0;
    public static final float SCALE_VALUE_DEFAULT = 1.0f;
    private int mHeight;
    private Lock mReadLock;
    private ReadWriteLock mReadWriteLock;
    private int mWidth;
    private Lock mWriteLock;
    protected final long nativeBitmap;

    static {
        System.loadLibrary("mttypes");
    }

    private NativeBitmap(final int mWidth, final int mHeight) {
        this.mReadWriteLock = null;
        this.mReadLock = null;
        this.mWriteLock = null;
        this.mWidth = mWidth;
        this.mHeight = mHeight;
        this.nativeBitmap = nativeCreate(mWidth, mHeight);
        Log.i(TAG, "info: NativeBitmap new address = " + this.nativeBitmap);
        this.mReadWriteLock = new ReentrantReadWriteLock();
        this.mReadLock = this.mReadWriteLock.readLock();
        this.mWriteLock = this.mReadWriteLock.writeLock();
    }

    public static NativeBitmap createBitmap() {
        return new NativeBitmap(0, 0);
    }

    public static NativeBitmap createBitmap(final int n, final int n2) {
        return new NativeBitmap(n, n2);
    }

    public static NativeBitmap createBitmap(final int[] array, final int n, final int n2, final int n3) {
        final NativeBitmap nativeBitmap = new NativeBitmap(0, 0);
        nativeSetPixels(nativeBitmap.nativeBitmap, array, n, n2, n3);
        return nativeBitmap;
    }

    public static void drawBitmap(final Bitmap image, final NativeBitmap nativeBitmap) {
        if (image != null && nativeBitmap != null) {
            nativeBitmap.setImage(image);
        }
    }

    public static void drawBitmap(final NativeBitmap nativeBitmap, final Bitmap bitmap) {
        if (nativeBitmap != null && bitmap != null) {
            nativeBitmap.readLock();
            nativeCopyToBitmap(nativeBitmap.nativeBitmap, bitmap);
            nativeBitmap.readUnLock();
        }
    }

    public static void drawBitmap(final NativeBitmap nativeBitmap, final NativeBitmap nativeBitmap2) {
        if (nativeBitmap != null && nativeBitmap2 != null) {
            nativeBitmap.readLock();
            nativeBitmap2.writeLock();
            nativeCopy(nativeBitmap.nativeBitmap, nativeBitmap2.nativeBitmap);
            nativeBitmap.readUnLock();
            nativeBitmap2.writeUnLock();
        }
    }

    private static native void finalizer(final long p0);

    private static native boolean nativeCopy(final long p0, final long p1);

    private static native boolean nativeCopyToBitmap(final long p0, final Bitmap p1);

    private static native long nativeCreate(final int p0, final int p1);

    private static native int nativeGetHeight(final long p0);

    private static native Bitmap nativeGetImage(final long p0, final int p1, final int p2);

    private static native int nativeGetWidth(final long p0);

    private static native boolean nativeIsRecycled(final long p0);

    private static native void nativeRelease(final long p0);

    private static native boolean nativeSetImage(final long p0, final Bitmap p1);

    private static native boolean nativeSetPixels(final long p0, final int[] p1, final int p2, final int p3, final int p4);

    private static native Bitmap nativeToBitmapBGRX(final long p0, final int p1, final int p2);

    private void readLock() {
        this.mReadLock.lock();
    }

    private void readUnLock() {
        this.mReadLock.unlock();
    }

    private void writeLock() {
        this.mWriteLock.lock();
    }

    private void writeUnLock() {
        this.mWriteLock.unlock();
    }

    public NativeBitmap copy() {
        final NativeBitmap nativeBitmap = new NativeBitmap(this.getWidth(), this.getHeight());
        this.readLock();
        nativeCopy(this.nativeBitmap, nativeBitmap.nativeInstance());
        this.readUnLock();
        return nativeBitmap;
    }

    @Override
    protected void finalize() {
        try {
            if (!this.isRecycled()) {
                Log.e(TAG, "ERROR: NativeBitmap is not recycle by yourself, May cause native memory leaks. address = " + this.nativeBitmap);
            }
            else {
                Log.i(TAG, "java finalize NativeBitmap obj address=" + this.nativeBitmap);
            }
            finalizer(this.nativeBitmap);
        }
        finally {
            try {
                super.finalize();
            } catch (Throwable throwable) {
                throwable.printStackTrace();
            }
        }
    }

    public Bitmap getBitmapBGRX() {
        this.readLock();
        final Bitmap nativeToBitmapBGRX = nativeToBitmapBGRX(this.nativeBitmap, this.getWidth(), this.getHeight());
        this.readUnLock();
        return nativeToBitmapBGRX;
    }

    public Bitmap getBitmapBGRX(final int n, final int n2) {
        if (n <= 0 || n2 <= 0) {
            throw new RuntimeException("NativeBitmap getBitmapBGRX width and height must be > 0");
        }
        this.readLock();
        final Bitmap nativeToBitmapBGRX = nativeToBitmapBGRX(this.nativeBitmap, n, n2);
        this.readUnLock();
        return nativeToBitmapBGRX;
    }

    public int getHeight() {
        return nativeGetHeight(this.nativeBitmap);
    }

    public Bitmap getImage() {
        this.readLock();
        final Bitmap nativeGetImage = nativeGetImage(this.nativeBitmap, this.getWidth(), this.getHeight());
        this.readUnLock();
        return nativeGetImage;
    }

    public Bitmap getImage(final int n, final int n2) {
        if (n <= 0 || n2 <= 0) {
            throw new RuntimeException("NativeBitmap getImage width and height must be > 0");
        }
        this.readLock();
        final Bitmap nativeGetImage = nativeGetImage(this.nativeBitmap, n, n2);
        this.readUnLock();
        return nativeGetImage;
    }

    public int getWidth() {
        return nativeGetWidth(this.nativeBitmap);
    }

    public void invalidate() {
        this.mWidth = this.getWidth();
        this.mHeight = this.getHeight();
    }

    public boolean isRecycled() {
        return nativeIsRecycled(this.nativeBitmap);
    }

    public long nativeInstance() {
        return this.nativeBitmap;
    }

    public void recycle() {
        this.writeLock();
        nativeRelease(this.nativeBitmap);
        this.writeUnLock();
    }

    public NativeBitmap scale(final int n, final int n2) {
        final NativeBitmap nativeBitmap = new NativeBitmap(n, n2);
        this.readLock();
        nativeCopy(this.nativeBitmap, nativeBitmap.nativeBitmap);
        this.readUnLock();
        return nativeBitmap;
    }

    public boolean setImage(final Bitmap bitmap) {
        this.writeLock();
        final boolean nativeSetImage = nativeSetImage(this.nativeBitmap, bitmap);
        this.invalidate();
        this.writeUnLock();
        return nativeSetImage;
    }

    public boolean setPixels(final int[] array, final int n, final int n2, final int n3) {
        this.writeLock();
        final boolean nativeSetPixels = nativeSetPixels(this.nativeBitmap, array, n, n2, n3);
        this.invalidate();
        this.writeUnLock();
        return nativeSetPixels;
    }
}
