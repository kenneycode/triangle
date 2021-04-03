/*
 * Copyright 2021 kenney
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <iostream>
#include <string>

namespace triangle {
#define GL_CHECK(f)                                                            \
  f;                                                                           \
  {                                                                            \
    auto error = glGetError();                                                 \
    if (error != 0) {                                                          \
      std::cout << "GL error = " << error << std::endl;                        \
    }                                                                          \
    assert(error == 0);                                                        \
  }

const std::string VERTEX_SHADER =
    "#version 300 es\n"
    "precision mediump float;\n"
    "layout(location = 0) in vec4 a_position;\n"
    "layout(location = 1) in vec2 a_normal;\n"
    "layout(location = 2) in vec2 a_texCoord0;\n"
    "out vec2 v_texCoord0;\n"
    "uniform mat4 u_modelViewProjectMatrix;\n"
    "void main() {\n"
    "    gl_Position = u_modelViewProjectMatrix * a_position;\n"
    "    v_texCoord0 = a_texCoord0;\n"
    "}";

const std::string FRAGMENT_SHADER =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec2 v_texCoord0;\n"
    "out vec4 outColor;\n"
    "uniform sampler2D u_baseColorTexture;\n"
    "void main() {\n"
    "    outColor = texture(u_baseColorTexture, v_texCoord0);\n"
    "}";

#define UNIFORM_BASE_COLOR_TEXTURE "u_baseColorTexture"
#define UNIFORM_MODEL_VIEW_PROJECT_MATRIX "u_modelViewProjectMatrix"

} // namespace triangle