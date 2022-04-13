#ifndef OPENGLES_GLOBAL_H
#define OPENGLES_GLOBAL_H

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#define ANDROID_LOG_TAG "OpenGL ES"
#define LOG_PRINT_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, __VA_ARGS__)
#define LOG_PRINT_WARN(...) __android_log_print(ANDROID_LOG_WARN, ANDROID_LOG_TAG, __VA_ARGS__)
#define LOG_PRINT_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, __VA_ARGS__)

using namespace std;
using namespace glm;

class Vertex {
    vec3 pos;
    vec3 nor;
    vec2 tex;
};

typedef GLushort Index;

#endif // OPENGLES_GLOBAL_H
