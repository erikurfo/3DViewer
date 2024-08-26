# 3DViewer v2.0

## Introduction

3DViewer v1.0 is a program developed for visualizing 3D wireframe models. This versatile tool allows users to load 3D models, apply various transformations, customize visualization settings, and even save images and screencasts. The program is developed in the C language following the C11 standard and can be built with a Makefile.

<img src="./viewer.png">

## Features

- GUI
- Loading of a wireframe model from an .obj file (supports only a list of vertices and surfaces)
- Moving the model by a specified distance relative to the X, Y, Z axes
- Rotating the model by a given angle relative to its X, Y, Z axes
- Scaling the model by a given value
- Selection of a projection type (parallel or central)
- Choosing the way the edges are displayed (solid or dotted)
- Choosing the way to display vertices (circle, square or not displayed)
- Changing the thickness of the edges
- Changing the size of the vertices
- Changing the color of the background, edges and vertices
- Saving the model image in .jpeg and .bmp format
- Saving a GIF image of the model in .gif format (640x480, 10fps, 5s)


## Dependencies

Make sure you have the following packages installed:

- 'GCC'
- 'G++'
- 'MAKE'
- 'CMAKE'
- 'QT5' and later
- 'OpenGL API'

## Installation

- From src folder, run the following command (executable file will be located in the src folder)

```sh
make install
```

## File compression

- From src folder, run the following command (the archive will be located in the src folder)

```sh
make dist
```

## Uninstallation

- From src folder, run the following command

```sh
make uninstall
```