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

#include "Node.h"
#include "Scene.h"
#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace triangle {

Node::Node(std::shared_ptr<Node> parent)
    : parent_(std::move(parent)), matrix_(glm::mat4(1.0f)) {}

void Node::draw() {
  if (mesh_ != nullptr) {
    mesh_->draw();
  }
}

void Node::setMatrix(const glm::mat4 &mat) { this->matrix_ = mat; }

const glm::mat4 &Node::getMatrix() { return matrix_; }

glm::mat4 Node::getWorldMatrix() {
  return parent_ != nullptr ? parent_->getWorldMatrix() * matrix_ : matrix_;
}

void Node::addChild(const std::shared_ptr<Node> &node) {
  children_.push_back(node);
}

void Node::setMesh(std::shared_ptr<Mesh> mesh) {
  this->mesh_ = std::move(mesh);
}

const std::vector<std::shared_ptr<Node>> &Node::getChildren() {
  return children_;
}

} // namespace triangle
