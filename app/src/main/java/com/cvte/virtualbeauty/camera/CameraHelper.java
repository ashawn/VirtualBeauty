package com.cvte.virtualbeauty.camera;

import android.content.Context;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.util.SparseIntArray;
import android.view.Surface;
import android.view.WindowManager;

import com.cvte.virtualbeauty.dlib.DetectorHelper;
import com.cvte.virtualbeauty.dlib.DetectorRet;

/**
 * <p>相机类，负责调用相机api打开android相机</p>
 *
 * @author laizhenqi
 * @since 2017/3/7
 */

public abstract class CameraHelper {

    private static final SparseIntArray ORIENTATIONS = new SparseIntArray();

    static {
        ORIENTATIONS.append(Surface.ROTATION_0, 90);
        ORIENTATIONS.append(Surface.ROTATION_90, 0);
        ORIENTATIONS.append(Surface.ROTATION_180, 270);
        ORIENTATIONS.append(Surface.ROTATION_270, 180);
    }

    Context mContext;

    SurfaceTexture mSurfaceTexture;

    DetectorHelper mDetectorHelper;
    OnDetectorAvailableListener mOnDetectorAvailableListener;

    public interface OnDetectorAvailableListener {
        void onDetected(int width, int height, DetectorRet lip);
    }

    public static CameraHelper instantiate(Context context, SurfaceTexture surfaceTexture) {
        return new CameraWrapper(context, surfaceTexture);
    }

    CameraHelper(Context context, SurfaceTexture surfaceTexture) {
        this.mContext = context;
        this.mSurfaceTexture = surfaceTexture;
        mDetectorHelper = new DetectorHelper();
    }

    public abstract void open(int width, int height);

    /**
     * 这里设置的检测区域是显示屏上的区域，而不是预览图的区域，需要进行相应缩放
     * @param region
     */
    public void setDetectRegion(Rect region) {
    }

    /**
     * 关闭相机
     */
    public abstract void close();

    public void setOnDetectorAvailableListener(OnDetectorAvailableListener onDetectorAvailableListener) {
        mOnDetectorAvailableListener = onDetectorAvailableListener;
    }

    int getCameraDisplayOrientation() {
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        return ORIENTATIONS.get(wm.getDefaultDisplay().getRotation());
    }
}
