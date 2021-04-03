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

#include "Engine.h"
#include "Common.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <utility>

namespace triangle {

Engine::Engine(unsigned int width, unsigned int height)
    : width(width), height(height) {
  preDefinedAttributes.emplace_back("POSITION", 0);
  preDefinedAttributes.emplace_back("NORMAL", 1);
  preDefinedAttributes.emplace_back("TEXCOORD_0", 2);
}

void Engine::loadGLTF(const std::string &path) {
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;
  loader.LoadASCIIFromFile(&model_, &err, &warn, path);
}

void Engine::init() {
  buildDefaultCamera();
  buildProgram();
  buildScenes();
}

void Engine::drawFrame() {
  if (!initialized_) {
    init();
    initialized_ = true;
  }
  program_->bind();
  GL_CHECK(glEnable(GL_DEPTH_TEST));
  GL_CHECK(glViewport(0, 0, width, height));
  GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  for (auto &scene : scenes_) {
    scene->traverse([&](const std::shared_ptr<Node> &node) {
      auto modelViewProjectMatrix = camera_->getProjectMatrix() *
                                    camera_->getViewMatrix() *
                                    node->getWorldMatrix();
      auto location = GL_CHECK(glGetUniformLocation(
          program_->getProgram(), UNIFORM_MODEL_VIEW_PROJECT_MATRIX));
      GL_CHECK(glUniformMatrix4fv(location, 1, false,
                                  glm::value_ptr(modelViewProjectMatrix)));
      node->draw();
    });
  }
}

void Engine::buildDefaultCamera() {
  if (camera_ == nullptr) {
    camera_ = std::make_shared<Camera>(
        glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / height, 1.0f, 10.0f);
  }
}

void Engine::buildProgram() {
  program_ = std::make_shared<Program>(VERTEX_SHADER, FRAGMENT_SHADER);
}

std::shared_ptr<std::vector<GLuint>>
Engine::buildBuffers(const tinygltf::Model &model) {
  auto buffers = std::make_shared<std::vector<GLuint>>(model.buffers.size(), 0);
  GL_CHECK(glGenBuffers(buffers->size(), buffers->data()));
  for (auto i = 0; i < model.buffers.size(); ++i) {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers->at(i)));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, model.buffers[i].data.size(),
                          model.buffers[i].data.data(), GL_STATIC_DRAW));
  }
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  return buffers;
}

std::shared_ptr<std::vector<GLuint>>
Engine::buildTextures(const tinygltf::Model &model) {
  auto textures = std::make_shared<std::vector<GLuint>>(model.textures.size());
  GL_CHECK(glGenTextures(textures->size(), textures->data()));
  for (auto i = 0; i < textures->size(); ++i) {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textures->at(i)));
    const auto &texture = model.textures[i];
    const auto &image = model.images[texture.source];
    auto minFilter =
        texture.sampler >= 0 && model.samplers[texture.sampler].minFilter != -1
            ? model.samplers[texture.sampler].minFilter
            : GL_LINEAR;
    auto magFilter =
        texture.sampler >= 0 && model.samplers[texture.sampler].magFilter != -1
            ? model.samplers[texture.sampler].magFilter
            : GL_LINEAR;
    auto wrapS = texture.sampler >= 0 ? model.samplers[texture.sampler].wrapS
                                      : GL_REPEAT;
    auto wrapT = texture.sampler >= 0 ? model.samplers[texture.sampler].wrapT
                                      : GL_REPEAT;
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height,
                          0, GL_RGBA, image.pixel_type, image.image.data()));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
    if (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_LINEAR_MIPMAP_LINEAR) {
      GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }
  }
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  return textures;
}

void Engine::buildScenes() {
  auto buffers = buildBuffers(model_);
  auto textures = buildTextures(model_);
  scenes_.resize(model_.scenes.size());
  for (auto i = 0; i < model_.scenes.size(); ++i) {
    scenes_[i] = buildScene(model_, i, buffers, textures);
  }
}

