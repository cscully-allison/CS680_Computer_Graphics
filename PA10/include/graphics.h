#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;
#include <btBulletDynamicsCommon.h>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, float mouseX, float mouseY);
    void Render();
    void collisionDetection();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint ball;

    Object *m_table;
    Object *m_ball;

    glm::vec4 pos;


    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif /* GRAPHICS_H */
