#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "objects.h"

Level tutorial_0();
Level tutorial_1();

void level_draw(Level *level, float dt);
void level_update(Level *level, float dt);

void fade_in(float fade, Rectangle screen);
Level getLevel(int level_index);


#endif
