#include <tleilax/graphics.h>

int main(int argc, char *argv[]) {
  print_version();

  const int screenWidth = 960;
  const int screenHeight = 640;

  Graphics.Initialize(screenWidth, screenHeight);

  while (!WindowShouldClose()) {
    Graphics.Update();
    Graphics.Render();
  }

  Graphics.Destroy();

  return 0;
}
