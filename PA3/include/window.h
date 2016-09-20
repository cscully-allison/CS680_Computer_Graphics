#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();

    ////RTEURN TO PRIVATE LATER/////
    SDL_Window* gWindow;


  private:
    SDL_GLContext gContext;
};

#endif /* WINDOW_H */
