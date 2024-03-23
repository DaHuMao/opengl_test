#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include "third_party/glm/ext/matrix_clip_space.hpp"
#include "third_party/glm/fwd.hpp"
#include "third_party/glm/glm.hpp"
#include "third_party/glm/gtc/matrix_transform.hpp"
#include "third_party/glm/gtc/type_ptr.hpp"
#include "src/vertex_array.h"
#include "src/vertex_buffer.h"
#include "src/index_buffer.h"
#include "src/shader.h"
#include "src/texture_load.h"
#include "src/render.h"
#include "src/camera.h"

template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))

constexpr int32_t kScreenWidth = 1000;
constexpr int32_t kScreenHeight = 800;
static bool GlCheckError() {
  bool res = false;
  while (GLenum error = glGetError()) {
    res = true;
    std::cout <<"[OpenGl Error]" << std::hex << error <<std::endl;
  }
  return res;
}

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float scaleSize = 1.0;
GLfloat yaw    = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch  =  0.0f;
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
GLfloat fov =  45.0f;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 2.0f * deltaTime;
    if (keys[GLFW_KEY_Q])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_R])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_W])
        scaleSize += cameraSpeed;
    if (keys[GLFW_KEY_S])
        scaleSize -= cameraSpeed;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;	// Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    cameraPos.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraPos.y = sin(glm::radians(pitch));
    cameraPos.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    std::cout << "pitch: " << pitch << " yaw: " << yaw << std::endl;
    std::cout << cameraFront.x <<  " " << cameraFront.y << " " << cameraFront.z << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void make_i(float* ptr, int N) {
  for (int i = 0; i < N; ++i) {
    int index = i * N + i;
    ptr[index] = 1;
  }
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
  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // GLFW Options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  if (GLEW_OK != glewInit()) {
    return -1;
  }
  {
    glEnable(GL_DEPTH_TEST);
    constexpr size_t kPointCount = 8;
    constexpr size_t kOneDimSize = 8;
    float positions[kPointCount * kOneDimSize] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // 右上
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // 左下
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f    // 左上
    };
    uint32_t indices[] = {
      // front
      0, 1, 2,
      2, 3, 0,

      // right
      4, 5, 1,
      1, 0, 4,

      // back
      4, 5, 6,
      6, 7, 4,

      // left
      6, 7, 3,
      3, 2, 6,

      // obove
      0, 4, 7,
      7, 3, 0,

      // under
      1, 5, 6,
      6, 2, 1
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    std::string work_path = "../";
    std::string vertex_shader = work_path + "example/shader.vs";
    std::string fragment_shader = work_path + "example/shader.frag";
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

    // 变
    glm::mat4 projection(1);
    projection = glm::perspective(glm::radians(45.f),
        kScreenWidth * 1.f / kScreenHeight, 0.1f, 100.0f);
    shader.SetUniform4fv("projection", glm::value_ptr(projection));
    CustomRender render;
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (GlCheckError()) {
        break;
      }
      texture_load0.Bind(0);
      texture_load1.Bind(1);
      glm::mat4 view(1);
      view = glm::lookAt(cameraPos * scaleSize, cameraFront, cameraUp);
      shader.SetUniform4fv("view", glm::value_ptr(view));
      for (GLuint i = 0; i < 10; i++) {
        glm::mat4 model(1);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 100.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.SetUniform4fv("model", glm::value_ptr(model));
        render.Draw(vao, ibo, shader);
      }
      glfwSwapBuffers(window);
      glfwPollEvents();
      GLfloat currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      do_movement();
      //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    vao.UnBind();
    vbo.UnBind();
    ibo.UnBind();
  }
  glfwTerminate();
  return 0;
}
