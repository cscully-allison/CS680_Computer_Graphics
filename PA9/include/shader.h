#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>

#include "graphics_headers.h"


class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, std::string shaderPath);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
    std::string loadShader(std::string path);
};

#endif  /* SHADER_H */