std::shared_ptr<Scene>
Engine::buildScene(const tinygltf::Model &model, unsigned int sceneIndex,
                   const std::shared_ptr<std::vector<GLuint>> &buffers,
                   const std::shared_ptr<std::vector<GLuint>> &textures) {
  auto scene = std::make_shared<triangle::Scene>();
  for (auto i = 0; i < model.scenes[sceneIndex].nodes.size(); ++i) {
    scene->addNode(
        buildNode(model, model.scenes[sceneIndex].nodes[i], buffers, textures));
  }
  return scene;
}

std::shared_ptr<Node>
Engine::buildNode(const tinygltf::Model &model, unsigned int nodeIndex,
                  const std::shared_ptr<std::vector<GLuint>> &buffers,
                  const std::shared_ptr<std::vector<GLuint>> &textures,
                  std::shared_ptr<Node> parent) {
  auto node = std::make_shared<Node>(parent);
  auto nodeMatrix = model.nodes[nodeIndex].matrix;
  glm::mat4 matrix(1.0f);
  if (nodeMatrix.size() == 16) {
    matrix[0].x = nodeMatrix[0], matrix[0].y = nodeMatrix[1],
    matrix[0].z = nodeMatrix[2], matrix[0].w = nodeMatrix[3];
    matrix[1].x = nodeMatrix[4], matrix[1].y = nodeMatrix[5],
    matrix[1].z = nodeMatrix[6], matrix[1].w = nodeMatrix[7];
    matrix[2].x = nodeMatrix[8], matrix[2].y = nodeMatrix[9],
    matrix[2].z = nodeMatrix[10], matrix[2].w = nodeMatrix[11];
    matrix[3].x = nodeMatrix[12], matrix[3].y = nodeMatrix[13],
    matrix[3].z = nodeMatrix[14], matrix[3].w = nodeMatrix[15];
  } else {
    if (model.nodes[nodeIndex].translation.size() == 3) {
      glm::translate(matrix, glm::vec3(model.nodes[nodeIndex].translation[0],
                                       model.nodes[nodeIndex].translation[1],
                                       model.nodes[nodeIndex].translation[2]));
    }
    if (model.nodes[nodeIndex].rotation.size() == 4) {
      matrix *= glm::mat4_cast(glm::quat(model.nodes[nodeIndex].rotation[3],
                                         model.nodes[nodeIndex].rotation[0],
                                         model.nodes[nodeIndex].rotation[1],
                                         model.nodes[nodeIndex].rotation[2]));
    }
    if (model.nodes[nodeIndex].scale.size() == 3) {
      glm::scale(matrix, glm::vec3(model.nodes[nodeIndex].scale[0],
                                   model.nodes[nodeIndex].scale[1],
                                   model.nodes[nodeIndex].scale[2]));
    }
  }
  node->setMatrix(matrix);
  if (model.nodes[nodeIndex].mesh >= 0) {
    node->setMesh(
        buildMesh(model, model.nodes[nodeIndex].mesh, buffers, textures));
  }
  for (auto &childNodeIndex : model.nodes[nodeIndex].children) {
    node->addChild(buildNode(model, childNodeIndex, buffers, textures, node));
  }
  return node;
}

std::shared_ptr<Mesh>
Engine::buildMesh(const tinygltf::Model &model, unsigned int meshIndex,
                  const std::shared_ptr<std::vector<GLuint>> &buffers,
                  const std::shared_ptr<std::vector<GLuint>> &textures) {
  auto meshPrimitives =
      std::make_shared<std::vector<std::shared_ptr<Primitive>>>();
  const auto &primitives = model.meshes[meshIndex].primitives;
  auto vaos = std::make_shared<std::vector<GLuint>>(primitives.size());
  GL_CHECK(glGenVertexArrays(vaos->size(), vaos->data()));
  for (auto i = 0; i < primitives.size(); ++i) {
    GL_CHECK(glBindVertexArray(vaos->at(i)));
    meshPrimitives->push_back(
        buildPrimitive(model, meshIndex, i, vaos, buffers, textures));
  }
  GL_CHECK(glBindVertexArray(0));
  return std::make_shared<Mesh>(meshPrimitives);
}

