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
}


void Scene::update(float deltaTime) {
    Scene::program->use();

    Scene::camera->update();

    //////////////////////////////
    /* TODO */

    // Line Drawer & Debugger
    glLineWidth(20);
    Scene::lineDraw->load({{vec3(-20.0f, 0.0f, 0.0f)}, {vec3(20.0f, 0.0f, 0.0f)}}, {0, 1});
    Scene::lineDraw->draw();

    // LOG_PRINT_DEBUG("You can also debug variables with this function: %f", M_PI);

    //////////////////////////////

    Scene::player->load(playerVertices, playerIndices);
    Scene::player->draw();
}



void Scene::mouseDownEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Optional problem
     * object rotating
     * Automatically called when mouse down
     */

    //////////////////////////////
}

void Scene::mouseMoveEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Optional problem
     * object rotating
     * Automatically called when mouse move
     */

    //////////////////////////////
}

