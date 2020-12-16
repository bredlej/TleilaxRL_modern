//
// Created by geoco on 12.11.2020.
//

void UpdateIntro() {
  if (IsKeyPressed(KEY_TAB)) {
    SM_Event(TleilaxStateMachine, TLX_ShowGalaxy, NULL);
  }
}

void RenderIntro() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("T L E I L A X", 120, 180, 100, RAYWHITE);
  DrawText("Press <Tab>", 30, 580, 30, RAYWHITE);
  EndDrawing();
}