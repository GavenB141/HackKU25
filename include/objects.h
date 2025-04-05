#ifndef OBJECTS_H
#define OBJECTS_H

#include "animation.h"
#include "raylib.h"
#include <stdbool.h>

#define MAX_PLATFORMS 20
#define MAX_TRANS 4
#define MAX_SPIKES 4
#define MAX_ORBS 4

typedef struct {
  Rectangle bounds;
  AnimationController sprite;
} Spike;

typedef struct {
  Vector2 position;
  Vector2 velocity;
  bool positive;
  bool free;
} MagneticOrb;

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
  Spike spikes[MAX_SPIKES];
  MagneticOrb orbs[MAX_ORBS];

  int platform_count;
  int transition_count;
  int spikes_count;
  int orbs_count;
  Vector2 startingPosition;
} Level;

typedef struct {
  Vector2 depth;
  Vector2 normal;
  float time;
  int platform_id;
} PlatformCollision;

AnimationController get_spike_animation();

void orb_update(MagneticOrb *orb, Level *level, float dt);

Rectangle get_orb_bounds(const MagneticOrb *orb);

#endif
