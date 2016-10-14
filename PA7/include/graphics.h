#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <libxml++/libxml++.h>

using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

struct Planet
{
    string name;
    Object *planet;
    Object *moon[4];
    float proportionToEarth;
    float rotationRadius;
    float rotationSpeed;
    float orbitSpeedRatio;
    bool hasRings;
    float numRings;
    bool hasMoons;
    float numMoons;
};

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    void FileReader (const xmlpp::Node* node, int planet);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;


    Planet solarSystem[10];

};

#endif /* GRAPHICS_H */
