#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include "src/vertex_buffer_layout.h"
#include "src/vertex_buffer.h"

class VerTexArray {
public:
    VerTexArray();
    ~VerTexArray();
    void AddBuffer(const VerTexBufferLayout& layout);
    void Bind() const;
    void UnBind() const;
private:
    uint32_t vao_ = 0;
};

#endif // VERTEX_ARRAY_H_
