########################
#     MFluidSolver     #
########################
   ##### Tests #####

# Boost Testing Framework
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_TEST_DYN_LINK)

set(MTEST_SRC
        main.cpp # All cpp test files are included in main
)

set(MTEST_LIBS
        MKernelFunctions
        MNeighborSearch
        MZCurve
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_executable(MTest ${MTEST_SRC})
target_link_libraries(MTest ${MTEST_LIBS})