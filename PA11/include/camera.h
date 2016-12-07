#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec3 GetPosition();
    void lookAt(glm::vec3 spotToLook, glm::vec3 cameraPos);;
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPosition;
};

#endif /* CAMERA_H */
