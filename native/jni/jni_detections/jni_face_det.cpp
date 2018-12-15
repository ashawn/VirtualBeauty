/*
 * jni_pedestrian_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: cvte
 *
 *  Copyright (c) 2015 cvte. All rights reserved.
 */
#include <android/bitmap.h>
#include <jni_common/jni_bitmap2mat.h>
#include <jni_common/jni_primitives.h>
#include <jni_common/jni_fileutils.h>
#include <jni_common/jni_utils.h>
#include <detector.h>


using namespace cv;

extern JNI_VisionDetRet *g_pJNI_VisionDetRet;
extern JNI_DetectorRet *g_pJNI_DetectorRet;
extern cv::Rect *g_Detect_Region;

namespace {

#define JAVA_NULL 0
    using DetectorPtr = DLibHOGFaceDetector *;

    class JNI_FaceDet {
    public:
        JNI_FaceDet(JNIEnv *env) {
            jclass clazz = env->FindClass(CLASSNAME_FACE_DET);
            mNativeContext = env->GetFieldID(clazz, "mNativeFaceDetContext", "J");
            env->DeleteLocalRef(clazz);
        }

        DetectorPtr getDetectorPtrFromJava(JNIEnv *env, jobject thiz) {
            DetectorPtr const p = (DetectorPtr) env->GetLongField(thiz, mNativeContext);
            return p;
        }

        void setDetectorPtrToJava(JNIEnv *env, jobject thiz, jlong ptr) {
            env->SetLongField(thiz, mNativeContext, ptr);
        }

        jfieldID mNativeContext;
    };

// Protect getting/setting and creating/deleting pointer between java/native
    std::mutex gLock;

    std::shared_ptr<JNI_FaceDet> getJNI_FaceDet(JNIEnv *env) {
        static std::once_flag sOnceInitflag;
        static std::shared_ptr<JNI_FaceDet> sJNI_FaceDet;
        std::call_once(sOnceInitflag, [env]() {
            sJNI_FaceDet = std::make_shared<JNI_FaceDet>(env);
        });
        return sJNI_FaceDet;
    }

    DetectorPtr const getDetectorPtr(JNIEnv *env, jobject thiz) {
        std::lock_guard<std::mutex> lock(gLock);
        return getJNI_FaceDet(env)->getDetectorPtrFromJava(env, thiz);
    }

// The function to set a pointer to java and delete it if newPtr is empty
    void setDetectorPtr(JNIEnv *env, jobject thiz, DetectorPtr newPtr) {
        std::lock_guard<std::mutex> lock(gLock);
        DetectorPtr oldPtr = getJNI_FaceDet(env)->getDetectorPtrFromJava(env, thiz);
        if (oldPtr != JAVA_NULL) {
            LG << "setMapManager delete old ptr : " << oldPtr;
            delete oldPtr;
        }

        if (newPtr != JAVA_NULL) {
            LG << "setMapManager set new ptr : " << newPtr;
        }

        getJNI_FaceDet(env)->setDetectorPtrToJava(env, thiz, (jlong) newPtr);
    }

