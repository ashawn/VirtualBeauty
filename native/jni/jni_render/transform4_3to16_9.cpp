//
// Created by 张硕 on 2017/3/30.
//

#include <jni_render/gl/gl_program_loader.h>
#include "transform4_3to16_9.h"

using namespace render;

void Transform4_3To16_9::init() {
    glGenBuffers(1, &m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CAMERA_VERTEX_DATA)* sizeof(float),
                 CAMERA_VERTEX_DATA, GL_STATIC_DRAW);

    glGenBuffers(1, &m_Index_GL_BUFF);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CAMERA_INDICES_DATA)* sizeof(GLubyte), CAMERA_INDICES_DATA,
                 GL_STATIC_DRAW);

    m_Program = glutil::get_program_by_type(TRANSFORM4_3TO16_9);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");

    mWidth = 0;
    mHeight = 0;
}

void Transform4_3To16_9::setTextureId(GLuint TextureId) {
    m_GL_TEXTURE = TextureId;
}

void Transform4_3To16_9::draw() {
    glViewport(0,0,mWidth,mHeight);

    glUseProgram(m_Program);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, sizeof(CAMERA_INDICES_DATA), GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glDisableVertexAttribArray(m_TexCoord_Handle);

    glUseProgram(0);
}


void Transform4_3To16_9::loadShader(GLuint program) {
    m_Program = program;
}

void Transform4_3To16_9::update(int width, int height) {
    mWidth = width;
    mHeight = height;
}

void Transform4_3To16_9::release() {
    glDeleteProgram(m_Program);
}


