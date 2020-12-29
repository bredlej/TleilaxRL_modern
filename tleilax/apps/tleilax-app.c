#include <tleilax/graphics.h>

int main(int argc, char *argv[]) {
  print_version();

  setvbuf(stdout, NULL, _IONBF, 0);

  const int screenWidth = 600;
  const int screenHeight = 600;

  Graphics.Initialize(screenWidth, screenHeight);

  while (!WindowShouldClose()) {
    Graphics.Update();
    Graphics.Render();
  }

  Graphics.Destroy();

  return 0;
}
