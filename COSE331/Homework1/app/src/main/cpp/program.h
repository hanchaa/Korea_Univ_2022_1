#ifndef OPENGLES_PROGRAM_H
#define OPENGLES_PROGRAM_H

#include "global.h"
#include "shader.h"

class Program {

public:
    Program(Shader* vertexShader, Shader* fragmentShader);
    virtual ~Program();
    virtual GLuint get() const { return program; };

    virtual void use() const;

private:
    GLuint program;

    virtual void create();
    virtual void shader(Shader* vertexShader, Shader* fragmentShader);
    virtual void link() const;
    virtual void destroy();
};

#endif // OPENGLES_PROGRAM_H
