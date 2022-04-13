#include "main.h"

void surfaceCreated(AAssetManager* aAssetManager) {

    // set clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    // setup scene
    Scene::setup(aAssetManager);
}

void surfaceChanged(int width, int height) {

    // set viewport
    glViewport(0, 0, width, height);

    // set scene screen
    Scene::screen(width, height);
}

void drawFrame(float deltaTime) {

    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update scene
    Scene::update(deltaTime);
}

void rotateCamera(float dx, float dy){


    Scene::rotateCamera(dx,dy);

}

void translateCamera(int direction){

    float translationAmount=0.5;
    switch (direction) {
        case 0://w
            Scene::translateFront(translationAmount);
            break;
        case 1://a
            Scene::translateLeft(translationAmount);
            break;
        case 2://s
            Scene::translateBack(translationAmount);
            break;
        case 3://d
            Scene::translateRight(translationAmount);
            break;
        default:
            break;

    }


}