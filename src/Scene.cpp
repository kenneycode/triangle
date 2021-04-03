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

#include "Scene.h"

#include <utility>

namespace triangle {

void Scene::addNode(const std::shared_ptr<Node> &node) {
  nodes_.push_back(node);
}

void Scene::traverse(
    const std::function<void(std::shared_ptr<Node> node)> &callback) {
  for (auto &node : nodes_) {
    traverseInternal(node, callback);
  }
}

void Scene::traverseInternal(
    const std::shared_ptr<Node> &node,
    const std::function<void(std::shared_ptr<Node>)> &nodeProcessor) {
  nodeProcessor(node);
  auto &children = node->getChildren();
  for (auto &child : children) {
    traverseInternal(child, nodeProcessor);
  }
}

} // namespace triangle