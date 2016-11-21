#include "object.h"


Object::Object(std::string filename, btScalar mass, btVector3 inertia, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping, int rotate)
{
  aiString texturename;
  GLuint tempTB;
  std::vector <Magick::Image> m_image;


  pressed = false;

  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/texturethings/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  aiVector3D textureCoords(0.0f,0.0f, 0.0f);


  btConvexHullShape* shape = new btConvexHullShape();


  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){


    //texture loading from file
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);
   

    //get texture file
    aiString filePath;
    filePath.Append("../assets/texturethings/");
    filePath.Append(texturename.C_Str());

    

    m_image.push_back(Magick::Image(filePath.C_Str()));
    Magick::Blob temp;
    m_image[meshNums].write(&temp, "RGBA");
    m_blob.push_back(temp);

    const aiMesh* mesh = scene->mMeshes[meshNums];

    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Texture matTex(Ka, glm::vec2(0.0f), Ks, e, t);



    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){


      //load tex coords per vertex
      matTex.texture = glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[0][vertex].x,
                                  scene->mMeshes[meshNums]->mTextureCoords[0][vertex].y);

        

      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            matTex));


      shape->addPoint (btVector3(mesh->mVertices[vertex].x, 
                                 mesh->mVertices[vertex].y, 
                                 mesh->mVertices[vertex].z));
      
    }

      
     

    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);
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


   // create collision shape
    shape->calculateLocalInertia(mass, inertia);
    btDefaultMotionState* motion;
    switch(rotate)
    {
      case 1:
         motion = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0), -.5235), startOrigin));
      break;

      case 2:
          motion = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0), .9), startOrigin));
      break;
        
      default:
        motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startOrigin));
      break;
    }
    
    // static bodies get a mass of 0
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion, shape, inertia);
    rigidBodyCI.m_friction = friction;
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_angularDamping = damping;
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);
    //body->setUserIndex(rotate);



}

/*Object::Object(std::string filename, btScalar mass, btVector3 inertia, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping, int rotate)
{  
  pressed = false;
  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);

  btConvexHullShape* shape = new btConvexHullShape();
  
  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){
    const aiMesh* mesh = scene->mMeshes[meshNums];
    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Color materialsColor (Ka,Kd, Ks, e, t);


    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            materialsColor));

      shape->addPoint (btVector3(mesh->mVertices[vertex].x, 
                                 mesh->mVertices[vertex].y, 
                                 mesh->mVertices[vertex].z));
      
    }


    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);
    }

      glGenBuffers(1, &VB);
      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(),  &Vertices[0], GL_STATIC_DRAW);

      glGenBuffers(1, &IB);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}*/

   // create collision shape
    shape->calculateLocalInertia(mass, inertia);
    btDefaultMotionState* motion;
    switch(rotate)
    {
      case 1:
         motion = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0), -.5235), startOrigin));
      break;

      case 2:
          motion = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0), .9), startOrigin));
      break;
        
      default:
        motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startOrigin));
      break;
    }
    
    // static bodies get a mass of 0
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion, shape, inertia);
    rigidBodyCI.m_friction = friction;
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_angularDamping = damping;
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);
    //body->setUserIndex(rotate);

}

Object::Object(btScalar mass, btVector3 inertia, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping)
{  

  aiString texturename;
  GLuint tempTB;
  std::vector <Magick::Image> m_image;


  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/texturethings/ball.obj", aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  aiVector3D textureCoords(0.0f,0.0f, 0.0f);

  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){


    //texture loading from file
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);


    //get texture file
    aiString filePath;
    filePath.Append("../assets/texturethings/");
    filePath.Append(texturename.C_Str());

    m_image.push_back(Magick::Image(filePath.C_Str()));
    Magick::Blob temp;
    m_image[meshNums].write(&temp, "RGBA");
    m_blob.push_back(temp);


    const aiMesh* mesh = scene->mMeshes[meshNums];

    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Texture matTex(Ka, glm::vec2(0.0f), Ks, e, t);

    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      //load tex coords per vertex
      matTex.texture = glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[0][vertex].x,
                                  scene->mMeshes[meshNums]->mTextureCoords[0][vertex].y);
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            matTex));

      
    }


    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);
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

   // create collision shape
    btCollisionShape* shape = new btSphereShape(.25f);
    shape->calculateLocalInertia(mass, inertia);
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin (startOrigin);
    btDefaultMotionState* motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startOrigin));
    
    // static bodies get a mass of 0
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion, shape, inertia);
    rigidBodyCI.m_friction = friction;
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_angularDamping = damping;
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);
    //body->setUserIndex(10);

}





