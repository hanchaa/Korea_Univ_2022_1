#include "camera.h"

Camera::Camera(Program* program)
        : eye(vec3(0.0f)),
          at(vec3(0.0f)),
          up(vec3(0.0f, 1.0f, 0.0f)),
          fovy(60.0f),
          aspect(1.0f),
          zNear(1.0f),
          zFar(150.0f) {

    // create camera
    create(program);

    // setup camera


//    updateViewMatrix();
//    updateProjectionMatrix();
    setup();
}

void Camera::create(Program* program) {
    LOG_PRINT_DEBUG("Create camera");

    // set attribute
    this->program = program;

    // check uniform locations
    if (glGetUniformLocation(program->get(), "viewMat") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "viewMat");
    }
    if (glGetUniformLocation(program->get(), "projMat") < 0) {
        LOG_PRINT_WARN("Fail to get uniform location: %s", "projMat");
    }
}

void Camera::setup() {


    //setUp Camera
    cameraN = glm::normalize(eye - at);
    cameraU = glm::normalize(glm::cross(up, cameraN));
    cameraV = glm::normalize(glm::cross(cameraN, cameraU));

    // update matrix
    updateViewMatrix();
    updateProjectionMatrix();

}

void Camera::updateViewMatrix() {

    //update View Matrix
    viewMatrix = glm::lookAt(
            eye,
            eye-cameraN,
            cameraV
    );
    GLint viewMatLoc = glGetUniformLocation(program->get(), "viewMat");
    // set uniform data
    if (viewMatLoc >= 0) glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMatrix));

}

void Camera::updateProjectionMatrix() {
    projMatrix = perspective(radians(60.0f), aspect, 1.0f, 150.0f);

    // get uniform locations
    GLint projMatLoc = glGetUniformLocation(program->get(), "projMat");
    // set uniform data

    if (projMatLoc >= 0) glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMatrix));

}

void Camera::updateCameraUVN() {
    cameraN = glm::normalize(eye - at);
    cameraU = glm::normalize(glm::cross(up, cameraN));
    cameraV = glm::normalize(glm::cross(cameraN, cameraU));
}


