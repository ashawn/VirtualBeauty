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

public interface IRenderAdapter {

    public enum Type {
        CAMERA, LIP, EYE_LEFT, EYE_RIGHT, EYE_BROW_LEFT, EYE_BROW_RIGHT, EYE_CONTACT_LEFT, EYE_CONTACT_RIGHT, BLUSH_LEFT, BLUSH_RIGHT
    }

    int getCameraTextureID();

    void update(int w, int h, int cw, int ch);

    void setData(Type pEnum, List<Point> vertexData, int width, int height);

    void setData(Type pEnum, List<Point> circle, List<Point> hole, int width, int height);

    void setRotateMatrix(float[] RotateMatrix);

    void load(GlComponent component);

    void draw();

    void updateImageTex(byte[] data,int width,int height);

}
