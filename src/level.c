#include "level.h"
#include "animation.h"
#include "objects.h"
#include "raylib.h"
#include <stdio.h>

static Texture2D spike = {0};

void level_draw(Level *level, float dt) {
  for (int i = 0; i < level->platform_count; i++) {
    DrawRectangleRec(level->platforms[i].bounds, WHITE);
  }

  for (int i = 0; i < level->spikes_count; i++) {
    animation_update(&level->spikes[i].sprite, dt);
    animation_draw(&level->spikes[i].sprite, (Vector2){level->spikes[i].bounds.x, level->spikes[i].bounds.y}, false);
  }
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
  level.startingPosition = (Vector2){100, 20};
  level.platforms[0] = (Platform){(Rectangle){0, 220, 320, 200000}};
  level.platforms[1] = (Platform){(Rectangle){40, 0, 5, 240}};
  level.transition[0] = (Transition){(Rectangle){340, 0, 100, 240}, 1};
  level.transition_count = 1;
  level.platform_count = 2;
  
  return level;
}
Level transition_level() {
  printf("Transition level\n");
  Level level = {0};
  level.startingPosition = (Vector2){100, 20};
  level.platforms[0] = (Platform){(Rectangle){0, 220, 320, 20}};
  level.platforms[1] = (Platform){(Rectangle){20, 0, 10, 240}};
  level.platforms[2] = (Platform){(Rectangle){150, 0, 5, 0}};
  level.platforms[3] = (Platform){(Rectangle){40, 0, 115, 10}};
  level.spikes[0] = (Spike){(Rectangle){200, 188, 32, 32}, get_spike_animation()};

  level.spikes_count = 1;
  level.platform_count = 4;
  return level;
}
