//
// Created by Patryk Szczypień on 27/12/2020.
//

#include <raylib.h>
#include <rlgl.h>
#include <random/random.h>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include <raygui.h>

#undef RAYGUI_IMPLEMENTATION
#include <stdlib.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#define AMOUNT_SPHERES 256
#define GLSL_VERSION 330

typedef struct {
    Camera camera;
    struct {
        float x;
        float y;
    } offset;
    Matrix *matrixBuffer;
    int amountStars;
} View;

View initialize() {
    View view;

    Vector3 cameraPosition = {0.0f, 17.0f, 0.1f};
    Vector3 cameraLookAt = {0.0f, 0.0f, 0.0f};
    Camera camera = {0};
    camera.type = CAMERA_PERSPECTIVE;
    camera.position = cameraPosition;
    camera.target = cameraLookAt;
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 60;

    view.camera = camera;

    return view;
}

int populateStars(View *view) {
   view->matrixBuffer = RL_CALLOC(AMOUNT_SPHERES, sizeof(Matrix));
   if (view->matrixBuffer) {
       view->amountStars = 0;

       int sectorY = 0;
       int sectorX = 0;

       Matrix rotations[AMOUNT_SPHERES];
       Matrix rotationsInc[AMOUNT_SPHERES];
       Matrix translations[AMOUNT_SPHERES];

       int index = 0;

       for (sectorY = 0; sectorY < 16; sectorY++) {
           for (sectorX = 0; sectorX < 16; sectorX++) {

               translations[index] = MatrixTranslate(sectorX - 8.0f, 1.0f, sectorY - 8.0f);

               Random.RandomizeSeedXY(sectorX + (int32_t)view->offset.x, sectorY + (int32_t)view->offset.y);
               bool isStarAtXY = Random.RndIntRange(0, 128) == 1;
               if (isStarAtXY) {
                   //matrices[index] = m;
                   float x = GetRandomValue(0, 360);
                   float y = GetRandomValue(0, 360);
                   float z = GetRandomValue(0, 360);
                   Vector3 axis = Vector3Normalize((Vector3){x, y, z});
                   float angle = (float)GetRandomValue(0, 10) * DEG2RAD;

                   rotationsInc[index] = MatrixRotate(axis, angle);
                   rotations[index] = MatrixIdentity();
                   index = index + 1;
                   view->amountStars += 1;
               }
           }
       }

       // Apply per-instance rotations
       for (int i = 0; i < view->amountStars; i++)
       {
           rotations[i] = MatrixMultiply(rotations[i], rotationsInc[i]);
           view->matrixBuffer[i] = MatrixMultiply(rotations[i], translations[i]);

       }
   }

   return 0;
}

int main() {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 600;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight,
               "Paralax stars");

    Color palette[4] = {
            (Color) {59, 39, 86, 255},
            (Color) {255, 148, 114, 255},
            (Color) {19, 202, 145, 255},
            (Color) {232, 71, 174, 255}
    };

    View view = initialize();
    populateStars(&view);

    Mesh sphereMesh = GenMeshSphere(1.0f, 32, 32);
    Model sphereModel = LoadModelFromMesh(sphereMesh);

    Shader shader = LoadShader(FormatText("assets/shaders/base_lighting_instanced.vs", GLSL_VERSION),
                               FormatText("assets/shaders/lighting.fs", GLSL_VERSION));

    // Get some shader loactions
    shader.locs[LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
    shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    shader.locs[LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instance");

    // Ambient light level
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, UNIFORM_VEC4);
    CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50, 50, 0 }, Vector3Zero(), WHITE, shader);

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Material m = LoadMaterialDefault();
    m.shader = shader;
    m.maps[MAP_DIFFUSE].color = palette[2];

    SetTargetFPS(60);
    RenderTexture2D framebuffer = LoadRenderTexture(screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        UpdateCamera(&view.camera);
        float cameraPos[3] = { view.camera.position.x, view.camera.position.y, view.camera.position.z };
        SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginTextureMode(framebuffer);
        BeginMode3D(view.camera);
        ClearBackground(BLACK);
        DrawGrid(16, 1.0f);
        //DrawModel(sphereModel, (Vector3) {3.0f, 10.0f, 0.0f}, 1.0f, palette[1]);
        rlDrawMeshInstanced(cube, m, view.matrixBuffer, view.amountStars);
        //DrawModel(sphereModel, (Vector3) {0.0f, 10.0f, 0.0f}, 1.0f, palette[2]);
        EndMode3D();
        EndTextureMode();

        DrawTextureRec(framebuffer.texture, (Rectangle){ 0, 0, framebuffer.texture.width, -framebuffer.texture.height}, (Vector2){ 0, 0 }, WHITE);

        view.camera.position.x = GuiSlider((Rectangle){ 50, 50, 165, 20 }, "Camera.x", TextFormat("%2.2f", view.camera.position.x), view.camera.position.x, -50, 100);
        view.camera.position.y = GuiSlider((Rectangle){ 50, 80, 165, 20 }, "Camera.y", TextFormat("%2.2f", view.camera.position.y), view.camera.position.y, -50, 100);
        view.camera.position.z = GuiSlider((Rectangle){ 50, 110, 165, 20 }, "Camera.z", TextFormat("%2.2f", view.camera.position.z), view.camera.position.z, -50, 100);

        if (GuiButton((Rectangle){ 300, 50, 30, 30 }, "UP")) {
            view.offset.y -= 1;
            populateStars(&view);
        }
        if (GuiButton((Rectangle){ 250, 80, 30, 30 }, "LEFT")) {
            view.offset.x -= 1;
            populateStars(&view);
        }
        if (GuiButton((Rectangle){ 350, 80, 30, 30 }, "RIGHT")) {
            view.offset.x += 1;
            populateStars(&view);
        }
        if (GuiButton((Rectangle){ 300, 110, 30, 30 }, "DOWN")) {
            view.offset.y += 1;
            populateStars(&view);
        }
        DrawFPS(20, 20);
        EndDrawing();
    }
    CloseWindow();


    return 0;
}