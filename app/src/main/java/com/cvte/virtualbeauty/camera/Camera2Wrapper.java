/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.camera;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.os.Trace;
import android.renderscript.RenderScript;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.util.Size;
import android.view.Surface;

import com.cvte.virtualbeauty.common.util.CameraUtils;
import com.cvte.virtualbeauty.common.util.ImageUtils;
import com.cvte.virtualbeauty.common.util.ToastUtils;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

/**
 * @deprecated 使用camera2渲染会导致接受帧数据卡顿，camera1暂时没有这个问题{@link CameraWrapper}
 * @author laizhenqi
 * @since 2017/2/6
 */
final class Camera2Wrapper extends CameraHelper implements ImageReader.OnImageAvailableListener {

    private final static String TAG = Camera2Wrapper.class.getSimpleName();

    private HandlerThread mBackgroundThread;
    private Handler mBackgroundHandler;

    private Semaphore mCameraOpenCloseLock = new Semaphore(1);

    private Size mPreviewSize;

    private String mCameraId;

    private CameraDevice mCameraDevice;
    private CameraCaptureSession mCameraCaptureSession;
    private CaptureRequest.Builder mPreviewRequestBuilder;

    private ImageReader mPreviewReader;
    private RenderScript mRs;

    private int[] mRGBBytes;
    private byte[][] mYUVBytes;
    private boolean mIsComputing = false;

    Camera2Wrapper(Context context, SurfaceTexture surfaceTexture) {
        super(context, surfaceTexture);
        mContext = context;
        mSurfaceTexture = surfaceTexture;

        mRs = RenderScript.create(context);
    }

