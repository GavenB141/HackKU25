#ifndef LEVEL_H
#define LEVEL_H

#include "player.h"
#include "raylib.h"
#include "objects.h"

Level tutorial_0();
Level tutorial_1();
Level tutorial_2();
Level tutorial_3();
Level spikes4();
Level tutorial_4();
Level gaven_level();
Level static_magnets();
Level repulse_fly();
Level back_and_forth();
Level level_8();
Level level_10();
<<<<<<< Updated upstream
Level level_11();
=======
Level level_12();
>>>>>>> Stashed changes

void level_draw(Level *level, Player *player, float dt);
void level_update(Level *level, float dt);

void fade_in(float fade, Rectangle screen);
Level getLevel(int level_index);


#endif
