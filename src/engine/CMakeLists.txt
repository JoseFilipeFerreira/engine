cmake_minimum_required(VERSION 3.5)

add_executable(engine
    main.cpp
    model.cpp
    camera.cpp
    parser.cpp
)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

find_package(DevIL REQUIRED)
include_directories(${IL_INCLUDE_DIR})
link_libraries(${IL_LIBRARIES})

target_link_libraries(
    engine
    utils
    deps
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    ${IL_LIBRARIES}
)

