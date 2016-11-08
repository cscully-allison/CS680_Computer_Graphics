#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "graphics_headers.h"
#include <btBulletDynamicsCommon.h>

//assimp headers
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects 



class Object
{
  public:
    Object();
    Object(std::string filename, std::string objectType);
    ~Object();
    void setOrientation();
    void setCylinder();
    void setPos(glm::vec3 position);
    void Update(unsigned int dt, btDiscreteDynamicsWorld* world);
    void UpdateMouse(unsigned int dt, btDiscreteDynamicsWorld* world, float mouseX, float mouseY);
    void Render();
    void Render(GLint scalarLoc, glm::vec3 scalar, GLint specLoc, glm::vec3 spec, GLint spotLoc, glm::vec3 spot,GLint heightLoc, GLfloat height);
    void setBodyTransform(btVector3);
    

    glm::mat4 GetModel();
    btRigidBody* getRigidBody();
    glm::vec3 getSpec();
    void setSpec (glm::vec3 s);
    
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
    
    glm::vec3 spec;

    /* bullet rigid body stuff */
    btCollisionShape* shape;
    btDefaultMotionState* motion;
    btRigidBody* body;

};




#endif /* OBJECT_H */
