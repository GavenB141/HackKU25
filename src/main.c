#include "raylib.h"
#include "raymath.h"
#include "level.h"
#include "player.h"

#define TOTAL_LEVELS 3

static const Vector2 resolution = {320, 240};
static Level levels[TOTAL_LEVELS];

/**
 * Main game state
 */
typedef struct {
  Player player;
  Level level;
  Camera2D camera;
  float fade;
} GameState;

static GameState state = {0};

void initialize_state(int level_index) {
  state.player.sprite = load_player_sprite();

  state.fade = 0.8f;
  state.level = getLevel(2);
  state.player.position = state.level.startingPosition;
  state.player.velocity = (Vector2){0, 0};

  state.camera.zoom = 1.0f;
}


/**
 * Driver code
 */


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

  for(int i = 0; i < TOTAL_LEVELS; i++){
    levels[i] = getLevel(i);
  }
  // Program state
  initialize_state(0);
  RenderTexture2D render_tex = LoadRenderTexture(resolution.x, resolution.y);
  Rectangle screen_target = calculate_screen_target(resolution);

  // Raylib options
  SetWindowMinSize(resolution.x, resolution.y);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F11)) ToggleBorderlessWindowed();
    if (IsWindowResized()) {
      screen_target = calculate_screen_target(resolution);
    }

    float dt = GetFrameTime();
    level_update(&state.level, dt);
    player_update(&state.player, &state.level, dt);

    // Render the frame to texture
    BeginTextureMode(render_tex);
    BeginMode2D(state.camera);
    ClearBackground(DARKGRAY);
    // printf("Drawing player at: %d")
    level_draw(&state.level, &state.player, dt);
    player_draw(&state.player);
    fade_in(state.fade, (Rectangle){0, 0, resolution.x, resolution.y});
    state.fade -= dt / 2;
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
    if (IsKeyPressed(KEY_R) || state.player.teleported == 1) {
      state.player.position = state.level.startingPosition;
      state.player.velocity = (Vector2){0,0};
      state.fade = 0.8f;
    }
    if (IsKeyPressed(KEY_EQUAL)){
      state.level = getLevel(0);
      state.player.position = state.level.startingPosition;
      state.fade = 0.8f;
    }
    if (IsKeyPressed(KEY_ONE)){
      state.level = getLevel(1);
      state.player.position = state.level.startingPosition;
      state.fade = 0.8f;
    }
    if (IsKeyPressed(KEY_TWO)){
      state.level = getLevel(2);
      state.player.position = state.level.startingPosition;
      state.fade = 0.8f;
    }
    if (IsKeyPressed(KEY_THREE)){
      state.level = getLevel(3);
      state.player.position = state.level.startingPosition;
      state.fade = 0.8f;
    }
    if (IsKeyPressed(KEY_FOUR)){
      state.level = getLevel(4);
      state.player.position = state.level.startingPosition;
      state.fade = 0.8f;
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
