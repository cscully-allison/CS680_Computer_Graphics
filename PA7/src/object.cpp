#include "object.h"
#include <sstream>

Object::Object()
{  
  std::string objectname;
  aiString texturename;
  int oldSize=0;
  GLuint tempTB;
  std::vector <Magick::Image> m_image;
  
  //Verticies and indicies needs to be initilized for run
  std::cout << "Object name?: " << std::endl;
  std::cin >> objectname;
  scene = importer.ReadFile("../assets/" + objectname, aiProcess_Triangulate);

  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){
 
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);

    //get texture file
    aiString filePath;
    filePath.Append("../assets/");
    filePath.Append(texturename.C_Str()); 
    
    m_image.push_back (Magick::Image(filePath.C_Str()));
    Magick::Blob temp;
    m_image[meshNums].write(&temp, "RGBA");
    m_blob.push_back(temp);

    for(unsigned int vertex = 0; vertex < scene->mMeshes[meshNums]->mNumVertices; vertex++){
        Vertices.push_back(
                    Vertex(
                        glm::vec3(scene->mMeshes[meshNums]->mVertices[vertex].x, 
                                  scene->mMeshes[meshNums]->mVertices[vertex].y, 
                                  scene->mMeshes[meshNums]->mVertices[vertex].z),
                        glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[0][vertex].x,
                                  1-scene->mMeshes[meshNums]->mTextureCoords[0][vertex].y))

        );
        
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
      


        glGenTextures(1, &tempTB);
        TB.push_back(tempTB);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TB[TB.size()-1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image[TB.size()-1].columns(), m_image[TB.size()-1].rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob[TB.size()-1].data());
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      

}
  angle = 0.0f;
}

Object::~Object()
{
    Vertices.clear();
    Indices.clear();
}


void Object::buildPlanet(glm::mat4 planet, std::string planetName)
{

}

void Object::buildMoon(glm::mat4 moon)
{
  
}

void Object::Update(unsigned int dt)
{
  angle += dt * M_PI/10000;

  model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  for (int i=0; i<TB.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TB[i]);
  }
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
