#include "material.h"

Material::Material(Program *program, GLubyte *textureData, GLsizei textureSize)
        : diffuse(vec3(0.8f)) {

    // create material
    create(program);

    // set texture data
    data(textureData, textureSize);

    // setup material
    setup();
}

Material::~Material() {

    // destroy material
    destroy();
}

void Material::create(Program *program) {
    LOG_PRINT_DEBUG("Create material");

    // set attribute
    this->program = program;

    // generate texture
    glGenTextures(1, &texture);

    // check texture
    if (!texture) {
        LOG_PRINT_ERROR("Fail to generate texture");
    }

    // check uniform locations
    if (glGetUniformLocation(program->get(), "materialDiff") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "materialDiff");
    }
}

void Material::data(GLubyte *textureData, GLsizei textureSize) {
    LOG_PRINT_DEBUG("Set texture data");

    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize, textureSize,
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

    // generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Material::setup() const {
//    LOG_PRINT_DEBUG("Set material");

    // get uniform location
    GLint materialDiffLoc = glGetUniformLocation(program->get(), "materialDiff");
    GLint s_tex0Loc = glGetUniformLocation(program->get(), "s_tex0");

    // active and bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // set uniform data
    if (materialDiffLoc >= 0) glUniform3fv(materialDiffLoc, 1, value_ptr(diffuse));
    if (s_tex0Loc >= 0) glUniform1i(s_tex0Loc, 0);
}

void Material::destroy() {
    LOG_PRINT_DEBUG("Destroy material");

    // delete texture
    glDeleteTextures(1, &texture);
}
