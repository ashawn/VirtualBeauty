//
// Created by 张硕 on 2017/3/16.
//

#include <jni_render/gl/gl_program_loader.h>
#include <glog/logging.h>
#include "gaussianblur.h"

using namespace render;

void GaussianBlur::init() {
    glGenBuffers(1, &m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CAMERA_VERTEX_DATA)* sizeof(float),
                 CAMERA_VERTEX_DATA, GL_STATIC_DRAW);

    glGenBuffers(1, &m_Index_GL_BUFF);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CAMERA_INDICES_DATA)* sizeof(GLubyte), CAMERA_INDICES_DATA,
                 GL_STATIC_DRAW);

    glGenFramebuffers(2,m_Frame_GL_Buffer);

    glGenTextures(2, mFrameTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_Program = glutil::get_program_by_type(GAUSSIAN_BLUR);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");
    m_TextureHandle = glGetUniformLocation(m_Program,"mTexture");
    m_HorizontalHandle = glGetUniformLocation(m_Program,"horizontal");
    m_WidthHandle = glGetUniformLocation(m_Program,"width");
    m_HeightHandle = glGetUniformLocation(m_Program,"height");

    mWidth = 0;
    mHeight = 0;
}

void GaussianBlur::setTextureId(GLuint TextureId) {
    mTEXTURE = TextureId;
}

GLuint GaussianBlur::getFrameTextureId() {
    return mFrameTexture[0];
}

void GaussianBlur::draw() {
    LOG(INFO) << "GaussianBlur draw";
    glViewport(0,0,mWidth,mHeight);

    glUseProgram(m_Program);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);

    bool first_iteration = true;
    int horizontal = 1;
    int amount = 4;
    for(int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer[horizontal]);

        if (first_iteration)
            glClearColor(0,0,0,0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? mTEXTURE : mFrameTexture[1 - horizontal]);
        glUniform1i(m_TextureHandle,0);

        glUniform1i(m_HorizontalHandle,horizontal);
        glUniform1f(m_WidthHandle,mWidth);
        glUniform1f(m_HeightHandle,mHeight);

        glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                              VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
        glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                              VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

        glDrawElements(GL_TRIANGLES, sizeof(CAMERA_INDICES_DATA), GL_UNSIGNED_BYTE, 0);

        glBindFramebuffer(GL_FRAMEBUFFER,0);

        horizontal = 1 - horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glDisableVertexAttribArray(m_TexCoord_Handle);

    glUseProgram(0);
}


void GaussianBlur::loadShader(GLuint program) {
    m_Program = program;
}

void GaussianBlur::update(int width, int height) {
    mWidth = width;
    mHeight = height;
    for(int i = 0;i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer[i]);
        glBindTexture(GL_TEXTURE_2D, mFrameTexture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mWidth,mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mFrameTexture[i],0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG(INFO) << "Framebuffer not complete!";

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }
}

void GaussianBlur::release() {
    glDeleteProgram(m_Program);
}


