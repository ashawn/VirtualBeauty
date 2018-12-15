/*
 *     Author : Darren
 * Created on : 06/20 2016
 *
 * Copyright (c) 2016 Darren. All rights reserved.
 */
#include <jni_common/jni_utils.h>
#include <jni_common/jni_primitives.h>
#include <glog/logging.h>
#include <sstream>
#include <unistd.h>
#include <opencv2/core/types.hpp>

// Java Integer/Float

JNI_Integer *g_pJniInteger;
JNI_Float *g_pJniFloat;
JNI_PointF *g_pJniPointF;

JNI_VisionDetRet *g_pJNI_VisionDetRet;
JNI_DetectorRet *g_pJNI_DetectorRet;
cv::Rect *g_Detect_Region;

JavaVM *g_javaVM = NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
            DLOG(INFO) << "JNI_OnLoad enter";
    g_javaVM = vm;
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    // Java Integer/Float
    g_pJniInteger = new JNI_Integer(env);
    g_pJniFloat = new JNI_Float(env);
    g_pJniPointF = new JNI_PointF(env);
    g_pJNI_VisionDetRet = new JNI_VisionDetRet(env);
    g_pJNI_DetectorRet = new JNI_DetectorRet(env);
    g_Detect_Region = new cv::Rect;
            DLOG(INFO) << "JNI_OnLoad exit";
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
            DLOG(INFO) << "JNI_OnUnload enter";
    g_javaVM = NULL;

    // Java Integer/Float
    delete g_pJniInteger;
    delete g_pJniFloat;
    delete g_pJniPointF;
    delete g_pJNI_VisionDetRet;
    delete g_pJNI_DetectorRet;
    delete g_Detect_Region;
            DLOG(INFO) << "JNI_OnUnload exit";
}

namespace jniutils {

    char *convertJStrToCStr(JNIEnv *env, jstring lString) {
        const char *lStringTmp;
        char *pstring;

        lStringTmp = env->GetStringUTFChars(lString, NULL);
        if (lStringTmp == NULL)
            return NULL;

        jsize lStringLength = env->GetStringUTFLength(lString);

        pstring = (char *) malloc(sizeof(char) * (lStringLength + 1));
        strcpy(pstring, lStringTmp);

        env->ReleaseStringUTFChars(lString, lStringTmp);

        return pstring;
    }

    std::string convertJStrToString(JNIEnv *env, jstring lString) {
        const char *lStringTmp;
        std::string str;

        lStringTmp = env->GetStringUTFChars(lString, NULL);
        if (lStringTmp == NULL)
            return NULL;

        str = lStringTmp;

        env->ReleaseStringUTFChars(lString, lStringTmp);

        return str;
    }

    int64_t getTimeNsec() {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return (int64_t) (now.tv_sec * 1000000000LL + now.tv_nsec)/1000000;
    }

    JNIEnv *vm2env(JavaVM *vm) {
        JNIEnv *env = NULL;
        if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
                    DLOG(WARNING) << "vm2env failed";
            env = NULL;
        }
        if (env == NULL) {
                    DLOG(WARNING) << "vm2env get NULL env";
        }
        return env;
    }
}
