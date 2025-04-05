#include "objects.h"
#include "animation.h"
#include "raylib.h"

static Texture spike_texture = {0};

AnimationController get_spike_animation() {
  if (spike_texture.id == 0) {
    spike_texture = LoadTexture("assets/Spike.png");
  }

  return (AnimationController) {
    .spritesheet = spike_texture,
    .sprite_size = (Vector2){32, 32},
    .indices = {0, 1, 2},
    .frames = 3,
    .frame_time = 0.1
  };
}
