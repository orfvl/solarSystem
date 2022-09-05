cmake_minimum_required(VERSION 3.2)

project(SOLAR_SYSTEM)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# some stuff to check other stuff
set(CMAKE_CXX_CLANG_TIDY)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)


find_package(Boost 1.66.0)
if(Boost_FOUND)
  add_executable(main main.cpp)
  target_link_libraries(main INTERFACE Boost::Boost)
  target_link_libraries(main INTERFACE Point_type)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(main PUBLIC glfw)

find_package(GLEW 2.2.0 REQUIRED)
target_link_libraries(main PUBLIC GLEW::GLEW)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 3.3 REQUIRED)
target_link_libraries(main PUBLIC OpenGL::OpenGL OpenGL::EGL)

add_subdirectory(systemFunction)
target_link_libraries(main PUBLIC solarSystemLogik)


add_subdirectory(graphics/shader)
target_link_libraries(main PUBLIC shaders)
target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/graphics/shader"
                          "${PROJECT_SOURCE_DIR}/extern/stb"
                          "${PROJECT_SOURCE_DIR}/systemFunction"
                          )

add_subdirectory(extern/stb)
target_link_libraries(main PUBLIC stb_image)
