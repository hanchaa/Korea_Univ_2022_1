#ifndef OPENGLES_MAIN_H
#define OPENGLES_MAIN_H

#include "global.h"
#include "scene.h"

void surfaceCreated(AAssetManager* aAssetManager);
void surfaceChanged(int width, int height);
void drawFrame(float deltaTime);
void rotateCamera(float dx, float dy);
void translateCamera(int direction);

extern "C"
JNIEXPORT void JNICALL
Java_kr_ac_korea_media_opengles_MainActivity_surfaceCreated(JNIEnv *env, jobject instance,
                                                            jobject assetManager) {
    // call native function
    surfaceCreated(AAssetManager_fromJava(env, assetManager));
}

extern "C"
JNIEXPORT void JNICALL
Java_kr_ac_korea_media_opengles_MainActivity_surfaceChanged(JNIEnv *env, jobject instance,
                                                            jint width, jint height) {
    // call native function
    surfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_kr_ac_korea_media_opengles_MainActivity_drawFrame(JNIEnv *env, jobject instance,
                                                       jfloat deltaTime) {
    // call native function
    drawFrame(deltaTime);
}

#endif // OPENGLES_MAIN_H

extern "C"
JNIEXPORT void JNICALL
Java_kr_ac_korea_media_opengles_MainActivity_rotateCamera(JNIEnv *env, jobject thiz, jfloat dx,
                                                          jfloat dy) {
    // TODO: implement rotateCamera()
    rotateCamera(dx,dy);
}
extern "C"
JNIEXPORT void JNICALL
Java_kr_ac_korea_media_opengles_MainActivity_translateCamera(JNIEnv *env, jobject thiz,
                                                             jint direction) {
   translateCamera(direction);
}