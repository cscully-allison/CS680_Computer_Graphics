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
    void Render(int keyboardInput);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    Shader *gouraund_shader;
    Shader *phong_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_scalar;
    GLint ball;

    glm::vec3 scalar;

    Object *m_table;
    Object *m_ball;
    Object *m_cylinder;
    Object *m_cube;
    glm::vec4 pos;

    btCollisionShape* ground;
    btCollisionShape* southWall;
    btCollisionShape* northWall;
    btCollisionShape* eastWall;
    btCollisionShape* westWall;
    btCollisionShape* cylinder;
    btRigidBody* groundBody;
    btRigidBody* southWallBody;
    btRigidBody* northWallBody;
    btRigidBody* eastWallBody;
    btRigidBody* westWallBody;
    btRigidBody* cylinderBody;



    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;




};

#endif /* GRAPHICS_H */
