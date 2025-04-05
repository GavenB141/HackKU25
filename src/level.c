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
  level.platform_count = 1;

  return level;
}
