//
// Created by 张硕 on 2017/4/12.
//

#include <glog/logging.h>
#include <jni_render/gl/gl_program_loader.h>
#include <jni_render/gl/glutils.h>
#include <jni_render/image/image_data.h>
#include <jni_render/gl/gl_data.h>
#include "alphatexturecolor.h"

using namespace render;

void AlphaTextureColor::init() {
    LOG(INFO) << "AlphaTextureColor init";
    m_Program = glutil::get_program_by_type(ALPHA_TEXTURE_COLOR);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");
    mTexture_Handle = glGetUniformLocation(m_Program, "mTexture");
    m_ColorHandle = glGetUniformLocation(m_Program, "mColor");
    m_AlphaFactorHandle = glGetUniformLocation(m_Program, "mAlphaFactor");

    m_Color[0] = 0.0f;
    m_Color[1] = 0.0f;
    m_Color[2] = 0.0f;
    m_Color[3] = 0.0f;

    m_AlphaFactor = 1.0f;

    glGenTextures(1, &mGLTexture);

    glGenBuffers(1, &m_Vertex_GL_Buffer);
    glGenBuffers(1, &m_Indice_GL_Buffer);
}

void AlphaTextureColor::setTexturePath(const char *texturepath) {
    glBindTexture(GL_TEXTURE_2D,
                  mGLTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    RawImageData rawImageData = glutil::load_png_asset_into_texture(texturepath);
    glTexImage2D(GL_TEXTURE_2D, 0, rawImageData.gl_color_format, rawImageData.width,
                 rawImageData.height,
                 0, rawImageData.gl_color_format, GL_UNSIGNED_BYTE, rawImageData.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,
                  0); // Unbind texture when done, so we won't accidentily mess up our texture.
    release_raw_image_data(&rawImageData);
}

void AlphaTextureColor::setColor(float f0, float f1, float f2, float f3) {

    m_Color[0] = f0;
    m_Color[1] = f1;
    m_Color[2] = f2;
    m_Color[3] = f3;
}

void AlphaTextureColor::setAlphaFactor(float AlphaFactor) {

    m_AlphaFactor = AlphaFactor;
}

void AlphaTextureColor::draw() {
    LOG(INFO) << "AlphaTextureColor draw";

    if (m_Vertex_Data.data == NULL || m_Indice_Data.data == NULL)
        return;

    glUseProgram(m_Program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mGLTexture);
    glUniform1i(mTexture_Handle, 0);

    glUniform4f(m_ColorHandle, m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

    glUniform1f(m_AlphaFactorHandle, m_AlphaFactor);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indice_GL_Buffer);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid *) POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid *) TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, m_Indice_Data.size / sizeof(GLubyte), GL_UNSIGNED_BYTE,
                   (GLvoid *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void AlphaTextureColor::setRectData(GLfloat *eyeVertexData, int vertex_len, GLbyte *eyeIndicesData,
                                    int indice_len) {
    LOG(INFO) << "set rect data";
    update_gl_data(&m_Vertex_Data, eyeVertexData, vertex_len * sizeof(GLfloat));
    update_gl_data(&m_Indice_Data, eyeIndicesData, indice_len * sizeof(GLubyte));

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBufferData(GL_ARRAY_BUFFER, m_Vertex_Data.size, m_Vertex_Data.data, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indice_GL_Buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indice_Data.size, m_Indice_Data.data, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AlphaTextureColor::loadShader(GLuint program) {

}

void AlphaTextureColor::update(int width, int height) {

}

void AlphaTextureColor::release() {
    LOG(INFO) << "AlphaTextureColor release";
    release_gl_data(&m_Vertex_Data);
    release_gl_data(&m_Indice_Data);
    glDeleteProgram(m_Program);
}

