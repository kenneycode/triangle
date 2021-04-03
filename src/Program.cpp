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

#include "Program.h"
#include "Common.h"

Program::Program(const std::string &vertexShaderCode,
                 const std::string &fragmentShaderCode) {

  program_ = GL_CHECK(glCreateProgram());

  auto vertexShader = GL_CHECK(glCreateShader(GL_VERTEX_SHADER));
  auto fragmentShader = GL_CHECK(glCreateShader(GL_FRAGMENT_SHADER));

  auto pVertexShader = vertexShaderCode.c_str();
  GL_CHECK(glShaderSource(vertexShader, 1, &pVertexShader, nullptr));
  auto pFragmentShader = fragmentShaderCode.c_str();
  GL_CHECK(glShaderSource(fragmentShader, 1, &pFragmentShader, nullptr));

  GL_CHECK(glCompileShader(vertexShader));
  GL_CHECK(glCompileShader(fragmentShader));

  GL_CHECK(glAttachShader(program_, vertexShader));
  GL_CHECK(glAttachShader(program_, fragmentShader));

  GL_CHECK(glLinkProgram(program_));
}

void Program::bind() { GL_CHECK(glUseProgram(program_)); }

GLuint Program::getProgram() { return program_; }
