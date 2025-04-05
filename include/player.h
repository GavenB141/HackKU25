#ifndef PLAYER_H
#define PLAYER_H

#include "animation.h"
#include "raylib.h"
#include "level.h"
#include <stdbool.h>

typedef enum {
  IDLE,
  WALK
} PlayerState;

typedef struct {
  AnimationController sprite;
  Vector2 position;
  Vector2 velocity;
  bool grounded;
  bool inverted;
  float jumptime;
  PlayerState state;
} Player;

typedef struct {
  Vector2 depth;
  Vector2 normal;
  float time;
  int platform_id;
} PlatformCollision;

void player_update(Player *state, Level *level, float dt);
void player_draw(Player *player);
AnimationController load_player_sprite();

#endif
