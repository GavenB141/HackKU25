#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
  Vector2 position;
  Vector2 velocity;
} Player;

void player_update(Player* player, float dt);
void player_draw(Player* player);

#endif
