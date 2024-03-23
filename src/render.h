#ifndef RENDER_H_
#define RENDER_H_

#include "src/vertex_array.h"
#include "src/index_buffer.h"
#include "src/shader.h"

class CustomRender {
public:
    CustomRender() = default;
    ~CustomRender() = default;
    void Draw(const VerTexArray& vao, const VerTexIndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
private:
};

#endif // RENDER_H_
