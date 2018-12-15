//
// Created by 赖振奇 on 2017/3/30.
//
#include <map>
#include <assert.h>
#include <algorithm>
#include "gl_program_loader.h"
#include "glutils.h"

std::map<uint, GLuint> mProgramMap;

std::string shaders[][2] = {
        {"shaders/Texture2DImage.vert",         "shaders/Texture2DImage.frag"},
        {"shaders/RawTexture2DImage.vert",      "shaders/RawTexture2DImage.frag"},
        {"shaders/Polygon.vert",                "shaders/Polygon.frag"},
        {"shaders/TextureOverlay.vert",         "shaders/TextureOverlay.frag"},
        {"shaders/TextureMultiplyColor.vert",   "shaders/TextureMultiplyColor.frag"},
        {"shaders/TextureMultiplyTexture.vert", "shaders/TextureMultiplyTexture.frag"},
        {"shaders/GaussianBlur.vert",           "shaders/GaussianBlur.frag"},
        {"shaders/PSGray.vert",                 "shaders/PSGray.frag"},
        {"shaders/PSLevels.vert",               "shaders/PSLevels.frag"},
        {"shaders/Lighten.vert",                "shaders/Lighten.frag"},
        {"shaders/Transform4_3To16_9.vert",     "shaders/Transform4_3To16_9.frag"},
        {"shaders/BilateralFilter.vert",        "shaders/BilateralFilter.frag"},
        {"shaders/SkinColor.vert",              "shaders/SkinColor.frag"},
        {"shaders/AlphaTextureColor.vert",      "shaders/AlphaTextureColor.frag"},
        {"shaders/YUV2RGB.vert",                "shaders/YUV2RGB.frag"},
        {"shaders/YUVSP2RGB.vert",              "shaders/YUVSP2RGB.frag"},
        {"shaders/PolygonColor.vert",           "shaders/PolygonColor.frag"},
        {"shaders/Mesh.vert",                   "shaders/Mesh.frag"},
};

GLuint glutil::get_program_by_type(uint type) {
    std::map<uint, GLuint>::iterator it = mProgramMap.find(type);
    if (it != mProgramMap.end()) return it->second;
    assert(type < sizeof(shaders) / sizeof(shaders[0]));
    std::string vertex = shaders[type][0];
    std::string fragment = shaders[type][1];
    GLuint program = glutil::createProgram(glutil::read_string_from_asset(vertex.c_str()),
                                           glutil::read_string_from_asset(
                                                   fragment.c_str()));
    mProgramMap.insert(std::pair<uint, GLuint>(type, program));
    return program;
}

void glutil::release_all_program() {
    std::for_each(mProgramMap.cbegin(), mProgramMap.cend(),
                  [&](std::pair<int, GLuint> p) -> void {
                      glDeleteProgram(p.second);
                  });
    mProgramMap.clear();
}