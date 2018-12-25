package com.cvte.virtualbeauty.render;

import android.graphics.Point;

import com.cvte.virtualbeauty.gles.GlComponent;

import java.util.List;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/4/13
 */

public class RenderThread implements IRenderAdapter,Runnable {

    public interface RenderListener {
        void onRequestDraw();
    }

    private static final int DEFAULT_MAX_TIME = 200;

    private boolean isQuit = false;
    private boolean isRunning = false;
    private boolean isWaiting = false;

    private final RenderListener mListener;

    private RenderController mRenderController ;

    private boolean isNeedRefresh = false;

    public RenderThread(RenderListener listener) {
        mRenderController = new RenderController();
        mRenderController.init();
        mListener = listener;
    }

    public synchronized void start() {
        Thread thread = new Thread(this);
        thread.start();
        resume();
    }

    public synchronized void resume() {
        isRunning = true;
    }

    public synchronized void pause() {
        isRunning = false;
    }

    public synchronized void stop() {
        isQuit = true;
        mRenderController.release();
    }

    @Override
    public int getCameraTextureID() {
        return mRenderController.getCameraTextureID();
    }

    @Override
    public void update(int w, int h, int cw, int ch) {
        mRenderController.update(w, h, cw, ch);
    }

    @Override
    public void setData(Type pEnum, List<Point> vertexData, int width, int height) {
        mRenderController.setData(pEnum, vertexData, width, height);
    }

    @Override
    public void setData(Type pEnum, List<Point> circle, List<Point> hole, int width, int height) {
        mRenderController.setData(pEnum, circle, hole, width, height);
    }

    @Override
    public void setRotateMatrix(float[] RotateMatrix) {
        mRenderController.setRotateMatrix(RotateMatrix);
    }

    @Override
    public void load(GlComponent component) {
        mRenderController.load(component);
    }

    public void setDetected(boolean flag) {
        mRenderController.setIsDetected(flag);
    }

    public void postDraw() {
        isNeedRefresh = true;
    }

    public void notifyDraw() {
        synchronized (this) {
            notify();
        }
    }

    @Override
    public void draw() {
        mRenderController.draw();
    }

    @Override
    public void updateImageTex(byte[] data, int width, int height) {
        mRenderController.updateImageTex(data, width, height);
    }

    @Override
    public void run() {
        while (true) {
            if (isQuit) break;
            if(isRunning) {
                synchronized (this) {
                    try {
                        wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                if (isNeedRefresh) {
                    mListener.onRequestDraw();
                }
            }
        }
    }

}
