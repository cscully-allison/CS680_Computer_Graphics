#include "object.h"


Object::Object(std::string filename, std::string objectType)
{  
  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/newObjects/" + filename, aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes;
  aiColor3D color (0.0f,0.0f, 0.0f);
  glm::vec3 Ks;
  glm::vec3 Ka;
  glm::vec3 Kd;
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++){
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    Kd = glm::vec3(color.r, color.g, color.b);
 
    for(unsigned int vertex = 0; vertex < scene->mMeshes[meshNums]->mNumVertices; vertex++){
      Vertices.push_back(Vertex(
                            glm::vec3(scene->mMeshes[meshNums]->mVertices[vertex].x, 
                                      scene->mMeshes[meshNums]->mVertices[vertex].y, 
                                      scene->mMeshes[meshNums]->mVertices[vertex].z), 
                            glm::vec3(scene->mMeshes[meshNums]->mNormals[vertex].x, 
                                      scene->mMeshes[meshNums]->mNormals[vertex].y, 
                                      scene->mMeshes[meshNums]->mNormals[vertex].z),
                            glm::vec3(color.r, color.g, color.b), Ka, Kd, Ks));


    }

    for(unsigned int index = 0; index < scene->mMeshes[meshNums]->mNumFaces; index++){
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[0]);
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[1]);
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[2]);
    }
     
      glGenBuffers(1, &VB);
      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(),  &Vertices[0], GL_STATIC_DRAW);

      glGenBuffers(1, &IB);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  }
  btScalar mass(0);

  if (objectType.compare("sphere") == 0)
  {
    shape = new btSphereShape(.25f);
    mass=btScalar(1);  
  }
  else if (objectType.compare("box") == 0)
  {
    shape = new btBoxShape(btVector3(.5f, .5f, 0.5f));
    mass=btScalar(50);
  }
  /*else if (objectType.compare("cylinder"))
  {
    shape = new btCylinderShape(btVector3(btScalar(5.0f), btScalar(5.0f), btScalar(5.0f)));
    mass=btScalar(0);  

  }*/

  
  if (objectType.compare(" ") != 0)
  { 
    motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(btScalar(0),btScalar(0),btScalar(0))));
    btVector3 inertia(btScalar(0),btScalar(0),btScalar(0));
    if (mass > 0)
      shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, motion, shape, inertia);
    shapeRigidBodyCI.m_friction = btScalar(0);
      if (objectType.compare("sphere") == 0)
      {
        shapeRigidBodyCI.m_restitution =btScalar(.25);

      }
      else
      {
        shapeRigidBodyCI.m_restitution =btScalar(0);
        shapeRigidBodyCI.m_angularDamping=btScalar(4);
      }
    
    body = new btRigidBody(shapeRigidBodyCI);
    body->setActivationState(DISABLE_DEACTIVATION);
  }
  angle = 0.0f;


}

Object::~Object()
{
    Vertices.clear();
    Indices.clear();
    delete shape;
    delete motion;
    delete body;

    shape = NULL;
    motion = NULL;
    body = NULL;
}

void Object::setOrientation(){
  model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.25f, 0.0f));
}

void Object::setPos(glm::vec3 position){
  btTransform trans;
  btScalar m[16];
  body->proceedToTransform(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(position.x), btScalar(position.y), btScalar(position.z))));
  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}

void Object::setCylinder()
{
  model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, -3.0f));
}

void Object::Update(unsigned int dt, btDiscreteDynamicsWorld* world)
{
  angle += dt * M_PI/10000;
  
  btTransform trans;
  btScalar m[16];


  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}

void Object::UpdateMouse(unsigned int dt, btDiscreteDynamicsWorld* world, float mouseX, float mouseY){

  btTransform trans;
  btScalar m[16];
  body->proceedToTransform(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(-mouseX/10), btScalar(0.0f), btScalar(-mouseY/10))));

  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}

void Object::setBodyTransform(btVector3 position)
{
  body->proceedToTransform(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), position));
}

glm::mat4 Object::GetModel()
{
  return model;
}

btRigidBody* Object::getRigidBody(){
  return body;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}


void Object::Render(GLint scalarLoc, glm::vec3 scalar, GLint specLoc, glm::vec3 spec, GLint spotLoc, glm::vec3 spot, GLint heightLoc, GLfloat height)
{

  glUniform3fv(scalarLoc, 1, glm::value_ptr(scalar));
  glUniform3fv(specLoc, 1, glm::value_ptr(spec));
  glUniform3fv(spotLoc, 1, glm::value_ptr(spot));
  glUniform1f (heightLoc, height);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Ka));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Kd));
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Ks));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
  glDisableVertexAttribArray(5);
}

glm::vec3 Object::getSpec(){
        return spec;
}

void Object::setSpec (glm::vec3 s){
        spec += s;
}
