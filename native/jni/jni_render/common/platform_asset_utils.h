//
// Created by 赖振奇 on 2017/3/10.
//

#ifndef VIRTUALBEAUTY_PLATFORM_ASSET_UTILS_H
#define VIRTUALBEAUTY_PLATFORM_ASSET_UTILS_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
typedef struct {
    const long data_length;
    const void *data;
    const void *file_handle;
} FileData;


JNIEXPORT void JNICALL
Java_com_cvte_virtualbeauty_gles_GlUtil_nativeInit(JNIEnv *env, jclass type, jobject context);

FileData get_asset_data(const char *relative_path);
void release_asset_data(const FileData *file_data);



#ifdef __cplusplus
}
#endif
#endif //VIRTUALBEAUTY_PLATFORM_ASSET_UTILS_H
