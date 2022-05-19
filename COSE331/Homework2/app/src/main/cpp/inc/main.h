//
// Created by I.B. Park on 2019-04-25.
//

#ifndef HW2_MAIN_H
#define HW2_MAIN_H

#include "global.h"
#include "scene.h"

void mouseDownEvents(float x, float y);
void mouseMoveEvents(float x, float y);


void surfaceCreated(AAssetManager* aAssetManager);
void surfaceChanged(int width, int height);
void drawFrame(float deltaTime);


extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw2_MainActivity_surfaceCreated(JNIEnv* env, jobject instance, jobject assetManager) {
    surfaceCreated(AAssetManager_fromJava(env, assetManager));
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw2_MainActivity_surfaceChanged(JNIEnv* env, jobject instance, jint width, jint height) {
    surfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw2_MainActivity_drawFrame(JNIEnv* env, jobject instance, jfloat deltaTime) {
    drawFrame(deltaTime);
}


extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw2_MainActivity_mouseDownEvents(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    mouseDownEvents(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw2_MainActivity_mouseMoveEvents(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    mouseMoveEvents(x, y);
}

#endif //HW2_MAIN_H