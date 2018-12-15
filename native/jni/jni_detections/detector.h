/*
 * detector.h using google-style
 *
 *  Created on: May 24, 2016
 *      Author: cvte
 *
 *  Copyright (c) 2016 cvte. All rights reserved.
 */

#pragma once

#include <jni_common/jni_fileutils.h>
#include <dlib/image_loader/load_image.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_loader/load_image.h>
#include <face_detection.h>
#include <glog/logging.h>
#include <jni.h>
#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

typedef struct LipData {
    std::vector<cv::Point> lip_out;
    std::vector<cv::Point> lip_in;

    void set_lip_value(std::vector<cv::Point> &point_list, int line_idx) {
        std::for_each(point_list.cbegin(), point_list.cend(),
                      [&](const cv::Point &p) -> void {
                          line_idx ? lip_out.push_back(p) : lip_in.push_back(p);
                      });
    }

    std::vector<cv::Point> get_lip_line(int line_idx) {
        return line_idx ? lip_out : lip_in;
    }
} Lip;

typedef struct EyeData {
    std::vector<cv::Point> data[6];

    /**
     * 添加眼睛数据
     * @param point_list 待添加数据列表
     * @param line_idx 0:left_eye;1:right_eye;2:left_eye_brow;3:right_eye_brow;4:left_eye_contact;5:right_eye_contact;
     */
    void set_eye_value(std::vector<cv::Point> &point_list, int line_idx) {
        assert(line_idx < sizeof(data));
        std::for_each(point_list.cbegin(), point_list.cend(),
                      [&](const cv::Point &p) -> void {
                          line_idx ? data[line_idx].push_back(p) : data[line_idx].push_back(p);
                      });
    }

    std::vector<cv::Point> get_eye_line(int line_idx) {
        assert(line_idx < sizeof(data));
        return data[line_idx];
    }
} Eye;

typedef struct FaceData {
    std::vector<cv::Point> data;

    void set_face_data(std::vector<cv::Point> &point_list) {
        std::for_each(point_list.cbegin(), point_list.cend(),
                      [&](const cv::Point &p) -> void {
                          data.push_back(p);
                      });
    }

    std::vector<cv::Point> get_face_data() {
        return data;
    }
} Face;

typedef struct BlushData {
    std::vector<cv::Point> data[2];

    void set_blush_value(std::vector<cv::Point> &point_list, int line_idx) {
        assert(line_idx < sizeof(data));
        std::for_each(point_list.cbegin(), point_list.cend(),
                      [&](const cv::Point &p) -> void {
                          line_idx ? data[line_idx].push_back(p) : data[line_idx].push_back(p);
                      });
    }

    std::vector<cv::Point> get_blush_line(int line_idx) {
        assert(line_idx < sizeof(data));
        return data[line_idx];
    }
} Blush;

class OpencvHOGDetctor {
public:
    OpencvHOGDetctor() {}

    inline int det(const cv::Mat &src_img) {
        if (src_img.empty())
            return 0;

        cv::HOGDescriptor hog;
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
        std::vector<cv::Rect> found, found_filtered;
        hog.detectMultiScale(src_img, found, 0, cv::Size(8, 8), cv::Size(32, 32),
                             1.05, 2);
        size_t i, j;
        for (i = 0; i < found.size(); i++) {
            cv::Rect r = found[i];
            for (j = 0; j < found.size(); j++)
                if (j != i && (r & found[j]) == r)
                    break;
            if (j == found.size())
                found_filtered.push_back(r);
        }

        for (i = 0; i < found_filtered.size(); i++) {
            cv::Rect r = found_filtered[i];
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.06);
            r.height = cvRound(r.height * 0.9);
            cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }
        mResultMat = src_img;
        // cv::imwrite(path, mResultMat);
        LOG(INFO) << "det ends";
        mRets = found_filtered;
        return found_filtered.size();
    }

    inline cv::Mat &getResultMat() { return mResultMat; }

    inline std::vector<cv::Rect> &getResult() { return mRets; }

private:
    cv::Mat mResultMat;
    std::vector<cv::Rect> mRets;
};

class DLibHOGDetector {
private:
    typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;
    dlib::object_detector<image_scanner_type> mObjectDetector;

    inline void init() {
        LOG(INFO) << "Model Path: " << mModelPath;
        if (jniutils::fileExists(mModelPath)) {
            dlib::deserialize(mModelPath) >> mObjectDetector;
        } else {
            LOG(INFO) << "Not exist " << mModelPath;
        }
    }

public:
    DLibHOGDetector(const std::string &modelPath = "/storage/emulated/legacy/person.svm")
            : mModelPath(modelPath) {
        init();
    }

