//
// Created by geoco on 21.12.2020.
//
#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define AMOUNT_SPHERES 100

int main() {

  Mesh spheres[AMOUNT_SPHERES];

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;


  InitWindow(screenWidth, screenHeight,
             "Render to texture playground");

  Mesh sphere = GenMeshSphere(1.0f, 32, 32);


  return 0;
}