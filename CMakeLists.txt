########################
#     MFluidSolver     #
########################

# Credit - base CMake config: Yining Karl Li, edited CMake config: Akshay Shah & Debanshu Singh

# Name your project
project(MFluidSolver)
cmake_minimum_required(VERSION 2.8)

# Versioning - https://cmake.org/cmake-tutorial/
set(MFluidSolver_VERSION_MAJOR 1)
set(MFluidSolver_VERSION_MINOR 0)
configure_file(
    "${PROJECT_SOURCE_DIR}/src/MFluidSolverConfig.hpp.in"
    "${PROJECT_BINARY_DIR}/MFluidSolverConfig.hpp"
)
include_directories("${PROJECT_BINARY_DIR}")

# Copy GLSL, JSON, and Texture files
file(GLOB CONFIG_FILE "${PROJECT_SOURCE_DIR}/config/*")
file(COPY ${CONFIG_FILE} DESTINATION "${PROJECT_BINARY_DIR}/")
file(GLOB GLSL_SRC "${PROJECT_SOURCE_DIR}/glsl/*.glsl")
file(COPY ${GLSL_SRC} DESTINATION "${PROJECT_BINARY_DIR}/glsl/")
file(GLOB JSON_SRC "${PROJECT_SOURCE_DIR}/scene/*.json")
file(COPY ${JSON_SRC} DESTINATION "${PROJECT_BINARY_DIR}/scene/")
file(GLOB TEXTURE_SRC "${PROJECT_SOURCE_DIR}/texture/*")
file(COPY ${TEXTURE_SRC} DESTINATION "${PROJECT_BINARY_DIR}/texture/")

# Set creates a variable
set(NUPARU "nuparu")
# Add include and src directories to path
include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${NUPARU}/include
        ${NUPARU}/src
)

# Add path for pre-compiled libraries here (we will later link them with our compiled source)
# Add Nuparu library to path for OSX, Linux and Windows
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/osx)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/linux /usr/lib64)
elseif(WIN32)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/win)
endif()

# Set include variables
set(GLFW_LIBRARY_DIR ${CMAKE_LIBRARY_PATH})
set(GLEW_LIBRARY_DIR ${CMAKE_LIBRARY_PATH})
set(GLFW_INCLUDE_DIR ${NUPARU}/include)
set(GLEW_INCLUDE_DIR ${NUPARU}/include)

# Use find_package & find_library to link with
find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW)
find_library(GLFW_LIBRARY "glfw3")
find_library(JSONCPP "jsoncpp")
find_library(LIBZ "z")
#find_library(OPENVDB "openvdb")
find_library(OPENEXR_HALF NAMES Half PATHS ${CMAKE_LIBRARY_PATH})
#find_library(PARTIO "partio")
find_library(TBB "tbb")

include_directories(${Boost_INCLUDE_DIRS})

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
        -DBOOST_TEST_DYN_LINK
)

set(CORE_LIBS ${GLFW_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
    ${JSONCPP} ${LIBZ} ${OPENVDB} ${OPENEXR_HALF} ${PARTIO} ${TBB})

# OSX-specific hacks/fixes
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #Link IOKit because this is where we get GL stuff for OSX
    set(COCOA "-framework Cocoa")
    set(COREVIDEO "-framework CoreVideo")
    set(IOKIT "-framework IOKit")
    set(CORE_LIBS ${CORE_LIBS} ${COCOA} ${IOKIT} ${COREVIDEO})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Linux specific hacks/fixes
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lX11 -lXxf86vm -lXrandr -lpthread -lXi")
endif()

# Set compiler flags for c++11
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -m64 -msse2 -w")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

if(MSVC)
    set(COMPILER_FLAGS
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
        )
endif()

# Add source files you want to compile (.cpp)
set(CORE_SRC
        nuparu/src/stb_image/stb_image_write.c
        src/fluidSolver/sphSolver/iiSphSolver.cpp
        src/fluidSolver/sphSolver/kernelFunctions.cpp
        src/fluidSolver/sphSolver/neighborSearch.cpp
        src/fluidSolver/sphSolver/sphGrid.cpp
        src/fluidSolver/sphSolver/sphIndexSortedUniformGrid.cpp
        src/fluidSolver/sphSolver/sphParticle.cpp
        src/fluidSolver/sphSolver/sphSolver.cpp
        src/fluidSolver/sphSolver/sphUniformGrid.cpp
        src/fluidSolver/sphSolver/sphZIndexSortedUniformGrid.cpp
        src/fluidSolver/fluidSolver.cpp
        src/fluidSolver/particle.cpp
        src/fluidSolver/zCurve.cpp
        src/geom/cube.cpp
        src/geom/drawable.cpp
        src/geom/geom.cpp
        src/geom/transform.cpp
        src/scene/camera.cpp
        src/scene/scene.cpp
        src/viewer/input.cpp
        src/viewer/particleShaderProgram.cpp
        src/viewer/shaderProgram.cpp
        src/viewer/viewer.cpp
        src/main.cpp
        src/utils.cpp
)

add_executable(MFluidSolver ${CORE_SRC})
target_link_libraries(MFluidSolver ${CORE_LIBS})

##### Testing #####
add_subdirectory(unittest)

# KernelFunctions Library
set(MKERNELFUNCTIONS_LIB
        src/fluidSolver/sphSolver/kernelFunctions.cpp
)
add_library(MKernelFunctions ${MKERNELFUNCTIONS_LIB})

# NeighborSearch Library
set(MNEIGHBORSEARCH_LIB
        src/fluidSolver/sphSolver/neighborSearch.cpp
        src/fluidSolver/sphSolver/sphGrid.cpp
        src/fluidSolver/sphSolver/sphIndexSortedUniformGrid.cpp
        src/fluidSolver/sphSolver/sphParticle.cpp
        src/fluidSolver/sphSolver/sphUniformGrid.cpp
        src/fluidSolver/sphSolver/sphZIndexSortedUniformGrid.cpp
        src/fluidSolver/particle.cpp
        src/fluidSolver/zCurve.cpp
        src/utils.cpp
)
add_library(MNeighborSearch ${MNEIGHBORSEARCH_LIB})
target_link_libraries(MNeighborSearch ${CORE_LIBS})

# ZCurve Library
set(MZCURVE_LIB
        src/fluidSolver/zCurve.cpp
)
add_library(MZCurve ${MZCURVE_LIB})

# Tests
enable_testing()
add_test(NAME MTest COMMAND MTest)