    virtual inline int det(const std::string &path) {
        using namespace jniutils;
        if (!fileExists(mModelPath) || !fileExists(path)) {
            LOG(WARNING) << "No modle path or input file path";
            return 0;
        }
        cv::Mat src_img = cv::imread(path, CV_LOAD_IMAGE_COLOR);
        if (src_img.empty())
            return 0;
        int img_width = src_img.cols;
        int img_height = src_img.rows;
        int im_size_min = MIN(img_width, img_height);
        int im_size_max = MAX(img_width, img_height);

        float scale = float(INPUT_IMG_MIN_SIZE) / float(im_size_min);
        if (scale * im_size_max > INPUT_IMG_MAX_SIZE) {
            scale = (float) INPUT_IMG_MAX_SIZE / (float) im_size_max;
        }

        if (scale != 1.0) {
            cv::Mat outputMat;
            cv::resize(src_img, outputMat,
                       cv::Size(img_width * scale, img_height * scale));
            src_img = outputMat;
        }

        // cv::resize(src_img, src_img, cv::Size(320, 240));
        dlib::cv_image<dlib::bgr_pixel> cimg(src_img);

        double thresh = 0.5;
        mRets = mObjectDetector(cimg, thresh);
        return mRets.size();
    }

    inline std::vector<dlib::rectangle> getResult() { return mRets; }

    virtual ~DLibHOGDetector() {}

protected:
    std::vector<dlib::rectangle> mRets;
    std::string mModelPath;
    const int INPUT_IMG_MAX_SIZE = 600;
    const int INPUT_IMG_MIN_SIZE = 400;
};

/*
 * DLib face detect and face feature extractor
 */
class DLibHOGFaceDetector : public DLibHOGDetector {
private:
    std::string mLandMarkModel;
    dlib::shape_predictor msp;
    dlib::full_object_detection mFaceShape;
    std::vector<dlib::full_object_detection> mFaceShapeMap;
    dlib::frontal_face_detector mDlibFaceDetector;
    std::vector<dlib::correlation_tracker> trackers;
    dlib::rectangle mRet;

    seeta::FaceDetection *mSeetaFaceDetector;
    cv::Rect searchRect;

    long time;

    cv::Mat rotation_vector;
    cv::Mat rotation_matrix;
    cv::Mat translation_vector;
    float matdata[9];

    inline void init() {
        LOG(INFO) << "Init mFaceDetector";
        mDlibFaceDetector = dlib::get_frontal_face_detector();
    }

public:
    DLibHOGFaceDetector() {
        init();
    }

    int mode;

    DLibHOGFaceDetector(const std::string &detectmodel, const std::string &landmarkmodel)
            : mLandMarkModel(landmarkmodel) {
        if (!detectmodel.empty() && jniutils::fileExists(detectmodel)) {
            LOG(INFO) << "Load seeta detect model from " << detectmodel;
//            mSeetaFaceDetector = new seeta::FaceDetection(detectmodel.c_str());
//            mSeetaFaceDetector->SetMinFaceSize(40);
//            mSeetaFaceDetector->SetScoreThresh(2.f);
//            mSeetaFaceDetector->SetImagePyramidScaleFactor(0.25f);
//            mSeetaFaceDetector->SetWindowStep(4, 4);
        } else {
            LOG(ERROR) << "Load seeta detect model failed";
        }
        init();
        if (!mLandMarkModel.empty() && jniutils::fileExists(mLandMarkModel)) {
            dlib::deserialize(landmarkmodel) >> msp;
            LOG(INFO) << "Load landmark model from " << mLandMarkModel;
        } else {
            LOG(ERROR) << "Load landmark modelfailed";
        }
    }

    virtual inline int det(const std::string &path) {
        LOG(INFO) << "Read path from :" << path;
        cv::Mat src_img = cv::imread(path, CV_LOAD_IMAGE_COLOR);
        return det(src_img);
    }

    // The format of mat should be BGR or Gray
    // If converting 4 channels to 3 channls because the format could be BGRA or
    // ARGB
    virtual inline bool det(const cv::Mat &image) {
        if (image.empty())
            return false;
        if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
        }
        LG << "image channels : " << image.channels();
        CHECK(image.channels() == 3);
        // It's unnecessary to use color image for face/landmark detection
        //instead detect with face tracker and downsampling to speed up
        int64_t tmpTime = jniutils::getTimeNsec();

