cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/..)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

## Define each subfolders
set(NATIVE_DIR ../native)
set(JNI_DETECTION_INCLUDE ${NATIVE_DIR}/jni/jni_detections)
set(JNI_DETECTION_SRC ${NATIVE_DIR}/jni/jni_detections)
set(JNI_COMMON_INCLUDE ${NATIVE_DIR}/jni)
set(JNI_COMMON_SRC ${NATIVE_DIR}/jni/jni_common)
set(DLIB_DIR ${NATIVE_DIR}/dlib)
set(DLIB_PREBUILT ${NATIVE_DIR}/../libs)
set(EXT_DIR ${NATIVE_DIR}/third_party)
set(GLOG_INCLUDE_DIR ${EXT_DIR}/miniglog)
SET(GLOG_PREBUILT ${NATIVE_DIR}/../libs)
set(EIGEN_INCLUDE_DIR ${EXT_DIR}/eigen3)
SET(SEETA_DIR ${NATIVE_DIR}/seetaface)
SET(SEETA_DETECTION_INCLUDE ${SEETA_DIR}/FaceDetection/include)
SET(SEETA_DETECTION_PREBUILT ${NATIVE_DIR}/../libs)

set(OpenCV_INCLUDE_DIRS ${EXT_DIR}/opencv/jni/include)
set(OPENCV_PREBUILT ${EXT_DIR}/opencv/libs)

# Include headers
include_directories(${DLIB_DIR} ${EIGEN_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS}
                    ${GLOG_INCLUDE_DIR} ${JNI_COMMON_INCLUDE} ${JNI_DETECTION_INCLUDE}
                    ${SEETA_DETECTION_INCLUDE} include)

add_library(android_dlib SHARED
            ${JNI_DETECTION_SRC}/jni_face_det.cpp
            ${JNI_DETECTION_SRC}/jni_imageutils.cpp
            ${JNI_DETECTION_SRC}/jni_pedestrian_det.cpp
            ${JNI_DETECTION_SRC}/ThinPlateSpline.cpp
            ${JNI_COMMON_SRC}/jni_bitmap2mat.cpp
            ${JNI_COMMON_SRC}/jni_fileutils.cpp
            ${JNI_COMMON_SRC}/jni_utils.cpp
            ${JNI_COMMON_SRC}/rgb2yuv.cpp
            ${JNI_COMMON_SRC}/yuv2rgb.cpp)

#add glog library
ADD_LIBRARY(libminiglog SHARED IMPORTED)
SET_TARGET_PROPERTIES(libminiglog  PROPERTIES IMPORTED_LOCATION
                    ${PROJECT_DIR}/dlib-android/${GLOG_PREBUILT}/${ANDROID_ABI}/libminiglog.so)

#add dlib library
ADD_LIBRARY(libdlib SHARED IMPORTED)
SET_TARGET_PROPERTIES(libdlib  PROPERTIES IMPORTED_LOCATION
                    ${PROJECT_DIR}/dlib-android/${DLIB_PREBUILT}/${ANDROID_ABI}/libdlib.so)

#add seeta-facedetection library
ADD_LIBRARY(libfacedetection SHARED IMPORTED)
SET_TARGET_PROPERTIES(libfacedetection  PROPERTIES IMPORTED_LOCATION
                    ${PROJECT_DIR}/dlib-android/${SEETA_DETECTION_PREBUILT}/${ANDROID_ABI}/libfacedetection.so)

#add opencv library
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_java3.so)

#add_library(libopencv_calib3d STATIC IMPORTED )
#set_target_properties(libopencv_calib3d PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_calib3d.a)

#add_library(libopencv_core STATIC IMPORTED )
#set_target_properties(libopencv_core PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_core.a)

#add_library(libopencv_features2d STATIC IMPORTED )
#set_target_properties(libopencv_features2d PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_features2d.a)

#add_library(libopencv_flann STATIC IMPORTED )
#set_target_properties(libopencv_flann PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_flann.a)

#add_library(libopencv_highgui STATIC IMPORTED )
#set_target_properties(libopencv_highgui PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_highgui.a)

#add_library(libopencv_imgcodecs STATIC IMPORTED )
#set_target_properties(libopencv_imgcodecs PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_imgcodecs.a)

#add_library(libopencv_imgproc STATIC IMPORTED )
#set_target_properties(libopencv_imgproc PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_imgproc.a)

#add_library(libopencv_ml STATIC IMPORTED )
#set_target_properties(libopencv_ml PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_ml.a)

#add_library(libopencv_objdetect STATIC IMPORTED )
#set_target_properties(libopencv_objdetect PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_objdetect.a)

#add_library(libopencv_photo STATIC IMPORTED )
#set_target_properties(libopencv_photo PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_photo.a)

#add_library(libopencv_shape STATIC IMPORTED )
#set_target_properties(libopencv_shape PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_shape.a)

#add_library(libopencv_stitching STATIC IMPORTED )
#set_target_properties(libopencv_stitching PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_stitching.a)

#add_library(libopencv_superres STATIC IMPORTED )
#set_target_properties(libopencv_superres PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_superres.a)

#add_library(libopencv_video STATIC IMPORTED )
#set_target_properties(libopencv_video PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_video.a)

#add_library(libopencv_videoio STATIC IMPORTED )
#set_target_properties(libopencv_videoio PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_videoio.a)

#add_library(libopencv_videostab STATIC IMPORTED )
#set_target_properties(libopencv_videostab PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_videostab.a)

#add_library(libopencv_ts STATIC IMPORTED )
#set_target_properties(libopencv_ts PROPERTIES IMPORTED_LOCATION ${PROJECT_DIR}/dlib-android/${OPENCV_PREBUILT}/${ANDROID_ABI}/libopencv_ts.a)

target_link_libraries(android_dlib
                      android
                      m
                      log
                      dl
                      z
                      jnigraphics
                      libminiglog
                      libdlib
                      libfacedetection
                      lib_opencv #libopencv_calib3d libopencv_core libopencv_features2d libopencv_flann
                      #libopencv_highgui libopencv_imgcodecs libopencv_imgproc libopencv_ml
                      #libopencv_objdetect libopencv_photo libopencv_shape libopencv_stitching
                      #libopencv_superres libopencv_video libopencv_videoio libopencv_videostab
                      )