#include <tleilax/graphics.h>
#include <tleilax/tleilax.h>

int main(int argc, char *argv[]) {
  print_version();

  const int screenWidth = 960;
  const int screenHeight = 640;

  Tleilax.Initialize();
  Graphics.Initialize(screenWidth, screenHeight);

  while (!WindowShouldClose()) {
    Graphics.Update();
    Graphics.Render();
  }

  Tleilax.Destroy();
  Graphics.Destroy();

  return 0;
}
