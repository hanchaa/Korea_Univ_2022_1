#ifndef OPENGLES_LIGHT_H
#define OPENGLES_LIGHT_H

#include "global.h"
#include "program.h"

class Light {

public:
    vec3 position;
    vec3 diffuse;

    Light(Program *program);

    virtual void setup() const;

private:
    Program *program;

    virtual void create(Program* program);
};

#endif // OPENGLES_LIGHT_H
