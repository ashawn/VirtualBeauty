//
// Created by 张硕 on 2017/3/16.
//

#include <jni_render/gl/gl_program_loader.h>
#include <glog/logging.h>
#include "texturemultiplycolor.h"

using namespace render;

void TextureMultiplyColor::init() {
    glGenBuffers(1, &m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CAMERA_VERTEX_DATA)* sizeof(float),
                 CAMERA_VERTEX_DATA, GL_STATIC_DRAW);

    glGenBuffers(1, &m_Index_GL_BUFF);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CAMERA_INDICES_DATA)* sizeof(GLubyte), CAMERA_INDICES_DATA,
                 GL_STATIC_DRAW);

    glGenFramebuffers(1,&m_Frame_GL_Buffer);

    glGenTextures(1, &mFrameTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_Program = glutil::get_program_by_type(TEXTURE_MULTIPLY_COLOR);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");
    m_TextureHandle = glGetUniformLocation(m_Program,"mTexture");
    m_ColorHandle = glGetUniformLocation(m_Program,"mColor");

    m_Color[0] = 1.0f;
    m_Color[1] = 0.0f;
    m_Color[2] = 0.0f;
    m_Color[3] = 1.0f;

    mWidth = 0;
    mHeight = 0;
}

void TextureMultiplyColor::setColor(float f0,float f1,float f2,float f3){

    m_Color[0] = f0;
    m_Color[1] = f1;
    m_Color[2] = f2;
    m_Color[3] = f3;
}

void TextureMultiplyColor::setTextureId(GLuint TextureId) {
    mTEXTURE = TextureId;
}

GLuint TextureMultiplyColor::getFrameTextureId() {
    return mFrameTexture;
}

void TextureMultiplyColor::draw() {
    LOG(INFO) << "TextureMultiplyColor draw";
    glViewport(0,0,mWidth,mHeight);

    glUseProgram(m_Program);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);

    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTEXTURE);
    glUniform1i(m_TextureHandle,0);

    glUniform4f(m_ColorHandle,m_Color[0],m_Color[1],m_Color[2],m_Color[3]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mFrameTexture);


    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, sizeof(CAMERA_INDICES_DATA), GL_UNSIGNED_BYTE, 0);

    glBindFramebuffer(GL_FRAMEBUFFER,0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glDisableVertexAttribArray(m_TexCoord_Handle);

    glUseProgram(0);
}

void TextureMultiplyColor::update(int width, int height) {
    mWidth = width;
    mHeight = height;
    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);
    glBindTexture(GL_TEXTURE_2D, mFrameTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mWidth,mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mFrameTexture,0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG(INFO) << "Framebuffer not complete!";

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}


void TextureMultiplyColor::loadShader(GLuint program) {
    m_Program = program;
}

void TextureMultiplyColor::release() {
    glDeleteProgram(m_Program);
}


