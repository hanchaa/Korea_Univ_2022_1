#include "shader.h"

Shader::Shader(GLenum type, const char* filename) {

    // create shader
    create(type);

    // set shader source
    source(filename);

    // compile shader
    compile();
}

Shader::~Shader() {

    // delete shader
    destroy();
}

void Shader::create(GLenum type) {
    LOG_PRINT_DEBUG("Create %s shader", type == GL_VERTEX_SHADER ? "vertex" : "fragment");

    // create shader
    shader = glCreateShader(type);

    // check shader
    if (!shader) {
        LOG_PRINT_ERROR("Fail to create shader");
    }

    return;
}

void Shader::source(const char* filename) const {
    LOG_PRINT_DEBUG("Set shader source");

    // load shader source from asset
    const char* src = Asset::load(filename);
    LOG_PRINT_DEBUG("Shader source: \n%s", src);

    // check source
    if (!src) {
        LOG_PRINT_ERROR("Fail to load shader source");
        return;
    }

    // set shader source
    glShaderSource(shader, 1, &src, NULL);
    delete[] src;

    return;
}

void Shader::compile() const {
    LOG_PRINT_DEBUG("Compile shader");

    // compile shader
    glCompileShader(shader);

    // check compile status
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {

        // check information log length
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength) {

            // print information log
            GLchar* infoLog = new GLchar[infoLogLength];
            glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
            LOG_PRINT_ERROR("Fail to compile shader: \n%s", infoLog);
            delete[] infoLog;
        }
    }

    return;
}

void Shader::destroy() {
    LOG_PRINT_DEBUG("Delete shader");

    // delete shader
    glDeleteShader(shader);

    return;
}
