#include "level.h"
#include "raylib.h"

void level_draw(Level *level) {
  for (int i = 0; i < level->platform_count; i++) {
    DrawRectangleRec(level->platforms[i].bounds, WHITE);
  }
}

Level sample_level() {
  Level level = {0};

  level.platforms[0] = (Platform){(Rectangle){0, 220, 320, 20}};
  level.platforms[1] = (Platform){(Rectangle){40, 0, 5, 240}};
  level.platforms[2] = (Platform){(Rectangle){150, 0, 5, 240}};
  level.platforms[3] = (Platform){(Rectangle){40, 0, 115, 10}};
  level.platform_count = 4;

  return level;
}
