#ifndef PLAYER_H
#define PLAYER_H

#define DASH_STRENGTH 360     
#define DASH_DURATION 10/60.0      
#define DASH_COOLDOWN 1 

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
  float dash_timer;         
  float dash_cooldown_timer; 
  int teleported;

  int targeted_orb;
  bool is_holding_orb;
} Player;

void player_update(Player *state, Level *level, float dt);
void player_draw(Player *player);
AnimationController load_player_sprite();

#endif
