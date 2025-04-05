#include "level.h"
#include "animation.h"
#include "objects.h"
#include "raylib.h"
#include <stdio.h>

static Texture2D spike = {0};

void level_draw(Level *level, float dt) {
  static Texture ground_texture = {0};
  if (ground_texture.id == 0) {
    ground_texture = LoadTexture("assets/metal_crate_sprite.png");
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
}

void fade_in(float fade, Rectangle screen){
  DrawRectangleRec(screen, ColorAlpha(BLACK, fade));
}

Level getLevel(int level_index) {
  switch (level_index) {
    case 0:
      return sample_level();
      break;
    case 1:
      return transition_level();
      break;
    default:
      return sample_level();
      break;
  }
}

Level sample_level() {
  Level level = {0};
  level.startingPosition = (Vector2){100, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){318, 160, 1000, 48}, 1};

  level.transition_count = 1;
  level.platform_count = 4;
  
  return level;
}

Level transition_level() {
  Level level = {0};

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

  return level;
}
