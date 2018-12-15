//
// Created by 张硕 on 2017/4/18.
//

#include <jni_render/gl/gl_program_loader.h>
#include <glog/logging.h>
#include "yuv2rgb.h"

using namespace render;

void YUV2RGB::init() {
    glGenBuffers(1, &m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CAMERA_VERTEX_DATA)* sizeof(float),
                 CAMERA_VERTEX_DATA, GL_STATIC_DRAW);

    glGenBuffers(1, &m_Index_GL_BUFF);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CAMERA_INDICES_DATA)* sizeof(GLubyte), CAMERA_INDICES_DATA,
                 GL_STATIC_DRAW);

    glGenTextures(3, m_GL_TEXTURE);

    glGenFramebuffers(1, &m_Frame_GL_Buffer);

    glGenTextures(1, &mFrameTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_Program = glutil::get_program_by_type(YUV_2_RGB);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");

    m_TexY_Handle = glGetAttribLocation(m_Program, "texY");
    m_TexU_Handle = glGetAttribLocation(m_Program, "texU");
    m_TexV_Handle = glGetAttribLocation(m_Program, "texV");

    mWidth = 0;
    mHeight = 0;
}

GLuint YUV2RGB::getFrameTextureId() {
    return mFrameTexture;
}

void YUV2RGB::draw() {
    glUseProgram(m_Program);

    glBindFramebuffer(GL_FRAMEBUFFER,m_Frame_GL_Buffer);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_GL_BUFF);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[0]);
    glUniform1i(m_TexY_Handle,0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[1]);
    glUniform1i(m_TexU_Handle,1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[2]);
    glUniform1i(m_TexV_Handle,2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mFrameTexture);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, sizeof(CAMERA_INDICES_DATA), GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glDisableVertexAttribArray(m_TexCoord_Handle);

    glUseProgram(0);
}


void YUV2RGB::loadShader(GLuint program) {
    m_Program = program;
}

void YUV2RGB::setYUV(GLbyte* YData,GLbyte* UData,GLbyte* VData,int width, int height){
    LOG(INFO) << "set YUV data";

    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[0]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,YData);

    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[1]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,UData);

    glBindTexture(GL_TEXTURE_2D, m_GL_TEXTURE[2]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,VData);
}

void YUV2RGB::update(int width, int height) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG(INFO) << "Framebuffer not complete!";

    glBindTexture(GL_TEXTURE_2D,
                  0); // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void YUV2RGB::release() {
    glDeleteProgram(m_Program);
}


