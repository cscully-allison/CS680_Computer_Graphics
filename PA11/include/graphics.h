#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;
#include <btBulletDynamicsCommon.h>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "tankAI.h"
#include "UserTank.h"
#include "health.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, vector <unsigned int>  keyPress, int mouseMovement, int launch);
    void Render(vector <unsigned int>  keyPress);
    void collisionDetection(unsigned int dt);
    bool getGameState();
    int getScore();
    void highScore(int score);
    GLint GetModelMatrix ();
    unsigned int getTime();
    void increaseTime(unsigned int dt);

  private:
    std::string ErrorString(GLenum error);
    unsigned int gameTime;

    Camera *m_camera;
    Shader *gouraund_shader;
    Shader *phong_shader;
    TankAI *m_AI;
    UserTank *m_user;
    Health *m_health;


    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint ball;

    Object *m_land;
    Object *m_sky;
    Object *terrian_objects[20];

    Uniform scalar;
    Uniform spotlight;
    Uniform specularity;
    Uniform spotlightHeight;
    Uniform eyePos;
    
    uint score;

    bool gamestate;
    bool printOnce;

    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif /* GRAPHICS_H */
