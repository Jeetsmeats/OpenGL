cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_COMPILER "C:/msys64/clang64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/clang64/bin/clang++.exe")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(assimp_DIR "C:/msys64/clang64/lib/cmake/assimp-6.0")

project(OPENGL)

enable_language(C)
enable_language(CXX)

set(SOURCES
	"src/main.cpp"
	"src/gl.c"
	"src/Shader.cpp"
	"src/Camera.cpp"
)
add_executable(OPENGL ${SOURCES})

target_include_directories(OPENGL PUBLIC
	"src/"
	"include/"
	"C:/msys64/clang64/include/assimp"
)

target_link_directories(OPENGL PRIVATE 
	"include/lib"
	"C:/msys64/clang64/lib"
)

find_package(assimp REQUIRED)

target_link_libraries(OPENGL
	PRIVATE
		glfw3
		assimp::assimp
)

target_compile_definitions(OPENGL PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_link_options(OPENGL PRIVATE -static -static-libstdc++ -static-libgcc)

add_custom_command(TARGET OPENGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/msys64/clang64/bin/libassimp-6.dll"
        $<TARGET_FILE_DIR:OPENGL>
)
