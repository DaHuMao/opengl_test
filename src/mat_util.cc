#include "src/mat_util.h"
#include "third_party/glm/gtc/type_ptr.hpp"
#include "third_party/glm/trigonometric.hpp"
#include "third_party/glm/gtc/matrix_transform.hpp"
namespace MatUtil {

glm::vec3 GetVec3(const vec3& vec) {
  return glm::vec3(vec[0], vec[1], vec[2]);
}

glm::vec3 RotateVec(glm::vec3 rotate_vec, glm::vec3 rotate_axis, float angle) {
  const float rad = glm::radians(angle);
  const float c = glm::cos(rad);
  const float s = glm::sin(rad);
  const glm::vec3& v = rotate_vec;
  return c * v + (1 - c) * glm::dot(v, rotate_axis) * v + s * glm::cross(v, rotate_axis);
}

glm::mat4 GetDefaultProjection(int width, int height) {
  return glm::perspective(
      glm::radians(45.f), width * 1.f / height, 0.1f, 100.0f);
}

Mat4& Mat4::Translate(const vec3& pos_diff) {
  mat_ = glm::translate(mat_, GetVec3(pos_diff));
  return *this;
}
Mat4& Mat4::Rotate(float angle, const vec3& axis) {
  mat_ = glm::rotate(mat_, glm::radians(angle), GetVec3(axis));
  return *this;
}
Mat4& Mat4::Scale(const vec3& scale_vec) {
  mat_ = glm::scale(mat_, GetVec3(scale_vec));
  return *this;
}
float* Mat4::Ptr() {
  return glm::value_ptr(mat_);
}

} // MatUtil
