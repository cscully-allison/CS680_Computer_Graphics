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
    void Update(unsigned int dt, vector <unsigned int>  keyPress, int force);
    void Render();
    int collisionDetection(unsigned int dt);
    void reinitateBall();

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
    Object *m_leftFlipper;
    Object *m_rightFlipper;
    Object *m_bump1;
    Object *m_bump2;
    Object *m_bump3;
    vector <Object*> numbers;
    vector <Object*> ScoreArray;
    vector <Object*> BallNum;
    glm::vec4 pos;

    btRigidBody* ceilingBody;

    uint score;
    int ballsLeft;
    bool resetable;
    bool ballCleared;
    int count1, count2, count3;

    btScalar leftLower;
    btScalar leftUpper;
    btScalar rightLower;
    btScalar rightUpper;

    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif /* GRAPHICS_H */
