#ifndef __GL_DEBUG_H__
#define __GL_DEBUG_H__

// Driver-reported OpenGL errors, printed the moment they happen.
//
// Requires a debug context: call glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,
// GLFW_TRUE) before glfwCreateWindow. Works on GL 4.3+ (or KHR_debug), which
// any modern NVIDIA driver provides even when you request a 3.3 context.
//
// Instead of calling glGetError() after every line, the driver tells you
// exactly which call failed and why, e.g.
//   [GL ERROR] GL_INVALID_OPERATION in glUniform1f: wrong uniform type
// Set a breakpoint inside the callback to see the offending call stack.

/// @brief Enable synchronous debug output if the context supports it.
/// @return true if debug output is active
bool enableGLDebugOutput();

#endif // __GL_DEBUG_H__
