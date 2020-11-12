//
// Created by geoco on 12.11.2020.
//

void UpdateIntro() {
  if (IsKeyPressed(KEY_TAB)) {
    TleilaxUIData *data = SM_XAlloc(sizeof(TleilaxUIData));
    data->Render = RenderGalaxyView;
    data->Update = UpdateGalaxyView;
    data->HandleInput = NULL;
    SM_Event(TleilaxUISM, TLX_ShowGalaxy, data);
  }
}

void RenderIntro() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Tleilax", 100, 100, 50, RAYWHITE);
  EndDrawing();
}