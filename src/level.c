#include "level.h"
#include "animation.h"
#include "objects.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"

static Texture2D spike = {0};

void level_draw(Level *level, Player *player, float dt) {
  static Font foont = {0};
  static Texture ground_texture = {0};
  static Texture gate_texture = {0};
  static Texture positive_orb_texture = {0};
  static Texture negative_orb_texture = {0};
  static Shader highlight_shader = {0};

  if (ground_texture.id == 0) {
    gate_texture = LoadTexture("assets/gate_texture_sprite.png"); //replace with path to gate image
    ground_texture = LoadTexture("assets/metal_crate_sprite.png");
    positive_orb_texture = LoadTexture("assets/positive_orb_sprite.png");
    negative_orb_texture = LoadTexture("assets/negative_orb_sprite.png");
    highlight_shader = LoadShader(0, "assets/shaders/highlight_outline.fs.glsl");
    foont = LoadFont("assets/setback.png");
  }

  for (int i = 0; i < level->sensor_count; i++) {
    DrawRectangleRec(level->sensors[i].bounds, ColorAlpha(level->sensors[i].sensed ? LIME : DARKGREEN, 0.25));
    DrawRectangleLinesEx(level->sensors[i].bounds, 1, ColorAlpha(level->sensors[i].sensed ? LIME : DARKGREEN, 0.5));
  }

  for (int i = 0; i < level->spikes_count; i++) {
    Spike *spike = &level->spikes[i];

    animation_update(&spike->sprite, dt);
    animation_draw(&spike->sprite, (Vector2){level->spikes[i].bounds.x, level->spikes[i].bounds.y}, false, spike->rotation);
  }

  for (int i = 0; i < level->platform_count; i++) {
    Rectangle bounds = level->platforms[i].bounds;
    Vector2 position = (Vector2){bounds.x, bounds.y};
    bounds.x = 0, bounds.y = 0;
    float alpha = 1;
    if(level->platforms[i].gate){
      if(level->sensors[level->platforms->sensor_idx].sensed) { alpha = 0.25; }
      DrawTextureRec(gate_texture, bounds, position, ColorAlpha(WHITE, alpha));
    }
    else{
      DrawTextureRec(ground_texture, bounds, position, WHITE);
    }
  }

  for (int i = 0; i < level->orbs_count; i++) {
    MagneticOrb *orb = &level->orbs[i];
    Color aura = orb->positive ? BLUE : RED;

    DrawCircleV(orb->position, orb->range, ColorAlpha(aura, 0.25));
    DrawCircleLinesV(orb->position, orb->range, ColorAlpha(aura, 0.5));

    if (i == player->targeted_orb && !player->is_holding_orb)
      BeginShaderMode(highlight_shader);

    DrawTexturePro(
       orb->positive ? positive_orb_texture : negative_orb_texture,
       (Rectangle) {0, 0, 16, 16},
       get_orb_bounds(orb),
       Vector2Zero(),
       0,
       WHITE
    );

    if (i == player->targeted_orb && !player->is_holding_orb)
      EndShaderMode();
  }



  switch (level->id){
    case 0: DrawTextEx(foont, "Press A or D to WALK", (Vector2){70,110},16, 1, WHITE);
      break;
    case 1: DrawTextEx(foont, "Press W to JUMP", (Vector2){70,110},16, 1, WHITE);
    break;
    case 2: DrawTextEx(foont, "Press R to RESET", (Vector2){85,110},16, 1, WHITE);
    break;
    case 3: DrawTextEx(foont, "Avoid Hazards", (Vector2){100,110},16, 1, WHITE);
    break;
    case 4: DrawTextEx(foont, "Move the ball to open the gate", (Vector2){20,20},16, 1, WHITE);
    DrawTextEx(foont, "Pickup or drop the magnets with SHIFT", (Vector2){20+24,36+6},16, 1, WHITE);
    break;
    default: break;
  }
}

void level_update(Level *level, float dt) {
  for (int i = 0; i < level->sensor_count; i++) {
    level->sensors[i].sensed = 0;
  }

  for (int i = 0; i < level->orbs_count; i++) {
    orb_update(&level->orbs[i], level, dt);
  }
}

void fade_in(float fade, Rectangle screen){
  DrawRectangleRec(screen, ColorAlpha(BLACK, fade));
}

