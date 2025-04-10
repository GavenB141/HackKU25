#ifndef OBJECTS_H
#define OBJECTS_H

#include "animation.h"
#include "raylib.h"
#include <stdbool.h>

#define MAX_PLATFORMS 20
#define MAX_TRANS 4
#define MAX_SPIKES 10
#define MAX_ORBS 4
#define MAX_SENSORS 4

typedef struct {
  Rectangle bounds;
  AnimationController sprite;
  float rotation;
} Spike;

typedef struct {
  Vector2 position;

  // Real velocity, computed for collision purposes
  Vector2 velocity;

  bool positive;
  bool is_static;
  bool free;

  float range;

  // Pull by other orbs, overridden by player
  Vector2 weak_pull;

  // Pull by fixed magnets, abusable by player
  Vector2 strong_pull;
} MagneticOrb;

typedef struct {
  Rectangle bounds;
  int sensor_idx;
  bool gate;
  bool inverted;
} Platform;

typedef struct {
  Rectangle bounds;
  int sensed;
} Sensor;

typedef struct {
  Rectangle bounds;
  int transition_index;
} Transition;

typedef struct {
  Platform platforms[MAX_PLATFORMS];
  Transition transition[MAX_TRANS];
  Spike spikes[MAX_SPIKES];
  MagneticOrb orbs[MAX_ORBS];
  Sensor sensors[MAX_SENSORS];

  int platform_count;
  int transition_count;
  int spikes_count;
  int orbs_count;
  int sensor_count;
  Vector2 startingPosition;
  int id;
  bool is_big_level; // should be true for a big level and false for a small level
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
