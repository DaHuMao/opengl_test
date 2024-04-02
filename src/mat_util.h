#ifndef SRC_MAT_UTIL_H_
#define SRC_MAT_UTIL_H_
#include "third_party/glm/ext/matrix_float4x4.hpp"
#include "third_party/glm/ext/vector_float3.hpp"
#include <array>
namespace MatUtil {
glm::vec3 RotateVec(glm::vec3 rotate_vec, glm::vec3 rotate_axis, float angle);
glm::mat4 GetDefaultProjection(int width, int height);

typedef std::array<float, 3> vec3;
class Mat4 {
 public:
   Mat4() :mat_(1.0f) {}
   Mat4& Translate(const vec3& pos_diff);
   Mat4& Rotate(float angle, const vec3& axis);
   Mat4& Scale(const vec3& vec);
   float* Ptr();
   Mat4& Reset() {
    mat_ = glm::mat4(1.f);
    return *this;
   }
 private:
   glm::mat4 mat_;
};
} // MatUtil
#endif // SRC_MAT_UTIL_H_
