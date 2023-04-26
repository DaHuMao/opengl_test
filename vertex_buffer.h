#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include <stdint.h>
#include <cstddef>

class VerTexBuffer {
public:
    VerTexBuffer(const void* data, size_t size);
    ~VerTexBuffer();
    void Bind() const;
    void UnBind() const;
private:
    uint32_t vbo_ = 0;
};

#endif // VERTEX_BUFFER_H_
