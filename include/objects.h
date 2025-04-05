#ifndef OBJECTS_H
#define OBJECTS_H

#include "animation.h"
#include "raylib.h"
#include <stdbool.h>

typedef struct {
  Rectangle bounds;
  AnimationController sprite;
} Spike;

typedef struct {
  Vector2 position;
  bool positive;
} MagneticOrb;

AnimationController get_spike_animation();
void orb_update(MagneticOrb *orb, float dt);

#endif
