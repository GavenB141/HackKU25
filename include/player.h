#ifndef PLAYER_H
#define PLAYER_H

#include "animation.h"
#include "raylib.h"
#include "objects.h"
#include <stdbool.h>

typedef enum {
  IDLE,
  WALK,
  JUMP
} PlayerState;

typedef struct {
  AnimationController sprite;
  Vector2 position;
  Vector2 velocity;
  bool grounded;
  bool inverted;
  float jumptime;
  PlayerState state;
  int teleported;

  int targeted_orb;
  bool is_holding_orb;
} Player;

void player_update(Player *state, Level *level, float dt);
void player_draw(Player *player);
AnimationController load_player_sprite();
void player_reset(Player *player, Level *level);

#endif
