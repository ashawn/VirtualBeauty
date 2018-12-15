/**
 * Copyright © 2013 CVTE. All Rights Reserved.
 */
package com.cvte.virtualbeauty.render;

import android.graphics.Point;
import android.support.annotation.Keep;

import com.cvte.virtualbeauty.common.utils.LogUtil;
import com.cvte.virtualbeauty.gles.GlComponent;
import com.cvte.virtualbeauty.gles.IGlComponentLoader;
import com.cvte.virtualbeauty.gles.LipTriangulate;

import org.poly2tri.triangulation.delaunay.DelaunayTriangle;

import java.util.ArrayList;
import java.util.List;

/**
 * RenderController 用于做渲染的整体控制
 *
 * @author zhangshuo
 * @since 2017/2/9
 */
public class RenderController implements IGlComponentLoader, IRenderAdapter {

    private static final String TAG = RenderController.class.getSimpleName();

    static {
        try {
            System.loadLibrary("glnative");
        } catch (Exception e) {
            LogUtil.e(TAG, "library not found");
        }
    }

    public void init() {
        nativeInit();
    }

    @Override
    public int getCameraTextureID() {
        return getNativeCameraTextureID();
    }

    @Override
    public void setData(Type pEnum, List<Point> vertexData, int width, int height) {
        if (vertexData.size() <= 3) {
            return;
        }
        if (pEnum == Type.LIP) {
            List<DelaunayTriangle> triangles = new ArrayList<>();
            LipTriangulate.process(vertexData, triangles);
            float[] vertexArray = new float[vertexData.size() * 3];
            byte[] indiceArray = new byte[(vertexData.size() - 2) * 3];
            for (int i = 0; i < vertexData.size(); i++) {
                vertexArray[3 * i] = vertexData.get(i).x * 1.0f / width * 2 - 1;
                vertexArray[3 * i + 1] = 1.0f - vertexData.get(i).y * 1.0f / height * 2;
                vertexArray[3 * i + 2] = 0;
            }
            for (int i = 0; i < triangles.size(); i++) {
                indiceArray[3 * i] = (byte) triangles.get(i).points[0].getZf();
                indiceArray[3 * i + 1] = (byte) triangles.get(i).points[1].getZf();
                indiceArray[3 * i + 2] = (byte) triangles.get(i).points[2].getZf();
            }
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_LEFT) {
            float eyex = vertexData.get(3).x - vertexData.get(0).x;
            float eyey = vertexData.get(3).y - vertexData.get(0).y;

            float eyeh1x = vertexData.get(1).x - vertexData.get(5).x;
            float eyeh1y = vertexData.get(1).y - vertexData.get(5).y;
            float eyeh2x = vertexData.get(2).x - vertexData.get(4).x;
            float eyeh2y = vertexData.get(2).y - vertexData.get(4).y;


            float eyelength = (float) Math.sqrt(Math.pow(eyey, 2) + Math.pow(eyex, 2));
            float eyeheight = (float) (Math.sqrt(Math.pow(eyeh1y, 2) + Math.pow(eyeh1x, 2)) + Math.sqrt(Math.pow(eyeh2y, 2) + Math.pow(eyeh2x, 2))) / 2.0f;

            float x1 = eyex / 2;
            float y1 = eyey / 2;
            float x2 = eyeheight * eyey / eyelength;
            float y2 = eyeheight * eyex / eyelength;

            float Orix = (vertexData.get(0).x + vertexData.get(3).x) / 2.0f;
            float Oriy = (vertexData.get(0).y + vertexData.get(3).y) / 2.0f;

            float[] vertexArray = {
                    (Orix - (x1 + x2) * 3) / width * 2 - 1, 1.0f - (Oriy - (y1 - y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (Orix + (x1 - x2) * 3) / width * 2 - 1, 1.0f - (Oriy + (y1 + y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (Orix - (x1 - x2) * 3) / width * 2 - 1, 1.0f - (Oriy - (y1 + y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (Orix + (x1 + x2) * 3) / width * 2 - 1, 1.0f - (Oriy + (y1 - y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_RIGHT) {
            float eyex = vertexData.get(3).x - vertexData.get(0).x;
            float eyey = vertexData.get(3).y - vertexData.get(0).y;

            float eyeh1x = vertexData.get(1).x - vertexData.get(5).x;
            float eyeh1y = vertexData.get(1).y - vertexData.get(5).y;
            float eyeh2x = vertexData.get(2).x - vertexData.get(4).x;
            float eyeh2y = vertexData.get(2).y - vertexData.get(4).y;

            float eyelength = (float) Math.sqrt(Math.pow(eyey, 2) + Math.pow(eyex, 2));
            float eyeheight = (float) (Math.sqrt(Math.pow(eyeh1y, 2) + Math.pow(eyeh1x, 2)) + Math.sqrt(Math.pow(eyeh2y, 2) + Math.pow(eyeh2x, 2))) / 2.0f;

            float x1 = eyex / 2;
            float y1 = eyey / 2;
            float x2 = eyeheight * eyey / eyelength;
            float y2 = eyeheight * eyex / eyelength;

            float Orix = (vertexData.get(0).x + vertexData.get(3).x) / 2.0f;
            float Oriy = (vertexData.get(0).y + vertexData.get(3).y) / 2.0f;

            float[] vertexArray = {
                    (Orix - (x1 + x2) * 3) / width * 2 - 1, 1.0f - (Oriy - (y1 - y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (Orix + (x1 - x2) * 3) / width * 2 - 1, 1.0f - (Oriy + (y1 + y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (Orix - (x1 - x2) * 3) / width * 2 - 1, 1.0f - (Oriy - (y1 + y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
                    (Orix + (x1 + x2) * 3) / width * 2 - 1, 1.0f - (Oriy + (y1 - y2) * 3 - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_BROW_LEFT) {
            float Orix = (vertexData.get(2).x + (vertexData.get(4).x + vertexData.get(0).x) / 2.0f) / 2.0f;
            float Oriy = (vertexData.get(2).y + (vertexData.get(4).y + vertexData.get(0).y) / 2.0f) / 2.0f;

            float deltaX = vertexData.get(4).x - vertexData.get(0).x;
            float deltaY = vertexData.get(4).y - vertexData.get(0).y;

            float Length = (float) Math.sqrt(Math.pow(deltaX, 2) + Math.pow(deltaY, 2));
            float BrowHeight = Length * 3 / 4;

            float x1 = deltaX * 5 / 8;
            float y1 = deltaY * 5 / 8;
            float x2 = BrowHeight * deltaY / Length / 2;
            float y2 = BrowHeight * deltaX / Length / 2;

            float[] vertexArray = {
                    (Orix - (x1 + x2)) / width * 2 - 1, 1.0f - (Oriy - (y1 - y2) - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (Orix + (x1 - x2)) / width * 2 - 1, 1.0f - (Oriy + (y1 + y2) - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (Orix - (x1 - x2)) / width * 2 - 1, 1.0f - (Oriy - (y1 + y2) - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
                    (Orix + (x1 + x2)) / width * 2 - 1, 1.0f - (Oriy + (y1 - y2) - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_BROW_RIGHT) {
            float Orix = (vertexData.get(2).x + (vertexData.get(4).x + vertexData.get(0).x) / 2.0f) / 2.0f;
            float Oriy = (vertexData.get(2).y + (vertexData.get(4).y + vertexData.get(0).y) / 2.0f) / 2.0f;

            float deltaX = vertexData.get(4).x - vertexData.get(0).x;
            float deltaY = vertexData.get(4).y - vertexData.get(0).y;

            float Length = (float) Math.sqrt(Math.pow(deltaX, 2) + Math.pow(deltaY, 2));
            float BrowHeight = Length * 3 / 4;

            float x1 = deltaX * 5 / 8;
            float y1 = deltaY * 5 / 8;
            float x2 = BrowHeight * deltaY / Length / 2;
            float y2 = BrowHeight * deltaX / Length / 2;

            float[] vertexArray = {
                    (Orix - (x1 + x2)) / width * 2 - 1, 1.0f - (Oriy - (y1 - y2) - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (Orix + (x1 - x2)) / width * 2 - 1, 1.0f - (Oriy + (y1 + y2) - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (Orix - (x1 - x2)) / width * 2 - 1, 1.0f - (Oriy - (y1 + y2) - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (Orix + (x1 + x2)) / width * 2 - 1, 1.0f - (Oriy + (y1 - y2) - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_CONTACT_LEFT) {
            float[] vertexArray = {
                    (vertexData.get(0).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(0).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (vertexData.get(1).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(1).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (vertexData.get(2).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(2).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (vertexData.get(3).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(3).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.EYE_CONTACT_RIGHT) {
            float[] vertexArray = {
                    (vertexData.get(0).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(0).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (vertexData.get(1).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(1).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
                    (vertexData.get(2).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(2).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (vertexData.get(3).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(3).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.BLUSH_LEFT) {
            float[] vertexArray = {
                    (vertexData.get(0).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(0).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (vertexData.get(1).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(1).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
                    (vertexData.get(2).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(2).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (vertexData.get(3).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(3).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        } else if (pEnum == Type.BLUSH_RIGHT) {
            float[] vertexArray = {
                    (vertexData.get(0).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(0).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 0.0f,
                    (vertexData.get(1).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(1).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 0.0f,
                    (vertexData.get(2).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(2).y - height / 8.0f) / height * 8 / 3, 0.0f, 0.0f, 1.0f,
                    (vertexData.get(3).x) * 1.0f / width * 2 - 1, 1.0f - (vertexData.get(3).y - height / 8.0f) / height * 8 / 3, 0.0f, 1.0f, 1.0f,
            };
            byte[] indiceArray = {
                    0, 1, 2,
                    1, 2, 3,
            };
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        }
    }

    @Override
    public synchronized void setData(Type pEnum, List<Point> circle, List<Point> hole, int width, int height) {
        if (hole.size() == 0) {
            setData(pEnum, circle, width, height);
            return;
        }
        if (circle.size() < 3) return;
        if (pEnum == Type.LIP) {
            List<DelaunayTriangle> triangles = new ArrayList<>();
            LipTriangulate.process(circle, hole, triangles);
            int size = triangles.size();
            if (size == 0) return;
            float[] vertexArray = new float[size * 3];
            byte[] indiceArray = new byte[size * 3];
            int offset = 0;
            for (int i = 0; i < circle.size(); i++) {
                vertexArray[3 * i + offset] = circle.get(i).x * 1.0f / width * 2 - 1;
                vertexArray[3 * i + 1 + offset] = 1.0f - circle.get(i).y * 1.0f / height * 2;
                vertexArray[3 * i + 2 + offset] = 0;
            }
            offset += circle.size() * 3;
            for (int i = 0; i < hole.size(); i++) {
                vertexArray[3 * i + offset] = hole.get(i).x * 1.0f / width * 2 - 1;
                vertexArray[3 * i + 1 + offset] = 1.0f - hole.get(i).y * 1.0f / height * 2;
                vertexArray[3 * i + 2 + offset] = 0;
            }
            for (int i = 0; i < triangles.size(); i++) {
                indiceArray[3 * i] = (byte) triangles.get(i).points[0].getZf();
                indiceArray[3 * i + 1] = (byte) triangles.get(i).points[1].getZf();
                indiceArray[3 * i + 2] = (byte) triangles.get(i).points[2].getZf();
            }
            nativeSetData(pEnum.ordinal(), vertexArray, indiceArray);
        }
    }

    @Override
    public synchronized void setRotateMatrix(float[] RotateMatrix) {
        nativeSetRotateMatrix(RotateMatrix);
    }

    @Override
    public void draw() {
        nativeDraw();
    }

    @Override
    public void updateImageTex(byte[] data, int width, int height) {
        nativeUpdateImageTex(data, width, height);
    }

    @Override
    public void update(int vwidth, int vheight, int cwidth, int cheight) {
        nativeUpdate(vwidth, vheight, cwidth, cheight);
    }

    synchronized void setIsDetected(boolean isDetected) {
        nativeSetIsDetected(isDetected);
    }

    @Override
    public void load(GlComponent component) {
        GlComponent.Type type;
        try {
            type = GlComponent.Type.valueOf(component.getType().toUpperCase());
        } catch (Exception e) {
            return;
        }
        nativeLoad(component.getPosition(), type.ordinal(), component.getValue());
    }

    public void release() {
        nativeRelease();
    }

    @Keep
    private static native void nativeInit();

    @Keep
    private static native void nativeSetData(int pEnum, float[] vertexArray, byte[] indiceArray);

    @Keep
    private static native void nativeSetRotateMatrix(float[] RotateMatrix);

    @Keep
    private static native void nativeDraw();

    @Keep
    private static native int getNativeCameraTextureID();

    @Keep
    private static native void nativeUpdate(int vwidth, int vheight, int cwidth, int cheight);

    @Keep
    private static native void nativeLoad(String position, int type, String value);

    @Keep
    private static native void nativeRelease();

    @Keep
    private static native void nativeSetIsDetected(boolean flag);

    private static native void nativeUpdateImageTex(byte[] data, int width, int height);

}
