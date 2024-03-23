#include "vertex_array.h"

VerTexArray::VerTexArray()  {
    glGenVertexArrays(1, &vao_);
}

VerTexArray::~VerTexArray() {
    glDeleteVertexArrays(1, &vao_);
}

void VerTexArray::AddBuffer(const VerTexBufferLayout& layout) {
    Bind();
    const auto& elements = layout.GetElement();
    uint32_t offset = 0;
    for (uint32_t i  = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalize,
                layout.GetStride(), reinterpret_cast<const void*>(offset));
        glEnableVertexAttribArray(i);
        offset += element.count * VerTexBufferElement::GetSizeOfType(element.type);
    }
}

void VerTexArray::Bind() const {
    glBindVertexArray(vao_);
}

void VerTexArray::UnBind() const {
    glBindVertexArray(0);
}
