#include "object.h"


Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

/*
  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };
*/
/*
  Indices = {
    2, 3, 4, 
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };
*/

  loadTexture("../assets/dragon.obj", &Vertices);

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle = 0.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  angle += dt * M_PI/10000;

  model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5, 1.0, 0.5));
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::loadTexture(std::string filePath, std::vector<Vertex> *geometry){
  std::FILE* fin = fopen(filePath.c_str(), "r");


  glm::vec3 vertexPoint;
  glm::vec2 uv;
  glm::vec3 normal;

  Vertex* fullVertex = new Vertex(glm::vec3(0.0f), glm::vec3(0.5f, 0.35f, .10f));

  char leadingCode[200];
  char garbageStr[200];
  char mtlFilePath[200];
  int status;

  int indicesFormatFlag = 0;  
        

  if(fin == NULL){
    std::cout << "Texture File Not Found" << std::endl;
    return;
  }

  status = fscanf(fin, "%s", leadingCode);

  while(status != EOF){
    if(strcmp(leadingCode, "#") == 0){

      fscanf(fin, "%s\n", garbageStr); 

    }
    
    else if(strcmp(leadingCode, "v") == 0){

       fscanf(fin, "%f %f %f\n", &vertexPoint.x, &vertexPoint.y, &vertexPoint.z);


       fullVertex->vertex = vertexPoint;
       fullVertex->color = glm::vec3(0.0f, ( float(rand()) / float(RAND_MAX)), 0.0f );

       geometry->push_back(*fullVertex);

    }
    
    else if(strcmp(leadingCode, "vn") == 0){

        fscanf(fin, "%s\n", garbageStr);

    }
    
    else if(strcmp(leadingCode, "f") == 0){
        int tempIndexHolder;        

        int vertexIndices[3];
        int uvIndices[3];
        int normalIndices[3];
        
        //use a three state flag to indicate
        if(indicesFormatFlag == 1){
            
            fscanf(fin, "%d//%d %d//%d %d//%d\n", &vertexIndices[0] , &normalIndices[0], &vertexIndices[1],  &normalIndices[1], &vertexIndices[2], &normalIndices[2]);
            
        } 
        else if(indicesFormatFlag == 2){    
            
            fscanf(fin, "%d %d %d\n", &vertexIndices[0], &vertexIndices[1], &vertexIndices[2]);

        }
        else if(indicesFormatFlag == 0){
          
            char seperator;

            fscanf(fin, "%d", &tempIndexHolder);
            seperator = fgetc(fin);            

            if(seperator == '/'){

                std::cout << "is called";                

                indicesFormatFlag = 1;
                vertexIndices[0] = tempIndexHolder;
                fscanf(fin, "/%d %d//%d %d//%d\n",  &normalIndices[0], &vertexIndices[1],  &normalIndices[1], &vertexIndices[2], &normalIndices[2]);

            } 
            else{
                

                indicesFormatFlag = 2;
                vertexIndices[0] = tempIndexHolder;
                fscanf(fin, "%d %d\n", &vertexIndices[1], &vertexIndices[2]);

            }

        } 
        

        
        for(int ndx = 0; ndx < 3; ndx++){

            Indices.push_back(vertexIndices[ndx]);
        } 

    }
    
    else if(strcmp(leadingCode, "mtllib") == 0){
        fscanf(fin, "%s", mtlFilePath);    
    }

    else if(strcmp(leadingCode,))

    status = fscanf(fin, "%s", leadingCode);
  }





  return;
}
