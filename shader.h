#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

class Shader {
    public:
        static GLuint Load(const char * vertex_file_path,const char * fragment_file_path);
};
#endif
