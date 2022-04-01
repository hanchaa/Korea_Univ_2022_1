#ifndef OPENGLES_OBJECT_H
#define OPENGLES_OBJECT_H

#include "global.h"
#include "program.h"
#include "material.h"

class Object {

public:
    mat4 worldMatrix;

    Object(Program* program, Material* material, GLfloat* verticesData, GLushort* indicesData, GLsizei verticesSize, GLsizei indicesSize);
    virtual ~Object();

    virtual void draw() const;

private:
    Program* program;
    Material* material;

    GLuint vbo;
    GLuint ibo;
    GLsizei indicesSize;

    virtual void create(Program* program, Material* material);
    virtual void data(GLfloat* verticesData, GLushort* indicesData, GLsizei verticesSize, GLsizei indicesSize);
    virtual void destroy();
};

#endif // OPENGLES_OBJECT_H
