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
  view = glm::lookAt( glm::vec3(-30.0, 17.0, 0.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
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

void Camera::Update(int input){
   // glTranslatef(0, 0, 12);

   switch (input){
      //move camera left
      case 1073741904:
        view = glm::translate(view, glm::vec3(0.0,0.0,1.0));
      break;
      //move camera right
      case 1073741903:
       view = glm::translate(view, glm::vec3(0.0,0.0,-1.0));
      break;
      //move camera up
      case 1073741906:
        view = glm::translate(view, glm::vec3(-1.0,0.0,0.0));
      break;
      // move camera down
      case 1073741905:
        view = glm::translate(view, glm::vec3(1.0,0.0,0.0));
      break;
   }
}


