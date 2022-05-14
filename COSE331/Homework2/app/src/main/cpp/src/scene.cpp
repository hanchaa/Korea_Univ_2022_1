#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;
int Scene::width=0;
int Scene::height=0;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
    Scene::width = width;
    Scene::height = height;
}


void Scene::update(float deltaTime) {
    Scene::program->use();

    Scene::camera->update();

    //////////////////////////////
    /* TODO */
    static float elapsedTime;
    elapsedTime += deltaTime;

    if (elapsedTime >= 4)
        elapsedTime -= 4;

    static vector<Vertex> originalVertices;
    if (originalVertices.empty())
        originalVertices = playerVertices;

    vector<mat4> toBone, toParent;

    toParent.push_back(translate(jOffsets[0]));
    toBone.push_back(inverse(toParent[0]));

    for (int i = 1; i < 28; i++) {
        mat4 parentMat = toBone[jParents[i]];
        toParent.push_back(translate(jOffsets[i]));

        toBone.push_back(inverse(toParent[i]) * parentMat);
    }

    vector<mat4> animatedToRoot;
    vector<float> prevMotion = motions[(int)elapsedTime], nextMotion = motions[(int)(elapsedTime + 1) % 4];

    mat4 prevRotate = rotate(radians(prevMotion[5]), vec3(0.0f, 0.0f, 1.0f))
                      * rotate(radians(prevMotion[3]), vec3(1.0f, 0.0f, 0.0f))
                      * rotate(radians(prevMotion[4]), vec3(0.0f, 1.0f, 0.0f));

    mat4 nextRotate = rotate(radians(nextMotion[5]), vec3(0.0f, 0.0f, 1.0f))
                      * rotate(radians(nextMotion[3]), vec3(1.0f, 0.0f, 0.0f))
                      * rotate(radians(nextMotion[4]), vec3(0.0f, 1.0f, 0.0f));

    quat prevRotateQuat = quat_cast(prevRotate);
    quat nextRotateQuat = quat_cast(nextRotate);
    quat interpolatedRotateQuat = slerp(prevRotateQuat, nextRotateQuat, elapsedTime - (int)elapsedTime);
    mat4 interpolatedRotate = mat4_cast(interpolatedRotateQuat);
    mat4 interpolatedTranslate = translate(mix(vec3(prevMotion[0], prevMotion[1], prevMotion[2]), vec3(nextMotion[0], nextMotion[1], nextMotion[2]), elapsedTime - (int)elapsedTime));

    animatedToRoot.push_back(toParent[0] * interpolatedTranslate * interpolatedRotate);

    for (int i = 1; i < 28; i++) {
        prevRotate = rotate(radians(prevMotion[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                     * rotate(radians(prevMotion[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                     * rotate(radians(prevMotion[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));

        nextRotate = rotate(radians(nextMotion[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                      * rotate(radians(nextMotion[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                      * rotate(radians(nextMotion[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));

        prevRotateQuat = quat_cast(prevRotate);
        nextRotateQuat = quat_cast(nextRotate);
        interpolatedRotateQuat = mix(prevRotateQuat, nextRotateQuat, elapsedTime - (int)elapsedTime);
        interpolatedRotate = mat4_cast(interpolatedRotateQuat);

        mat4 parentMat = animatedToRoot[jParents[i]];
        animatedToRoot.push_back(parentMat * toParent[i] * interpolatedRotate);
    }

    for (int i = 0; i < playerVertices.size(); i++) {
        vec3 pos = originalVertices[i].pos, normal = originalVertices[i].nor;
        vec4 weight = originalVertices[i].weight;
        ivec4 bone = originalVertices[i].bone;

        mat4 M = mat4(0.0f);

        for (int j = 0; j < 4; j++) {
            if (bone[j] == -1)
                continue;

            M += weight[j] * animatedToRoot[bone[j]] * toBone[bone[j]];
        }

        playerVertices[i].pos = vec3(M * vec4(pos, 1.0f));
        playerVertices[i].nor = transpose(inverse(mat3(M))) * normal;
    }

    //////////////////////////////

    Scene::player->load(playerVertices, playerIndices);
    Scene::player->draw();
}


vec3 prevV;
void Scene::mouseDownEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Optional problem
     * object rotating
     * Automatically called when mouse down
     */
    x = (2 * x / width) - 1;
    y = 1 - (2 * y / height);

    if (x * x + y * y > 1)
        prevV = normalize(vec3(x, y, 0.0f));
    else
        prevV = vec3(x, y, sqrt(1 - x * x - y * y));

    LOG_PRINT_DEBUG("%f %f", x, y);
    //////////////////////////////
}

void Scene::mouseMoveEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Optional problem
     * object rotating
     * Automatically called when mouse move
     */
    x = (2 * x / width) - 1;
    y = 1 - (2 * y / height);

    vec3 v;
    if (x * x + y * y > 1)
        v = normalize(vec3(x, y, 0.0f));
    else
        v = vec3(x, y, sqrt(1 - x * x - y * y));

    float degree = acos(dot(prevV, v));

    vec3 rotateAxis = cross(prevV, v);
    rotateAxis = vec3(inverse(player->worldMat) * inverse(camera->viewMatrix) * vec4(rotateAxis, 0.0f));

    if (rotateAxis[0] == 0 && rotateAxis[1] == 0 && rotateAxis[2] == 0)
        return;

    if (isnan(degree))
        return;

    player->worldMat = player->worldMat * rotate(mat4(1.0f), degree, rotateAxis);

    prevV = v;
    //////////////////////////////
}

