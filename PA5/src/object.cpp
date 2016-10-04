#include "object.h"


Object::Object()
{  
  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/pinball.obj", aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes;
  aiColor3D color (0.0f,0.0f, 0.0f);
  
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++){


    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    for(unsigned int vertex = 0; vertex < scene->mMeshes[meshNums]->mNumVertices; vertex++){
      Vertices.push_back(Vertex(
                            glm::vec3(scene->mMeshes[meshNums]->mVertices[vertex].x, 
                                      scene->mMeshes[meshNums]->mVertices[vertex].y, 
                                      scene->mMeshes[meshNums]->mVertices[vertex].z), glm::vec3(color.r, color.g, color.b)));


    }

    for(unsigned int index = 0; index < scene->mMeshes[meshNums]->mNumFaces; index++){
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[0]);
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[1]);
      Indices.push_back(scene->mMeshes[meshNums]->mFaces[index].mIndices[2]);
    }
      //if(scene->HasMeshes()){
      glGenBuffers(1, &VB);
      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(),  &Vertices[0], GL_STATIC_DRAW);

      glGenBuffers(1, &IB);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


  //}
}




  angle = 0.0f;


}

Object::~Object()
{
    Vertices.clear();
    Indices.clear();
}

void Object::Update(unsigned int dt)
{
  angle += dt * M_PI/10000;

  model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0, 1.0, 0.0));
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


