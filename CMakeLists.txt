cmake_minimum_required(VERSION 3.26)

project(OpenGLProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SOURCES
	"src/main.cpp"
	"src/shader.cpp"
	"src/camera.cpp"
)
add_subdirectory(
	"vendor/"
)

add_executable(OpenGLProject ${SOURCES})

target_include_directories(OpenGLProject PRIVATE 
	"src/"
	"include/"		
)

target_link_libraries(OpenGLProject
	PRIVATE
		glfw
        glad
		stb
		glm
)

