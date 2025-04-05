#ifndef LEVEL_H
#define LEVEL_H

#include "player.h"
#include "raylib.h"
#include "objects.h"

Level sample_level();
Level transition_level();
Level gaven_level();

void level_draw(Level *level, Player *player, float dt);
void level_update(Level *level, float dt);

void fade_in(float fade, Rectangle screen);
Level getLevel(int level_index);


#endif
