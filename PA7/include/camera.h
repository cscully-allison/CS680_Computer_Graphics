#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
enum cameraControl{
	FORWARD = 119,
	BACK = 115,
	RIGHT = 100,
	LEFT = 97,
	UP = 1,
	DOWN = 3,
	// ROTATEUP = 888,
	// ROTATEDOWN = 222,
	 ROTATERIGHT = 113,
	 ROTATELEFT = 101
};


class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    bool Update(int userInput);
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