std::shared_ptr<Material>
Engine::buildMaterial(const tinygltf::Model &model, unsigned int materialIndex,
                      const std::shared_ptr<std::vector<GLuint>> &textures) {
  auto baseColorIndex = model.materials[materialIndex]
                            .pbrMetallicRoughness.baseColorTexture.index;
  auto baseColorTexture =
      (baseColorIndex >= 0 ? textures->at(baseColorIndex)
                           : buildDefaultBaseColorTexture(model));
  const auto baseColorTextureLocation = GL_CHECK(
      glGetUniformLocation(program_->getProgram(), UNIFORM_BASE_COLOR_TEXTURE));
  return std::make_shared<Material>(baseColorTexture, baseColorTextureLocation);
}

std::shared_ptr<Primitive>
Engine::buildPrimitive(const tinygltf::Model &model, unsigned int meshIndex,
                       unsigned int primitiveIndex,
                       const std::shared_ptr<std::vector<GLuint>> &vaos,
                       const std::shared_ptr<std::vector<GLuint>> &buffers,
                       const std::shared_ptr<std::vector<GLuint>> &textures) {
  const auto &primitive = model.meshes[meshIndex].primitives[primitiveIndex];
  for (auto &attribute : preDefinedAttributes) {
    const auto &attributeName = attribute.first;
    const auto &attributeLocation = attribute.second;
    const auto iterator = primitive.attributes.find(attributeName);
    if (iterator == primitive.attributes.end()) {
      continue;
    }
    const auto &accessor = model.accessors[(*iterator).second];
    const auto &bufferView = model.bufferViews[accessor.bufferView];
    const auto bufferIdx = bufferView.buffer;

    GL_CHECK(glEnableVertexAttribArray(attributeLocation));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers->at(bufferIdx)));

    const auto byteOffset = accessor.byteOffset + bufferView.byteOffset;
    GL_CHECK(glVertexAttribPointer(
        attributeLocation, accessor.type, accessor.componentType, GL_FALSE,
        bufferView.byteStride, (const GLvoid *)byteOffset));
  }
  std::shared_ptr<Primitive> meshPrimitive;
  if (primitive.indices >= 0) {
    const auto &accessor = model.accessors[primitive.indices];
    const auto &bufferView = model.bufferViews[accessor.bufferView];
    const auto bufferIndex = bufferView.buffer;
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->at(bufferIndex)));
    meshPrimitive = std::make_shared<Primitive>(
        vaos->at(primitiveIndex), primitive.mode, accessor.count,
        accessor.componentType, accessor.byteOffset + bufferView.byteOffset);
  } else {
    const auto accessorIndex = (*begin(primitive.attributes)).second;
    const auto &accessor = model.accessors[accessorIndex];
    meshPrimitive =
        std::make_shared<Primitive>(vaos->at(primitiveIndex), primitive.mode,
                                    accessor.count, accessor.componentType);
  }
  meshPrimitive->setMaterial(
      buildMaterial(model, primitive.material, textures));
  return meshPrimitive;
}

GLuint Engine::buildDefaultBaseColorTexture(const tinygltf::Model &model) {
  float defaultBaseColorTextureColor[] = {1, 1, 1};
  GLuint defaultBaseColorTexture = 0;
  GL_CHECK(glGenTextures(1, &defaultBaseColorTexture));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, defaultBaseColorTexture));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT,
                        defaultBaseColorTextureColor));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  return defaultBaseColorTexture;
}

void Engine::setDefaultCamera(std::shared_ptr<Camera> defaultCamera) {
  camera_ = std::move(defaultCamera);
}

} // namespace triangle