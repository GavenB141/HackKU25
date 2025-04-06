#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_ANIMATION_LENGTH 6

typedef struct {
  Texture spritesheet;
  Vector2 sprite_size;
  Vector2 origin;

  int indices[MAX_ANIMATION_LENGTH];
  int frames;

  int current_frame;
  float current_time;
  float frame_time;
} AnimationController;

void animation_update(AnimationController *controller, float dt);
void animation_draw(AnimationController *controller, Vector2 position, bool invert, float rotation);

#endif
