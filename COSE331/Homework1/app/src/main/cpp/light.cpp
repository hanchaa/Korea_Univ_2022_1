#include "light.h"

Light::Light(Program* program)
        : position(vec3(0.0f)),
          diffuse(vec3(0.8f)) {

    // create light
    create(program);

    // setup light
    setup();
}

void Light::create(Program* program) {
    LOG_PRINT_DEBUG("Create light");

    // set attribute
    this->program = program;

    // check uniform locations
    if (glGetUniformLocation(program->get(), "lightPos") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "lightPos");
    }
    if (glGetUniformLocation(program->get(), "sourceDiff") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "sourceDiff");
    }
}

void Light::setup() const {
//    LOG_PRINT_DEBUG("Set light");

    // get uniform locations
    GLint lightPosLoc = glGetUniformLocation(program->get(), "lightPos");
    GLint sourceDiffLoc = glGetUniformLocation(program->get(), "sourceDiff");

    // set uniform data
    if (lightPosLoc >= 0) glUniform3fv(lightPosLoc, 1, value_ptr(position));
    if (sourceDiffLoc >= 0) glUniform3fv(sourceDiffLoc, 1, value_ptr(diffuse));
}
