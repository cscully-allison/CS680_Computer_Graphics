#include <fstream>
#include "object.h"

Object::Object(std::string file)
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

  if (loadOBJ (file)){
  
    // The index works at a 0th index
    for(unsigned int i = 0; i < Indices.size(); i++)
    {
        Indices[i] = Indices[i] - 1;
    }

    rotateAngle = 0.0f;
    translateAngle = 0.0f;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  }
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

bool Object::loadOBJ (std::string file){
  // open Object file
  std::ifstream fin("../objects/" + file);
  char temp;  
  fin >> temp;

  // read in code til end of file. 
  if (fin){
      while (!fin.eof()){
          fin >> temp;
          // comment, ignore
          if (temp == '#'){
                std::string line;
                std::getline (fin, line);
          }
          // There should currently be only 1 object per file, so ignore
          else if (temp == 'o'){
            std::string line;
            std::getline (fin, line);
          }
          // get vertices
          else if (temp == 'v') {
            int t = fin.peek();
            // check if texture vertice
            // do nothing for right now
            if (t == 't'){
                std::string line;
                std::getline (fin, line);
            }
            // check if vertex normal
            // Im not sure what to do with these yet
            else if ( t == 'n'){
                std::string line;
                std::getline (fin, line);
            }
            glm::vec3 vertex;
            glm::vec3 color;
            fin >> vertex.x >> vertex.y >> vertex.z;
            Vertex temp (vertex,color);
            Vertices.push_back (temp);   
          }
          // idk what to do with the smoothing int
          else if (temp == 's'){
            std::string line;
            std::getline (fin, line);
            std::cout << line;
          }
          // get Indices
          else if (temp == 'f'){
            int x, y,z;
            fin >> x >> y >> z;
            Indices.push_back (x);
            Indices.push_back (y);
            Indices.push_back (z);
          }
          // material library
          else if (temp = 'm'){
            
            std::string line;
            std::getline (fin, line);
          }
          
         // material name
         else if (temp = 'u'){            
            std::string line;
            std::getline (fin, line);
          }  

      }
      return true;
  }
  
    // if file is unable to open, output error
  else{
    std::cerr << "Error opening object file" << file << std::endl;
    return false;
  }
    
  // close the file  
  fin.close();
}

void Object::Update(unsigned int dt, unsigned int pressedKey, glm::mat4 planet)
{
  // move the model in a circle
  // The multiplication makes it go in a wider circle
  switch (pressedKey){
      // A
      // translates the square counter clockwise
      case 97:
          direction.y = -1;
          break;
     // D
     // translates the square clockwise
      case 100:
          direction.y = 1;
        break;
     // W
     // rotates square counter clockwise
      case 119:
            direction.x = -1;
          break;
     // S
     // rotates square clockwise
      case 115:
            direction.x = 1;
          break;
// moon does not pause
//       case 1:
//           if (direction.x == 0)
//               direction.x = 1;
//           else
//             direction.x = 0;
//           break;   
//       case 3:
//           if (direction.y == 0)
//               direction.y = 1;
//           else
//             direction.y = 0;
//           break;
//       default:
//       break;
    }
    
    rotateAngle += direction.x * dt * M_PI/500;
    translateAngle += direction.y * dt * M_PI/500;
    
    glm::vec4 position = planet * glm::vec4 (1.0,1.0,1.0,1.0);
    glm::mat4 planetModel = glm::mat4( 1.0 );
    planetModel[3] = position;

    model = glm::translate (planetModel, glm::vec3(5*cos (translateAngle), 0.0f , 5*sin (translateAngle)));
    model = glm::rotate(model, (rotateAngle), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale (model, glm::vec3(0.5, 0.5, 0.5));
}


void Object::Update(unsigned int dt, unsigned int pressedKey)
{
  // move the model in a circle
  // The multiplication makes it go in a wider circle
  switch (pressedKey){
      // SDLK_LEFT
      // translates the square counter clockwise
      case 1073741904:
          direction.y = -1;
          break;
     // SDLK_RIGHT
     // translates the square clockwise
      case 1073741903:
          direction.y = 1;
        break;
     // SDLK_UP
     // rotates square counter clockwise
      case 1073741906:
            direction.x = -1;
          break;
     // SDLK_DOWN
     // rotates square clockwise
      case 1073741905:
            direction.x = 1;
          break;
     // left key press
     // un/pauses sqaure rotation
      case 1:
          if (direction.x == 0)
              direction.x = 1;
          else
            direction.x = 0;
          break;
     // right key press
     // un/pauses square translation     
      case 3:
          if (direction.y == 0)
              direction.y = 1;
          else
            direction.y = 0;
          break;
      default:
      break;
    }
    
    
    rotateAngle += direction.x * dt * M_PI/1000;
    translateAngle += direction.y * dt * M_PI/1000;
    
    model = glm::translate (glm::mat4(1.0f), glm::vec3(8*cos (translateAngle), 0.0f , 8*sin (translateAngle)));
    model = glm::rotate(model, (rotateAngle), glm::vec3(0.0, 1.0, 0.0));
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

