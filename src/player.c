#include "player.h"
#include "animation.h"
#include "level.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>

#define PLAYER_WIDTH 39
#define PLAYER_HEIGHT 19
#define JUMP_STRENGTH 2000
#define MOVE_STRENGTH 400
#define VEL_X_MAX 125
#define FRICTION 80

// Pixels per second per second
#define GRAVITY 600

static int s_gravity = GRAVITY;

Rectangle get_player_bounds(const Player *player) {
  return (Rectangle) {
    player->position.x - PLAYER_WIDTH / 2.0,
    player->position.y - PLAYER_HEIGHT / 2.0,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };
}

void player_draw(Player* player) {
  const Rectangle bounds = get_player_bounds(player);

  animation_draw(&player->sprite, (Vector2){bounds.x, bounds.y}, player->inverted);
}

// Collision storage
static PlatformCollision platform_collisions[MAX_PLATFORMS] = {0};
static int collision_count = 0;

int detect_transition_collision(Player *player, Level *level) {
  const Rectangle player_bounds = get_player_bounds(player);
  for (int i = 0; i < level->transition_count; i++)
  {
    const Rectangle trans_bounds = level->transition[i].bounds;
    const Rectangle overlap = GetCollisionRec(player_bounds, trans_bounds);
    if (overlap.width == 0 || overlap.height == 0) {
      continue;
    }
    return level->transition->transition_index;
  }
  return -1;
}

void detect_stage_collisions(Player *player, Level *level, float dt) {
  collision_count = 0;

  const Rectangle player_bounds = get_player_bounds(player);
  const Vector2 translation = Vector2Scale(player->velocity, dt);

  for (int i = 0; i < level->platform_count; i++) {
    const Rectangle plat_bounds = level->platforms[i].bounds;
    const Rectangle overlap = GetCollisionRec(player_bounds, plat_bounds);
    if (overlap.width == 0 || overlap.height == 0) {
      continue;
    }

    const Vector2 depth = {overlap.width, overlap.height};
    Vector2 times = {dt, dt};

    if (translation.x != 0) {
      times.x = fabs(depth.x / translation.x) * dt;
    }
    if (translation.y != 0) {
      times.y = fabs(depth.y / translation.y) * dt;
    }

    Vector2 normal;
    float time;
    if (times.x <= times.y) {
      normal = Vector2Normalize((Vector2){-translation.x, 0});
      time = times.x;
    } else {
      normal = Vector2Normalize((Vector2){0, -translation.y});
      time = times.y;
    }

    platform_collisions[collision_count++] = (PlatformCollision) {
      .depth = depth,
      .normal = normal,
      .time = time,
      .platform_id = i
    };
  }
}

void resolve_stage_collisions(Player *player, Level *level) {
  player->grounded = false;
  s_gravity = GRAVITY;

  for (int i = 0; i < collision_count; i++) {
    Vector2 normal_depth = Vector2Multiply(platform_collisions[i].normal, platform_collisions[i].depth);
    player->position = Vector2Add(player->position, normal_depth);
    Vector2 velocity_correction = Vector2Multiply(player->velocity, platform_collisions[i].normal);

    if (platform_collisions[i].normal.y) {
      if (player->velocity.y < 0) {
        velocity_correction = Vector2Negate(velocity_correction);
      }
    } else if (platform_collisions[i].normal.x) {
      if (player->velocity.x < 0) {
        velocity_correction = Vector2Negate(velocity_correction);
      }
    }

    player->velocity = Vector2Add(player->velocity, velocity_correction);

    if (platform_collisions[i].normal.y < 0) {
      player->grounded = true;
      s_gravity = 0;
      player->position.y -= platform_collisions[i].depth.y;
      player->velocity.y /= 2;
    }
  }
}

