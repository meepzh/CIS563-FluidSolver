# CIS563-FluidSolver #
### How Does It Work ###
This fluid solver uses GLFW, GLM, and GLEW. It reads a JSON scene file,
creating a fluid container and a fluid source. The fluid generates particles
once, then all the particles are subject to fluid forces (currently gravity).
Upon exiting the fluid container, particles are detected by a collision
detection system which colors the particles appropriately. The scene is
initially paused. The camera is controlled by a standard arcball interface
described below.

### How To Use ###
Press P to toggle pausing.

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
For some reason, when I enable color, the box wireframes just disappear. If you
take out the `in_Color` from particle.frag.glsl, the wireframe will appear
again.

### Extra Features ###
None

### Credits ###
- GLFW Window adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/) [(WTF Public License)](http://www.wtfpl.net/).
- Compiling GLSL shaders from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/) [(WTF Public License)](http://www.wtfpl.net/).
- Some OpenGL specifics on rendering particles from [opengl-tutorial.org](http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/) [(WTF Public License)](http://www.wtfpl.net/).
- DDS File Loader adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/) [(WTF Public License)](http://www.wtfpl.net/).
- 2-line implementation for reading text files from [Tyler McHenry](http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring).
- Organization of the camera class from CIS460 staff.
- Transform class adapted from CIS460 staff.
- Organization of shaderProgram/drawable classes from CIS277 staff.
- Wire shader from CIS277 staff.