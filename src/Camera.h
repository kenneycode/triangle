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

#include <glm/glm.hpp>
#include <memory>

namespace triangle {

class Camera {

public:
  Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float fov,
         float aspectRatio, float nearPlane, float farPlane);
  const glm::mat4 &getViewMatrix();
  const glm::mat4 &getProjectMatrix();
  void setPosition(glm::vec3 position);

private:
  glm::vec3 position_;
  glm::vec3 lookAt_;
  glm::vec3 up_;
  float fov_;
  float aspectRatio_;
  float nearPlane_;
  float farPlane_;
  glm::mat4 viewMatrix_{};
  glm::mat4 projectMatrix_{};
};

} // namespace triangle