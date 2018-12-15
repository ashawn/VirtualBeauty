//
// Created by 赖振奇 on 2017/2/22.
//
#include <jni_render/image/image_data.h>
#include <jni_render/common/platform_asset_utils.h>
#include <glog/logging.h>
#include <assert.h>
#include "glutils.h"

GLuint glutil::createProgram(char *vertexSource, char *fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        LOG(WARNING) << "create vertex shader failed";
        return 0;
    }
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (pixelShader == 0) {
        LOG(WARNING) << "create pixel shader failed";
        return 0;
    }
    GLuint program = glCreateProgram();
    if (program == 0) {
        LOG(WARNING) << "create program failed";
        return 0;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        LOG(WARNING) << "link status:" << linkStatus;
        LOG(WARNING) << vertexSource;
        LOG(WARNING) << fragmentSource;
        glDeleteProgram(program);
        program = 0;
    }
    free(vertexSource);
    free(fragmentSource);
    return program;
}

GLuint glutil::loadShader(GLenum shaderType, const char *source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG(WARNING) << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog;
    }
    return shader;
}

RawImageData glutil::load_png_asset_into_texture(const char *path) {
    assert(path != NULL);
    const FileData png_file = get_asset_data(path);
    RawImageData rawImageData = get_raw_image_data_from_png(png_file.data,
                                                            png_file.data_length);
    release_asset_data(&png_file);
    return rawImageData;
}

char *glutil::read_string_from_asset(const char *path) {
    const FileData data = get_asset_data(path);
    char *str = (char *)malloc(sizeof(char)*(data.data_length+1));
    memcpy(str,data.data, sizeof(char)*data.data_length);
    str[data.data_length] = '\0';
    release_asset_data(&data);
    return str;
}

int64_t glutil::getTimeNsec() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t) (now.tv_sec * 1000000000LL + now.tv_nsec) / 1000000;
}

char* glutil::jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);

        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

int glutil::myatoi(char * str)
{
    int value = 0;
    int sign = 1;
    int radix;


    if(*str == '-')
    {
        sign = -1;
        str++;
    }
    if(*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
    {
        radix = 16;
        str += 2;
    }
    else if(*str == '0')
    {
        radix = 8;
        str++;
    }
    else
        radix = 10;
    while(*str)
    {
        if(radix == 16)
        {
            if(*str >= '0' && *str <= '9')
                value = value * radix + *str - '0';    //0到9的字符串
            else if(*str >= 'A' && *str <= 'F')
                value = value * radix + *str - 'A' + 10;   //大写十六进制的ABCDEF的字符串的情况
            else if(*str >= 'a' && *str <= 'f')
                value = value * radix + *str - 'a' + 10;  // 小写十六进制的abcdef的字符串的情况
        }
        else
            value = value * radix + *str - '0';
        str++;
    }
    return sign*value;
}

