//
// Created by geoco on 12.11.2020.
//

void UpdateIntro() {
  if (IsKeyPressed(KEY_TAB)) {
    SM_Event(TleilaxUISM, TLX_ShowGalaxy, NULL);
  }
}

void RenderIntro() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Tleilax", 100, 100, 50, RAYWHITE);
  EndDrawing();
}