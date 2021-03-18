#ifndef TOOL_HPP
#define TOOL_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <exception>

#define GLEW_STATIC
#include "GL/glew.h"

std::string ErrorCode2Str(GLenum error);

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline void GLPrintError(const char* function, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("[OpenGL Error](%s - %d): %s\n", function, line, ErrorCode2Str(error).c_str());
    }
}

#define GLCall(x) \
    do { \
        GLClearError(); \
        x; \
        GLPrintError(__FUNCTION__, __LINE__); \
    } while(0)

using std::string;

static string ReadShaderFile(string filename) {
    std::ifstream ifile(filename);
    if (ifile.fail()) {
        throw std::runtime_error("shader " + filename + " not exists");
    }
    string content((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    ifile.close();
    return content;
}

namespace tinyrenderer3d {

enum FlipType {
    FLIP_NONE = 0x00,
    FLIP_VERTICAL = 0x01,
    FLIP_HORIZONTAL = 0x02,
    FLIP_BOTH = FLIP_VERTICAL|FLIP_HORIZONTAL
};

};

#endif