Level getLevel(int level_index) {
  switch (level_index) {
    case 0:
      return tutorial_0();
      break;
    case 1:
      return tutorial_1();
      break;
    case 2:
      return tutorial_2();
      break;
    case 3:
      return tutorial_3();
      break;
    case 4:
      return tutorial_4();
      break;
    case 5:
      return spikes4();
      break;
    case 6:
      return static_magnets();
      break;
    default:
      return tutorial_0();
      break;
  }
}

static MagneticOrb construct_orb(Vector2 position, float range, bool positive, bool is_static) {
  MagneticOrb orb = {0};
  orb.position = position;
  orb.positive = positive;
  orb.free = true;
  orb.range = range;
  orb.is_static = is_static;
  return orb;
}

Level tutorial_0() {
  Level level = {0};
  level.id = 0;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 1};

  level.transition_count = 1;
  level.platform_count = 4;
  
  return level;
}

Level tutorial_1() {
  Level level = {0};
  level.id = 1;
  level.startingPosition = (Vector2){50, 185};

  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){256, 112, 64, 96}};
  level.platforms[3] = (Platform){(Rectangle){256-64-64, 112+48+16, 64, 96-48-16}};
  level.platforms[4] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){338, 100, 1000, 100}, 2};

  level.transition_count = 1;
  level.platform_count = 5;
  return level;
}

Level tutorial_2() {
  Level level = {0};
  level.id = 2;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 6*16, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 3};

  level.platforms[4] = (Platform){(Rectangle){14*16, 208, 10*16, 32}};

  level.transition_count = 1;
  level.platform_count = 5;
  
  return level;
}

Level tutorial_3() {
  Level level = {0};
  level.id = 3;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 4};

  level.spikes[0] = (Spike){(Rectangle){160-32*2, 176, 32, 32}, get_spike_animation()};
  level.spikes[1] = (Spike){(Rectangle){160+32*4, 176-64, 32, 32}, get_spike_animation(), 270.0f};
  level.spikes[2] = (Spike){(Rectangle){192, 176, 32, 32}, get_spike_animation()};
  level.spikes[3] = (Spike){(Rectangle){224, 176, 32, 32}, get_spike_animation()};

  level.spikes_count = 4;

  
  level.transition_count = 1;
  level.platform_count = 4;
  
  return level;
}

Level spikes4() {
  Level level = {0};
  level.id = 5;
  level.startingPosition = (Vector2){50, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 320}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 0};

  level.spikes[0] = (Spike){(Rectangle){160-32, 176, 32, 32}, get_spike_animation()};
  level.spikes[1] = (Spike){(Rectangle){160, 176, 32, 32}, get_spike_animation()};
  level.spikes[2] = (Spike){(Rectangle){192, 176, 32, 32}, get_spike_animation()};
  level.spikes[3] = (Spike){(Rectangle){224, 176, 32, 32}, get_spike_animation()};

  level.spikes_count = 4;

  
  level.transition_count = 1;
  level.platform_count = 4;
  
  return level;
}

Level tutorial_4() {
  Level level = {0};
  level.id = 4;
  level.startingPosition = (Vector2){20, 200};
  
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 320}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 176}};
  level.platforms[3] = (Platform){(Rectangle){0, 144, 320, 32}};
  level.platforms[4] = (Platform){(Rectangle){300, 176, 32, 32},0,1}; // gate reading sensor 0
  level.platform_count = 5;

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 1};
  level.transition_count = 1;

  level.sensors[0] = (Sensor){(Rectangle){0, 112, 32, 32}, 0};
  level.sensor_count = 1;

  level.orbs[0] = construct_orb((Vector2){40, 200}, 60.0, true, false);
  level.orbs[1] = construct_orb((Vector2){300, 20}, 60.0, false, false);
  level.orbs_count = 2;

  return level;
}

Level static_magnets() {
  Level level = {0};

  level.id = 6;
  level.startingPosition = (Vector2){20, 200};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 64, 32}};
  level.platforms[1] = (Platform){(Rectangle){256, 208, 64, 32}};
  level.platforms[2] = (Platform){(Rectangle){0, 0, 320, 96}};
  level.platform_count = 3;

  level.orbs[0] = construct_orb((Vector2){160, 80}, 90.0, false, true); 
  level.orbs[1] = construct_orb((Vector2){40, 200}, 60.0, true, false); 
  level.orbs_count = 2;

  return level;
}
