#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "../src/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
    public:
        GLuint ID;
        Shader(const char* vertexFile, const char* fragmentFile);

        void SetToVec2(const char* uniform, const GLfloat* value);

        void SetToVec3(const char* uniform, const GLfloat* value);

        void SetToFloat(const char* uniform, GLfloat value);

        void Activate();
        void Delete();
};


#endif