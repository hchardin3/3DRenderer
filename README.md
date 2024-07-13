# 3DRenderer

The goal of this project is to create a C++-based 3D renderer that allows users to create a scene with a camera, custom lights and several objects, and to render the scene.

## Custom objects

### Camera

The camera is assumed to be initialized at the origin of the scene, facing towards the y-axis. However, it can be moved and rotated at the user's will.

### Triangle

This is a basic triangular mesh, made of three points and a normal.

### Scene

This is the whole scene, that contains one camera and several meshes.


## Custom commands

These are the custom commands to use our renderer:

    make && ./main