void move(Player *player, float dt) {
  if(IsKeyDown(KEY_W) && (player->jumptime > 0)){
    player->velocity.y -= JUMP_STRENGTH * dt;
    player->jumptime -= dt;
  }
  else player->jumptime = 0;
  if(IsKeyDown(KEY_W) && player->grounded){
    player->velocity.y -= JUMP_STRENGTH * dt * .4;
    player->jumptime = 10.0/60.0;
  }
  float mult = 1;
  if((IsKeyDown(KEY_A) && (player->velocity.x > 0)) || (IsKeyDown(KEY_D) && (player->velocity.x < 0))){
    mult = 1.3;
  }
  float del_x = MOVE_STRENGTH * dt * mult;
  if(IsKeyDown(KEY_A)){
    player->velocity.x -= MOVE_STRENGTH * dt;
    if (player->velocity.x > 0) player->velocity.x = 0;
    player->inverted = true;
    if (player->velocity.x > 0) player->velocity.x = 0;
    player->inverted = true;
  }
  if(IsKeyDown(KEY_D)){
    player->velocity.x += MOVE_STRENGTH * dt;
    if (player->velocity.x < 0) player->velocity.x = 0;
    player->inverted = false;
    if (player->velocity.x < 0) player->velocity.x = 0;
    player->inverted = false;
  }
  if(player->grounded){
    if (!(IsKeyDown(KEY_A) || IsKeyDown(KEY_D))) player->velocity.x /= FRICTION * dt;
  }
  if(fabs(player->velocity.x) > VEL_X_MAX) player->velocity.x *= VEL_X_MAX/fabs(player->velocity.x);
}

void activate_walk_animation(Player *player) {
  int walk_indices[6] = {0,1,2,3,4,5};
  memcpy(&player->sprite.indices, walk_indices, 6 * sizeof(int));
  player->sprite.frames = 6;
  player->sprite.current_time = 0;
}

void activate_idle_animation(Player *player) {
  int idle_indices[1] = {0};
  memcpy(&player->sprite.indices, idle_indices, sizeof(int));
  player->sprite.frames = 1;
  player->sprite.current_time = 0;
}

// Indexed by PlayerState
static void (*animation_selectors[])(Player *player) = {
  activate_idle_animation,
  activate_walk_animation
};

void select_animation(Player *player) {
  PlayerState new_state;
  if (player->grounded == false) {
    new_state = IDLE;
  } else if (player->velocity.x > 0.1) {
    new_state = WALK;
    player->inverted = false;
  } else if (player->velocity.x < -0.1) {
    new_state = WALK;
    player->inverted = true;
  } else {
    new_state = IDLE;
  }

  if (new_state != player->state)
    animation_selectors[new_state](player);

  player->state = new_state;
}

void player_reset(Player *player, Level *level) {
  player->position = level->startingPosition;
  player->velocity = (Vector2){0, 0};
  player->grounded = false;
  s_gravity = GRAVITY;
  player->jumptime = 0;
}

void detect_death_collisions(Player *player, Level *level) {
  const Rectangle player_bounds = get_player_bounds(player);
  for (int i = 0; i < level->death_count; i++)
  {
    const Rectangle bounds = level->death[i].bounds;
    const Rectangle overlap = GetCollisionRec(player_bounds, bounds);
    if (overlap.width < 3 || overlap.height < 7) {
      continue;
    }
    player_reset(player, level);
  }
}

void player_update(Player *player, Level *level, float dt) {
  player->velocity.y += s_gravity * dt;
  player->position = Vector2Add(player->position, Vector2Scale(player->velocity, dt));

  detect_stage_collisions(player, level, dt);
  resolve_stage_collisions(player, level);
  int level_index = detect_transition_collision(player, level);
  detect_death_collisions(player, level);
  if(level_index != -1){
    printf("Level index: %d\n", level_index);    
    *level = getLevel(level_index);
    player_reset(player, level);
  }
  move(player, dt);
  select_animation(player);
  animation_update(&player->sprite, dt);

  //printf("%f\n", player->velocity.y);
}

AnimationController load_player_sprite(){
  return (AnimationController) {
    .spritesheet = LoadTexture("assets/lizard_sprite.png"),
    .sprite_size = (Vector2){39, 19},
    .current_frame = 0,
    .frame_time = 0.15,
    .current_time = 0,
    .frames = 1,
    .indices = {0}
  };
}

