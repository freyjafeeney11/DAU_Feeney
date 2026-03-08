# CANNON
https://greensdictofslang.com/entry/d3ixpfi

A 2D side-scrolling pickpocket game built in C++ using the Ubisoft Next API.
You play as a thief on a moving train set in a Paris\London hybrid city, sneaking up on NPCs, stealing items
from their pockets using a dice-roll risk system, and hiding from the Robot train patroller. NPCs have gold, as well as unique items that reveal info about their stories.
All of the assets and animations are drawn by me using Procreate!

---

## Current State

The following has been implemented:

- **Player movement & animations** — walk, run (hold Shift), idle, hide, and steal (hide and steal need new sprites) animations using the keyboard arrows
- **NPC system** — three unique NPCs (Rosamund, Randy, Granny), each with their own loot table and steal difficulty rating
- **Pickpocket UI** — approach an NPC and press Space to open their inventory; navigate slots with arrow keys and press Enter to attempt a steal
- **Dice roll mechanic** — stealing rolls a d20 against the NPC's difficulty; failure alerts the NPC and closes the UI
- **Alert system** — alerted NPCs display a bobbing exclamation icon and can no longer be stolen from
- **Player inventory** — stolen items are added to your inventory, viewable by holding I
- **Level** — scrolling train background with parallax cityscape, rain animation, and a roaming background NPC
- **Patroller** — a simple enemy that chases the player on a failed roll

---

## Setup Instructions

1. Clone the repository: `git clone https://github.com/freyjafeeney11/DAU_Feeney.git`
2. Open `GameTest.sln` in **Visual Studio 2022**
3. Set the configuration to **Debug x64**
4. Build and run with **F5**

> The project uses OpenGL via GLUT. The required `.dll` files are copied to the 
> output directory automatically on build via the post-build step.

---

## Controls

| Input | Action |
|---|---|
| Keyboard Arrows | Move left / right |
| Hold Shift | Sprint |
| Space | Open pickpocket UI (near NPC) |
| Arrow Keys | Navigate loot slots |
| Enter | Attempt steal |
| Ctrl | Close UI |
| I | View your inventory |

Hide from the patroller by standing in a crowd clump.

## Attributions / Sources
Train sounds came from ZapSplat.com: Paris, France, ambience, train station, subway, underground, metro train arrive and depart, people talk, announcement
Music from https://freemusicarchive.org/search/?quicksearch=jazz&search-genre=
