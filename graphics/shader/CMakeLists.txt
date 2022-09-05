add_library(shaders shaders.h shaders.cpp)

set_target_properties(shaders PROPERTIES LINKER_LANGUAGE CXX)
find_package(GLEW 2.2.0 REQUIRED)
target_link_libraries(shaders PUBLIC GLEW::GLEW)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders_src/shader.fs
${PROJECT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders_src/shader.vs
${PROJECT_BINARY_DIR} COPYONLY)