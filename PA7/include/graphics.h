#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>

using namespace std;

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
    void Update(unsigned int dt, int userInput);
    void Render();
    // void FileReader (const xmlpp::Node* node, int planet);
    void FileReader ();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;


    Planet solarSystem[10];
    int chosenPlanet = 0;
    char type = 'o';
    float storedValues [10];

};

#endif /* GRAPHICS_H */
