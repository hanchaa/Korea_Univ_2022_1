#ifndef OPENGLES_MATERIAL_H
#define OPENGLES_MATERIAL_H

#include "global.h"
#include "program.h"

class Material {

public:
    Program *program;
    GLuint texture;

    vec3 diffuse;

    Material(Program* program, GLubyte* textureData, GLsizei textureSize);
    virtual ~Material();

    virtual void create(Program *program);
    virtual void data(GLubyte *textureData, GLsizei textureSize);
    virtual void setup() const;
    virtual void destroy();
};

#endif // OPENGLES_MATERIAL_H
