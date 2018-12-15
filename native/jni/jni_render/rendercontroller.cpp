//
// Created by 赖振奇 on 2017/2/22.
//

#include <jni_render/gl/glutils.h>
#include <jni_render/gl/gl_program_loader.h>
#include <glog/logging.h>
#include "rendercontroller.h"

using namespace render;

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeInit)(JNIEnv *env, jclass type) {
    LOG(INFO) << "init camera texture";
    if (m_CameraTexture == NULL) {
        m_CameraTexture = new CameraTexture;
        m_CameraTexture->init();
        LOG(INFO) << "init camera finish";
    }

    LOG(INFO) << "init YUV2RGB";
    if (m_YUV2RGB == NULL) {
        m_YUV2RGB = new YUV2RGB;
        m_YUV2RGB->init();
        LOG(INFO) << "init YUV2RGB";
    }

    LOG(INFO) << "init YUVSP2RGB";
    if (m_YUVSP2RGB == NULL) {
        m_YUVSP2RGB = new YUVSP2RGB;
        m_YUVSP2RGB->init();
        LOG(INFO) << "init YUVSP2RGB";
    }

    LOG(INFO) << "init eyelash left";
    if (m_EyeLash_Left == NULL) {
        m_EyeLash_Left = new Texture2D;
        m_EyeLash_Left->init();
        m_EyeLash_Left->setTexturePath("test/eyelash.png");
    }

    LOG(INFO) << "init eyelash right";
    if (m_EyeLash_Right == NULL) {
        m_EyeLash_Right = new Texture2D;
        m_EyeLash_Right->init();
        m_EyeLash_Right->setTexturePath("test/eyelash.png");
    }

    LOG(INFO) << "init eyeline left";
    if (m_EyeLine_Left == NULL) {
        m_EyeLine_Left = new Texture2D;
        m_EyeLine_Left->init();
        m_EyeLine_Left->setTexturePath("test/eyeline.png");
    }

    LOG(INFO) << "init eyeline right";
    if (m_EyeLine_Right == NULL) {
        m_EyeLine_Right = new Texture2D;
        m_EyeLine_Right->init();
        m_EyeLine_Right->setTexturePath("test/eyeline.png");
    }

    LOG(INFO) << "init eyeshadow left";
    if (m_EyeShadow_Left == NULL) {
        m_EyeShadow_Left = new AlphaTextureColor;
        m_EyeShadow_Left->init();
        m_EyeShadow_Left->setTexturePath("test/eyeshadow.png");
        m_EyeShadow_Left->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    LOG(INFO) << "init eyeshadow right";
    if (m_EyeShadow_Right == NULL) {
        m_EyeShadow_Right = new AlphaTextureColor;
        m_EyeShadow_Right->init();
        m_EyeShadow_Right->setTexturePath("test/eyeshadow.png");
        m_EyeShadow_Right->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    LOG(INFO) << "init eyebrow left";
    if (m_EyeBrow_Left == NULL) {
        m_EyeBrow_Left = new Texture2D;
        m_EyeBrow_Left->init();
        m_EyeBrow_Left->setTexturePath("test/eyebrow.png");
    }

    LOG(INFO) << "init eyebrow right";
    if (m_EyeBrow_Right == NULL) {
        m_EyeBrow_Right = new Texture2D;
        m_EyeBrow_Right->init();
        m_EyeBrow_Right->setTexturePath("test/eyebrow.png");
    }

    LOG(INFO) << "init eyecontact left";
    if (m_EyeContact_Left == NULL) {
        m_EyeContact_Left = new Texture2D;
        m_EyeContact_Left->init();
        m_EyeContact_Left->setTexturePath("test/eyecontact.png");
    }

    LOG(INFO) << "init eyecontact right";
    if (m_EyeContact_Right == NULL) {
        m_EyeContact_Right = new Texture2D;
        m_EyeContact_Right->init();
        m_EyeContact_Right->setTexturePath("test/eyecontact.png");
    }

    LOG(INFO) << "init blush left";
    if (m_Blush_Left == NULL) {
        m_Blush_Left = new AlphaTextureColor;
        m_Blush_Left->init();
        m_Blush_Left->setTexturePath("test/blush_l.png");
        m_Blush_Left->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        m_Blush_Left->setAlphaFactor(8.0f);
    }

    LOG(INFO) << "init blush right";
    if (m_Blush_Right == NULL) {
        m_Blush_Right = new AlphaTextureColor;
        m_Blush_Right->init();
        m_Blush_Right->setTexturePath("test/blush_r.png");
        m_Blush_Right->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        m_Blush_Right->setAlphaFactor(8.0f);
    }

    LOG(INFO) << "init TextureMultiplyTexture";
    if (m_TextureMultiplyTexture == NULL) {
        m_TextureMultiplyTexture = new TextureMultiplyTexture;
        m_TextureMultiplyTexture->init();
    }

    LOG(INFO) << "init GaussianBlur";
    if (m_GaussianBlur == NULL) {
        m_GaussianBlur = new GaussianBlur;
        m_GaussianBlur->init();
    }

    LOG(INFO) << "init Transform4_3To16_9";
    if (m_Transform4_3To16_9 == NULL) {
        m_Transform4_3To16_9 = new Transform4_3To16_9;
        m_Transform4_3To16_9->init();
    }

    LOG(INFO) << "init BilateralFilter";
    if (m_BilateralFilter == NULL) {
        m_BilateralFilter = new BilateralFilter;
        m_BilateralFilter->init();
    }

    LOG(INFO) << "init SkinColor";
    if (m_SkinColor == NULL) {
        m_SkinColor = new SkinColor;
        m_SkinColor->init();
    }

    LOG(INFO) << "init LipColor";
    if (m_LipColor == NULL) {
        m_LipColor = new PolygonColor;
        m_LipColor->init();
        m_LipColor->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    }

    LOG(INFO) << "init Glasses";
    if (m_Glasses == NULL) {
        m_Glasses = new OBJMesh;
        m_Glasses->init();
        //m_Glasses->setOBJPath("mnt/sdcard/test/glasses3.obj");
    }
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeSetData)(JNIEnv *env, jclass type, jint pEnum,
                                       jfloatArray vertexArray, jbyteArray indiceArray) {
    jfloat *vertex_array = env->GetFloatArrayElements(vertexArray, 0);
    int vertex_len = env->GetArrayLength(vertexArray);
    jbyte *indice_array = env->GetByteArrayElements(indiceArray, 0);
    int indice_len = env->GetArrayLength(indiceArray);
    switch (pEnum) {
        case LIP: {
            if (m_LipColor != NULL)
                m_LipColor->setVertexData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_LEFT: {
            if (m_EyeLash_Left != NULL)
                m_EyeLash_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            if (m_EyeLine_Left != NULL)
                m_EyeLine_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            if (m_EyeShadow_Left != NULL)
                m_EyeShadow_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_RIGHT: {
            if (m_EyeLash_Right != NULL)
                m_EyeLash_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            if (m_EyeLine_Right != NULL)
                m_EyeLine_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            if (m_EyeShadow_Right != NULL)
                m_EyeShadow_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_BROW_LEFT: {
            if (m_EyeBrow_Left != NULL)
                m_EyeBrow_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_BROW_RIGHT: {
            if (m_EyeBrow_Right != NULL)
                m_EyeBrow_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_CONTACT_LEFT: {
            if (m_EyeContact_Left != NULL)
                m_EyeContact_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case EYE_CONTACT_RIGHT: {
            if (m_EyeContact_Right != NULL)
                m_EyeContact_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case BLUSH_LEFT: {
            if (m_Blush_Left != NULL)
                m_Blush_Left->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        case BLUSH_RIGHT: {
            if (m_Blush_Right != NULL)
                m_Blush_Right->setRectData(vertex_array, vertex_len, indice_array, indice_len);
            break;
        }
        default: {
            break;
        }
    }
    env->ReleaseFloatArrayElements(vertexArray, vertex_array, 0);
    env->ReleaseByteArrayElements(indiceArray, indice_array, 0);
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeSetRotateMatrix)(JNIEnv *env, jclass type, jfloatArray RotateMatrix) {
    jfloat *fRotateMatrix = env->GetFloatArrayElements(RotateMatrix, 0);
    int fRotateMatrix_len = env->GetArrayLength(RotateMatrix);

    if (m_Glasses != NULL)
        m_Glasses->setRotateMatrix(fRotateMatrix, fRotateMatrix_len);

    env->ReleaseFloatArrayElements(RotateMatrix, fRotateMatrix, 0);
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeDraw)(JNIEnv *env, jclass type) {
    int64_t t = glutil::getTimeNsec();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_DEPTH_TEST);

    mCameraTexture = drawCamera();

    mFrameTexture = drawLip();

    if (m_Is_Detected) {
        if (m_Transform4_3To16_9 != NULL) {
            m_Transform4_3To16_9->setTextureId(mFrameTexture);
            m_Transform4_3To16_9->draw();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (m_EyeShadow_Left != NULL) {
            m_EyeShadow_Left->draw();
        }
        if (m_EyeShadow_Right != NULL) {
            m_EyeShadow_Right->draw();
        }
        if (m_EyeLash_Left != NULL) {
            m_EyeLash_Left->draw();
        }
        if (m_EyeLash_Right != NULL) {
            m_EyeLash_Right->draw();
        }
        if (m_EyeLine_Left != NULL) {
            m_EyeLine_Left->draw();
        }
        if (m_EyeLine_Right != NULL) {
            m_EyeLine_Right->draw();
        }
//    if (m_EyeBrow_Left != NULL) {
//        m_EyeBrow_Left->draw();
//    }
//    if (m_EyeBrow_Right != NULL) {
//        m_EyeBrow_Right->draw();
//    }
//    if (m_EyeContact_Left != NULL) {
//        m_EyeContact_Left->draw();
//    }
//    if (m_EyeContact_Right != NULL) {
//        m_EyeContact_Right->draw();
//    }
        if (m_Blush_Left != NULL) {
            m_Blush_Left->draw();
        }
        if (m_Blush_Right != NULL) {
            m_Blush_Right->draw();
        }
        glDisable(GL_BLEND);
    } else {
        if (m_Transform4_3To16_9 != NULL) {
            m_Transform4_3To16_9->setTextureId(mCameraTexture);
            m_Transform4_3To16_9->draw();
        }
    }
//    if (m_Transform4_3To16_9 != NULL) {
//        m_Transform4_3To16_9->setTextureId(mCameraTexture);
//        m_Transform4_3To16_9->draw();
//    }
//    if (m_Glasses != NULL) {
//        m_Glasses->draw();
//    }

    LOG(INFO) << "native draw spend time " << glutil::getTimeNsec() - t << " nanotime";
}

JNIEXPORT jint JNICALL
RENDERCONTROLLER_METHOD(getNativeCameraTextureID)(JNIEnv *env, jclass type) {
    if (m_CameraTexture != NULL) {
        return m_CameraTexture->getCameraTextureId();
    }
    return 0;
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeUpdate)(JNIEnv *env, jclass type, jint vwidth, jint vheight,
                                      jint cwidth, jint cheight) {
    if (m_CameraTexture != NULL) {
        m_CameraTexture->update(cwidth, cheight);
    }
    if (m_YUV2RGB != NULL) {
        m_YUV2RGB->update(cwidth, cheight);
    }
    if (m_YUVSP2RGB != NULL) {
        m_YUVSP2RGB->update(cwidth, cheight);
    }
    if (m_BilateralFilter != NULL) {
        m_BilateralFilter->update(cwidth, cheight);
    }
    if (m_SkinColor != NULL) {
        m_SkinColor->update(cwidth, cheight);
    }
    if (m_TextureMultiplyTexture != NULL) {
        m_TextureMultiplyTexture->update(cwidth, cheight);
    }
    if (m_GaussianBlur != NULL) {
        m_GaussianBlur->update(cwidth, cheight);
    }
    if (m_Transform4_3To16_9 != NULL) {
        m_Transform4_3To16_9->update(vwidth, vheight);
    }
    if (m_LipColor != NULL) {
        m_LipColor->update(cwidth, cheight);
    }
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeLoad)(JNIEnv *env, jclass clazz, jstring position, jint type,
                                    jstring value) {
    //position代表部位
    //type为0代表color，1代表texture
    //如果type为0，value为颜色的argb值，如0xffff0000
    //如为1，则value为texture的路径，路径格式为assets://XX/XX，其中scheme表示路径类型
    char* position_c = glutil::jstringTostring(env,position);
    std::string position_s = position_c;
    if(position_s == "mouth"){
        char* value_c = glutil::jstringTostring(env,value);
        int color = glutil::myatoi(value_c);
        float color_r = (float)(((color & 0x00FF0000)>>16)/255.0);
        float color_g = (float)(((color & 0x0000FF00)>>8)/255.0);
        float color_b = (float)(((color & 0x000000FF))/255.0);
        if (m_LipColor != NULL) {
            m_LipColor->setColor(color_r, color_g, color_b, 1);
        }
    }
    else if(position_s == "blush"){
        char* value_c = glutil::jstringTostring(env,value);
        int color = glutil::myatoi(value_c);
        float color_r = (float)(((color & 0x00FF0000)>>16)/255.0);
        float color_g = (float)(((color & 0x0000FF00)>>8)/255.0);
        float color_b = (float)(((color & 0x000000FF))/255.0);
        if (m_Blush_Left != NULL) {
            m_Blush_Left->setColor(color_r, color_g, color_b, 1);
        }
        if (m_Blush_Right != NULL) {
            m_Blush_Right->setColor(color_r, color_g, color_b, 1);
        }
    }
    else if(position_s == "eyelash"){
        char* value_c = glutil::jstringTostring(env,value);
        if (m_EyeLash_Left != NULL) {
            m_EyeLash_Left->setTexturePath(value_c);
        }

        if (m_EyeLash_Right != NULL) {
            m_EyeLash_Right->setTexturePath(value_c);
        }
    }
    else if(position_s == "eyeline"){
        char* value_c = glutil::jstringTostring(env,value);
        if (m_EyeLine_Left != NULL) {
            m_EyeLine_Left->setTexturePath(value_c);
        }

        if (m_EyeLine_Right != NULL) {
            m_EyeLine_Right->setTexturePath(value_c);
        }
    }
    else if(position_s == "eyeshadow"){
        char* value_c = glutil::jstringTostring(env,value);
        if (m_EyeShadow_Left != NULL) {
            m_EyeShadow_Left->setTexturePath(value_c);
        }

        if (m_EyeShadow_Right != NULL) {
            m_EyeShadow_Right->setTexturePath(value_c);
        }
    }
    else if(position_s == "blush_l"){
        char* value_c = glutil::jstringTostring(env,value);
        if (m_Blush_Left != NULL) {
            m_Blush_Left->setTexturePath(value_c);
        }
    }
    else if(position_s == "blush_r"){
        char* value_c = glutil::jstringTostring(env,value);
        if (m_Blush_Right != NULL) {
            m_Blush_Right->setTexturePath(value_c);
        }
    }
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeRelease)(JNIEnv *env, jclass type) {
    if (m_CameraTexture != NULL) {
        m_CameraTexture->release();
        delete m_CameraTexture;
        m_CameraTexture = NULL;
    }
    if (m_YUV2RGB != NULL) {
        m_YUV2RGB->release();
        delete m_YUV2RGB;
        m_YUV2RGB = NULL;
    }
    if (m_YUVSP2RGB != NULL) {
        m_YUVSP2RGB->release();
        delete m_YUVSP2RGB;
        m_YUVSP2RGB = NULL;
    }
    if (m_EyeLash_Left != NULL) {
        m_EyeLash_Left->release();
        delete m_EyeLash_Left;
        m_EyeLash_Left = NULL;
    }
    if (m_EyeLash_Right != NULL) {
        m_EyeLash_Right->release();
        delete m_EyeLash_Right;
        m_EyeLash_Right = NULL;
    }
    if (m_EyeLine_Left != NULL) {
        m_EyeLine_Left->release();
        delete m_EyeLine_Left;
        m_EyeLine_Left = NULL;
    }
    if (m_EyeLine_Right != NULL) {
        m_EyeLine_Right->release();
        delete m_EyeLine_Right;
        m_EyeLine_Right = NULL;
    }
    if (m_EyeShadow_Left != NULL) {
        m_EyeShadow_Left->release();
        delete m_EyeShadow_Left;
        m_EyeShadow_Left = NULL;
    }
    if (m_EyeShadow_Right != NULL) {
        m_EyeShadow_Right->release();
        delete m_EyeShadow_Right;
        m_EyeShadow_Right = NULL;
    }
    if (m_EyeBrow_Left != NULL) {
        m_EyeBrow_Left->release();
        delete m_EyeBrow_Left;
        m_EyeBrow_Left = NULL;
    }
    if (m_EyeBrow_Right != NULL) {
        m_EyeBrow_Right->release();
        delete m_EyeBrow_Right;
        m_EyeBrow_Right = NULL;
    }
    if (m_EyeContact_Left != NULL) {
        m_EyeContact_Left->release();
        delete m_EyeContact_Left;
        m_EyeContact_Left = NULL;
    }
    if (m_EyeContact_Right != NULL) {
        m_EyeContact_Right->release();
        delete m_EyeContact_Right;
        m_EyeContact_Right = NULL;
    }
    if (m_Blush_Left != NULL) {
        m_Blush_Left->release();
        delete m_Blush_Left;
        m_Blush_Left = NULL;
    }
    if (m_Blush_Right != NULL) {
        m_Blush_Right->release();
        delete m_Blush_Right;
        m_Blush_Right = NULL;
    }
    if (m_TextureMultiplyTexture != NULL) {
        m_TextureMultiplyTexture->release();
        delete m_TextureMultiplyTexture;
        m_TextureMultiplyTexture = NULL;
    }
    if (m_GaussianBlur != NULL) {
        m_GaussianBlur->release();
        delete m_GaussianBlur;
        m_GaussianBlur = NULL;
    }
    if (m_Transform4_3To16_9 != NULL) {
        m_Transform4_3To16_9->release();
        delete m_Transform4_3To16_9;
        m_Transform4_3To16_9 = NULL;
    }
    if (m_BilateralFilter != NULL) {
        m_BilateralFilter->release();
        delete m_BilateralFilter;
        m_BilateralFilter = NULL;
    }
    if (m_SkinColor != NULL) {
        m_SkinColor->release();
        delete m_SkinColor;
        m_SkinColor = NULL;
    }
    if (m_LipColor != NULL) {
        m_LipColor->release();
        delete m_LipColor;
        m_LipColor = NULL;
    }
    if (m_Glasses != NULL) {
        m_Glasses->release();
        delete m_Glasses;
        m_Glasses = NULL;
    }
    glutil::release_all_program();
}


JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeSetIsDetected)(JNIEnv *env, jclass type, jboolean isDetected) {
    //修改是否检测到人脸
    m_Is_Detected = isDetected;
}

JNIEXPORT void JNICALL
RENDERCONTROLLER_METHOD(nativeUpdateImageTex)(JNIEnv *env, jclass type, jbyteArray dataArray,
                                              jint width,
                                              jint height) {
    if (m_YUVSP2RGB != NULL) {
        jbyte *data = env->GetByteArrayElements(dataArray, 0);
        jsize length = env->GetArrayLength(dataArray);
        m_YUVSP2RGB->setYUV(data, data + length * 2 / 3, width, height);
    }
}

GLuint drawCamera() {
    if (m_YUVSP2RGB != NULL) {
        m_YUVSP2RGB->draw();
    }

    if (m_BilateralFilter != NULL) {
        m_BilateralFilter->setTextureId(m_YUVSP2RGB->getFrameTextureId());
        m_BilateralFilter->draw();
    }

    if (m_SkinColor != NULL) {
        m_SkinColor->setCameraTextureId(m_YUVSP2RGB->getFrameTextureId());
        m_SkinColor->setTextureId(m_BilateralFilter->getFrameTextureId());
        m_SkinColor->draw();
    }
    return m_SkinColor->getFrameTextureId();
}

GLuint drawLip() {
    if (m_LipColor != NULL) {
        m_LipColor->draw();
    }

    if (m_GaussianBlur != NULL) {
        m_GaussianBlur->setTextureId(m_LipColor->getFrameTextureId());
        m_GaussianBlur->draw();
    }

    if (m_TextureMultiplyTexture != NULL) {
        m_TextureMultiplyTexture->setBaseTextureId(mCameraTexture);
        m_TextureMultiplyTexture->setBlendTextureId(m_GaussianBlur->getFrameTextureId());
        m_TextureMultiplyTexture->draw();
    }

    return m_TextureMultiplyTexture->getFrameTextureId();
}
