package com.cvte.virtualbeauty.camera;

import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.view.WindowManager;

import com.cvte.virtualbeauty.common.utils.LogUtil;

import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import java.io.IOException;
import java.util.List;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/3/7
 */

public class CameraWrapper extends CameraHelper implements Camera.PreviewCallback {

    private static final String TAG = CameraWrapper.class.getSimpleName();

    private Camera mCamera;

    private int mCameraId = -1;

    private int mPreviewWidth = 0;
    private int mPreviewHeight = 0;

    private int mPictureWidth = 640;
    private int mPictureHeigth = 480;

    private byte[] mBuffer;
    private byte[] mYUVBytes;
    private int[] mRGBColors;

    public CameraWrapper(Context context, SurfaceTexture surfaceTexture) {
        super(context, surfaceTexture);
    }

    @Override
    public void open(int width, int height) {
        mDetectorHelper.start();
        int count = Camera.getNumberOfCameras();
        for (int i = 0; i < count; i++) {
            Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
            Camera.getCameraInfo(i, cameraInfo);
            if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                mCameraId = i;
            }
        }
        if (mCameraId == -1) throw new RuntimeException("Not Found Front Facing Camera");
        mCamera = Camera.open(mCameraId);
        setMinParameters();
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setRecordingHint(true);
//        mCamera.setDisplayOrientation(getCameraDisplayOrientation());
        mCamera.setParameters(parameters);
//
        int buffersize = mPreviewWidth * mPreviewHeight * ImageFormat.getBitsPerPixel(parameters.getPreviewFormat()) / 8;
        mBuffer = new byte[buffersize];
        mYUVBytes = new byte[buffersize];
        mRGBColors = new int[mPreviewWidth * mPreviewHeight];

        try {
            mCamera.setPreviewTexture(mSurfaceTexture);
            mCamera.addCallbackBuffer(mBuffer);
            mCamera.setPreviewCallbackWithBuffer(this);
        } catch (IOException e) {
            e.printStackTrace();
        }

        mCamera.startPreview();
    }

    @Override
    public void setDetectRegion(Rect region) {
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        Point screen = new Point();
        wm.getDefaultDisplay().getRealSize(screen);
        //求将预览缩放后的大小
        //如果实际显示区域比预览长宽比大，则固定宽度，对高度进行缩放
        float width = screen.x;
        float height = screen.y;
        if (screen.x * mPreviewWidth > screen.y * mPreviewHeight) {
            height = width * mPreviewHeight / mPreviewWidth;
        } else {
            width = height * mPreviewWidth / mPreviewHeight;
        }
        //获取相对于实际预览的裁剪区域
        region.offset((int) (width - screen.x) / 2, (int) (height - screen.y) / 2);
        float factor = mPreviewWidth / width;
        mDetectorHelper.setDetectRegion(new Rect((int) (region.left * factor),
                (int) (region.top * factor),
                (int) (region.right * factor),
                (int) (region.bottom * factor)));
    }

    @Override
    public void close() {
        if (mCamera != null) {
            mCamera.setPreviewCallbackWithBuffer(null);
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
        mDetectorHelper.stop();
    }

    public byte[] getImageTex() {
        return mYUVBytes;
    }

    private void setMinParameters() {
        if (mCamera != null) {
            Camera.Parameters parameters = mCamera.getParameters();
            List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
            Camera.Size optimalSize = getMinPreviewSize(sizes);
            parameters.setPreviewSize(optimalSize.width, optimalSize.height);
            mPreviewWidth = optimalSize.width;
            mPreviewHeight = optimalSize.height;
            sizes = parameters.getSupportedPictureSizes();
            Camera.Size pictureSize = getOptimalPreviewSize(sizes, mPictureWidth, mPictureHeigth);
            mPictureWidth = pictureSize.width;
            mPictureHeigth = pictureSize.height;
            parameters.setPictureSize(mPictureWidth, mPictureHeigth);
            parameters.setPreviewFormat(ImageFormat.NV21);
            mCamera.setParameters(parameters);
        }
    }

    private Camera.Size getMinPreviewSize(List<Camera.Size> sizes) {
        int min = Integer.MAX_VALUE;
        Camera.Size minus = null;
        for (Camera.Size size : sizes) {
            int result = size.width * size.height;
            if (result < min && result > 150000) {
                min = result;
                minus = size;
            }
        }
        return minus;
    }

    private Camera.Size getOptimalPreviewSize(List<Camera.Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.05;
        double targetRatio = (double) w / h;

        if (sizes == null) {
            return null;
        }

        Camera.Size optimalSize = null;

        double minDiff = Double.MAX_VALUE;
        int targetHeight = h;
        for (Camera.Size size : sizes) {
            double ratio = (double) size.width / size.height;

            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
                continue;

            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }
        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Camera.Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }

    private boolean mIsComputing = false;

    private long time = 0;

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        LogUtil.i(TAG, "onPreviewFrame");
        LogUtil.i(TAG, "image available gap " + (System.currentTimeMillis() - time) + " ms");
        camera.addCallbackBuffer(mBuffer);
        if (mIsComputing) return;
        mIsComputing = true;
        time = System.currentTimeMillis();
        Mat bgr = new Mat();
        Mat yuv = new Mat(mPreviewHeight + mPreviewHeight / 2, mPreviewWidth, CvType.CV_8UC1);
        yuv.put(0, 0, data);
        Imgproc.cvtColor(yuv, bgr, Imgproc.COLOR_YUV2BGR_NV21, 3);
        yuv.release();
//        ImageUtils.convertYUV420SPToARGB8888(data, mRGBColors, mPreviewWidth, mPreviewHeight, false);

        LogUtil.i(TAG, "yuv transform spend time " + (System.currentTimeMillis() - time) + " ms");
        time = System.currentTimeMillis();
        System.arraycopy(mBuffer, 0, mYUVBytes, 0, mBuffer.length);
//        Bitmap bitmap = Bitmap.createBitmap(mRGBColors, mPreviewWidth, mPreviewHeight, Bitmap.Config.ARGB_8888);
        boolean ret = mDetectorHelper.detect(bgr, result -> {
            if (mOnDetectorAvailableListener != null && result != null) {
                mOnDetectorAvailableListener.onDetected(mPreviewWidth, mPreviewHeight, result);
            }
            LogUtil.i(TAG, "detect face spend time " + (System.currentTimeMillis() - time) + " ms");
            mIsComputing = false;
            bgr.release();
        });
        if (!ret) {
            mIsComputing = false;
            bgr.release();
        }
        time = System.currentTimeMillis();
    }
}
