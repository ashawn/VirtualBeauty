LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC_FILES := \
        src/util/nms.cpp \
        src/util/image_pyramid.cpp \
        src/io/lab_boost_model_reader.cpp \
        src/io/surf_mlp_model_reader.cpp \
        src/feat/lab_feature_map.cpp \
        src/feat/surf_feature_map.cpp \
        src/classifier/lab_boosted_classifier.cpp \
        src/classifier/mlp.cpp \
        src/classifier/surf_mlp.cpp \
        src/face_detection.cpp \
        src/fust.cpp

LOCAL_CPPFLAGS += -pthread -fPIC -fexceptions -frtti -std=c++11 -O2
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(SRC_FILES)
LOCAL_MODULE := facedetection

include $(BUILD_SHARED_LIBRARY)