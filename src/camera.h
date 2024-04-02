#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_
#include "third_party/glm/ext/matrix_float4x4.hpp"
#include "third_party/glm/gtc/type_ptr.hpp"
#include <array>
#include <iostream>
#include <vector>
class Camera {
 public:
  Camera() = default;
  ~Camera() = default;
  // pitch rotate x
  // yaw rotate y
  // roll roate z
  Camera& RotateAroundX(float pitch);
  Camera& RotateAroundY(float yaw);
  Camera& RotateAroundZ(float roll);
  Camera& ScaleCameraDistance(float scale_size) {
    scale_size_ += scale_size;
    return *this;
  }
  Camera& SetCameraCenter(const std::array<float, 3>& center) {
    camera_center_ = glm::vec3(center[0], center[1], center[2]);
    return *this;
  }
  Camera& MoveCenterX(float x_diff) {
    camera_center_.x += x_diff;
    return *this;
  }
  Camera& MoveCenterY(float y_diff) {
    camera_center_.y += y_diff;
    return *this;
  }
  float* GetViewMatrix() {
    view_ = glm::lookAt(camera_front_ * scale_size_, camera_center_, camera_up_);
    return glm::value_ptr(view_);
  }
 private:
  float scale_size_ = 3.0f;
  glm::mat4 view_;
  glm::vec3 camera_center_ = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 camera_front_ = glm::vec3(0.f, 0.f, 1.f);
  glm::vec3 camera_up_ = glm::vec3(0.f, 1.f, 0.f);
};
#endif // SRC_CAMERA_H_
