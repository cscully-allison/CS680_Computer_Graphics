#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <map>
#include <fstream>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

    void loadTexture(std::string filePath, std::vector<Vertex> *geometry);
    void fetchMaterial(std::string filePath, std::map<std::string,glm::vec3> *materials);
    
  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    float angle;


};

#endif /* OBJECT_H */
