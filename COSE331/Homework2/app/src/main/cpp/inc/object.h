#ifndef HW2_OBJECT_H
#define HW2_OBJECT_H

#include "global.h"
#include "program.h"
#include "material.h"
#include "vertex.h"

class Object {
public:
    Object(Program* program, Material* material,
            const vector<Vertex>& vertices, const vector<Index>& indices, GLenum primitive=GL_TRIANGLES);
    virtual ~Object();

    virtual void draw() const;

    mat4 worldMat;
    Material* material;
    vector<Vertex> vertices;
    vector<Index> indices;
    vec3 origin;
    virtual void load(const vector<Vertex>& vertices, const vector<Index>& indices);

private:
    Program* program;
    GLuint vbo, ibo;
    GLenum primitive;
    virtual void create(Program* program, Material* material);
    virtual void destroy();


};

#endif //HW2_OBJECT_H
