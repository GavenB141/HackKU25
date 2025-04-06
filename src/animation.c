#include "animation.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

void animation_update(AnimationController *controller, float dt) {
  controller->current_time = fmod(
    controller->current_time + dt,
    controller->frames * controller->frame_time
  );

  controller->current_frame = floor(controller->current_time / controller->frame_time); 
}

void animation_draw(AnimationController *controller, Vector2 position, bool inverted, float rotation) {
  DrawTexturePro(
    controller->spritesheet,
    (Rectangle) {
      controller->sprite_size.x * controller->indices[controller->current_frame],
      0,
      controller->sprite_size.x * (inverted ? -1 : 1),
      controller->sprite_size.y
    },
    (Rectangle) {
      controller->origin.x + position.x,
      controller->origin.y + position.y,
      controller->sprite_size.x,
      controller->sprite_size.y
    },
    controller->origin, rotation, WHITE
  );
}
