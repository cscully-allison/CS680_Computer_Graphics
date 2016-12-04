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
#include <Magick++.h>

class Object
{
  public:
    Object();
    //dynamic object
    Object(std::string filename,btScalar mass, btVector3 interia, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping, int index);
    // ball
    Object(btScalar mass, btVector3 inertia, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping);
    //static object
    Object (std::string filename, btScalar friction, btScalar restitution, btScalar damping, int indexNumber);
    Object (std::string filename);
    ~Object();
    void setOrientation();
    void Update();
    void ScoreUpdate();

    void Render(Uniform scalar, Uniform spec, Uniform spot, Uniform height);
    void setBodyTransform(btVector3);
    void setGrav(btVector3 grav);
    void applyForce (int force);
    void rotate(const glm::vec3);
    void translate(const glm::vec3);

    glm::mat4 GetModel();
    btRigidBody* GetRigidBody();
    glm::vec3 getSpec();
    void setSpec (glm::vec3 s);

    glm::vec4 getPosition();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    std::vector<GLuint> TB;

    Assimp::Importer importer;
    const aiScene* scene;

    std::vector<Magick::Blob> m_blob;

    glm::vec3 spec;
    
    /* bullet rigid body stuff */
    btTriangleMesh* mTriMesh;
    btRigidBody* body;

    unsigned int dtInitial;

};

#endif /* OBJECT_H */
