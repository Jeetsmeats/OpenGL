cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(OPENGL)

enable_language(C)
enable_language(CXX)

find_package(assimp REQUIRED)

set(SOURCES
 "src/main.cpp"
 "src/glad.c"
 "src/Shader.cpp"
 "src/Camera.cpp"
 "src/Texture.cpp"
 "src/Mesh.cpp"
)
add_executable(OPENGL ${SOURCES})

target_include_directories(OPENGL PUBLIC
 "src/"
 "include/"
)

target_link_directories(OPENGL PRIVATE
 "lib/"
)

target_link_libraries(OPENGL
 PRIVATE
  glfw3
  assimp::assimp
)

target_compile_definitions(OPENGL PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_link_options(OPENGL PRIVATE -static-libstdc++ -static-libgcc)
