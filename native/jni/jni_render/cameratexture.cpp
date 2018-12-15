//
// Created by 赖振奇 on 2017/2/22.
//

#include <GLES2/gl2ext.h>
#include <jni_render/gl/gl_program_loader.h>
#include <glog/logging.h>
#include "cameratexture.h"

using namespace render;

void CameraTexture::init() {
    glGenBuffers(1, &m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CAMERA_VERTEX_DATA)* sizeof(float),
                 CAMERA_VERTEX_DATA, GL_STATIC_DRAW);

    glGenBuffers(1, &m_Index_GL_BUFF);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CAMERA_INDICES_DATA)* sizeof(GLubyte), CAMERA_INDICES_DATA,
                 GL_STATIC_DRAW);

    glGenTextures(1, &m_GL_TEXTURE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_GL_TEXTURE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &m_Frame_GL_Buffer);

    glGenTextures(1, &mFrameTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

    m_Program = glutil::get_program_by_type(RAW_TEXTURE2D_IMAGE);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");
    m_CameraTex_Handle = glGetUniformLocation(m_Program, "cameraTex");

    mWidth = 0;
    mHeight = 0;
}

GLuint CameraTexture::getCameraTextureId() {
    return m_GL_TEXTURE;
}

GLuint CameraTexture::getFrameTextureId() {
    return mFrameTexture;
}

void CameraTexture::draw() {
    LOG(INFO) << "CameraTexture draw";

    glUseProgram(m_Program);

    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,mWidth,mHeight);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_GL_TEXTURE);
    glUniform1i(m_CameraTex_Handle,0);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, sizeof(CAMERA_INDICES_DATA), GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glDisableVertexAttribArray(m_TexCoord_Handle);

    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glUseProgram(0);
}


void CameraTexture::loadShader(GLuint program) {
    m_Program = program;
}


void CameraTexture::update(int width, int height) {
    mWidth = width;
    mHeight = height;
    glBindFramebuffer(GL_FRAMEBUFFER, m_Frame_GL_Buffer);
    glBindTexture(GL_TEXTURE_2D,
                  mFrameTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG(INFO) << "Framebuffer not complete!";

    glBindTexture(GL_TEXTURE_2D,
                  0); // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CameraTexture::release() {
    glDeleteProgram(m_Program);
}


