#include "raylib.h"
#include "raymath.h"
#include "level.h"
#include "player.h"

#define FADE_VAL 0.5f
static Vector2 resolution = {640, 480};

/**
 * Main game state
 */
typedef struct {
  Player player;
  Player dragon;
  Level level;
  Camera2D camera;
  float fade;
} GameState;

static GameState state = {0};

void initialize_state(int level_index) {
  state.player.sprite = load_player_sprite();
  state.dragon.sprite = load_dragon_sprite();

  state.fade = FADE_VAL;
  state.level = getLevel(0);
  state.player.position = state.level.startingPosition;
  state.dragon.position = (Vector2){150, 100};
  state.player.velocity = (Vector2){0, 0};

  state.camera.zoom = 2.0f;
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

void set_camera_zoom() {
  if (state.level.is_big_level) {
    state.camera.zoom = 1.0f;
  }
  else
    state.camera.zoom = 2.0f;
}

int main () {
  InitWindow(resolution.x, resolution.y, "HackKU 2025");

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
    if (state.level.id == 13) player_update(&state.dragon, &state.level, dt); // dragon

    // Render the frame to texture
    BeginTextureMode(render_tex);
    set_camera_zoom(&state);
    BeginMode2D(state.camera);
    ClearBackground(DARKGRAY);
    level_draw(&state.level, &state.player, dt);
    level_draw(&state.level, &state.player, dt);
    player_draw(&state.player);
    if (state.level.id == 13) player_draw(&state.dragon); // dragon

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
      state.level = getLevel(state.level.id);
      state.fade = FADE_VAL;
      player_reset(&state.player, &state.level);
    }
    if (IsKeyPressed(KEY_EQUAL)){
      state.level = getLevel(0);
      state.fade = FADE_VAL;
      player_reset(&state.player, &state.level);
    }
    if (IsKeyPressed(KEY_MINUS)){
      state.level = getLevel(state.level.id+1);
      state.fade = FADE_VAL;
      player_reset(&state.player, &state.level);
    }
    if (IsKeyPressed(KEY_ZERO)){
      state.level = getLevel(state.level.id-1);
      state.fade = FADE_VAL;
      player_reset(&state.player, &state.level);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
