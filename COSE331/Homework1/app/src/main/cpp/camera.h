#ifndef OPENGLES_CAMERA_H
#define OPENGLES_CAMERA_H

#include "global.h"
#include "program.h"

class Camera {

public:
    vec3 eye;
    vec3 at;
    vec3 up;
    float fovy;
    float aspect;
    float zNear;
    float zFar;

    vec3 cameraU,cameraV,cameraN;

    mat4 viewMatrix,projMatrix;

    Camera(Program *program);

    virtual void setup();
    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateCameraUVN();
private:
    Program *program;

    virtual void create(Program* program);
};

#endif // OPENGLES_CAMERA_H
