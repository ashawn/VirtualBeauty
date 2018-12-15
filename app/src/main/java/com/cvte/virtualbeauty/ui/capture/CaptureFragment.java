/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.capture;

import android.app.Fragment;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.LinearSnapHelper;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.cvte.virtualbeauty.BuildConfig;
import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.camera.CameraHelper;
import com.cvte.virtualbeauty.camera.CameraWrapper;
import com.cvte.virtualbeauty.common.util.ToastUtils;
import com.cvte.virtualbeauty.common.utils.LogUtil;
import com.cvte.virtualbeauty.common.view.ExtRecyclerView;
import com.cvte.virtualbeauty.data.render.RenderScheme;
import com.cvte.virtualbeauty.dlib.DetectorRet;
import com.cvte.virtualbeauty.render.IRenderAdapter;
import com.cvte.virtualbeauty.render.RenderThread;

import java.lang.ref.WeakReference;
import java.util.List;
import java.util.Locale;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import butterknife.BindView;
import butterknife.ButterKnife;
import io.reactivex.Observable;

/**
 * capture界面fragment组件，包含一个用来预览的glSurfaceView、操作按钮、渲染效果列表<br>
 * 通过选取渲染效果，动态变化加载的shader
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
public class CaptureFragment extends Fragment implements CaptureView {

    private static final String TAG = CaptureFragment.class.getSimpleName();

    static final int MESSAGE_OPEN_CAMERA_PREVIEW = 0X00;
    static final int MESSAGE_UPDATE_FPS = 0x01;

    @BindView(R.id.glSurfaceView_capture_fragment)
    GLSurfaceView mGLView;

    @BindView(R.id.button_share_capture_fragment)
    Button mBtnShar;

    @BindView(R.id.view_frame_capture_fragment)
    View mCaptureFrame;

    @BindView(R.id.recyclerView_render_scheme_capture_fragment)
    ExtRecyclerView mListRenderScheme;

    @BindView(R.id.textView_fps)
    TextView mFps;

    private CustomHandler mFragmentHandler;

    private CustomRender mRender;
    private RenderSchemeAdapter mAdapter;

    private CapturePresenter mPresenter;

    private CameraHelper mCameraHelper;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        LogUtil.i(TAG, "onCreateView");
        View root = inflater.inflate(R.layout.fragment_capture, container, false);
        ButterKnife.bind(this, root);

        mFragmentHandler = new CustomHandler(this);
        initViews();
        return root;
    }

    @Override
    public void onStart() {
        LogUtil.i(TAG, "onStart");
        super.onStart();
        mPresenter.start();
    }

    @Override
    public void onResume() {
        LogUtil.i(TAG, "onResume");
        super.onResume();
        mPresenter.resume();
        mGLView.onResume();
    }

    @Override
    public void onPause() {
        LogUtil.i(TAG, "onPause");
        closeCamera();
        mGLView.queueEvent(() -> mRender.notifyPausing());
        mGLView.onPause();
        mPresenter.pause();
        super.onPause();
    }

    @Override
    public void onStop() {
        LogUtil.i(TAG, "onStop");
        mPresenter.stop();
        super.onStop();
    }

    @Override
    public void setPresenter(CapturePresenter presenter) {
        this.mPresenter = presenter;
    }

    @Override
    public void updateRenderSchemeList(List<RenderScheme> list) {
        mAdapter.refreshList(list);
    }

    private void initViews() {
        mGLView.setEGLContextClientVersion(2);
        mGLView.setRenderer(mRender = new CustomRender(mFragmentHandler));
        mGLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        mListRenderScheme.setAdapter(mAdapter = new RenderSchemeAdapter());
        mListRenderScheme.setSelectMode(ExtRecyclerView.MODE_SINGLE);
        mListRenderScheme.setLayoutManager(new LinearLayoutManager(getActivity(), LinearLayoutManager.HORIZONTAL, false));
        mListRenderScheme.addItemDecoration(new RecyclerView.ItemDecoration() {
            @Override
            public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
                //保持[10,5,10,5]的边距
                outRect.top = 5;
                outRect.bottom = 5;
                outRect.right = 10;
                outRect.left = 10;
            }
        });
        mListRenderScheme.setOnItemClicked(position -> {
            RenderScheme renderScheme = mAdapter.getItemAt(position);
            loadRenderscheme(renderScheme);
            mAdapter.selected(position);
        });
        new LinearSnapHelper().attachToRecyclerView(mListRenderScheme);//使每次都能完全选中一个item
    }

    private void openCamera() {
        if (mRender.getSurfaceTexture() != null) {
            mCameraHelper = CameraHelper.instantiate(getActivity(), mRender.getSurfaceTexture());
            mCameraHelper.open(1280, 720);
            Rect rect = new Rect();
            mCaptureFrame.getGlobalVisibleRect(rect);
            mCameraHelper.setDetectRegion(rect);
            LogUtil.d(TAG, "detect region : " + rect);
            mCameraHelper.setOnDetectorAvailableListener(mRender);
        }
    }

    private void closeCamera() {
        if (mCameraHelper != null) {
            mCameraHelper.close();
            mCameraHelper = null;
        }
    }

    private void loadRenderscheme(RenderScheme renderScheme) {
        LogUtil.d(TAG, "load RenderScheme:" + renderScheme.toString());
        if (!renderScheme.isCached()) {
            ToastUtils.getInstance().showToast(getActivity(), "U must download shader file before load");
        } else {
            mGLView.queueEvent(() -> mRender.loadRenderScheme(renderScheme));
        }
    }

    private final class CustomRender implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener, CameraHelper.OnDetectorAvailableListener {

        private String tag = CustomRender.class.getSimpleName();

        private final float[] mSTMatrix = new float[16];
        private SurfaceTexture mSurfaceTexture;
        private int mTextureID;

        private RenderThread mRenderThread;

        private Handler mHandler;

        private final Object mSurfaceTextureLock = new Object();

        CustomRender(Handler handler) {
            this.mHandler = handler;
        }

        void notifyPausing() {
            synchronized (mSurfaceTextureLock) {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.release();
                    mSurfaceTexture = null;
                }
            }
            if (mRenderThread != null) {
                mRenderThread.stop();
                mRenderThread = null;
            }
        }

        void loadRenderScheme(RenderScheme renderScheme) {
            Observable.fromIterable(renderScheme.getComponentList())
                    .subscribe(glComponent -> {
                        if (mRenderThread != null)
                            mRenderThread.load(glComponent);
                    });
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            LogUtil.i(tag, "onSurfaceCreated");

            mRenderThread = new RenderThread(() -> mGLView.requestRender());
            mTextureID = mRenderThread.getCameraTextureID();
            mSurfaceTexture = new SurfaceTexture(mTextureID);
            mSurfaceTexture.setOnFrameAvailableListener(this);

            mRenderThread.start();
            mHandler.sendEmptyMessage(MESSAGE_OPEN_CAMERA_PREVIEW);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            LogUtil.i(tag, "onSurfaceChanged");
            mRenderThread.update(width, height, 640, 480);
        }

        double totalTime = 0;
        long mFrameCount = 0;
        long deltaTime = 0;

        @Override
        public void onDrawFrame(GL10 gl) {
            if (BuildConfig.DEBUG) {
                if (deltaTime == 0) {
                    deltaTime = System.nanoTime();
                } else {
                    totalTime += (System.nanoTime() - deltaTime) / 1000000000.0;
                    deltaTime = System.nanoTime();
                    mFrameCount++;
                    if (mFrameCount > 50)
                        mFragmentHandler.obtainMessage(MESSAGE_UPDATE_FPS,
                                String.format(Locale.getDefault(), "%.2f fps", mFrameCount / totalTime))
                                .sendToTarget();
                }
            }
            synchronized (mSurfaceTextureLock) {
                mSurfaceTexture.updateTexImage();
                mSurfaceTexture.getTransformMatrix(mSTMatrix);
            }
            mRenderThread.draw();
        }

        @Override
        public void onFrameAvailable(SurfaceTexture surfaceTexture) {
            if (mRenderThread != null) {
                mRenderThread.postDraw();
            }
        }

        SurfaceTexture getSurfaceTexture() {
            synchronized (mSurfaceTextureLock) {
                return mSurfaceTexture;
            }
        }

        @Override
        public void onDetected(int width, int height, DetectorRet detectorRet) {
            LogUtil.i(TAG, "onDetected");
            mGLView.queueEvent(() -> {
                if (mRenderThread == null) return;
                mRenderThread.setDetected(detectorRet.isDetected());
                mRenderThread.updateImageTex(((CameraWrapper) mCameraHelper).getImageTex(), width, height);
                LogUtil.d(tag, "face detect spend time : " + detectorRet.getSpendTime());
                mRenderThread.setData(IRenderAdapter.Type.LIP,
                        detectorRet.get(DetectorRet.TYPE_LIP_OUT),detectorRet.get(DetectorRet.TYPE_LIP_IN),
                        width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_LEFT,
                        detectorRet.get(DetectorRet.TYPE_EYE_LEFT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_RIGHT,
                        detectorRet.get(DetectorRet.TYPE_EYE_RIGHT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_BROW_LEFT,
                        detectorRet.get(DetectorRet.TYPE_EYE_BROW_LEFT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_BROW_RIGHT,
                        detectorRet.get(DetectorRet.TYPE_EYE_BROW_RIGHT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_CONTACT_LEFT,
                        detectorRet.get(DetectorRet.TYPE_EYE_CONTACT_LEFT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.EYE_CONTACT_RIGHT,
                        detectorRet.get(DetectorRet.TYPE_EYE_CONTACT_RIGHT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.BLUSH_LEFT,
                        detectorRet.get(DetectorRet.TYPE_BLUSH_LEFT), width, height);
                mRenderThread.setData(IRenderAdapter.Type.BLUSH_RIGHT,
                        detectorRet.get(DetectorRet.TYPE_BLUSH_RIGHT), width, height);
                mRenderThread.setRotateMatrix(detectorRet.getRotateMatrix());
                mRenderThread.notifyDraw();
            });
        }
    }

    private static final class CustomHandler extends Handler {

        WeakReference<CaptureFragment> mFragmentWeakReference;

        CustomHandler(CaptureFragment fragment) {
            mFragmentWeakReference = new WeakReference<>(fragment);
        }

        @Override
        public void handleMessage(Message msg) {
            CaptureFragment fragment = mFragmentWeakReference.get();
            if (fragment == null || fragment.getActivity() == null || fragment.getActivity().isFinishing()) {
                removeCallbacksAndMessages(null);
                return;
            }
            switch (msg.what) {
                case MESSAGE_OPEN_CAMERA_PREVIEW:
                    fragment.openCamera();
                    break;
                case MESSAGE_UPDATE_FPS:
                    fragment.mFps.setText((String) msg.obj);
                    break;
            }
        }
    }
}
