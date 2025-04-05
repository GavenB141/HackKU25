#include "raylib.h"

int main () {
  InitWindow(640, 480, "HackKU 2025");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Hello World!", 20, 20, 20, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