    jobject detect(JNIEnv *env,jobject thiz,Mat bgr) {
        jobject jDetectorRet = JNI_DetectorRet::createJObject(env);
        //if (g_Detect_Region->area() <= 0) {
            LOG(WARNING) << "NOT SET DETECT REGION !";
            g_Detect_Region->x = 0;
            g_Detect_Region->y = 0;
            g_Detect_Region->width = bgr.cols;
            g_Detect_Region->height = bgr.rows;
        //}
        cv::Point offset = cv::Point(g_Detect_Region->x, g_Detect_Region->y);
        cv::Mat cropMat = bgr(*g_Detect_Region);
        DetectorPtr detPtr = getDetectorPtr(env, thiz);
        if (!detPtr->det(cropMat)) {
            LOG(WARNING) << "det face failed";
            g_pJNI_DetectorRet->setDetected(env, jDetectorRet, false);
            g_pJNI_DetectorRet->setSpendTime(env, jDetectorRet, 0);
            return jDetectorRet;
        }
        g_pJNI_DetectorRet->setDetected(env, jDetectorRet, true);
        g_pJNI_DetectorRet->setSpendTime(env, jDetectorRet, detPtr->getSpendTime());
        int matrixLen ;
        float* matrix = detPtr->get_face_rotate_matrix(matrixLen);
        for (int i = 0; i < matrixLen; ++i) {
            g_pJNI_DetectorRet->setRotateMatrixAtPosition(env,jDetectorRet,matrix[i],i);
        }
        Lip l = detPtr->get_lip(0);
        Eye e = detPtr->get_eye(0);
        Blush b = detPtr->get_blush(0);
        //写入外嘴唇
        std::vector<cv::Point> line0 = l.get_lip_line(0);
        for (int i = 0; i < line0.size(); i++) {
            int x = line0[i].x + offset.x;
            int y = line0[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 0, x, y);
        }
        //写入内嘴唇
        std::vector<cv::Point> line1 = l.get_lip_line(1);
        for (int i = 0; i < line1.size(); i++) {
            int x = line1[i].x + offset.x;
            int y = line1[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 1, x, y);
        }
        // 写入左眼
        std::vector<cv::Point> line2 = e.get_eye_line(0);
        for (int i = 0; i < line2.size(); i++) {
            int x = line2[i].x + offset.x;
            int y = line2[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 2, x, y);
        }
        //写入右眼
        std::vector<cv::Point> line3 = e.get_eye_line(1);
        for (int i = 0; i < line3.size(); i++) {
            int x = line3[i].x + offset.x;
            int y = line3[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 3, x, y);
        }
        //写入左眉
        std::vector<cv::Point> line4 = e.get_eye_line(2);
        for (int i = 0; i < line4.size(); i++) {
            int x = line4[i].x + offset.x;
            int y = line4[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 4, x, y);
        }
        //写入右眉
        std::vector<cv::Point> line5 = e.get_eye_line(3);
        for (int i = 0; i < line5.size(); i++) {
            int x = line5[i].x + offset.x;
            int y = line5[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 5, x, y);
        }
        //写入左眼珠
        std::vector<cv::Point> line6 = e.get_eye_line(4);
        for (int i = 0; i < line6.size(); i++) {
            int x = line6[i].x + offset.x;
            int y = line6[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 6, x, y);
        }
        //写入右眼珠
        std::vector<cv::Point> line7 = e.get_eye_line(5);
        for (int i = 0; i < line7.size(); i++) {
            int x = line7[i].x + offset.x;
            int y = line7[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 7, x, y);
        }
        //写入左脸
        std::vector<cv::Point> line8 = b.get_blush_line(0);
        for (int i = 0; i < line8.size(); i++) {
            int x = line8[i].x + offset.x;
            int y = line8[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 8, x, y);
        }
        //写入右脸
        std::vector<cv::Point> line9 = b.get_blush_line(1);
        for (int i = 0; i < line9.size(); i++) {
            int x = line9[i].x + offset.x;
            int y = line9[i].y + offset.y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 9, x, y);
        }
        //写入脸部
        line0 = detPtr->get_face().get_face_data();
        for (int i = 0; i < line0.size(); i++) {
            int x = line0[i].x;
            int y = line0[i].y;
            g_pJNI_DetectorRet->addPoint(env, jDetectorRet, 10, x, y);
        }
        return jDetectorRet;
    }

}  // end unnamespace

