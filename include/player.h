#ifndef PLAYER_H
#define PLAYER_H

#define DASH_STRENGTH 360     
#define DASH_DURATION 10/60.0      
#define DASH_COOLDOWN .5 

#include "animation.h"
#include "raylib.h"
#include "level.h"
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
