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
    void Render(vector <unsigned int>  keyPress);
    void collisionDetection(unsigned int dt);
    bool getGameState();
    int getScore();
    void highScore(int score);

    //void ScoreDisplay (int scoreVar, Object);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *gouraund_shader;
    Shader *phong_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint ball;

    GLint m_scalar;
    GLint m_spot;
    GLint m_spec;
    GLint m_height;

    glm::vec3 scalar;
    glm::vec3 spot;
    GLfloat height;


    uint score;

    bool gamestate;

    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif /* GRAPHICS_H */
