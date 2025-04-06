#include "player.h"
#include "animation.h"
#include "level.h"
#include "objects.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
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

#define TAIL_SPACE 11
// For physics (doesn't include tail)
Rectangle get_player_bounds(const Player *player) {
  return (Rectangle) {
    player->position.x - PLAYER_WIDTH / 2.0 + (player->inverted ? 0 : 11),
    player->position.y - PLAYER_HEIGHT / 2.0,
    PLAYER_WIDTH - TAIL_SPACE,
    PLAYER_HEIGHT
  };
}

// For drawing
Rectangle get_player_sprite_bounds(const Player *player) {
  return (Rectangle) {
    player->position.x - PLAYER_WIDTH / 2.0,
    player->position.y - PLAYER_HEIGHT / 2.0,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };
}

void player_draw(Player* player) {
  const Rectangle bounds = get_player_sprite_bounds(player);

  animation_draw(&player->sprite, (Vector2){bounds.x, bounds.y}, player->inverted, 0);
}


void move(Player *player, float dt) {
  if((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && (player->jumptime > 0)){
    player->velocity.y -= JUMP_STRENGTH * dt;
    player->jumptime -= dt;
  }
  else player->jumptime = 0;
  if((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && player->grounded){
    player->velocity.y -= JUMP_STRENGTH * dt * .4;
    player->jumptime = 10.0/60.0;
  }
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


/**
 * Animation selection code
 */

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

void activate_jump_animation(Player *player) {
  int jump_indices[1] = {6};
  memcpy(&player->sprite.indices, jump_indices, sizeof(int));
  player->sprite.frames = 1;
  player->sprite.current_time = 0;
}

// Indexed by PlayerState
static void (*animation_selectors[])(Player *player) = {
  activate_idle_animation,
  activate_walk_animation,
  activate_jump_animation
};

void select_animation(Player *player) {
  PlayerState new_state = IDLE;

  if (player->velocity.x > 0.1) {
    new_state = WALK;
    player->inverted = false;
  } else if (player->velocity.x < -0.1) {
    new_state = WALK;
    player->inverted = true;
  }

  if (!player->grounded)
    new_state = JUMP;

  if (new_state != player->state)
    animation_selectors[new_state](player);

  player->state = new_state;
}

void player_reset(Player *player, Level *level) {
  player->position = level->startingPosition;
  player->velocity = (Vector2){0, 0};
  player->grounded = false;
  player->inverted = false;

  if (player->is_holding_orb) {
    level->orbs[player->targeted_orb].free = true;
    // level->orbs[player->targeted_orb].position.y += 5;
  }

  player->is_holding_orb = false;
  player->targeted_orb = -1;
  player->jumptime = 0;
  player->teleported = 1;
}

/**
 * Collision code
 */

static PlatformCollision platform_collisions[MAX_PLATFORMS] = {0};
static int collision_count = 0;

int detect_transition_collision(Player *player, Level *level) {
  const Rectangle player_bounds = get_player_bounds(player);
  for (int i = 0; i < level->transition_count; i++)
  {
    const Rectangle trans_bounds = level->transition[i].bounds;
    if (CheckCollisionRecs(player_bounds, trans_bounds)) {
      return level->transition->transition_index;
    }
  }
  return -1;
}

void detect_stage_collisions(Player *player, Level *level, double dt) {
  collision_count = 0;

  const Rectangle player_bounds = get_player_bounds(player);
  const Vector2 translation = Vector2Scale(player->velocity, dt);

  for (int i = 0; i < level->platform_count; i++) {
    if(level->platforms[i].gate){
      if(level->sensors[level->platforms->sensor_idx].sensed) { 
        if(level->platforms[i].inverted) {
        }
        else
        {
          continue;
        } 
      }
      if(level->sensors[level->platforms->sensor_idx].sensed == 0 && level->platforms[i].inverted) {
        continue;
      }
    }
    const Rectangle plat_bounds = level->platforms[i].bounds;
    const Rectangle overlap = GetCollisionRec(player_bounds, plat_bounds);
    if (overlap.width == 0 || overlap.height == 0) {
      continue;
    }

    if (overlap.width < 3 && overlap.height < 3) { // probably a corner
      continue;
    }

    Vector2 depth = {overlap.width, overlap.height};
    Vector2 times = {dt, dt};

    // full overlap correction
    // if (depth.x == PLAYER_WIDTH) {
    //   if (translation.x > 0) {
    //     depth.x = plat_bounds.width - (plat_bounds.width + plat_bounds.x - (player_bounds.x + player_bounds.width));
    //   } else {
    //     depth.x = plat_bounds.width - (player_bounds.x - plat_bounds.x);
    //   }
    // }
    //
    // if (depth.y == PLAYER_HEIGHT) {
    //   if (translation.y > 0) {
    //     depth.y = plat_bounds.height - (plat_bounds.height + plat_bounds.y - (player_bounds.y + player_bounds.height));
    //   } else {
    //     depth.y = plat_bounds.height - (player_bounds.y - plat_bounds.y);
    //   }
    // }

    if (translation.x != 0) {
      times.x = fabs(depth.x / translation.x) * dt;
    }
    if (translation.y != 0) {
      times.y = fabs(depth.y / translation.y) * dt;
    }

    Vector2 normal;
    float time;
    if (times.x >= times.y || fabs(translation.x) < 0.1) {
      normal = Vector2Normalize((Vector2){0, -translation.y});
      time = times.y; 
    } else {
      normal = Vector2Normalize((Vector2){-translation.x, 0});
      time = times.x;
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
    }
  }
}

void detect_death_collisions(Player *player, Level *level) {
  const Rectangle player_bounds = get_player_bounds(player);
  const Vector2 vertices[4] = {
    {player_bounds.x, player_bounds.y},
    {player_bounds.x, player_bounds.y + player_bounds.height},
    {player_bounds.x + player_bounds.width, player_bounds.y},
    {player_bounds.x + player_bounds.width, player_bounds.y + player_bounds.height},
  };

  for (int i = 0; i < level->spikes_count; i++) {
    const Vector2 position = {level->spikes[i].bounds.x + 16, level->spikes[i].bounds.y + 16};
    const float angle = level->spikes[i].rotation;

    Vector2 spike_vertices[3] = {
      Vector2Add(position, Vector2Rotate((Vector2){-12, 12}, angle)),
      Vector2Add(position, Vector2Rotate((Vector2){12, 12}, angle)),
      Vector2Add(position, Vector2Rotate((Vector2){0, -12}, angle)),
    };

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++) {
        bool touch = CheckCollisionLines(
          vertices[i], vertices[i == 3 ? 0 : i + 1],
          spike_vertices[j], spike_vertices[j == 2 ? 0 : j + 1], 0
        );

        if (touch) {
          player_reset(player, level);
        }
      }
    }
  }
}

void orb_manipulation(Player *player, Level *level) {
  if (player->is_holding_orb) {
    MagneticOrb *orb = &level->orbs[player->targeted_orb];
    orb->free = false;

    // Not used while held, but will be preserved when dropped
    // orb->velocity = player->velocity;
    orb->position = (Vector2){floor(player->position.x), floor(player->position.y) - 5}; 

    if(IsKeyPressed(KEY_LEFT_SHIFT)) {
      orb->free = true;
      orb->position.y += 5;
      player->is_holding_orb = false;
    } 

    return;
  }

  float min_distance = 25;
  int selectable_orb = -1;

  // Find nearest orb within a range
  for (int i = 0; i < level->orbs_count; i++) {
    MagneticOrb *orb = &level->orbs[i];

    if (orb->is_static) {
      continue;
    }

    const float distance = Vector2Distance(player->position, orb->position); 

    if (distance <= min_distance) {
      min_distance = distance;
      selectable_orb = i;
    }
  }

  player->targeted_orb = selectable_orb;

  if (IsKeyPressed(KEY_LEFT_SHIFT)) {
    player->is_holding_orb = true;
  }
}

/**
 * Public update code
 */
void player_update(Player *player, Level *level, float dt) {
  player->teleported = 0;

  Vector2 pull = Vector2Zero();
  // Handle magnetic flight
  if (player->is_holding_orb) {
    MagneticOrb *held = &level->orbs[player->targeted_orb];
    pull = held->strong_pull;
  }

  if (pull.y >= 0) {
    player->velocity.y += GRAVITY * dt;
  } else {
    player->velocity.y += GRAVITY * dt * 0.35;
  }

  player->velocity = Vector2Add(player->velocity, pull);

  player->position = Vector2Add(player->position, Vector2Scale(player->velocity, dt));
  detect_stage_collisions(player, level, dt);
  resolve_stage_collisions(player, level);

  player->velocity = Vector2Subtract(player->velocity, pull);


  int level_index = detect_transition_collision(player, level);
  detect_death_collisions(player, level);
  if(level_index != -1){
    printf("Level index: %d\n", level_index);    
    *level = getLevel(level_index);
    player_reset(player, level);
  }

  move(player, dt);
  orb_manipulation(player, level);

  select_animation(player);
  animation_update(&player->sprite, dt);
}

AnimationController load_player_sprite(){
  return (AnimationController) {
    .spritesheet = LoadTexture("assets/lizard_sprite.png"),
    .sprite_size = (Vector2){39, 19},
    .current_frame = 0,
    .frame_time = 0.1,
    .current_time = 0,
    .frames = 1,
    .indices = {0},
    .origin = Vector2Zero()
  };
}

