#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt, unsigned int pressedKey);
    void Render();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float rotateAngle;
    float translateAngle;
    glm::mediump_vec2 direction = {1,1};
    glm:: tvec2 <bool> pause = {true,true};
};

#endif /* OBJECT_H */
