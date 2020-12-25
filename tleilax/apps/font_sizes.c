//
// Created by geoco on 17.12.2020.
//
#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
int main()
{
  InitWindow(320, 240, "Font sizes bug");

  Vector2 textPos = {160, 120};
  SetTargetFPS(10);
  Font font;
  while (!WindowShouldClose()) {
    font = LoadFontEx("assets/fonts/excel.ttf",100, NULL, 256);
    BeginDrawing();
    ClearBackground(WHITE);

    DrawTextEx(font, "Test draw", textPos, 100, 1, BLACK);
    EndDrawing();
    UnloadFont(font);
  }
  CloseWindow();
}
