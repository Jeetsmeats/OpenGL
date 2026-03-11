cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_COMPILER "C:/msys64/clang64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/clang64/bin/clang++.exe")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

project(OPENGL)

enable_language(C)
enable_language(CXX)

set(SOURCES
	"src/main.cpp"
	"src/gl.c"
	"src/Shader.cpp"
)
add_executable(OPENGL ${SOURCES})

target_include_directories(OPENGL PUBLIC
	"src/"
	"include/"
)

target_link_directories(OPENGL PRIVATE
	"include/lib"
)

target_link_libraries(OPENGL
	PRIVATE
		glfw3
)

target_link_options(OPENGL PRIVATE -static -static-libstdc++ -static-libgcc)