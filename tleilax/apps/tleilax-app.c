#include <tleilax/tleilax.h>
#include <tleilax/graphics.h>

int main(int argc, char *argv[]) {
    print_version();

    const int screenWidth = 1800;
    const int screenHeight = 1450;

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