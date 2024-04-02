#include "src/gl_util.h"
#include <GL/glew.h>
#include <iostream>
bool GlCheckError() {
  bool res = false;
  while (GLenum error = glGetError()) {
    res = true;
    std::cout <<"[OpenGl Error]" << std::hex << error <<std::endl;
  }
  return res;
}
