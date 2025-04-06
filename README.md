## Play now
[https://polar-lizard.vercel.app]

## Inspiration
The C programming language distinctly lacks many things that many programmers take for granted today, meaning it can be an intimidating choice for performing even simple tasks. Yet, you can do anything with C, and there's something liberating about its simplicity. So, we set out with the simple goal of making a game in C, and this is where it brought us.
## How we built it
We used Raylib, a simple library wrapping common functionalities needed in games. Much of the game state is contained in a "Level" struct, which contains most of the items on screen at any given point. Whenever we added a new type of object, the Level struct was given a new array field for that object. Each of the game's stages is created with a function that populates the struct with the needed objects. The only dynamic allocations in the game are for a handful of assets, which are held for the whole runtime.
## Challenges we ran into
Since Raylib was the only external library we used, we had to roll our own physics and collision resolution, which are very difficult to get right. Even now there are jarring, but thankfully scarce physics bugs.
## Accomplishments that we're proud of
- Mostly functional physics
- 15 Stages
- Magnetism mechanics

## What we learned
You can do a lot in plain C.
## What's next for Polar Lizard
Bipolar Lizard, perhaps
