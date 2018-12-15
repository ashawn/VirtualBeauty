//
// Created by 赖振奇 on 2017/3/10.
//

#include <android/asset_manager.h>
#include <assert.h>
#include <android/asset_manager_jni.h>
#include "platform_asset_utils.h"

static AAssetManager *asset_manager;

JNIEXPORT void JNICALL
Java_com_cvte_virtualbeauty_gles_GlUtil_nativeInit(JNIEnv *env, jclass type,
                                                       jobject contextObj) {
    jclass context_class = env->FindClass("android/content/Context");
    jmethodID getassets_method = env->GetMethodID(context_class, "getAssets",
                                                  "()Landroid/content/res/AssetManager;");
    jobject assetmanager_obj = env->CallObjectMethod(contextObj, getassets_method);
    asset_manager = AAssetManager_fromJava(env, assetmanager_obj);
}

FileData get_asset_data(const char *relative_path) {
    assert(relative_path != NULL);
    AAsset *asset =
            AAssetManager_open(asset_manager, relative_path, AASSET_MODE_STREAMING);
    assert(asset != NULL);
    return (FileData) {AAsset_getLength(asset), AAsset_getBuffer(asset), asset};
}

void release_asset_data(const FileData *file_data) {
    assert(file_data != NULL);
    assert(file_data->file_handle != NULL);
    AAsset_close((AAsset *) file_data->file_handle);
}
