#ifndef SRC_GL_UTIL_H_
#define SRC_GL_UTIL_H_
#include <cstddef>
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))
bool GlCheckError();
#endif // SRC_GL_UTIL_H_