/*********************
STATIC OBJ CONTRUCTOR
***************************/

Object::Object(std::string filename, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping, int indexNumber)
{  
 aiString texturename;
  GLuint tempTB;
  std::vector <Magick::Image> m_image;


  pressed = false;

  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/texturethings/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  aiVector3D textureCoords(0.0f,0.0f, 0.0f);



  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){

   

    //texture loading from file
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);


    //get texture file
    aiString filePath;
    filePath.Append("../assets/texturethings/");
    filePath.Append(texturename.C_Str());

    m_image.push_back(Magick::Image(filePath.C_Str()));
    Magick::Blob temp;
    m_image[meshNums].write(&temp, "RGBA");
    m_blob.push_back(temp);

    // create triangle mesh
    mTriMesh = new btTriangleMesh();

    const aiMesh* mesh = scene->mMeshes[meshNums];

    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Texture matTex(Ka, glm::vec2(0.0f), Ks, e, t);

    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      //load tex coords per vertex
      matTex.texture = glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[0][vertex].x,
                                  scene->mMeshes[meshNums]->mTextureCoords[0][vertex].y);
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            matTex));
      
    }

    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);

      btVector3 v0 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[0]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].z);

      btVector3 v1 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[1]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].z);

      btVector3 v2 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[2]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].z);                            

      mTriMesh->addTriangle (v0,v1,v2);
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

   // create collision shape

    btCollisionShape* shape = new btBvhTriangleMeshShape (mTriMesh, true);  
    btVector3 interia (0,0,0);
    shape->calculateLocalInertia(0, interia);
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin (startOrigin);
    btDefaultMotionState* motion = new btDefaultMotionState(bodyTransform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motion, shape, interia);
    rigidBodyCI.m_friction = friction;
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_angularDamping = damping;
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);
    //body->setUserIndex(indexNumber);

  
}

/*

Object::Object(std::string filename, btVector3 startOrigin, btScalar friction, btScalar restitution, btScalar damping, int indexNumber)
{  

  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  // create triangle mesh
  mTriMesh = new btTriangleMesh();

  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){
    const aiMesh* mesh = scene->mMeshes[meshNums];
    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Color materialsColor (Ka,Kd, Ks, e, t);

    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            materialsColor));
    }

    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);

      btVector3 v0 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[0]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].z);

      btVector3 v1 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[1]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].z);

      btVector3 v2 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[2]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].z);                            

      mTriMesh->addTriangle (v0,v1,v2);
    }

      glGenBuffers(1, &VB);
      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(),  &Vertices[0], GL_STATIC_DRAW);

      glGenBuffers(1, &IB);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}
   // create collision shape

    btCollisionShape* shape = new btBvhTriangleMeshShape (mTriMesh, true);  
    btVector3 interia (0,0,0);
    shape->calculateLocalInertia(0, interia);
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin (startOrigin);
    btDefaultMotionState* motion = new btDefaultMotionState(bodyTransform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motion, shape, interia);
    rigidBodyCI.m_friction = friction;
    rigidBodyCI.m_restitution = restitution;
    rigidBodyCI.m_angularDamping = damping;
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);
    //body->setUserIndex(indexNumber);
}*/

Object::Object(std::string filename)
{  

  aiString texturename;
  GLuint tempTB;
  std::vector <Magick::Image> m_image;


  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/Numbers/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  aiVector3D textureCoords(0.0f,0.0f, 0.0f);

  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){


    //texture loading from file
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texturename);

    //get texture file
    aiString filePath;
    filePath.Append("../assets/");
    filePath.Append(texturename.C_Str());

    m_image.push_back(Magick::Image(filePath.C_Str()));
    Magick::Blob temp;
    m_image[meshNums].write(&temp, "RGBA");
    m_blob.push_back(temp);


    const aiMesh* mesh = scene->mMeshes[meshNums];

    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    glm::vec3 e = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    glm::vec3 t = glm::vec3(color.r, color.g, color.b);

    Texture matTex(Ka, glm::vec2(0.0f), Ks, e, t);

    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      //load tex coords per vertex
      matTex.texture = glm::vec2(scene->mMeshes[meshNums]->mTextureCoords[0][vertex].x,
                                  scene->mMeshes[meshNums]->mTextureCoords[0][vertex].y);
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            matTex));

      
    }

    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);
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
}


