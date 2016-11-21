# PA10: Bullet/Pinball

## Connor Scully-Allison
## Hannah Munoz
## Kurt Andersen

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

This project uses Assimp 3.2. Instructions for downloading and running Assimp can be found at [Main Downloads](http://www.assimp.org/main_downloads.html) 

This project uses ImagicMagick 6.8.9-9. Instructions for downloading and running ImageMagick can be found at [Install Source](http://www.imagemagick.org/script/install-source.php)

This project uses Bullet 2.86. Instructions for downloading and running Bullet can be found at [Releases](https://github.com/bulletphysics/bullet3/releases)

# Building and Running

In the PA10 Directory type in the following order:

```
mkdir build
cd build
cmake ..
make
./Haramball
```

# Operation Instructions

## Before Running

```
1) Make sure that any .obj files you intend to load and the config.txt are placed in the asset folder
2) Ensure that the associated .mtl files to the objects are located in the asset folder as well
```

## Controls

```
Spacebar - press and hold to add force to launch the ball, release to launch ball
Left shift – moves left paddle, press and hold to stay in up position
Right shift – moves right paddle, press and hold to stay in up position
Esc – Quits the game.
```

Camera Controls
```
Left Arrow – pans the camera to the left.
Right Arrow – pans the camera to the right.
Up Arrow – pans the camera upwards.
Down Arrow – pans the camera downwards.
'r' – reset camera position.
```

Shader Controls:
```
'p' – switch to Phong shading.
'g' – switch to Gourand shading.
```
Numpad Shader Controls:
```
'0' – increases specularity on table. 
'.' - decreases specularity on table. 
'1' – increases bumper specularity.
'2' – decreases bumper specularity.
'4' – increases ball specularity. 
'5' – decrease ball specularity. 
'7' – increases flipper specularity.
'8' – decrease flipper specularity.
'6' – decrease spotlight height. 
'9' – increase spotlight height. 
'+' - increase ambient lighting. 
'-' - decrease ambient lighting. 
'*' -  increase spotlight ambient lighting. 
'/' - decrease spotlight ambient lighting.
```

# Misc Notes



