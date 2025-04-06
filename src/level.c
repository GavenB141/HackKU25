#include "level.h"
#include "animation.h"
#include "objects.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"

void level_draw(Level *level, Player *player, float dt) {
  static Font foont = {0};
  static Texture ground_texture = {0};
  static Texture gate_texture = {0};
  static Texture positive_orb_texture = {0};
  static Texture negative_orb_texture = {0};
  static Shader highlight_shader = {0};
  static Shader repeat_shader = {0};

  if (ground_texture.id == 0) {
    // Load drawing assets for duration of process
    gate_texture = LoadTexture("assets/gate_texture_sprite.png");
    ground_texture = LoadTexture("assets/metal_crate_sprite.png");
    positive_orb_texture = LoadTexture("assets/positive_orb_sprite.png");
    negative_orb_texture = LoadTexture("assets/negative_orb_sprite.png");
    #ifdef PLATFORM_WEB
    highlight_shader = LoadShader(0, "assets/shaders/highlight_outline100.fs.glsl");
    repeat_shader = LoadShader(0, "assets/shaders/texture_repeat.fs.glsl");
    #else
    highlight_shader = LoadShader(0, "assets/shaders/highlight_outline.fs.glsl");
    #endif
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

  #ifdef PLATFORM_WEB
  BeginShaderMode(repeat_shader);
  #endif

  for (int i = 0; i < level->platform_count; i++) {
    Rectangle bounds = level->platforms[i].bounds;
    Vector2 position = (Vector2){bounds.x, bounds.y};
    bounds.x = 0, bounds.y = 0;
    float alpha = 1;
    if(level->platforms[i].gate){
      if(level->platforms[i].inverted)
      {
        if(!level->sensors[level->platforms[i].sensor_idx].sensed) { alpha = 0.25; }
        DrawTextureRec(gate_texture, bounds, position, ColorAlpha(WHITE, alpha));
      }
      else
      {
        if(level->sensors[level->platforms[i].sensor_idx].sensed) { alpha = 0.25; }
        DrawTextureRec(gate_texture, bounds, position, ColorAlpha(WHITE, alpha));
      }
    }
    else{
      DrawTextureRec(ground_texture, bounds, position, WHITE);
    }
  }
  #ifdef PLATFORM_WEB
  EndShaderMode();
  #endif

  for (int i = 0; i < level->orbs_count; i++) {
    MagneticOrb *orb = &level->orbs[i];
    Color aura = orb->positive ? BLUE : RED;

    DrawCircleV(orb->position, orb->range, ColorAlpha(aura, 0.25));
    DrawCircleLinesV(orb->position, orb->range, ColorAlpha(aura, 0.5));
  }

  for (int i = 0; i < level->orbs_count; i++) {
    MagneticOrb *orb = &level->orbs[i];

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
    case 4: 
    DrawTextEx(foont, "Move the ball to open the gate", (Vector2){20,20},16, 1, WHITE);
    DrawTextEx(foont, "Pickup or drop the", (Vector2){75,36+6},16, 1, WHITE);
    DrawTextEx(foont, "magnets with SHIFT", (Vector2){75,52+6},16, 1, WHITE);
    break;
    case 10: DrawTextEx(foont, "Press R to RESET", (Vector2){85,40},16, 1, WHITE);
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
    case 7:
      return back_and_forth();
      break;
    case 8:
      return level_8();
      break;
    case 9:
      return repulse_fly();
      break;
    case 10:
      return level_10();
      break;
    case 11:
      return level_11();
    case 12:
      return level_12();
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
  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 6};

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
  level.startingPosition = (Vector2){30, 200};
  
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 320}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 176}};
  level.platforms[3] = (Platform){(Rectangle){0, 144, 320, 32}};
  level.platforms[4] = (Platform){(Rectangle){300-12, 176, 32, 32},0,1}; // gate reading sensor 0
  level.platform_count = 5;

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 6};
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

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 1};
  level.transition_count = 1;

  level.orbs[0] = construct_orb((Vector2){160, 80}, 90.0, false, true); 
  level.orbs[1] = construct_orb((Vector2){40, 200}, 60.0, true, false); 
  level.orbs_count = 2;

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 7};
  level.transition_count = 1;

  return level;
}

Level back_and_forth() {
  Level level = {0};
  level.id = 7;
  level.startingPosition = (Vector2){20, 185};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 174}};
  level.platforms[4] = (Platform){(Rectangle){300-12, 176, 32, 32},0,1}; 
  level.platforms[3] = (Platform){(Rectangle){300-12, 176-32*6, 32, 32+32*5}};
  level.platforms[5] = (Platform){(Rectangle){0, -16, 32*9, 64}};


  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 8};
  level.spikes[0] = (Spike){(Rectangle){160-32*3, 176, 32, 32}, get_spike_animation()};
  level.spikes[1] = (Spike){(Rectangle){192-32, 176, 32, 32}, get_spike_animation()};
  level.spikes[2] = (Spike){(Rectangle){224-32, 176, 32, 32}, get_spike_animation()};

  level.sensors[0] = (Sensor){(Rectangle){0, 180, 32, 32}, 0};
  level.sensor_count = 1;

  level.orbs[0] = construct_orb((Vector2){280, 200}, 60.0, true, false);

  level.orbs_count = 1;
  level.spikes_count = 3;
  level.transition_count = 1;
  level.platform_count = 6;

  return level;
}

