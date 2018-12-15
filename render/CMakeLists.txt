CMAKE_MINIMUM_REQUIRED(VERSION 3.4.1)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/..)

SET(NATIVE_DIR ../native)
SET(JNI_INCLUDE ${NATIVE_DIR}/jni)
SET(JNI_SRC ${NATIVE_DIR}/jni)
SET(GLOG_DIR ${NATIVE_DIR}/third_party/miniglog)
SET(GLOG_PREBUILT ${GLOG_DIR}/jniLibs)
SET(LIBPNG_DIR ${NATIVE_DIR}/third_party/libpng)
SET(LIBPNG_INCLUDE ${LIBPNG_DIR})
SET(LIBPNG_PREBUILT ${NATIVE_DIR}/../libs)
SET(GLM_DIR ${NATIVE_DIR}/third_party/glm)

ADD_DEFINITIONS("-DDYNAMIC_ES3")
SET(OPENGL_LIB GLESv2)

FILE(GLOB JNI_RENDER_SRC ${JNI_SRC}/jni_render/*.cpp)
FILE(GLOB JNI_RENDER_COMMON_SRC ${JNI_SRC}/jni_render/common/*.cpp)
FILE(GLOB JNI_RENDER_IMAGE_SRC ${JNI_SRC}/jni_render/image/*.cpp)
FILE(GLOB JNI_RENDER_GL_SRC ${JNI_SRC}/jni_render/gl/*.cpp)

INCLUDE_DIRECTORIES(${JNI_INCLUDE} ${LIBPNG_DIR} ${GLOG_DIR} ${GLM_DIR})

ADD_LIBRARY(libpng SHARED IMPORTED)
SET_TARGET_PROPERTIES(libpng PROPERTIES IMPORTED_LOCATION
                    ${PROJECT_DIR}/render/${LIBPNG_PREBUILT}/${ANDROID_ABI}/libpng.so)

ADD_LIBRARY(libminiglog SHARED IMPORTED)
SET_TARGET_PROPERTIES(libminiglog PROPERTIES IMPORTED_LOCATION
                    ${PROJECT_DIR}/render/${GLOG_PREBUILT}/${ANDROID_ABI}/libminiglog.so)

ADD_LIBRARY(glnative SHARED
            ${JNI_RENDER_SRC}
            ${JNI_RENDER_COMMON_SRC}
            ${JNI_RENDER_IMAGE_SRC}
            ${JNI_RENDER_GL_SRC})

TARGET_LINK_LIBRARIES(glnative
                        android
                        log
                        ${OPENGL_LIB}
                        EGL
                        z
                        libpng
                        libminiglog)
