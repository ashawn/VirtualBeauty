//
// Created by 张硕 on 2017/3/16.
//

#include <jni_render/image/image_data.h>
#include <glog/logging.h>
#include <jni_render/gl/gl_program_loader.h>
#include <jni_render/gl/gl_data.h>
#include "lipregion.h"

using namespace render;

void LipRegion::init() {
    m_Program = glutil::get_program_by_type(POLYGON);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");

    glGenFramebuffers(1,&m_Frame_GL_Buffer);
    glGenTextures(1, &mFrameTexture);

    mWidth = 0;
    mHeight = 0;

    glGenBuffers(1, &m_Vertex_GL_Buffer);
    glGenBuffers(1, &m_Index_GL_Buffer);
}

void LipRegion::draw() {
    LOG(INFO) << "LipRegion draw";
    glViewport(0,0,mWidth,mHeight);

    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_Program);

    glEnableVertexAttribArray(m_Position_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_Buffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mFrameTexture);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *) 0);

    glDrawElements(GL_TRIANGLES, m_Lip_Indices_Data.size/ sizeof(GLubyte), GL_UNSIGNED_BYTE, (GLvoid *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void LipRegion::update(int width, int height) {
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

void LipRegion::release() {
    release_gl_data(&m_Lip_Vertex_Data);
    release_gl_data(&m_Lip_Indices_Data);
    glDeleteProgram(m_Program);
}

void LipRegion::setLipData(GLfloat *lipVertexData, int vertex_len, GLbyte *lipIndicesData,
                                  int indice_len) {
    LOG(INFO) << "set lip data";
    update_gl_data(&m_Lip_Vertex_Data,lipVertexData,vertex_len * sizeof(GLfloat));
    update_gl_data(&m_Lip_Indices_Data,lipIndicesData,indice_len * sizeof(GLubyte));

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBufferData(GL_ARRAY_BUFFER, m_Lip_Vertex_Data.size, m_Lip_Vertex_Data.data,
                 GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_Buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Lip_Indices_Data.size, m_Lip_Indices_Data.data,
                 GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void LipRegion::loadShader(GLuint program) {
    m_Program = program;
}

GLuint LipRegion::getLipRegionTextureId() {
    return mFrameTexture;
}