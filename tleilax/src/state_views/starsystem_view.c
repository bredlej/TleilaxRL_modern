//
// Created by geoco on 12.11.2020.
//

void UpdateStarSystemView()
{
  if (IsKeyPressed(KEY_TAB)) {
    SM_Event(TleilaxStateMachine, TLX_ShowGalaxy, NULL);
  }
}

void RenderStarSystemView()
{
  BeginDrawing();

  ClearBackground(BLACK);
  char coords[20];
  sprintf(coords, "[%d, %d, %d]", Tleilax.selectedCoordinates->x, Tleilax.selectedCoordinates->y, Tleilax.selectedCoordinates->z);
  DrawText(coords,100, 100, 50, RAYWHITE);
  EndDrawing();
}