#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "texture_load.h"
#include "render.h"
#include "camera.h"
#include "third_party/glm/glm.hpp"
#include "third_party/glm/gtc/matrix_transform.hpp"
#include "third_party/glm/gtc/type_ptr.hpp"

template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))

constexpr int32_t kScreenWidth = 800;
constexpr int32_t kScreenHeight = 600;
static bool GlCheckError() {
  bool res = false;
  while (GLenum error = glGetError()) {
    res = true;
    std::cout <<"[OpenGl Error]" << std::hex << error <<std::endl;
  }
  return res;
}
int main(int argc, char *argv[]) {
  GLFWwindow* window;
  /* Initialize the library */
  if (GLFW_TRUE != glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Hello World", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  if (GLEW_OK != glewInit()) {
    return -1;
  }
  {
    constexpr size_t kPointCount = 4;
    constexpr size_t kOneDimSize = 8;
    float positions[kPointCount * kOneDimSize] = {
      -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.25, 0.75,
      0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  0.75, 0.75,
      0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.75, 0.25,
      -0.5f, -0.5f, 0.0f,  0.3f, 0.3f, 0.3f,  0.25, 0.25
    };
    uint32_t indices[] = {
      0, 1, 2,
      3, 0, 2
    };
    std::string work_path = "/Users/ztx/Desktop/workspace/opengl_test/";
    std::string vertex_shader = work_path + "shader.vs";
    std::string fragment_shader = work_path + "shader.frag";
    TextureLoad texture_load0(work_path + "res/container.jpg");
    TextureLoad texture_load1(work_path + "res/awesomeface.jpg");
    VerTexBuffer vbo(positions, sizeof(positions));
    VerTexArray vao;
    VerTexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    vao.AddBuffer(layout);
    VerTexIndexBuffer ibo(indices, ARRAY_SIZE(indices));
    Shader shader(vertex_shader, fragment_shader);
    shader.Bind();
    shader.SetUniform1i("selectTexture1", 0);
    shader.SetUniform1i("selectTexture2", 1);
    vao.UnBind();
    vbo.UnBind();
    ibo.UnBind();
    CustomRender render;
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (GlCheckError()) {
        break;
      }
      texture_load0.Bind(0);
      texture_load1.Bind(1);
      render.Draw(vao, ibo, shader);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}
