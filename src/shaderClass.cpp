#include "../headers/shaderClass.h"

std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // This is an unsigned-int 
   // glShaderSource replaces the source code of vertexShader with the source code of vertexShaderSource (its memory address)
   // GPU can't understand source code, so we compile the shader as well
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexSource, NULL);
   glCompileShader(vertexShader);

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
   glCompileShader(fragmentShader);

   // Wrap the fragment and vertex shaders into a shader program
   ID = glCreateProgram();
   glAttachShader(ID, vertexShader);
   glAttachShader(ID, fragmentShader);
   glLinkProgram(ID);

   // Delete the shaders since we now have them in our shader program (memory management!)
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void Shader::Activate() 
{
    glUseProgram(ID);
}

void Shader::Delete() 
{
    glDeleteProgram(ID);
}
