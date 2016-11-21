#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Color{
    glm::vec3 Ks;
    glm::vec3 Kd;
    glm::vec3 Ka;
    glm::vec3 emissive;
    glm::vec3 transparent;

    Color(glm::vec3 a, glm::vec3 d, glm::vec3 s, glm::vec3 e, glm::vec3 t): Ka(a), Kd(d), Ks(s), emissive (e), transparent (t) {}
};

struct Texture{
    glm::vec3 Ks;
    glm::vec2 texture;
    glm::vec3 Ka;
    glm::vec3 emissive;
    glm::vec3 transparent;

    Texture(glm::vec3 a, glm::vec2 d, glm::vec3 s, glm::vec3 e, glm::vec3 t): Ka(a), texture(d), Ks(s), emissive(e), transparent(t) {}
};

struct Vertex
{
  glm::vec3 vertex;
  glm::vec3 normals;
  Texture properties;

  Vertex(glm::vec3 v, glm::vec3 n, Texture p): vertex(v), normals (n), properties(p) {}
};

#endif /* GRAPHICS_HEADERS_H */
