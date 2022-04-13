#include "object.h"

Object::Object(Program *program, Material *material,
               GLfloat *verticesData, GLushort *indicesData,
               GLsizei verticesSize, GLsizei indicesSize) {

    // create object
    create(program, material);

    // set object data
    data(verticesData, indicesData, verticesSize, indicesSize);

    // draw object
    draw();
}

Object::~Object() {

    // delete buffer objects
    destroy();
}

void Object::create(Program* program, Material* material) {
    LOG_PRINT_DEBUG("Create object");

    // set attributes
    this->program = program;
    this->material = material;

    // generate buffer objects
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    // check buffer object ID
    if (!vbo || !ibo) {
        LOG_PRINT_ERROR("Fail to generate buffer");
    }

    // check world matrix uniform location
    if (glGetUniformLocation(program->get(), "worldMat") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "worldMat");
    }
}

void Object::data(GLfloat *verticesData, GLushort *indicesData,
                  GLsizei verticesSize, GLsizei indicesSize) {
    LOG_PRINT_DEBUG("Set object data");

    // set attributes
    this->indicesSize = indicesSize;
//    this->vertices = vertices;
//    this->indices = indices;

    // bind buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // set buffer data
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat),
                 verticesData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLushort),
                 indicesData, GL_STATIC_DRAW);
}

void Object::draw() const {
//    LOG_PRINT_DEBUG("Draw object");

    // get world matrix uniform location
    GLint loc = glGetUniformLocation(program->get(), "worldMat");

    // set world matrix uniform data
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(worldMatrix));

    // bind buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // enable vertex attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // set vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (6 * sizeof(GLfloat)));

    // setup material
    material->setup();

    // draw elements
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_SHORT, (const void *) 0);
}

void Object::destroy() {
    LOG_PRINT_DEBUG("Delete object");

    // delete buffer objects
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}
