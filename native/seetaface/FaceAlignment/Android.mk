LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
SRC_FILES := \
            src/cfan.cpp \
            src/face_alignment.cpp \
            src/sift.cpp

LOCAL_CPPFLAGS += -pthread -fPIC -fexceptions -frtti -std=c++11
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(SRC_FILES)
LOCAL_MODULE := facealignment

include $(BUILD_SHARED_LIBRARY)