    @Override
    public void open(int width, int height) {
        startBackground();
        setUpCameraOutputs(width, height);
        CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        try {
            if (!mCameraOpenCloseLock.tryAcquire(2500, TimeUnit.MILLISECONDS)) {
                throw new RuntimeException("time out to wait camera open");
            }
            if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                throw new CameraAccessException(CameraAccessException.CAMERA_DISABLED);
            }
            manager.openCamera(mCameraId, mStateCallback, null);
        } catch (CameraAccessException | InterruptedException e) {
            e.printStackTrace();
        }
        mDetectorHelper.start();
    }

    private void startBackground() {
        mBackgroundThread = new HandlerThread(TAG, Process.THREAD_PRIORITY_DISPLAY);
        mBackgroundThread.start();
        mBackgroundHandler = new Handler(mBackgroundThread.getLooper());
    }

    private void stopBackground() {
        if (mBackgroundThread != null) {
            mBackgroundHandler.removeCallbacksAndMessages(null);
            mBackgroundThread.quitSafely();
            try {
                mBackgroundThread.join();
                mBackgroundThread = null;
                mBackgroundHandler = null;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void close() {
        try {
            mCameraOpenCloseLock.acquire();
            if (mCameraCaptureSession != null) {
                mCameraCaptureSession.close();
                mCameraCaptureSession = null;
            }
            if (mCameraDevice != null) {
                mCameraDevice.close();
                mCameraDevice = null;
            }
            if (mPreviewReader != null) {
                mPreviewReader.close();
                mPreviewReader = null;
            }
            mDetectorHelper.stop();
            stopBackground();
        } catch (InterruptedException e) {
            Log.e(TAG, e.getMessage(), e);
        } finally {
            mCameraOpenCloseLock.release();
        }
    }

    long time = 0;

    @Override
    public void onImageAvailable(ImageReader reader) {
        Log.i(TAG, "onImageAvailable");
        Log.i(TAG, "image available gap " + (System.currentTimeMillis() - time) + " ms");
        Image image = reader.acquireLatestImage();
        if (image == null) return;
        try {
            Trace.beginSection("imageAvailable");

            final Image.Plane[] planes = image.getPlanes();
            if (mPreviewSize.getWidth() != image.getWidth() || mPreviewSize.getHeight() != image.getHeight() || mRGBBytes == null || mYUVBytes == null) {
                mPreviewSize = new Size(image.getWidth(), image.getHeight());

                mRGBBytes = new int[image.getWidth() * image.getHeight()];
                mYUVBytes = new byte[planes.length][];
                for (int i = 0; i < planes.length; i++) {
                    mYUVBytes[i] = new byte[planes[i].getBuffer().capacity()];
                }
            }

            for (int i = 0; i < planes.length; i++) {
                planes[i].getBuffer().get(mYUVBytes[i]);
            }

            int yRowStride = planes[0].getRowStride();
            int uvRowStride = planes[1].getRowStride();
            int uvPixelStride = planes[2].getPixelStride();
            ImageUtils.convertYUV420ToARGB8888(
                    mYUVBytes[0],
                    mYUVBytes[1],
                    mYUVBytes[2],
                    mRGBBytes,
                    mPreviewSize.getWidth(),
                    mPreviewSize.getHeight(),
                    yRowStride,
                    uvRowStride,
                    uvPixelStride,
                    false);

            Log.i(TAG, "yuv transform spend time:" + (System.currentTimeMillis() - time));
            image.close();
        } catch (Exception e) {
            if (image != null) {
                image.close();
            }
            Trace.endSection();
            mIsComputing = false;

            return;
        }

        time = System.currentTimeMillis();
        if (mIsComputing) {
            return;
        }
        mIsComputing = true;
        mBackgroundHandler.post(() -> {
            Bitmap bitmap = Bitmap.createBitmap(mRGBBytes, mPreviewSize.getWidth(), mPreviewSize.getHeight(), Bitmap.Config.ARGB_8888);

            boolean ret = mDetectorHelper.detect(bitmap, result -> {
                if (mOnDetectorAvailableListener != null && result != null && result.isDetected()) {
                    mOnDetectorAvailableListener.onDetected(mPreviewSize.getWidth(), mPreviewSize.getHeight(),
                            result);
                }
                mIsComputing = false;
            });
            if (!ret) mIsComputing = false;
        });
    }

    private void setUpCameraOutputs(int width, int height) {
        CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        try {
            for (String cameraId : manager.getCameraIdList()) {
                CameraCharacteristics characteristics = manager.getCameraCharacteristics(cameraId);

                //不处理前置摄像头
                if (CameraCharacteristics.LENS_FACING_FRONT == characteristics.get(CameraCharacteristics.LENS_FACING)) {
                    continue;
                }

                StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);

                mPreviewSize = CameraUtils.chooseOptimalSize(map.getOutputSizes(SurfaceTexture.class),
                        width, height,
                        CameraUtils.chooseVideoSize(map.getOutputSizes(MediaRecorder.class)));
                Log.i(TAG, "preview width:" + mPreviewSize.getWidth() + " height:" + mPreviewSize.getHeight());

                mCameraId = cameraId;
            }
        } catch (CameraAccessException e) {
            e.printStackTrace();
        } catch (Exception e) {
            ToastUtils.getInstance().showToast(mContext, "error");
        }
    }

    private void startPreview() {
        SurfaceTexture texture = mSurfaceTexture;

        try {
            List<Surface> surfaces = new ArrayList<>();
            mPreviewRequestBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_RECORD);
            if (texture != null) {
                texture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());
                Surface surface = new Surface(texture);
                mPreviewRequestBuilder.addTarget(surface);
                surfaces.add(surface);
            }

            mPreviewReader = ImageReader.newInstance(mPreviewSize.getWidth(), mPreviewSize.getHeight(), ImageFormat.YUV_420_888, 2);
            mPreviewReader.setOnImageAvailableListener(this, mBackgroundHandler);
            mPreviewRequestBuilder.addTarget(mPreviewReader.getSurface());
            surfaces.add(mPreviewReader.getSurface());

            mCameraDevice.createCaptureSession(surfaces, new CameraCaptureSession.StateCallback() {
                @Override
                public void onConfigured(@NonNull CameraCaptureSession session) {
                    Log.i(TAG, "onConfigured");
                    if (mCameraDevice == null) {
                        return;
                    }

                    mCameraCaptureSession = session;
                    try {
                        mPreviewRequestBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_OFF);
                        mCameraCaptureSession.setRepeatingRequest(mPreviewRequestBuilder.build(),
                                null, mBackgroundHandler);
                    } catch (CameraAccessException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                    Log.i(TAG, "onConfigureFailed");
                    if (mContext != null) {
                        ToastUtils.getInstance().showToast(mContext, "failed");
                    }
                }
            }, mBackgroundHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            Log.i(TAG, "onOpened");
            mCameraDevice = camera;
            startPreview();
            mCameraOpenCloseLock.release();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice camera) {
            Log.i(TAG, "onDisconnected");
            mCameraOpenCloseLock.release();
            camera.close();
            mCameraDevice = null;
        }

        @Override
        public void onError(@NonNull CameraDevice camera, int error) {
            Log.i(TAG, "onError [error:" + error + "]");
            mCameraOpenCloseLock.release();
            camera.close();
            mCameraDevice = null;

            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(0);
        }
    };
}
