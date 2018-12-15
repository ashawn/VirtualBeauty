//
// Created by 张硕 on 2017/5/9.
//

#include <jni_render/image/image_data.h>
#include <glog/logging.h>
#include <jni_render/gl/gl_program_loader.h>
#include <jni_render/gl/gl_data.h>
#include "polygoncolor.h"

using namespace render;

void PolygonColor::init() {
    m_Program = glutil::get_program_by_type(POLYGON_COLOR);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_ColorHandle = glGetUniformLocation(m_Program,"mColor");

    glGenFramebuffers(1,&m_Frame_GL_Buffer);
    glGenTextures(1, &mFrameTexture);

    mWidth = 0;
    mHeight = 0;

    m_Color[0] = 1.0f;
    m_Color[1] = 0.0f;
    m_Color[2] = 0.0f;
    m_Color[3] = 1.0f;

    glGenBuffers(1, &m_Vertex_GL_Buffer);
    glGenBuffers(1, &m_Index_GL_Buffer);
}

void PolygonColor::draw() {
    LOG(INFO) << "PolygonColor draw";
    glViewport(0,0,mWidth,mHeight);

    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_Program);

    glEnableVertexAttribArray(m_Position_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_Buffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mFrameTexture);

    glUniform4f(m_ColorHandle,m_Color[0],m_Color[1],m_Color[2],m_Color[3]);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *) 0);

    glDrawElements(GL_TRIANGLES, m_Indices_Data.size/ sizeof(GLubyte), GL_UNSIGNED_BYTE, (GLvoid *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void PolygonColor::update(int width, int height) {
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

void PolygonColor::release() {
    release_gl_data(&m_Vertex_Data);
    release_gl_data(&m_Indices_Data);
    glDeleteProgram(m_Program);
}

void PolygonColor::setColor(float f0,float f1,float f2,float f3){

    m_Color[0] = f0;
    m_Color[1] = f1;
    m_Color[2] = f2;
    m_Color[3] = f3;
}

void PolygonColor::setVertexData(GLfloat *VertexData, int vertex_len, GLbyte *IndicesData,
                                  int indice_len) {
    LOG(INFO) << "set lip data";
    update_gl_data(&m_Vertex_Data,VertexData,vertex_len * sizeof(GLfloat));
    update_gl_data(&m_Indices_Data,IndicesData,indice_len * sizeof(GLubyte));

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBufferData(GL_ARRAY_BUFFER, m_Vertex_Data.size, m_Vertex_Data.data,
                 GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_Buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices_Data.size, m_Indices_Data.data,
                 GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void PolygonColor::loadShader(GLuint program) {
    m_Program = program;
}

GLuint PolygonColor::getFrameTextureId() {
    return mFrameTexture;
}