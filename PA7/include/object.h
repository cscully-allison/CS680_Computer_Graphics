#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <iostream>

#include "graphics_headers.h"
//#include "graphics.h"

//assimp headers
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects 
#include <Magick++.h>  // magick header
struct Planet;

class Object
{
  public:
    Object(std::string objectname);
    ~Object();
    void Update(unsigned int dt, Planet current);
    void UpdateMoon(unsigned int dt,  glm::mat4 planet);
    void Render();

    glm::mat4 GetModel();
    
  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint TB;
    Magick::Blob m_blob;
    float angle;
    unsigned int meshNumber;

    Assimp::Importer importer;
    const aiScene* scene;

};

struct Planet
{
    std::string name;
    Object *planet;
    Object *moon[4];
    float proportionToEarth;
    float rotationRadius;
    float rotationSpeed;
    float orbitSpeedRatio;
    bool hasRings;
    float numRings;
    bool hasMoons;
    float numMoons;
};

#endif /* OBJECT_H */
