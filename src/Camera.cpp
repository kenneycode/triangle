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

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace triangle {
Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float fov,
               float aspectRatio, float nearPlane, float farPlane)
    : position_(position), lookAt_(lookAt), up_(up), fov_(fov),
      aspectRatio_(aspectRatio), nearPlane_(nearPlane), farPlane_(farPlane) {}

const glm::mat4 &Camera::getViewMatrix() {
  viewMatrix_ = glm::lookAt(position_, lookAt_, up_);
  return viewMatrix_;
}

const glm::mat4 &Camera::getProjectMatrix() {
  projectMatrix_ = glm::perspective(fov_, aspectRatio_, nearPlane_, farPlane_);
  return projectMatrix_;
}

void Camera::setPosition(glm::vec3 position) { position_ = position; }

} // namespace triangle