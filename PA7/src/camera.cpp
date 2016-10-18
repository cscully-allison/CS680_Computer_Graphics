#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, -400.0, 8.0), //Eye Position
                      glm::vec3(0.0, 1.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 1000.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

bool Camera::Update(int userInput){
  //std::cout << userInput << std::endl;
  switch(userInput){
    case FORWARD: //w
      //view *=  glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.5, 0.0, 0.0));
      view *= glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-0.5));
      break;
    case BACK: //s
      view *= glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,0.5));
      break;
    case LEFT: //a
      view *= glm::translate(glm::mat4(1.0), glm::vec3(0.5,0.0,0.0));
      break;
    case RIGHT: //d
      view *= glm::translate(glm::mat4(1.0), glm::vec3(-0.5,0.0,0.0));
      break;
    case UP: //LMB
      view *= glm::translate(glm::mat4(1.0), glm::vec3(0.0,-1.1,0.0));
      break;
    case DOWN: //RMB
      view *= glm::translate(glm::mat4(1.0), glm::vec3(0.0,1.1,0.0));
      break;
    case ROTATELEFT: //q
      view *=  glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0, 0.0, 5.0));
      break;
    case ROTATERIGHT: //e
      view *=  glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0, 0.0, -5.0));
      break;
  }

}

