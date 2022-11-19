Surface of Revolution Visualizer
--------------------------------
This is a OpenGL program where you can find the 3D surface that is generated
when you rotate the graph of a function that is ploted on the 2D plane around
the y-axis.

You can generate some pretty interesting shapes this way and to edit the 
function that gets revolved, merely edit "res/shaders/3d.vert" and change
the return value of "fx".

Controls
--------
Move mouse around to rotate the camera.
Use W/A/S/D to move around
Space/Shift to move up/down respectively
Use ESC to release mouse cursor/regrab mouse cursor
1 - view the final 3D shape
2 - view the original 2D graph
3 - view the animation of the 2D graph being rotated around the y-axis

Credits
-------
Libraries used:
 - GLFW (https://glfw.org)
 - glm (https://github.com/g-truc/glm)
 - glad (https://github.com/Dav1dde/glad)

License
-------
Code is public domain, you may use it for any purpose you wish.

Project Ideas
-------------
These are some things that I considered doing but was too lazy to implement
so if you have knowledge of OpenGL/C++, feel free to try to do these for fun.

 - Add support for rotation around the x axis
 - Add support for rotation around an axis that is not the y or x axis
 - Add support for texture coordinates so the final 3D shape can be textured
 - Add visualization for how one might calculate the volume of the 3D shape
 that is generated using integrals
 - Have multiple surfaces of revolution be displayed at once
 - Reimplement this program in a different programming language that you like
 - Create a program that can generate a surface of revolution's points but
 then export it as an .obj file/3D model 
