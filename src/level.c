#include "level.h"
#include "raylib.h"
#include <stdio.h>

void level_draw(Level *level) {
  for (int i = 0; i < level->platform_count; i++) {
    DrawRectangleRec(level->platforms[i].bounds, WHITE);
  }
  for (int i = 0; i < level->death_count; i++) {
    DrawRectangleRec(level->death[i].bounds, RED);
  }
}

Level getLevel(int level_index) {
  Level level;
  switch (level_index) {
    case 0:
      level = sample_level();
      break;
    case 1:
      level = transition_level();
      break;
    default:
      break;
  }
  return level;
}

Level sample_level() {
  Level level = {0};
  level.startingPosition = (Vector2){100, 20};
  level.platforms[0] = (Platform){(Rectangle){0, 220, 320, 20}};
  level.platforms[1] = (Platform){(Rectangle){40, 0, 5, 240}};
  level.transition[0] = (Transition){(Rectangle){150, 0, 5, 240}, 1};
  level.transition_count = 1;
  level.platform_count = 2;
  
  return level;
}
Level transition_level() {
  printf("Transition level\n");
  Level level = {0};
  level.startingPosition = (Vector2){100, 20};
  level.platforms[0] = (Platform){(Rectangle){0, 220, 320, 20}};
  level.platforms[1] = (Platform){(Rectangle){40, 0, 5, 240}};
  level.platforms[2] = (Platform){(Rectangle){150, 0, 5, 0}};
  level.platforms[3] = (Platform){(Rectangle){40, 0, 115, 10}};
  level.death[0] = (Death){(Rectangle){300, 0, 10, 200}};

  level.death_count = 1;
  level.platform_count = 4;
  return level;
}
