#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "shader.h"
#include "GL/glew.h"

static std::string ParseShader(const std::string& file_path) {
  std::ifstream ifs(file_path);
  if (!ifs) {
    return "";
  }
  std::stringstream ss;
  std::string line;
  while(getline(ifs, line)) {
    ss << line << '\n';
  }
  return ss.str();
}

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
    : vertex_shader_(ParseShader(vertex_shader_path)),
      fragment_shader_(ParseShader(fragment_shader_path)) {
  //std::cout << vertex_shader_ << '\n' << fragment_shader_ << '\n';
  if ("" != vertex_shader_ && "" != fragment_shader_) {
    render_id_ = CreateShader(vertex_shader_, fragment_shader_);
  }
}

Shader::~Shader() {
  if(0 != render_id_) {
    glDeleteProgram(render_id_);
  }
}

void Shader::Bind() const {
    glUseProgram(render_id_);
}

void Shader::UnBind() const {
    glUseProgram(0);
}

void Shader::CompileShader() {

}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4fv(const std::string& name, float* ptr) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, ptr);
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)  {
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glValidateProgram(program);
    glDeleteProgram(vs);
    glDeleteProgram(fs);
    return program;
}

uint32_t Shader::CompileShader(uint32_t type, const std::string source) {
    uint32_t id = glCreateShader(type);
    const char* str = source.c_str();
    glShaderSource(id, 1, &str, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message =  (char*)alloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << "failed to compile "  <<
            (GL_VERTEX_SHADER == type ? "vertex：" : "fragment: ") << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

int32_t Shader::GetUniformLocation(const std::string& name) {
    auto it = uniform_cache_.find(name);
    if (uniform_cache_.end() != it) {
        return it->second;
    } else {
        int ret = glGetUniformLocation(render_id_, name.c_str());
        if (ret < 0) {
            std::cout << "glGetUniformLocation error, name: " << name <<
                " ret: " << ret;
        }
        uniform_cache_[name] = ret;
        return ret;
    }
}
