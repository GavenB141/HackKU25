#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

#define MAX_PLATFORMS 20
#define MAX_TRANS 4

typedef struct {
  Rectangle bounds;
} Platform;

typedef struct {
  Rectangle bounds;
  int transition_index;
} Transition;

typedef struct {
  Platform platforms[MAX_PLATFORMS];
  Transition transition[MAX_TRANS];
  int platform_count;
  int transition_count;
  Vector2 startingPosition;
} Level;

Level sample_level();
Level transition_level();

void level_draw(Level *level);
Level getLevel(int level_index);


#endif
