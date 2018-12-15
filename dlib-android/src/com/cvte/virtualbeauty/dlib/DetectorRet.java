/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.dlib;

import android.graphics.Point;
import android.support.annotation.Nullable;
import android.util.SparseArray;

import java.util.ArrayList;
import java.util.List;

/**
 * 人脸检测结果
 *
 * @author laizhenqi
 * @since 2017/3/1
 */
public class DetectorRet {

    SparseArray<List<Point>> mSparseArray = new SparseArray<>();

    public static final int TYPE_LIP_OUT = 0;
    public static final int TYPE_LIP_IN = 1;
    public static final int TYPE_EYE_LEFT = 2;
    public static final int TYPE_EYE_RIGHT = 3;
    public static final int TYPE_EYE_BROW_LEFT = 4;
    public static final int TYPE_EYE_BROW_RIGHT = 5;
    public static final int TYPE_EYE_CONTACT_LEFT = 6;
    public static final int TYPE_EYE_CONTACT_RIGHT = 7;
    public static final int TYPE_BLUSH_LEFT = 8;
    public static final int TYPE_BLUSH_RIGHT = 9;
    public static final int TYPE_FACE = 10;

    private long mSpendTime;
    private boolean mDetected;

    private float[] mRotateMatrix = new float[9];

    private DetectorRet() {
        mSparseArray.put(TYPE_LIP_OUT, new ArrayList<Point>());
        mSparseArray.put(TYPE_LIP_IN, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_LEFT, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_RIGHT, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_BROW_LEFT, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_BROW_RIGHT, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_CONTACT_LEFT, new ArrayList<Point>());
        mSparseArray.put(TYPE_EYE_CONTACT_RIGHT, new ArrayList<Point>());
        mSparseArray.put(TYPE_BLUSH_LEFT, new ArrayList<Point>());
        mSparseArray.put(TYPE_BLUSH_RIGHT, new ArrayList<Point>());
        mSparseArray.put(TYPE_FACE, new ArrayList<Point>());
    }

    public boolean addPoint(int pEnum, int x, int y) {
        List<Point> list = mSparseArray.get(pEnum);
        if (list == null) return false;
        return list.add(new Point(x, y));
    }

    @Nullable
    public List<Point> get(int pEnum) {
        return mSparseArray.get(pEnum);
    }

    public boolean isDetected() {
        return mDetected;
    }

    public void setDetected(boolean detected) {
        mDetected = detected;
    }

    public long getSpendTime() {
        return mSpendTime;
    }

    public void setSpendTime(long spendTime) {
        mSpendTime = spendTime;
    }

    public float[] getRotateMatrix() {return mRotateMatrix;}

    public void setRotateMatrixAtPosition(float value,int pos) {
        mRotateMatrix[pos] = value;
    }
}