Object::~Object()
{
    Vertices.clear();
    Indices.clear();

}

void Object::setOrientation(){
  btTransform trans;
  btScalar m[16];

  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}

void Object::setGrav(btVector3 grav)
{
  body->setGravity(grav);
}

void Object::Update()
{

  btTransform trans;
  btScalar m[16];

  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}

void Object::applyForce(int force){
      body->applyForce(btVector3(force*200.0f, 0.0f, 0.0f), btVector3(0, 0, 0));
}

void Object::ScoreUpdate(){
    model = glm::translate (glm::mat4(1.0f), glm::vec3(15.0f, 0.0f , 2));

}

void Object::BallNumUpdate(){
    model = glm::translate (glm::mat4(1.0f), glm::vec3(15.0f, 0.0f , -7));

}

void Object::UpdateBumper(int scale){
  btTransform trans;
  btScalar m[16];

  body->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
  model *= glm::scale (model, glm::vec3(scale, scale, scale));
}

void Object::UpdateFlipper(int side, unsigned int keyPress, unsigned int dt)
{
  btTransform trans;
  btTransform rotation;
  btScalar m[16];
 if (!side){
  if (keyPress == 1073742049 && pressed == false)
  {
    dtInitial = dt;
    pressed = true;
    body->applyTorqueImpulse(btVector3(2000.0f, 2000.0f, 2000.0f));
    body->setRestitution(.25);
  }
  else if (keyPress == 1073742049 && ((dt - dtInitial) > 50) && pressed == true)
  {
    body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setRestitution(.25);

  }
  else if (keyPress != 1073742053 && keyPress != 1073742049 && ((dt - dtInitial) > 50) )
  {
    body->setMotionState(new btDefaultMotionState(btTransform(btQuaternion(btVector3(0.0, 1, 0.0), -.5235), btVector3(-10.9, 0, -3.3))));
    body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    pressed = false;
    body->setRestitution(1);
  }
}
else if (side){
  if (keyPress == 1073742053 && pressed == false)
  {
    dtInitial = dt;
    pressed = true;
    body->applyTorqueImpulse(btVector3(-2000.0f, -2000.0f, -2000.0f));
    body->setRestitution(.25);   
  }
    else if (keyPress == 1073742053 && ((dt - dtInitial) > 50) && pressed == true)
  {
    body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setRestitution(.25);

  }
   else if (keyPress != 1073742053 && keyPress != 1073742049 && ((dt - dtInitial) > 50) )
  {
    body->setMotionState(new btDefaultMotionState(btTransform(btQuaternion(btVector3(0.0, 1, 0.0), .9), btVector3(-10.9, 0, 2.5))));
    body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    pressed = false;
    body->setRestitution(1);
  }
}
  //get transform
  body->getMotionState()->getWorldTransform(trans); 

  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}


void Object::setBodyTransform(btVector3 position)
{
 // body->proceedToTransform(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), position));
}

glm::mat4 Object::GetModel()
{
  return model;
}


btRigidBody* Object::GetRigidBody(){
  return body;
}

void Object::Render(GLint scalarLoc, glm::vec3 scalar, GLint specLoc, glm::vec3 spec, GLint spotLoc, glm::vec3 spot, GLint heightLoc, GLfloat height)
{
    for (int i=0; i<TB.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TB[i]);
  }


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
  glEnableVertexAttribArray(6);    

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, properties.texture));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, properties.Ka));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, properties.Ks));
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, properties.emissive));
  glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, properties.transparent));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);  
  glDisableVertexAttribArray(5);
  glDisableVertexAttribArray(6);
}

glm::vec3 Object::getSpec(){
        return spec;
}

void Object::setSpec (glm::vec3 s){
        spec += s;
}