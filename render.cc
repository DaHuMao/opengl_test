#include "render.h"

void CustomRender::Draw(const VerTexArray& vao, const VerTexIndexBuffer& ib, const Shader& shader) const {
    vao.Bind();
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    vao.UnBind();
}

void CustomRender::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

