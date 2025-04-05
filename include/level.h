#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

#define MAX_PLATFORMS 20

typedef struct {
  Rectangle bounds;
} Platform;

typedef struct {
  Platform platforms[MAX_PLATFORMS];
  int platform_count;
} Level;

Level sample_level();

void level_draw(Level *level);

#endif
