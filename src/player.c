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

void player_update(Player *player, Level *level, float dt) {
  player->position = Vector2Add(player->position, Vector2Scale(player->velocity, dt));
  player->velocity.y += GRAVITY * dt;
}

void player_draw(Player* player) {
  DrawRectangleRec(get_player_bounds(player), ORANGE);
}



void detect_stage_collisions(Player *player, Level *level, float dt) {
  const Rectangle player_bounds = get_player_bounds(player);
  const Vector2 translation = Vector2Scale(player->velocity, dt);

  for (int i = 0; i < level->platform_count; i++) {
    const Rectangle plat_bounds = level->platforms[i].bounds;
    const Rectangle overlap = GetCollisionRec(player_bounds, plat_bounds);
    if (overlap.width == 0 || overlap.height == 0) {
      continue;
    }

    const Vector2 depth = {overlap.width, overlap.height};

    const Vector2 times = Vector2Subtract(translation, depth);
  }
}
