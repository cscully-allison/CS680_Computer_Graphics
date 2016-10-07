#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "graphics_headers.h"

//assimp headers
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects 
#include <Magick++.h>  // magick header
class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    
  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    float angle;
    unsigned int meshNumber;

    Assimp::Importer importer;
    const aiScene* scene;

};

#endif /* OBJECT_H */
