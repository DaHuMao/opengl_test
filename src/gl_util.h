#ifndef SRC_WINDOWS_H_
#define SRC_WINDOWS_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))
bool GlCheckError();
bool* getKey();
GLFWwindow* CreateAndInit(int width, int height);
#endif // SRC_WINDOWS_H_
