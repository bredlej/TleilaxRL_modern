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
  TleilaxState *tleilaxUIState = TleilaxStateMachineObj.pInstance;
  char starSystemName[20];
  assert(tleilaxUIState->starSystem);
  assert(tleilaxUIState->starSystem->star);
  sprintf(starSystemName, "%s", tleilaxUIState->starSystem->star->name);
  DrawText(starSystemName,100, 100, 50, RAYWHITE);
  EndDrawing();
}