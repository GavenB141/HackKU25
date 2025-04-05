#include "level.h"
#include "animation.h"
#include "objects.h"
#include "raylib.h"
#include <stdio.h>

static Texture2D spike = {0};

void level_draw(Level *level, float dt) {
  static Font foont = {0};
  static Texture ground_texture = {0};
  if (ground_texture.id == 0) {
    ground_texture = LoadTexture("assets/metal_crate_sprite.png");
    foont = LoadFont("assets/setback.png");
  }

  for (int i = 0; i < level->platform_count; i++) {
    Rectangle bounds = level->platforms[i].bounds;
    Vector2 position = (Vector2){bounds.x, bounds.y};
    bounds.x = 0, bounds.y = 0;
    DrawTextureRec(ground_texture, bounds, position, WHITE);
  }

  for (int i = 0; i < level->spikes_count; i++) {
    animation_update(&level->spikes[i].sprite, dt);
    animation_draw(&level->spikes[i].sprite, (Vector2){level->spikes[i].bounds.x, level->spikes[i].bounds.y}, false);
  }
  switch (level->id){
    case 0: DrawTextEx(foont, "Press A or D to WALK", (Vector2){70,120},16, 1, WHITE);
      break;
    case 1: break;
    default: break;
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
  level.transition[0] = (Transition){(Rectangle){318, 160, 1000, 48}, 0};

  level.transition_count = 1;
  level.platform_count = 5;
  return level;
}
//level.spikes[0] = (Spike){(Rectangle){160, 176, 32, 32}, get_spike_animation()};