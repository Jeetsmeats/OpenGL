cmake_minimum_required(VERSION 3.26)

project(OpenGLProject)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SOURCES
	"src/main.cpp"
	"src/callbacks.cpp"
	"src/key_input.cpp"
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
)

