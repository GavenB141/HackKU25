#include "raylib.h"
#include "player.h"

/**
 * Main game state
 */

typedef struct {
  Player player;
} GameState;

static GameState state = {0};

void initialize_state() {
  state.player.position = (Vector2){20, 20};
  state.player.velocity = (Vector2){10, -50};
}


/**
 * Driver code
 */

int main () {
  InitWindow(640, 480, "HackKU 2025");

  initialize_state();

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    player_update(&state.player, dt);

    BeginDrawing();
    ClearBackground(BLACK);
    player_draw(&state.player);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
