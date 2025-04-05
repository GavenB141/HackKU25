#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "level.h"

typedef struct {
  Vector2 position;
  Vector2 velocity;
} Player;

typedef struct {
  Vector2 depth;
  Vector2 normal;
  float time;
  int platform_id;
} PlatformCollision;

void player_update(Player *state, Level *level, float dt);
void player_draw(Player *player);

#endif