        dlib::cv_image<dlib::bgr_pixel> img(image);
        bool flag = false;
        if (!mode) {
            cv::Mat grayMat;
            cv::cvtColor(image, grayMat, cv::COLOR_BGR2GRAY);

            seeta::ImageData img_data;
            img_data.data = grayMat.data;
            img_data.width = grayMat.cols;
            img_data.height = grayMat.rows;
            img_data.num_channels = 1;

            std::vector<seeta::FaceInfo> faces = mSeetaFaceDetector->Detect(img_data);
            if (faces.size() > 0) {
                seeta::FaceInfo faceInfo = faces[0];
                mRet.set_left(faceInfo.bbox.x);
                mRet.set_top(faceInfo.bbox.y);
                mRet.set_right(faceInfo.bbox.x + faceInfo.bbox.width);
                mRet.set_bottom(faceInfo.bbox.y + faceInfo.bbox.height);
                searchRect.x = faceInfo.bbox.x;
                searchRect.y = faceInfo.bbox.y;
                searchRect.width = faceInfo.bbox.width;
                searchRect.height = faceInfo.bbox.height;
            } else {
                flag = true;
            }
        } else {
            mRet = face_track_and_downsampling(image);
//            std::vector<dlib::rectangle> rects = mDlibFaceDetector(img);
//            if (rects.size() > 0) mRet = rects[0];
//            else {
            if(mRet.area()<=0)
                flag = true;
//            }
        }

