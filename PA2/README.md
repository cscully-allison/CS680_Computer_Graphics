# PA2: Interaction

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)


## Building and Running
The project will work using either CMake or the make instructions

### CMake Instructions
```
mkdir build
cd build
cmake ..
make
./Tutorial
```

### Makefile Instructions 
```
mkdir build
cd build
cp ../makefile .
make
./Tutorial
```

## Controller Instructions

Keyboard:
```
W: rotate cube clockwise
S: rotate cube conter clockwise
D: translate cube clockwise
A: translate cube counter clockwise
```

Mouse:
```
Left Click: Pause/Unpause cube rotation
Right Click: Pause/Unpause cube translation
```