#ifdef __cplusplus
extern "C" {
#endif


#define DLIB_FACE_JNI_METHOD(METHOD_NAME) \
  Java_com_cvte_virtualbeauty_dlib_FaceDet_##METHOD_NAME

void JNIEXPORT
DLIB_FACE_JNI_METHOD(jniNativeClassInit)(JNIEnv *env, jclass _this) {}

jobjectArray getDetectResult(JNIEnv *env, DetectorPtr faceDetector,
                             const int &size) {
    LG << "getFaceRet";
    jobjectArray jDetRetArray = JNI_VisionDetRet::createJObjectArray(env, size);
    for (int i = 0; i < size; i++) {
        jobject jDetRet = JNI_VisionDetRet::createJObject(env);
        env->SetObjectArrayElement(jDetRetArray, i, jDetRet);
        dlib::rectangle rect = faceDetector->getResult()[i];
        g_pJNI_VisionDetRet->setRect(env, jDetRet, rect.left(), rect.top(),
                                     rect.right(), rect.bottom());
        g_pJNI_VisionDetRet->setLabel(env, jDetRet, "face");
        std::vector<dlib::full_object_detection> &faceShapeMap =
                faceDetector->getFaceShapeMap();
        if (i < faceShapeMap.size()) {
            dlib::full_object_detection shape = faceShapeMap[i];
            for (unsigned long j = 0; j < shape.num_parts(); j++) {
                int x = shape.part(j).x();
                int y = shape.part(j).y();
                // Call addLandmark
                g_pJNI_VisionDetRet->addLandmark(env, jDetRet, x, y);
            }
        }
    }
    return jDetRetArray;
}

JNIEXPORT jobjectArray JNICALL
DLIB_FACE_JNI_METHOD(jniDetect)(JNIEnv *env, jobject thiz,
                                jstring imgPath) {
    LOG(INFO) << "jniFaceDet";
    const char *img_path = env->GetStringUTFChars(imgPath, 0);
    DetectorPtr detPtr = getDetectorPtr(env, thiz);
    int size = detPtr->det(std::string(img_path));
    env->ReleaseStringUTFChars(imgPath, img_path);
    LOG(INFO) << "det face size: " << size;
    return getDetectResult(env, detPtr, size);
}

JNIEXPORT jobjectArray JNICALL
DLIB_FACE_JNI_METHOD(jniBitmapDetect)(JNIEnv *env, jobject thiz,
                                      jobject bitmap) {
    LOG(INFO) << "jniBitmapFaceDet";
    cv::Mat rgbaMat;
    cv::Mat bgrMat;
    jniutils::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
    cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
    DetectorPtr detPtr = getDetectorPtr(env, thiz);
    jint size = detPtr->det(bgrMat);
#if 0
    cv::Mat rgbMat;
    cv::cvtColor(bgrMat, rgbMat, cv::COLOR_BGR2RGB);
    cv::imwrite("/sdcard/ret.jpg", rgbaMat);
#endif
    LG << "det face size: " << size;
    return getDetectResult(env, detPtr, size);
}

JNIEXPORT jobject JNICALL
DLIB_FACE_JNI_METHOD(jniDetectBitmapWithResult)(JNIEnv *env, jobject thiz,
                                          jobject bitmap) {
    LOG(INFO) << "jniDetectWithResult";
    cv::Mat rgbaMat;
    cv::Mat bgrMat;
    jniutils::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
    cv::cvtColor(rgbaMat,bgrMat,cv::COLOR_RGBA2BGR);
    return detect(env,thiz,bgrMat);
}

JNIEXPORT jobject JNICALL
DLIB_FACE_JNI_METHOD(jniDetectMatWithResult)(JNIEnv *env, jobject thiz,
                                                jlong matAddress) {
    return detect(env,thiz,*(Mat *)(matAddress));

}
JNIEXPORT void JNICALL
DLIB_FACE_JNI_METHOD(naiveSetDetectRegion)(JNIEnv *env, jobject thiz,
                                           jint left, jint top, jint right, jint bottom) {
    g_Detect_Region->x = left;
    g_Detect_Region->y = top;
    g_Detect_Region->width = right - left;
    g_Detect_Region->height = bottom - top;
}

JNIEXPORT void JNICALL
DLIB_FACE_JNI_METHOD(jniSetMode)(JNIEnv *env, jobject thiz, jint mode) {
    DetectorPtr det = getDetectorPtr(env,thiz);
    det->mode = mode;
}

jint JNIEXPORT JNICALL DLIB_FACE_JNI_METHOD(jniInit)(JNIEnv *env, jobject thiz,
                                                     jstring jDetectPath, jstring jLandmarkPath) {
    std::string detectpath = jniutils::convertJStrToString(env, jDetectPath);
    std::string landmarkPath = jniutils::convertJStrToString(env, jLandmarkPath);
    DetectorPtr detPtr = new DLibHOGFaceDetector(detectpath, landmarkPath);
    setDetectorPtr(env, thiz, detPtr);;
    return JNI_OK;
}

jint JNIEXPORT JNICALL
DLIB_FACE_JNI_METHOD(jniDeInit)(JNIEnv *env, jobject thiz) {
    LOG(INFO) << "jniDeInit";
    setDetectorPtr(env, thiz, JAVA_NULL);
    return JNI_OK;
}

#ifdef __cplusplus
}
#endif
