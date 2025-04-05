#include "player.h"
#include "raylib.h"
#include "raymath.h"

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32

// Pixels per second per second
#define GRAVITY 100

void player_update(Player* player, float dt) {
  player->position = Vector2Add(player->position, Vector2Scale(player->velocity, dt));

  player->velocity.y += GRAVITY * dt;
}

void player_draw(Player* player) {
  DrawRectangleLines(
    player->position.x - PLAYER_WIDTH / 2.0,
    player->position.y - PLAYER_HEIGHT / 2.0,
    PLAYER_WIDTH,
    PLAYER_HEIGHT,
    ORANGE
  );
}
