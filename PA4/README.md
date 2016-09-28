# PA4: Texture Loader

## Connor Scully-Allison

# Building and Running

In the PA4 Directory type in the following order:

```
   mkdir build
   cp makefile build/
   cd build/
   make
   ./texture_loader 
```

# Operation Instructions

## Before Running

```
	1) Make sure that any .obj files you intend to load are placed in the asset folder
	2) Ensure that thier .mtl files are located in the asset folder as well
	3) A .obj has been uploaded to the github for quick testing (pinballbox.obj)
```

## Upon Running

```
1) The window will hang as a prompt appears in the terminal
2) Put in only the filename with the .obj extension (the program will handle the pathing to the assets folder)
3) The program will continue normally upon input
```

#Misc Notes

## The .mtl loader

Presently the program sucessfully retrieves all the colors in the MTL file.
Unfortunately I have only been able to sucessfully map 1 color to a given rendered object.

When I comment out some of my mappings (lines 162 - 169 in object.cpp) the
blue is visible on the bottom in clipped triangular lines. 

I suspect that any mappings I'm making to a specific vertex are being overwritten as color
is re-ascribed to it later for a different face. I was unable to solve this problem, however
all other aspects of the .mtl loader work.

