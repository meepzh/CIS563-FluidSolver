# MFluidSolver #
### How Does It Work ###
This fluid solver uses GLFW, GLM, and GLEW. It reads a JSON configuration file
and a JSON scene file, creating a fluid container and a scene container. The
fluid generates particles once, then all the particles are subject to fluid
forces (currently none). The scene is initially paused. The camera is controlled
by a standard arcball interface described below. The solver currently set to
display the neighbor particles of the first particle, which can be changed
randomly. The solver can also export the backend grid for the fluid particles to
the VDB format.

### Dependencies ###

- OpenVDB 2.3.0
- Boost (Filesytem, System, Unit Test Framework)

### Build Instructions (*nix) ###
By default, the program is set to default debugging mode as set in
`src/MFluidSolverConfig.hpp.in`. There are many other options there including
logging settings and defaults, but note that defaults will be overwritten by
the configuration file `config/config.json`.

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
Press S to write out a screenshot to `screenshot_%05d.tga`.

All SPH parameters, as well as several general program parameters, can be found
in `config/config.json`. The scene file, which contains dimensions and particle
separation, can be edited from `scene/scene.json`. Note the following
case-insensitive options available for several parameters:

neighborSearchType

- naive
- uniform
- indexSortedUniform
- indexSortedUniformInsertionSort
- zIndexSortedUniform
- zIndexSortedUniformInsertionSort

visualization
- type
    - neighbors
    - none
    - pressure
    - velocity
    - velocityDir
- maxPressure
- maxVelocity
- velocityColor (8-bit rgb array)

The `numUpdates` parameter can be used to set the number of frames to simulate
in order to get accurate and comparable performance ratings.

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

### Neighbor Search Query Performance ###
CPU: Intel i7-4800MQ limited to 2.4GHz<br />
GPU: NVIDIA GeForce GTX 765M

![Exponential Improvement with Uniform Grid over Naive Neighbor Search](images/nsPerfGraph1.png?raw=true "The uniform grid neighbor search dramatically reduces search time to O(n).")

- Index Sorted Uniform Grid search dramatically improves on the uniform grid,
likely because access to the particles is very simple without memory offsets
due to other grid cells.
- However, Z Index Sorted Uniform Grid search appears to suffer from its
computation of the Z index, which takes a number of operations. My system also
may not have enough cache to take advantage of the spatial locality. Caching of
the Z indices was attempted, but proved to be about the same.
- Insertion sort significantly improves query time, however, compared to the
standard std::sort, as particles don't change cells frequently.

### Missing Required Features ###

- IISPH
- Test bicubic spline kernel
- Performance Analysis
- Optional: Cache neighbor distances

### Extra Features ###
None

### Credits ###
- [Nuparu Dependency and Foundation Libraries](https://github.com/betajippity/Nuparu)
- DDS File Loader adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/) [(WTF Public License)](http://www.wtfpl.net/).