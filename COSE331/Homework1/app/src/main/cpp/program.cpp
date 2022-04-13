#include "program.h"

Program::Program(Shader *vertexShader, Shader *fragmentShader) {

    // create program
    create();

    // attach shader
    shader(vertexShader, fragmentShader);

    // link program
    link();

    // use program
    use();
}

Program::~Program() {

    // delete program
    destroy();
}

void Program::create() {
    LOG_PRINT_DEBUG("Create program");

    // create program
    program = glCreateProgram();

    // check program
    if (!program) {
        LOG_PRINT_ERROR("Fail to create program");
    }
}

void Program::shader(Shader* vertexShader, Shader* fragmentShader) {
    LOG_PRINT_DEBUG("Attach shader to program");

    // attach shader
    glAttachShader(program, vertexShader->get());
    glAttachShader(program, fragmentShader->get());
}

void Program::link() const {
    LOG_PRINT_DEBUG("Link program");

    // link program
    glLinkProgram(program);

    // check link status
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {

        // check information log length
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength) {

            // print information log
            GLchar *infoLog = new GLchar[infoLogLength];
            glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
            LOG_PRINT_ERROR("Fail to link program: \n%s", infoLog);
            delete[] infoLog;
        }
    }
}

void Program::use() const {
//    LOG_PRINT_DEBUG("Use program");

    // use program
    glUseProgram(program);
}

void Program::destroy() {
    LOG_PRINT_DEBUG("Delete program");

    // delete program
    glDeleteProgram(program);
}
