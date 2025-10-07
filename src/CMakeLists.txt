cmake_minimum_required(VERSION 3.10)
project(Learning_openGL)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(executable exercise3_chapter1.cpp glad.c)

target_include_directories(executable PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} include)

target_link_libraries(executable PUBLIC ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

