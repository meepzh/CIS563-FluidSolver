# CIS563-FluidSolver #
### How Does It Work ###


### How To Use ###
The camera uses the arcball interface and can be controlled with the left and
right mouse buttons and the scroll wheel.

The left mouse button is handled by an arcball interface. Clicking and dragging
across the center of the GL widget rotates the camera around its focus point in
the X and Y directions. Clicking and dragging in a circular motion around the
outer edges of the GL widget rotates the camera around its focus point in the Z
direction. The movements of the camera should follow the movements of the mouse
intuitively.

The scroll wheel zooms the camera. Scrolling up zooms out and scrolling down
zooms in.

The right mouse button pans the camera. This changes both the camera's and its
focus point's position. Right-click and drag to move the camera in the opposite
direction of the mouse. It will look like you're dragging the scene with you.

### Missing Required Features ###


### Extra Features ###


### Credits ###
Credit for adapted code to create a GLFW window: [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/).
Credit to CIS460's camera class organization. Majority of implementation from my own CIS277 project.
Credit to compiling GLSL shaders: [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/).
Credit to CIS277's shader/drawable class organization. Implementation of shader program class vaguely influenced by [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/).
Credit to the CIS277 wire shader.
Credit for reading in shader files to [Tyler McHenry](http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring).