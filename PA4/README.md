# PA2: Interaction

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)


## Building and Running
The project will work using CMake instructions


### CMake Instructions
```
mkdir build
cd build
cmake ..
make
./PA3
```

## Controller Instructions

### Moon Instructions:

Keyboard:
```
W: rotate moon clockwise
S: rotate moon conter clockwise
D: translate moon clockwise
A: translate moon counter clockwise
```

### Planet Instructions:

Keyboard:
```
Up Arrow: rotate moon clockwise
Down Arrow: rotate moon conter clockwise
Right Arrow: translate moon clockwise
Left Arrow: translate moon counter clockwise
```
Mouse:
```
Left Click: Pause/Unpause cube rotation
Right Click: Pause/Unpause cube translation
```



