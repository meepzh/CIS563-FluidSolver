########################
#     MFluidSolver     #
########################
   ##### Tests #####

set(MTEST_SRC
    main.cpp
    testNeighborSearch.cpp
)

set(MTEST_LIBS
    MNeighborSearch
)

add_executable(MTest ${MTEST_SRC})
target_link_libraries(MTest ${MTEST_LIBS})