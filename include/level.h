#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

#define MAX_PLATFORMS 20
#define MAX_TRANS 4
#define MAX_DEATH 4

typedef struct {
  Rectangle bounds;
} Platform;

typedef struct {
  Rectangle bounds;
  int transition_index;
} Transition;

typedef struct {
  Rectangle bounds;
} Death;

typedef struct {
  Platform platforms[MAX_PLATFORMS];
  Transition transition[MAX_TRANS];
  Death death[MAX_DEATH];
  int platform_count;
  int transition_count;
  int death_count;
  Vector2 startingPosition;
} Level;

Level sample_level();
Level transition_level();

void level_draw(Level *level);
Level getLevel(int level_index);


#endif
