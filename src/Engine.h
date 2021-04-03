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

#include "Material.h"
#include "Program.h"
#include "Scene.h"
#include <string>
#include <tiny_gltf.h>
#include <vector>

class Scene;

namespace triangle {

class Engine {

public:
  Engine(unsigned int width, unsigned int height);
  void loadGLTF(const std::string &path);
  void setDefaultCamera(std::shared_ptr<Camera> defaultCamera);
  void drawFrame();

private:
  void init();
  void buildDefaultCamera();
  void buildProgram();
  void buildScenes();
  std::shared_ptr<Scene>
  buildScene(const tinygltf::Model &model, unsigned int sceneIndex,
             const std::shared_ptr<std::vector<GLuint>> &buffers,
             const std::shared_ptr<std::vector<GLuint>> &textures);
  std::shared_ptr<std::vector<GLuint>>
  buildBuffers(const tinygltf::Model &model);
  std::shared_ptr<std::vector<GLuint>>
  buildTextures(const tinygltf::Model &model);
  std::shared_ptr<Node>
  buildNode(const tinygltf::Model &model, unsigned int nodeIndex,
            const std::shared_ptr<std::vector<GLuint>> &buffers,
            const std::shared_ptr<std::vector<GLuint>> &textures,
            std::shared_ptr<Node> parent = nullptr);
  std::shared_ptr<Mesh>
  buildMesh(const tinygltf::Model &model, unsigned int meshIndex,
            const std::shared_ptr<std::vector<GLuint>> &buffers,
            const std::shared_ptr<std::vector<GLuint>> &textures);
  std::shared_ptr<Material>
  buildMaterial(const tinygltf::Model &model, unsigned int materialIndex,
                const std::shared_ptr<std::vector<GLuint>> &textures);
  std::shared_ptr<Primitive>
  buildPrimitive(const tinygltf::Model &model, unsigned int meshIndex,
                 unsigned int primitiveIndex,
                 const std::shared_ptr<std::vector<GLuint>> &vaos,
                 const std::shared_ptr<std::vector<GLuint>> &buffers,
                 const std::shared_ptr<std::vector<GLuint>> &textures);
  GLuint buildDefaultBaseColorTexture(const tinygltf::Model &model);
  std::vector<std::shared_ptr<Scene>> scenes_;
  std::shared_ptr<Camera> camera_;
  std::shared_ptr<Program> program_;
  bool initialized_ = false;
  unsigned int width = 0;
  unsigned int height = 0;
  std::vector<std::pair<std::string, GLuint>> preDefinedAttributes;
  tinygltf::Model model_;
};

} // namespace triangle