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

#include "Mesh.h"
#include <utility>

namespace triangle {

Mesh::Mesh(std::shared_ptr<std::vector<std::shared_ptr<Primitive>>> primitives)
    : primitives_(std::move(primitives)) {}

void Mesh::draw() {
  for (auto &primitive : *primitives_) {
    primitive->draw();
  }
}

} // namespace triangle