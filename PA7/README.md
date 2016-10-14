# PA7: Solar System

## Connor Scully-Allison
## Hannah Munoz
## Kurt Andersen

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

This project uses Assimp 3.2. Instructions for downloading and running Assimp can be found at [Main Downloads](http://www.assimp.org/main_downloads.html) 

This project uses ImagicMagick 6.8.9-9. Instructions for downloading and running ImageMagick can be found at [Install Source](http://www.imagemagick.org/script/install-source.php)

This project uses libxml++ 2.6. Instructions for downloading and running libxml++ can be found at [Manual](http://libxmlplusplus.sourceforge.net/docs/manual/html/) 
libxml++ uses gtkmm which can be downloaded at [Download](http://www.gtkmm.org/en/download.html)

# Building and Running

In the PA7 Directory type in the following order:

```
mkdir build
cd build
cmake ..
make
./SolarSystem
```

# Operation Instructions

## Before Running

```
	1) Make sure that any .obj files you intend to load are placed in the asset folder
	2) Ensure that thier .mtl files are located in the asset folder as well
	3) A .obj has been uploaded to the github for quick testing (capsule.obj)
```

## Upon Running

```
1) The window will hang as a prompt appears in the terminal
2) Put in only the filename with the .obj extension (the program will handle the pathing to the assets folder)
3) The program will continue normally upon input
```

# Misc Notes


