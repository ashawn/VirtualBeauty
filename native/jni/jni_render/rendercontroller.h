//
// Created by 赖振奇 on 2017/2/22.
//

#ifndef VIRTUALBEAUTY_RENDERCONTROLLER_H
#define VIRTUALBEAUTY_RENDERCONTROLLER_H

#include <jni.h>
#include "cameratexture.h"
#include "texture2d.h"
#include "lipregion.h"
#include "pslevels.h"
#include "texturemultiplycolor.h"
#include "texturemultiplytexture.h"
#include "gaussianblur.h"
#include "psgray.h"
#include "cameralighten.h"
#include "textureoverlay.h"
#include "transform4_3to16_9.h"
#include "bilateralfilter.h"
#include "skincolor.h"
#include "alphatexturecolor.h"
#include "yuv2rgb.h"
#include "yuvsp2rgb.h"
#include "polygoncolor.h"
#include "objmesh.h"

#define RENDERCONTROLLER_METHOD(METHOD_NAME) \
    Java_com_cvte_virtualbeauty_render_RenderController_##METHOD_NAME

#define CAMERA 0
#define LIP 1
#define EYE_LEFT 2
#define EYE_RIGHT 3
#define EYE_BROW_LEFT 4
#define EYE_BROW_RIGHT 5
#define EYE_CONTACT_LEFT 6
#define EYE_CONTACT_RIGHT 7
#define BLUSH_LEFT 8
#define BLUSH_RIGHT 9

#ifdef __cplusplus
extern "C" {
#endif

render::CameraTexture *m_CameraTexture = NULL;
render::YUV2RGB *m_YUV2RGB = NULL;
render::YUVSP2RGB *m_YUVSP2RGB = NULL;
render::Texture2D *m_EyeLash_Left = NULL;
render::Texture2D *m_EyeLash_Right = NULL;
render::Texture2D *m_EyeLine_Left = NULL;
render::Texture2D *m_EyeLine_Right = NULL;
render::AlphaTextureColor *m_EyeShadow_Left = NULL;
render::AlphaTextureColor *m_EyeShadow_Right = NULL;
render::Texture2D *m_EyeBrow_Left = NULL;
render::Texture2D *m_EyeBrow_Right = NULL;
render::AlphaTextureColor *m_Blush_Left = NULL;
render::AlphaTextureColor *m_Blush_Right = NULL;
render::Texture2D *m_EyeContact_Left = NULL;
render::Texture2D *m_EyeContact_Right = NULL;

render::PolygonColor *m_LipColor = NULL;
render::TextureMultiplyTexture *m_TextureMultiplyTexture = NULL;
render::GaussianBlur *m_GaussianBlur = NULL;
render::Transform4_3To16_9 *m_Transform4_3To16_9 = NULL;
render::BilateralFilter *m_BilateralFilter = NULL;
render::SkinColor *m_SkinColor = NULL;

render::OBJMesh *m_Glasses = NULL;

static bool m_Is_Detected = false;
GLuint mFrameTexture, mCameraTexture;

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeInit)(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeSetData)(JNIEnv *env, jclass type, jint pEnum,
                                       jfloatArray vertexArray, jbyteArray indiceArray);

JNIEXPORT void JNICALL
        RENDERCONTROLLER_METHOD(nativeSetRotateMatrix)(JNIEnv *env, jclass type, jfloatArray RotateMatrix);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeDraw)(JNIEnv *env, jclass type);

JNIEXPORT jint JNICALL
RENDERCONTROLLER_METHOD(getNativeCameraTextureID)(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeUpdate)(JNIEnv *env, jclass type, jint vwidth, jint vheight,
                                      jint cwidth, jint cheight);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeLoad)(JNIEnv *env, jclass clazz,jstring position,jint type,jstring value);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeRelease)(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeSetIsDetected)(JNIEnv *env, jclass type, jboolean isDetected);

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeUpdateImageTex)(JNIEnv *env, jclass type, jbyteArray, jint width,
                                              jint height);

GLuint drawCamera();

GLuint drawLip();

#ifdef __cplusplus
}
#endif
#endif //VIRTUALBEAUTY_RENDERCONTROLLER_H
