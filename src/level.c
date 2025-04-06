#include "level.h"
#include "animation.h"
#include "objects.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

static Texture2D spike = {0};

void level_draw(Level *level, Player *player, float dt) {
  static Font foont = {0};
  static Texture ground_texture = {0};
  static Texture positive_orb_texture = {0};
  static Texture negative_orb_texture = {0};
  static Shader highlight_shader = {0};

  if (ground_texture.id == 0) {
    ground_texture = LoadTexture("assets/metal_crate_sprite.png");
    positive_orb_texture = LoadTexture("assets/positive_orb_sprite.png");
    negative_orb_texture = LoadTexture("assets/negative_orb_sprite.png");
    highlight_shader = LoadShader(0, "assets/shaders/highlight_outline.fs.glsl");
    foont = LoadFont("assets/setback.png");
  }

  for (int i = 0; i < level->spikes_count; i++) {
    animation_update(&level->spikes[i].sprite, dt);
    animation_draw(&level->spikes[i].sprite, (Vector2){level->spikes[i].bounds.x, level->spikes[i].bounds.y}, false);
  }

  for (int i = 0; i < level->platform_count; i++) {
    Rectangle bounds = level->platforms[i].bounds;
    Vector2 position = (Vector2){bounds.x, bounds.y};
    bounds.x = 0, bounds.y = 0;
    DrawTextureRec(ground_texture, bounds, position, WHITE);
  }

  for (int i = 0; i < level->orbs_count; i++) {
    if (i == player->targeted_orb && !player->is_holding_orb)
      BeginShaderMode(highlight_shader);

    DrawTexturePro(
       level->orbs[i].positive ? positive_orb_texture : negative_orb_texture,
       (Rectangle) {0, 0, 16, 16},
       get_orb_bounds(&level->orbs[i]),
       Vector2Zero(),
       0,
       WHITE
    );

    if (i == player->targeted_orb && !player->is_holding_orb)
      EndShaderMode();
  }
  switch (level->id){
    case 0: DrawTextEx(foont, "Press A or D to WALK", (Vector2){70,120},16, 1, WHITE);
      break;
    case 1: DrawTextEx(foont, "Press W to JUMP", (Vector2){70,120},16, 1, WHITE);
    break;
    case 2: DrawTextEx(foont, "Press R to RESET", (Vector2){70,120},16, 1, WHITE);
    default: break;
  }
}

void level_update(Level *level, float dt) {
  for (int i = 0; i < level->orbs_count; i++) {
    orb_update(&level->orbs[i], level, dt);
  }
  for (int i = 0; i < level->spikes_count; i++) {
    animation_update(&level->spikes[i].sprite, dt);
    animation_draw(&level->spikes[i].sprite, (Vector2){level->spikes[i].bounds.x, level->spikes[i].bounds.y}, false);
  }
}

void fade_in(float fade, Rectangle screen){
  DrawRectangleRec(screen, ColorAlpha(BLACK, fade));
}

Level getLevel(int level_index) {
  switch (level_index) {
    case 0:
      return tutorial_0();
      break;
    case 1:
      return tutorial_1();
      break;
    case 2:
      return tutorial_2();
      break;
    case 3:
      return gaven_level();
      break;
    default:
      return tutorial_0();
      break;
  }
}

Level tutorial_0() {
  Level level = {0};
  level.id = 0;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){318, 160, 1000, 48}, 1};

  level.transition_count = 1;
  level.platform_count = 4;
  
  return level;
}

Level tutorial_1() {
  Level level = {0};
  level.id = 1;
  level.startingPosition = (Vector2){50, 185};

  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){256, 112, 64, 96}};
  level.platforms[3] = (Platform){(Rectangle){256-64-64, 112+48+16, 64, 96-48-16}};
  level.platforms[4] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){318, 100, 1000, 100}, 2};

  level.transition_count = 1;
  level.platform_count = 5;
  return level;
}

Level tutorial_2() {
  Level level = {0};
  level.id = 2;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 6*16, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){318, 160, 1000, 48}, 0};

  level.platforms[4] = (Platform){(Rectangle){14*16, 208, 10*16, 32}};

  level.transition_count = 1;
  level.platform_count = 5;
  
  return level;
}

Level gaven_level() {
  Level level = {0};
  level.id = 6969;
  level.startingPosition = (Vector2){50, 20};
  
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 176}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 176}};
  level.platforms[3] = (Platform){(Rectangle){0, 176, 160, 32}};
  level.platforms[4] = (Platform){(Rectangle){256, 176, 64, 32}};
  level.platform_count = 5;

  level.spikes[0] = (Spike){(Rectangle){160, 176, 32, 32}, get_spike_animation()};
  level.spikes[1] = (Spike){(Rectangle){192, 176, 32, 32}, get_spike_animation()};
  level.spikes[2] = (Spike){(Rectangle){224, 176, 32, 32}, get_spike_animation()};
  level.spikes_count = 3;

  level.orbs[0] = (MagneticOrb){{20, 20}, (Vector2){0, 0}, false, true};
  level.orbs[1] = (MagneticOrb){{300, 20}, (Vector2){0, 0}, true,  true};
  level.orbs_count = 2;

  return level;
}

//level.spikes[0] = (Spike){(Rectangle){160, 176, 32, 32}, get_spike_animation()};