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

#include "Camera.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace triangle {

class Scene;

class Node {

public:
  Node(std::shared_ptr<Node> parent = nullptr);
  void setMatrix(const glm::mat4 &mat);
  const glm::mat4 &getMatrix();
  glm::mat4 getWorldMatrix();
  void addChild(const std::shared_ptr<Node> &node);
  void setMesh(std::shared_ptr<Mesh> mesh);
  const std::vector<std::shared_ptr<Node>> &getChildren();
  void draw();

private:
  std::vector<std::shared_ptr<Node>> children_;
  std::shared_ptr<Node> parent_;
  glm::mat4 matrix_;
  std::shared_ptr<Mesh> mesh_;
};

} // namespace triangle