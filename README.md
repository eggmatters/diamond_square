# diamond_square
Qt OpenGL project showcasing dynamic landscape generation utlizing diamond square algorithm

# Overview
This is a Qt Project Utilizing OpenGL & Qt5 Creator to generate a random landscape utilizng the diamond square algorithm https://en.wikipedia.org/wiki/Diamond-square_algorithm
The landscape is generated across a variety of consraints to create varying terrains. Each terrain is tiled into seprate grids. 
The addition or subtraction of grids, as well as changing the constraints for terrains can be easily configured.

# Installation
This Installation is current for Ubuntu 17.04
## Prerequisites
* Qt5 Framework (installation steps found [here](https://wiki.qt.io/Install_Qt_5_on_Ubuntu))
* Qt5 Creator (Open Source edition found [here](https://www.qt.io/download)
* Ensure that you've installed the mesa-common-dev & libglu1-mesa-dev libraries (if Ubuntu)
In Qt5 Creator, you should be able to go to File->Open File or Project and navigat to the `diamond_square.pro` file in this repo.

The project should compile and build

# Terrain
The terrain is generated in 8 buffered segments. Only one segment is rendered initially. Also, only one texture is used. As you navigate the terrain, once you cross a threshold, the next sector will be rendered. 

## Navigation
Navigation is basic, 
`w` forward
`s` backwards
`a` strafe left
`d` strafe right
`arrow keys` turn left and right
`esc` Turn off normalization bounds checking

Pressing escape enables `s` to ascend `down arrow` to descend.
