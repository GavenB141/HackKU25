#ifndef OBJECTS_H
#define OBJECTS_H

#include "animation.h"
#include "raylib.h"

typedef struct {
  Rectangle bounds;
  AnimationController sprite;
} Spike;

AnimationController get_spike_animation();

#endif
