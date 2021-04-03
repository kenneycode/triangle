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

#include "Primitive.h"
#include "Common.h"
#include <utility>

namespace triangle {
Primitive::Primitive(GLuint vao, int mode, int count, int componentType,
                     int offset)
    : vao_(vao), mode_(mode), count_(count), componentType_(componentType),
      offset_(offset) {}

void Primitive::setMaterial(std::shared_ptr<Material> material) {
  material_ = std::move(material);
}

void Primitive::draw() {
  material_->bind();
  GL_CHECK(glBindVertexArray(vao_));
  if (offset_ >= 0) {
    GL_CHECK(glDrawElements(mode_, count_, componentType_, (void *)offset_));
  } else {
    GL_CHECK(glDrawArrays(mode_, 0, count_));
  }
  GL_CHECK(glBindVertexArray(0));
}

} // namespace triangle
