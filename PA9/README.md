# PA9: Lighting

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

In the PA9 Directory type in the following order:

```
mkdir build
cd build
cmake ..
make
./Tutorial
```

# Operation Instructions
```
Input Commands:
Mouse Movement : moves box around table
Esc : exits the program
	Numpad Controls
	+ and - : controls world ambient brightness
	* and / : controls the spotlight ambient brightness
	6 and 9 : controls spotlight size
	0 and . : controls table specularity
	1 and 2 : controls cylinder specularity
	4 and 5 : controls ball specularity
	7 and 8 : controls cube specularity
``` 
## Before Running

```
1) Make sure that any .obj files you intend to load and the config.txt are placed in the asset folder
2) Ensure that the associated .mtl files to the objects are located in the asset folder as well
```

# Misc Notes


