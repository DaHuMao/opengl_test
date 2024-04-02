#ifndef SRC_WINDOWS_H_
#define SRC_WINDOWS_H_
#include "src/camera.h"
#include <functional>
struct GLFWwindow;
class Windows {
 public:
  ~Windows() = default;
  int Init(int width, int height);
  bool join(std::function<void()> func);
  int Destroy();
  float* GetViewMatrix();
  static Windows& Instance() {
    static Windows win;
    return win;
  }
 private:
  Windows() = default;
  GLFWwindow* window_ = nullptr;
};
#endif // SRC_WINDOWS_H_
