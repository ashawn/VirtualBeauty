//
// Created by 赖振奇 on 2017/2/22.
//
#ifndef GLUTILS_H
#define GLUTILS_H

#include <GLES2/gl2.h>
#include <jni_render/image/image_data.h>
#include <jni.h>

namespace glutil {

    GLuint createProgram(char *vertexSource, char *fragmentSource);

    GLuint loadShader(GLenum shaderType, const char *source);

    RawImageData load_png_asset_into_texture(const char * path);

    char *read_string_from_asset(const char *path);

    int64_t getTimeNsec();

    char* jstringTostring(JNIEnv* env, jstring jstr);

    int myatoi(char * str);
}

#endif
