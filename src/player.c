#include "player.h"
#include "level.h"
#include "raylib.h"
#include "raymath.h"

#define PLAYER_WIDTH 38
#define PLAYER_HEIGHT 19

// Pixels per second per second
#define GRAVITY 100

Rectangle get_player_bounds(const Player *player) {
  return (Rectangle) {
    player->position.x - PLAYER_WIDTH / 2.0,
    player->position.y - PLAYER_HEIGHT / 2.0,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };
}

void player_draw(Player* player) {
  DrawRectangleRec(get_player_bounds(player), ORANGE);
}

// Collision storage
static PlatformCollision platform_collisions[MAX_PLATFORMS] = {0};
static int collision_count = 0;

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

    if (platform_collisions[i].normal.y == -1) {
      player->grounded = true;
    }
  }
}

void player_update(Player *player, Level *level, float dt) {
  player->position = Vector2Add(player->position, Vector2Scale(player->velocity, dt));
  player->velocity.y += GRAVITY * dt;

  detect_stage_collisions(player, level, dt);
  resolve_stage_collisions(player, level);
}
