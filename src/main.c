#include "raylib.h"
#include "raymath.h"
#include "level.h"
#include "player.h"

/**
 * Main game state
 */
typedef struct {
  Player player;
  Level level;
  Camera2D camera;
} GameState;

static GameState state = {0};

void initialize_state() {
  state.player.position = (Vector2){100, 20};
  state.player.velocity = (Vector2){0, 0};
  state.player.sprite = load_player_sprite();

  state.level = sample_level();

  state.camera.zoom = 1.0f;
}


/**
 * Driver code
 */

static const Vector2 resolution = {320, 240};

Rectangle calculate_screen_target(Vector2 resolution) {
  Vector2 screen_size = {GetScreenWidth(), GetScreenHeight()};
  Vector2 scaled = resolution; 

  while (scaled.x <= screen_size.x && scaled.y <= screen_size.y) {
    scaled = Vector2Add(scaled, resolution);
  }
  scaled = Vector2Subtract(scaled, resolution);

  return (Rectangle) {
    (screen_size.x - scaled.x) / 2.0,
    (screen_size.y - scaled.y) / 2.0,
    scaled.x,
    scaled.y
  };
}

int main () {
  InitWindow(resolution.x * 3, resolution.y * 3, "HackKU 2025");

  // Program state
  initialize_state();
  RenderTexture2D render_tex = LoadRenderTexture(resolution.x, resolution.y);
  Rectangle screen_target = calculate_screen_target(resolution);

  // Raylib options
  SetWindowMinSize(resolution.x, resolution.y);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      screen_target = calculate_screen_target(resolution);
    }

    float dt = GetFrameTime();
    player_update(&state.player, &state.level, dt);

    // Render the frame to texture
    BeginTextureMode(render_tex);
    BeginMode2D(state.camera);
    ClearBackground(DARKGRAY);
    // printf("Drawing player at: %d")
    player_draw(&state.player);
    level_draw(&state.level);
    EndMode2D();
    EndTextureMode();

    // Draw frame texture to screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
      render_tex.texture,
      (Rectangle) {0, 0, resolution.x, -resolution.y},
      screen_target,
      (Vector2) {0, 0},
      0,
      WHITE
    );
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
