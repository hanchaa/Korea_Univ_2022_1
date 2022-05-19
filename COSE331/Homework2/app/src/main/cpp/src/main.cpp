#include "main.h"

void mouseDownEvents(float x, float y) {
    Scene::mouseDownEvents(x, y);
}

void mouseMoveEvents(float x, float y) {
    Scene::mouseMoveEvents(x, y);
}



void surfaceCreated(AAssetManager* aAssetManager) {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    Scene::setup(aAssetManager);
}

void surfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    Scene::screen(width, height);
}

void drawFrame(float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::update(deltaTime);
}