Level level_8() {
  Level level = {0};
  level.id = 8;
  level.startingPosition = (Vector2){30, 200};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};

  level.platforms[4] = (Platform){(Rectangle){128+64, 80+32, 64, 128-32}};
  level.platforms[5] = (Platform){(Rectangle){128, 80+32*3, 64, 32}};
  level.platforms[6] = (Platform){(Rectangle){128, 80, 64*2, 32}};
  level.platforms[7] = (Platform){(Rectangle){0, 80+32, 64, 64}};

  level.transition[0] = (Transition){(Rectangle){338, 100, 1000, 100}, 9};

  level.transition_count = 1;
  level.platform_count = 8;
  
  return level;
}

Level repulse_fly() {
  Level level = {0};
  level.is_big_level = true;

  level.id = 9;
  level.startingPosition = (Vector2){20, 200};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 64, 32}};
  level.platforms[1] = (Platform){(Rectangle){256, 208, 64, 32}};
  level.platforms[2] = (Platform){(Rectangle){0, 0, 320, 32}};
  level.platform_count = 3;

  level.orbs[0] = construct_orb((Vector2){20, 210}, 100.0, true, true); 
  level.orbs[1] = construct_orb((Vector2){20, 210}, 100.0, true, false); 
  level.orbs_count = 2;

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 10};
  level.transition_count = 1;

  return level;
}

Level level_10() {
  Level level = {0};
  level.id = 10;
  level.startingPosition = (Vector2){30, 200};
  
  level.platforms[0] = (Platform){(Rectangle){0, 208, 64, 32}};
  level.platforms[1] = (Platform){(Rectangle){256, 208, 64, 32}};
  level.platforms[2] = (Platform){(Rectangle){0, 144-36+16, 320, 32}};
  level.platforms[3] = (Platform){(Rectangle){32*2, 208, 32*6, 32},0,1};

  level.transition[0] = (Transition){(Rectangle){338, 100, 1000, 100}, 11};

  level.sensors[0] = (Sensor){(Rectangle){0, 144-36-10+16+5, 320, 5}};

  level.orbs[0] = construct_orb((Vector2){30, 200}, 50.0, true, false);
  level.orbs[1] = construct_orb((Vector2){170-36*2, 144-36-5+16}, 50.0, true, false);

  level.orbs_count = 2;
  level.transition_count = 1;
  level.sensor_count = 1;
  level.platform_count = 5;
  
  return level;
}

Level level_11(){
  Level level = {0};
  level.id = 11;
  level.startingPosition = (Vector2){50, 185};

  level.sensors[0] = (Sensor){(Rectangle){0, 0, 400, 220}, 0};
  level.platforms[0] = (Platform){(Rectangle){0, 208, 6*16, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, 0, 2, 208}};
  level.platforms[2] = (Platform){(Rectangle){318, 0, 2, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, 0, 320, 32}};

  level.transition[0] = (Transition){(Rectangle){338, 160, 1000, 48}, 12};
  level.platforms[4] = (Platform){(Rectangle){14*16, 208, 32*4, 32}};

  level.platforms[5] = (Platform){(Rectangle){300-12, 176, 32, 32},0,1, true}; 
  level.platforms[6] = (Platform){(Rectangle){300-12, 176-32*6, 32, 32+32*5}};
  level.platforms[7] = (Platform){(Rectangle){0, -16, 32*9, 64}};

  level.platforms[8] = (Platform){(Rectangle){32*4 -16, 32*5-16 , 32*2, 32}};

  level.platforms[9] = (Platform){(Rectangle){0, 32*4-16 , 32, 32}};
  level.platforms[10] = (Platform){(Rectangle){32*8, 32*4-16 , 32, 32}};

  level.orbs[0] = construct_orb((Vector2){280, 200}, 10.0, false, false);
  level.orbs[1] = construct_orb((Vector2){280, 32*3}, 10.0, false, false);
  level.orbs[2] = construct_orb((Vector2){10, 32*3}, 10.0, false, false);

  level.orbs_count = 3;
  level.sensor_count = 1;
  level.transition_count = 1;
  level.platform_count = 11;
  
  return level;
}

Level level_12(){
  Level level = {0};
  level.id = 12;
  level.startingPosition = (Vector2){30, 200};
  
  level.platforms[0] = (Platform){(Rectangle){0, 208, 320, 32}};
  level.platforms[1] = (Platform){(Rectangle){0, -16, 2, 370}};
  level.platforms[2] = (Platform){(Rectangle){318-62, 16, 64, 160}};
  level.platforms[3] = (Platform){(Rectangle){0, -16, 320, 32}};

  level.platforms[4] = (Platform){(Rectangle){0, 64+16, 64+32, 64*2}};

  level.platforms[5] = (Platform){(Rectangle){318-62, 176, 32, 32},1,1,0};
  level.platforms[6] = (Platform){(Rectangle){318-62+32, 176, 32, 32},0,1,0};

  level.spikes[0] = (Spike){(Rectangle){32*3, 208-32, 32, 32}, get_spike_animation()};
  level.spikes[1] = (Spike){(Rectangle){32*4, 208-32, 32, 32}, get_spike_animation()};

  level.orbs[0] = construct_orb((Vector2){15, 215}, 25.0, true, false);
  level.orbs[1] = construct_orb((Vector2){75, 215}, 25.0, true, false);

  level.sensors[1] = (Sensor){(Rectangle){318-62-32, 176, 32, 32}, 0};
  level.sensors[0] = (Sensor){(Rectangle){318-62-32*3, 176, 32, 32}, 0};

  level.transition[0] = (Transition){(Rectangle){338, 100, 1000, 100}, 13};

  level.orbs_count = 2;
  level.sensor_count = 2;
  level.transition_count = 1;
  level.spikes_count = 2;
  level.platform_count = 7;
  return level;
}
