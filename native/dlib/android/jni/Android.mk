LOCAL_PATH := $(call my-dir)
# C++ full library
# =======================================================
## Import JNI
include $(CLEAR_VARS)

LOCAL_MODULE := dlib

LOCAL_EXPORT_C_INCLUDES := ${LOCAL_PATH}/../..
LOCAL_C_INCLUDES := ${LOCAL_PATH}/../..

LIBDLIB_SRC_FILES := ../../dlib/base64/base64_kernel_1.cpp \
                    ../../dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
                    ../../dlib/threads/threads_kernel_shared.cpp \
                    ../../dlib/threads/threads_kernel_1.cpp \
                    ../../dlib/threads/threads_kernel_2.cpp \

LOCAL_SRC_FILES += $(LIBDLIB_SRC_FILES)

LOCAL_CPPFLAGS += -pthread -fPIC -fexceptions -frtti -std=c++11

TARGET_OUT='jniLibs/$(TARGET_ARCH_ABI)'

include $(BUILD_SHARED_LIBRARY)
