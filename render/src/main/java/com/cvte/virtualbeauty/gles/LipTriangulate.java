/**
 * Copyright © 2013 CVTE. All Rights Reserved.
 */
package com.cvte.virtualbeauty.gles;

import android.graphics.Point;
import android.support.annotation.NonNull;
import android.util.Log;

import org.poly2tri.Poly2Tri;
import org.poly2tri.geometry.polygon.Polygon;
import org.poly2tri.geometry.polygon.PolygonPoint;
import org.poly2tri.triangulation.delaunay.DelaunayTriangle;

import java.util.ArrayList;
import java.util.List;

/**
 * LipTriangulate 获取嘴唇点坐标后的三角化算法
 *
 * @author zhangshuo
 * @since 2017/2/10
 */
public class LipTriangulate {

    private static final String TAG = LipTriangulate.class.getSimpleName();

    public static void process(@NonNull List<Point> srcPoints, @NonNull List<DelaunayTriangle> resultTriangles) {
        if (srcPoints.size() == 0) return;
        List<PolygonPoint> lipPointList = new ArrayList<>();
        for (int i = 0; i < srcPoints.size(); i++) {
            Point p = srcPoints.get(i);
            lipPointList.add(new PolygonPoint(p.x, p.y, i));
        }

        Polygon polygon = new Polygon(lipPointList);
        try {
            Poly2Tri.triangulate(polygon);
        } catch (Exception e) {
            Log.e(TAG, "poly2tri failed");
        }
        List<DelaunayTriangle> triangles = polygon.getTriangles();

        if (triangles.size() != srcPoints.size() - 2) return;
        resultTriangles.addAll(triangles);
    }

    public static void process(@NonNull List<Point> circle, @NonNull List<Point> hole,
                               @NonNull List<DelaunayTriangle> resultTriangles) {
        int offset = 0;
        List<PolygonPoint> circleList = new ArrayList<>();
        for (int i = 0; i < circle.size(); i++) {
            Point p = circle.get(i);
            circleList.add(new PolygonPoint(p.x, p.y, i + offset));
        }
        offset += circle.size();
        List<PolygonPoint> holeList = new ArrayList<>();
        for (int i = 0; i < hole.size(); i++) {
            Point p = hole.get(i);
            holeList.add(new PolygonPoint(p.x, p.y, i + offset));
        }

        Polygon polygon = new Polygon(circleList);
        polygon.addHole(new Polygon(holeList));
        try {
            Poly2Tri.triangulate(polygon);
        } catch (Exception e) {
            Log.e(TAG, "poly2tri failed");
        }
        List<DelaunayTriangle> triangles = polygon.getTriangles();

        if (triangles.size() != circle.size() + hole.size()) return;
        resultTriangles.addAll(triangles);
    }

}
