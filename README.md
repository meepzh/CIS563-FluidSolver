# MFluidSolver #

IISPH Demo: [Vimeo](https://vimeo.com/groups/cis563/videos/164437486)

![IISPH Cube in Cube Snapshot](images/cube-iisph-sprint1_0.png?raw=true "IISPH Cube in Cube snapshot.")

### How Does It Work ###
This fluid solver uses GLFW, GLM, and GLEW. It reads a JSON configuration file
and a JSON scene file, creating a fluid container and a scene container. The
fluid generates particles once, then all the particles are subject to fluid
forces (currently none). The scene is initially paused. The camera is controlled
by a standard arcball interface described below. The solver currently set to
display the neighbor particles of the first particle, which can be changed
randomly. The solver can also export the backend grid for the fluid particles to
the VDB format. The solver runs both SESPH and IISPH.

### Dependencies ###

- Boost (Filesytem, System, Unit Test Framework)
- OpenVDB 2.3.0

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
in `config/config.json`. The scene file, which contains dimensions, particle
parameters, camera settings, and particle spawn information, can be edited from
`scene/scene.json`. Parameter names are case sensitive, but options are not. If
you change parameters outside of the build folder, you will need to rerun
`cmake ..`. You can avoid this issue by editing the files in the build folder.
Note the following case-insensitive string options available for several
parameters:

neighborSearchType
- naive
- uniform
- indexSortedUniform
- indexSortedUniformInsertionSort
- zIndexSortedUniform
- zIndexSortedUniformInsertionSort

spawnMethod
- jittered
- poissonDisk
- uniform

visualization
- type
    - density (set maxDensityDifference)
    - index
    - neighbors
    - none
    - particle (set particleId)
    - pressure (set maxPressure)
    - velocity (set maxVelocity)
    - velocityDir
- maxDensityDifference
- maxPressure
- maxVelocity
- particleId
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

### IISPH Simulation Performance ###
CPU: Intel i7-4800MQ limited to 2.7GHz<br />
GPU: NVIDIA GeForce GTX 765M<br />
Compiler: GCC 5.3.0<br />
Measured in seconds per frame. Note that this includes time spent updating the
viewer and logging input events.

![Graph of the simulation performance for different neighbor search types](images/nsPerfGraph2.png?raw=true "Index sorting improves performance without forced insertion sort.")

- The performance gain from index sorting is a signficant improvement over a
standard uniform grid, especially when insertion sort is not forced.
- The fact that the uniform grid uses the Y axis last (i.e.: change in the
Y-coordinate causes the greatest index change) may play a role in making uniform
grids comparable to Z-indexed grids, as fluids tend to stay within the XZ-plane.
It would be interesting to explore this further.
- Z-indexing may be more beneficial in cases where the XZ-plane is large (i.e.:
when there are larger index changes in the uniform grid), as it is in the Cube
in Cube scene.
- Index sorted grids sort every frame with std::sort, which is a hybrid of
introsort and insertion sort in [GCC](https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.4/a01027.html).
This likely explains the deficiencies of using insertion sort only.
- Insertion sort may also be less effective in IISPH where timesteps can be
larger than in traditional SESPH, so particles travel much farther and therefore
change indices much more frequently.

### Missing Required Features ###

- Test Partio point export
- OpenVDB level set export
- Optional: Update derivatives
- Optional: Ihmsen boundary conditions
- Optional: Cache neighbor distances
- Optional: Better seeding (poisson distribution)
- Optional: Test bicubic spline kernel
- Optional: Solid body coupling
- Optional: Headless mode
- Optional: Refactor IISPH because update() is way too hard to read
- Optional: Flyaway prevention

### Extra Features ###
None

### Credits ###
- [Nuparu Dependency and Foundation Libraries](https://github.com/betajippity/Nuparu)
- DDS File Loader adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/) [(WTF Public License)](http://www.wtfpl.net/).