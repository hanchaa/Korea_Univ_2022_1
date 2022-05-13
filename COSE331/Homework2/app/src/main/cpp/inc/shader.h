#ifndef HW2_SHADER_H
#define HW2_SHADER_H

#include "global.h"
#include "asset.h"

class Shader {
public:
    Shader(GLenum type, const char* fileName);
    virtual ~Shader();
    virtual GLuint get() const { return shader; };

private:
    GLuint shader;

    virtual void create(GLenum type);
    virtual void source(const char* fileName) const;
    virtual void compile() const;
    virtual void destroy() const;
};

#endif //HW2_SHADER_H
