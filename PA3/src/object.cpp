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

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle = 0.0f;
  angle_r = 0.0f; //inlcuded for rotation angle

  coef = coef_r = 1;

  transDirection = 0;

  paused = false;

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

void Object::Update(unsigned int dt, int key_press_val)
{
    Update(dt, key_press_val, glm::mat4(1.0));
}

void Object::Update(unsigned int dt, int key_press_val, glm::mat4 ptOfOrbt)
{
     int scalar = 8;
     std::cout << key_press_val << std::endl;
      switch(key_press_val){
        case 32: //spacebar
            paused = true;
            key_press_val = 0;
            break;

        case 999: //unpause
            paused = false;
            break;

        case 100: //"d"
            transDirection = 0;
            break;
            
        case 97: //"a"
            transDirection = 1;
            break;
        
        case 119: //"w"
            coef_r = 1;
            break;

        case 115: //"s"
            coef_r = -1;
            break;

        case 200:
            t_pause = true;
            break;

        case 201:
            t_pause = false;
            break;

        case 202:
            r_pause = true;
            break;

        case 203:
            r_pause = false;
            break;
    }
      
     //only update the angle if not paused
     if(!paused && !t_pause){
      if(transDirection == 0){ 
        angle += dt * M_PI/1000;
      }
      else{
        angle -= dt * M_PI/1000;
      }
     }

     if(!paused && !r_pause){
        angle_r += dt * M_PI/60000;
     }
    
    //translate model first
    //then rotate
    //model = glm::translate(ptOfOrbt, glm::vec3(glm::cos(angle)*scalar, 0, glm::sin(angle)*scalar));
    model = glm::rotate(model, angle_r*coef_r, glm::vec3(0.0, 1.0, 0.0));


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



/*Moon Object-------------------
--------------------------------
--------------------------------*/

/**
  * Polymorphic update function sets particular orbit of the moon
  * 
  */
void Moon::Update(unsigned int dt, int key_press_val, glm::mat4 ptOfOrbt){

    int scalar = 6;

     switch(key_press_val){
      case 106: //"J"
            transDirection = 0;
            break;
            
        case 108: //"L"
            transDirection = 1;
            break;
        
        case 107: //"k"
            coef_r = 1;
            break;

        case 105: //"I"
            coef_r = -1;
            break;
    }

    if(transDirection == 0){
      angle += dt * M_PI/400;
    }else{
      angle -= dt * M_PI/400;
    }

    angle_r += dt * M_PI/600;


 
    //perform necessary reductions
    //to unbind rotation from position of moon
    glm::vec4 staticPos = ptOfOrbt * glm::vec4(1.0, 1.0, 1.0, 1.0);
    ptOfOrbt = glm::mat4(1.0);
    ptOfOrbt[3] = staticPos;

    //translate model first
    //then rotate
    model = glm::translate(ptOfOrbt, glm::vec3(8.0f, 0.0f, 8.0f));
    //model = glm::rotate(model, angle_r*coef_r, glm::vec3(0.0, 1.0, 0.0));
    //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));


}
