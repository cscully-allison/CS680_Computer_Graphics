#include "object.h"
#include <sstream>

Object::Object()
{  
  std::string objectname;
  unsigned int oldSize = 0;

  aiString texturename;
  aiVector3D uv;
  //Verticies and indicies needs to be initilized for run
  std::cout << "Object name?: " << std::endl;
  std::cin >> objectname;
  scene = importer.ReadFile("../assets/" + objectname, aiProcess_Triangulate);

  meshNumber = scene->mNumMeshes;

  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++){
 
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);

    //get texture file
    aiString filePath;
    filePath.Append("../assets/");
    filePath.Append(texturename.C_Str()); 
    Magick::Image* m_image = new Magick::Image(filePath.C_Str());
    std::cout << filePath.C_Str() << std::endl;
    m_image->write(&m_blob, "RGBA");

    // glActiveTexture(TextureUnit);
    // glBindTexture(GL_TEXTURE_2D, TB);

    std::cout << scene->mMeshes[meshNums]->mNumVertices << std::endl;
    for(unsigned int vertex = 0; vertex < scene->mMeshes[meshNums]->mNumVertices; vertex++){
        Vertices.push_back(
                    Vertex(
                        glm::vec3(scene->mMeshes[meshNums]->mVertices[vertex].x, 
                                  scene->mMeshes[meshNums]->mVertices[vertex].y, 
                                  scene->mMeshes[meshNums]->mVertices[vertex].z))
        );

                        // glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[vertex].x,
                        //           scene->mMeshes[meshNums]->mTextureCoords[vertex].y)
        
    }
    std::cout << Vertices.size() <<" " <<  AI_MAX_NUMBER_OF_TEXTURECOORDS << std::endl;
    // std::cout << "oldsize: " << oldSize << std::endl;
    // for (oldSize; oldSize < Vertices.size(); oldSize++)
    //     {
    //     if (oldSize > 300000)
    //     {
    //       std::cout << "ass" << std::endl;
    //     }
    //     uv =scene->mMeshes[meshNums]->mTextureCoords[0][oldSize];
    //     //std::cout << uv.x << "  " << uv.y << std::endl;
    //     Vertices[oldSize].texture = glm::vec2 (uv.x, uv.y);
    //     }


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

      glGenTextures(1, &TB);
      glBindTexture(GL_TEXTURE_2D, TB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->columns(), m_image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