        LG << "face detect spend time:" << jniutils::getTimeNsec() - tmpTime;
        time = (long) (jniutils::getTimeNsec() - tmpTime);
        tmpTime = jniutils::getTimeNsec();
        // Process shape
        if (!flag && !mRet.is_empty() && !mLandMarkModel.empty()) {
            mFaceShape = msp(img, mRet);
            LG << " face landmark detect spend time:" << jniutils::getTimeNsec() - tmpTime;
            time += jniutils::getTimeNsec() - tmpTime;
            FacesolvePnP(image,mFaceShape);
            return true;
        }
        LG << "face detect failed";
        return false;
    }

    inline long getSpendTime() { return time; }

    dlib::full_object_detection &getFaceShape() {
        return mFaceShape;
    }

    std::vector<dlib::full_object_detection> &getFaceShapeMap() {
        return mFaceShapeMap;
    }

    Lip get_lip(int face_idx = 0) {
        Lip l;
        dlib::full_object_detection shape = mFaceShape;

        std::vector<cv::Point> tmp;
        for (unsigned long i = 48; i <= 59; i++)
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        l.set_lip_value(tmp, 0);

        tmp.clear();
        for (unsigned long i = 60; i <= 67; i++) {
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        }
        l.set_lip_value(tmp, 1);
        return l;
    }

    Eye get_eye(int idx = 0) {
        dlib::full_object_detection shape = mFaceShape;
        Eye e;
        std::vector<cv::Point> tmp;
        for (unsigned long i = 36; i < 42; i++)
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        e.set_eye_value(tmp, 0);

        tmp.clear();
        for (unsigned long i = 42; i < 48; i++)
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        e.set_eye_value(tmp, 1);

        tmp.clear();
        for (unsigned long i = 17; i < 22; i++)
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        e.set_eye_value(tmp, 2);

        tmp.clear();
        for (unsigned long i = 22; i < 27; i++)
            tmp.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        e.set_eye_value(tmp, 3);

        tmp.clear();
        tmp.push_back(cv::Point(shape.part(37).x(), shape.part(37).y()));
        tmp.push_back(cv::Point(shape.part(38).x(), shape.part(38).y()));
        tmp.push_back(cv::Point(shape.part(41).x(), shape.part(41).y()));
        tmp.push_back(cv::Point(shape.part(40).x(), shape.part(40).y()));
        e.set_eye_value(tmp, 4);

        tmp.clear();
        tmp.push_back(cv::Point(shape.part(43).x(), shape.part(43).y()));
        tmp.push_back(cv::Point(shape.part(44).x(), shape.part(44).y()));
        tmp.push_back(cv::Point(shape.part(47).x(), shape.part(47).y()));
        tmp.push_back(cv::Point(shape.part(46).x(), shape.part(46).y()));
        e.set_eye_value(tmp, 5);

        return e;
    }

    Face get_face() {
        Face f;
        std::vector<cv::Point> tmp;
        tmp.push_back(cv::Point(mRet.left(), mRet.top()));
        tmp.push_back(cv::Point(mRet.right(), mRet.top()));
        tmp.push_back(cv::Point(mRet.right(), mRet.bottom()));
        tmp.push_back(cv::Point(mRet.left(), mRet.bottom()));
        f.set_face_data(tmp);
        return f;
    }

    Blush get_blush(int idx = 0) {
        dlib::full_object_detection shape = mFaceShape;
        Blush e;
        std::vector<cv::Point> tmp;
        tmp.push_back(cv::Point(shape.part(0).x(), shape.part(0).y()));
        tmp.push_back(cv::Point(shape.part(7).x(), shape.part(0).y()));
        tmp.push_back(cv::Point(shape.part(0).x(), shape.part(7).y()));
        tmp.push_back(cv::Point(shape.part(7).x(), shape.part(7).y()));
        e.set_blush_value(tmp, 0);

        tmp.clear();
        tmp.push_back(cv::Point(shape.part(9).x(), shape.part(16).y()));
        tmp.push_back(cv::Point(shape.part(16).x(), shape.part(16).y()));
        tmp.push_back(cv::Point(shape.part(9).x(), shape.part(9).y()));
        tmp.push_back(cv::Point(shape.part(16).x(), shape.part(9).y()));
        e.set_blush_value(tmp, 1);

        return e;
    }

    std::vector<cv::Point3d> get_3d_model_points()
    {
        std::vector<cv::Point3d> modelPoints;

        modelPoints.push_back(cv::Point3d(0.0f, 0.0f, 0.0f)); //The first must be (0,0,0) while using POSIT
        modelPoints.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));
        modelPoints.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));
        modelPoints.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));
        modelPoints.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));
        modelPoints.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));

        return modelPoints;

    }

    std::vector<cv::Point2d> get_2d_image_points(dlib::full_object_detection &d)
    {
        std::vector<cv::Point2d> image_points;
        image_points.push_back( cv::Point2d( d.part(30).x(), d.part(30).y() ) );    // Nose tip
        image_points.push_back( cv::Point2d( d.part(8).x(), d.part(8).y() ) );      // Chin
        image_points.push_back( cv::Point2d( d.part(36).x(), d.part(36).y() ) );    // Left eye left corner
        image_points.push_back( cv::Point2d( d.part(45).x(), d.part(45).y() ) );    // Right eye right corner
        image_points.push_back( cv::Point2d( d.part(48).x(), d.part(48).y() ) );    // Left Mouth corner
        image_points.push_back( cv::Point2d( d.part(54).x(), d.part(54).y() ) );    // Right mouth corner
        return image_points;

    }

    cv::Mat get_camera_matrix(float focal_length, cv::Point2d center)
    {
        cv::Mat camera_matrix = (cv::Mat_<float>(3,3) << focal_length, 0.0f, center.x, 0.0f , focal_length, center.y, 0.0f, 0.0f, 1.0f);

        return camera_matrix;
    }

    void FacesolvePnP(cv::Mat im,dlib::full_object_detection &d)
    {
        std::vector<cv::Point3d> model_points = get_3d_model_points();
        std::vector<cv::Point2d> image_points = get_2d_image_points(d);
        double focal_length = im.cols;
        cv::Mat camera_matrix = get_camera_matrix(focal_length, cv::Point2d(im.cols/2,im.rows/2));
        cv::Mat dist_coeffs = cv::Mat::zeros(4,1,cv::DataType<double>::type);

        cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
        cv::Rodrigues(rotation_vector, rotation_matrix);
    }


    float* get_face_rotate_matrix(int &len)
    {
        len = 9;
        matdata[0] = rotation_matrix.at<float>(0, 0);
        matdata[1] = rotation_matrix.at<float>(0, 1);
        matdata[2] = rotation_matrix.at<float>(0, 2);
        matdata[3] = rotation_matrix.at<float>(1, 0);
        matdata[4] = rotation_matrix.at<float>(1, 1);
        matdata[5] = rotation_matrix.at<float>(1, 2);
        matdata[6] = rotation_matrix.at<float>(2, 0);
        matdata[7] = rotation_matrix.at<float>(2, 1);
        matdata[8] = rotation_matrix.at<float>(2, 2);

        return matdata;
    }

    dlib::rectangle face_track_and_downsampling(const cv::Mat &image,
                                                int downsampling_scale = 2,
                                                double tracker_update_threshold = 10.) {
        dlib::rectangle rect;
        std::vector<dlib::rectangle> mRets_tmp;
        if (trackers.size() < 1) {
            dlib::cv_image<dlib::bgr_pixel> img_src(image);
            if (downsampling_scale > 1) {
                cv::Mat outputMat;
                cv::pyrDown(image, outputMat, cv::Size(image.cols / downsampling_scale,
                                                       image.rows/ downsampling_scale));
                dlib::cv_image<dlib::bgr_pixel> img(outputMat);
                mRets_tmp = mDlibFaceDetector(img);
            } else
                mRets_tmp = mDlibFaceDetector(img_src);

            if (mRets_tmp.size() > 0) {
                int left = mRets_tmp[0].left() * downsampling_scale;
                int top = mRets_tmp[0].top() * downsampling_scale;
                int right = mRets_tmp[0].right() * downsampling_scale;
                int bottom = mRets_tmp[0].bottom() * downsampling_scale;
                rect = dlib::rectangle(left, top, right, bottom);
                dlib::correlation_tracker tracker;
                tracker.start_track(img_src, rect);
                trackers.push_back(tracker);
            }
        } else {
            dlib::cv_image<dlib::bgr_pixel> img(image);
            double result = trackers[0].update(img);
            if (result - tracker_update_threshold > 0.000001)
                rect = trackers[0].get_position();
            else
                trackers.erase(trackers.begin());
        }
        return rect;
    }
};

