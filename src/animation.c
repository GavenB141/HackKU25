#include "animation.h"
#include <math.h>

void animation_update(AnimationController *controller, float dt) {
  controller->current_time += fmod(
    controller->current_time + dt,
    controller->frames * controller->frame_time
  );

  controller->current_frame = floor(controller->current_time / controller->frame_time); 
}
