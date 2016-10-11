#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    virtual void Update(unsigned int dt, int key_press_val);
    virtual void Update(unsigned int dt, int key_press_val, glm::mat4 ptOfOrbt);
    void Render();

    glm::mat4 GetModel();

  public:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    float angle, angle_r;
    int coef, coef_r;
    int transDirection;

    bool paused, t_pause, r_pause;
};


class Moon: public Object
{
    public:
    Moon():Object(){};
	void Update(unsigned int dt, int key_press_val, glm::mat4 ptOfOrbt);
};
#endif /* OBJECT_H */
