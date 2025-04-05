#include "objects.h"
#include "animation.h"
#include "raylib.h"
#include "raymath.h"

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

/**********
  * ORBS *
 **********/

/**
 * Sloppy modification of player collision code for orbs
 */

void orb_handle_stage_collisions(MagneticOrb *orb, Level *level, double dt) {
  PlatformCollision orb_platform_collisions[MAX_PLATFORMS] = {0};
  int orb_collision_count = 0;

  const Rectangle orb_bounds = get_orb_bounds(orb);
  const Vector2 translation = Vector2Scale(orb->velocity, dt);

  // Detection
  for (int i = 0; i < level->platform_count; i++) {
    const Rectangle plat_bounds = level->platforms[i].bounds;
    const Rectangle overlap = GetCollisionRec(orb_bounds, plat_bounds);
    if (overlap.width == 0 || overlap.height == 0) {
      continue;
    }

    Vector2 depth = {overlap.width, overlap.height};
    Vector2 times = {dt, dt};

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

    orb_platform_collisions[orb_collision_count++] = (PlatformCollision) {
      .depth = depth,
      .normal = normal,
      .time = time,
      .platform_id = i
    };
  }

  // resolve
  for (int i = 0; i < orb_collision_count; i++) {
    Platform *platform = &level->platforms[orb_platform_collisions[i].platform_id];
    Vector2 normal_depth = Vector2Multiply(orb_platform_collisions[i].normal, orb_platform_collisions[i].depth);

    orb->position = Vector2Add(orb->position, normal_depth);
    Vector2 velocity_correction = Vector2Multiply(orb->velocity, orb_platform_collisions[i].normal);

    if (orb_platform_collisions[i].normal.y) {
      if (orb->velocity.y < 0) {
        velocity_correction = Vector2Negate(velocity_correction);
      }
    } else if (orb_platform_collisions[i].normal.x) {
      if (orb->velocity.x < 0) {
        velocity_correction = Vector2Negate(velocity_correction);
      }
    }

    orb->velocity = Vector2Add(orb->velocity, velocity_correction);
  }

}

#define ORB_GRAVITY 600

void orb_update(MagneticOrb *orb, Level *level, float dt) {
  orb->velocity.y += ORB_GRAVITY * dt;
  orb->position = Vector2Add(orb->position, Vector2Scale(orb->velocity, dt));
  orb_handle_stage_collisions(orb, level, dt);
}

Rectangle get_orb_bounds(const MagneticOrb *orb) {
  return (Rectangle) {
    orb->position.x - 8,
    orb->position.y - 8,
    16, 16
  };
}
