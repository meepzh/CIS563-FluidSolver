# CIS563-FluidSolver #
### How Does It Work ###
This fluid solver uses GLFW, GLM, and GLEW. It reads a JSON configuration file
and a JSON scene file, creating a fluid container and a scene container. The
fluid generates particles once, then all the particles are subject to fluid
forces (currently none). The scene is initially paused. The camera is controlled
by a standard arcball interface described below. The solver currently set to
display the neighbor particles of the first particle, which can be changed
randomly. The solver can also export the backend grid for the fluid particles to
the VDB format.

### Build Instructions (*nix) ###
By default, the program is set to default debugging mode as set in
`src/MFluidSolverConfig.hpp.in`. There are many other options there including
logging settings and defaults, but note that defaults will be overwritten by
the configuration file.

    mkdir build
    cd build
    cmake ..
    make
    ./MFluidSolver

### Unit Test Instructions (*nix) ###
This will run all tests in the unittest folder. Test results will be printed
to stdout, and details are found in `build/Testing/Temporary/LastTest.log`.
This does include a brief performance test on neighbor search.

    make test

### How To Use ###
Press P to toggle pausing. Press Press N to randomly pick a particle to show
its neighbors. Press E to export a VDB file of the graph to `export.vdb` (if
OpenVDB is enabled in `MFluidSolverConfig`). Press R to reseed the scene
using the fluid source. Press right to step frame by frame (0.001 seconds).

All SPH parameters, as well as several general program parameters, can be found
in `config/config.json`. The scene file, which contains dimensions and particle
separation, can be edited from `scene/scene.json`. Note the following
case-insensitive options available for several parameters:

neighborSearchType

- naive
- uniform
- indexSortedUniform
- zIndexSortedUniform
- zIndexSortedUniformInsertionSort

visualization

- neighbors
- none
- velocity

The `numUpdates` parameter can be used to set the number of frames to simulate
in order to get accurate and comparable performance ratings. Set `numUpdates`
to a non-positive number to disable this feature. Note that the
`RECORD_PERFORMANCE` configuration needs to be enabled for this feature.

#### Arcball ####
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

### Neighbor Search Performance ###
![Exponential Improvement with Uniform Grid over Naive Neighbor Search](images/nsPerfGraph0.png?raw=true "The uniform grid neighbor search dramatically reduces search time to O(n).")

### Missing Required Features ###
None

### Extra Features ###
None

### Credits ###
- DDS File Loader adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/) [(WTF Public License)](http://www.wtfpl.net/).