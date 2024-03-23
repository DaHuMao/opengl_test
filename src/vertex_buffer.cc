#include "vertex_buffer.h"
#include "GL/glew.h"

VerTexBuffer::VerTexBuffer(const void* data, size_t size) {
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

VerTexBuffer::~VerTexBuffer() {
    glDeleteBuffers(1, &vbo_);
}

void VerTexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void VerTexBuffer::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
