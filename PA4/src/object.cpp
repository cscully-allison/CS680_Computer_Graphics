#include "object.h"


Object::Object()
{  
  std::string texturePath;


  std::cout << "Please input only the filename object you would like loaded (make sure it's in the \"assets\" folder) : ";
  std::cin >> texturePath;

  texturePath = "../assets/" + texturePath;

  loadTexture(texturePath, &Vertices);

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

  model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0, 1.0, -1.0));
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

  glm::vec3 vertexPoint;

  Vertex* fullVertex = new Vertex(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

  std::map<std::string, glm::vec3> colorMap;

  char leadingCode[200];
  char garbageStr[200];
  char mtlFilePath[200];
  char currentMtl[200];
  int status;

  

  int indicesFormatFlag = 0;  

  std::FILE* fin = fopen(filePath.c_str(), "r");

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

       geometry->push_back(*fullVertex);

    }
    
    else if(strcmp(leadingCode, "vn") == 0){

        fscanf(fin, "%s\n", garbageStr);

    }
    
    else if(strcmp(leadingCode, "f") == 0){
        int tempIndexHolder;        

        int vertexIndices[3];
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
        

        
        //apply color to vertex at index
        (*geometry)[vertexIndices[0]].color = colorMap[currentMtl];
        (*geometry)[vertexIndices[1]].color = colorMap[currentMtl];
        (*geometry)[vertexIndices[2]].color = colorMap[currentMtl];

        for(int ndx = 0; ndx < 3; ndx++){

            Indices.push_back(vertexIndices[ndx]);
        } 

    }
    
    else if(strcmp(leadingCode, "mtllib") == 0){
        fscanf(fin, "%s", mtlFilePath);
        fetchMaterial(mtlFilePath, &colorMap);
    }
   
    else if(strcmp(leadingCode, "usemtl") == 0){
        fscanf(fin, "%s", currentMtl);
    }


    status = fscanf(fin, "%s", leadingCode);
  }


  fclose(fin);  
  return;
}

void Object::fetchMaterial(std::string filePath, std::map<std::string, glm::vec3> *materials){
  std::ifstream fin;
  std::string leadingCode, tempKey;
  float r, g, b;

  filePath = "../assets/" + filePath;

  fin.open(filePath);

  if(!fin.good()){
    std::cout << "Error Opening Material File: Please make sure it is in the same folder as the obj file." << std::endl;
    return;
  }

  fin >> leadingCode;


  while(fin.good()){
    if(leadingCode == "#"){
      fin.ignore(1000, '\n');
    }
    else if(leadingCode == "newmtl"){
      fin >> tempKey;
    }
    else if(leadingCode == "Kd"){
      fin >> r  >> g >> b;
      (*materials)[tempKey] = glm::vec3(r,g,b);
    } else {
      fin.ignore(1000, '\n');
    }

    fin >> leadingCode;
  }

  fin.close();
  return;
}
