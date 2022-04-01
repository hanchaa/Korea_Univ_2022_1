#ifndef OPENGLES_SHADER_H
#define OPENGLES_SHADER_H

#include "global.h"
#include "asset.h"

class Shader {

public:
    Shader(GLenum type, const char* filename);
    virtual ~Shader();
    virtual GLuint get() const { return shader; };

private:
    GLuint shader;

    virtual void create(GLenum type);
    virtual void source(const char* filename) const;
    virtual void compile() const ;
    virtual void destroy();
};

#endif // OPENGLES_SHADER_H
