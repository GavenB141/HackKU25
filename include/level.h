#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "objects.h"

Level sample_level();
Level transition_level();

void level_draw(Level *level, float dt);
void level_update(Level *level, float dt);

void fade_in(float fade, Rectangle screen);
Level getLevel(int level_index);


#endif
