#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;
out vec3 ourColor;
out vec2 ourTexture;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  ourColor = aColor;
  ourTexture = aTexture;
}
