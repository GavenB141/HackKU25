#include "player.h"
#include "raylib.h"

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32

void player_update(Player* player, float dt) {
  
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
