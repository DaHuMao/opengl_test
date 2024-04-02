#include "src/windows.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/camera.h"
#include "src/gl_util.h"

static bool keys[1024];
int32_t screenWidth = 1000;
int32_t screenHeight = 800;

Camera camera;
GLfloat yaw =
    -90.0f; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in
            // a direction vector pointing to the right (due to how Eular angles
            // work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX =  screenHeight / 2.0;
GLfloat lastY = screenHeight / 2.0;
GLfloat fov = 45.0f;

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

void do_movement() {
  // Camera controls
  GLfloat cameraSpeed = 2.0f * deltaTime;
  GLfloat angle_diff = cameraSpeed * 40;
  GLfloat camera_center_diff = cameraSpeed;
  if (keys[GLFW_KEY_Q])
    camera.ScaleCameraDistance(cameraSpeed);
  if (keys[GLFW_KEY_E])
    camera.ScaleCameraDistance(-cameraSpeed);
  if (keys[GLFW_KEY_W])
    camera.RotateAroundX(angle_diff);
  if (keys[GLFW_KEY_S])
    camera.RotateAroundX(-angle_diff);
  if (keys[GLFW_KEY_A])
    camera.RotateAroundY(angle_diff);
  if (keys[GLFW_KEY_D])
    camera.RotateAroundY(-angle_diff);
  if (keys[GLFW_KEY_T])
    camera.RotateAroundZ(angle_diff);
  if (keys[GLFW_KEY_Y])
    camera.RotateAroundZ(-angle_diff);
  if (keys[GLFW_KEY_RIGHT])
    camera.MoveCenterX(-camera_center_diff);
  if (keys[GLFW_KEY_LEFT])
    camera.MoveCenterX(camera_center_diff);
  if (keys[GLFW_KEY_UP])
    camera.MoveCenterY(-camera_center_diff);
  if (keys[GLFW_KEY_DOWN])
    camera.MoveCenterY(camera_center_diff);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset =
      lastY - ypos; // Reversed since y-coordinates go from bottom to left
  lastX = xpos;
  lastY = ypos;

  GLfloat sensitivity = 0.05; // Change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if (fov <= 1.0f)
    fov = 1.0f;
  if (fov >= 45.0f)
    fov = 45.0f;
}

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

int Windows::Init(int width, int height) {
  /* Initialize the library */
  if (GLFW_TRUE != glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  /* Create a window_ed mode window_ and its OpenGL context */
  window_ = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!window_)
  {
    glfwTerminate();
    return -1;
  }
  screenWidth = width;
  screenHeight = height;
  glfwMakeContextCurrent(window_);
  // GLFW Options
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (GLEW_OK != glewInit()) {
    return -1;
  }
  if (!window_) {
    return -1;
  }
  // Set the required callback functions
  glfwSetKeyCallback(window_, key_callback);
  glfwSetCursorPosCallback(window_, mouse_callback);
  glfwSetScrollCallback(window_, scroll_callback);
  glEnable(GL_DEPTH_TEST);
  return 0;
}

bool Windows::join(std::function<void()> func) {
  if (!window_) {
    return false;
  }
  while (!glfwWindowShouldClose(window_)) {
      glfwPollEvents();
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (GlCheckError()) {
        break;
      }
      if (func) {
        func();
      }
      glfwSwapBuffers(window_);
      do_movement();
      auto current_time = glfwGetTime();
      deltaTime = current_time - lastFrame;
      lastFrame = current_time;
  }
  return 0;
}

int Windows::Destroy() {
  if (window_) {
    glfwTerminate();
  }
  return 0;
}

float* Windows::GetViewMatrix() {
  return camera.GetViewMatrix();
}
