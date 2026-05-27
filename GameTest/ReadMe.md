# CANNON
Freyja Feeney

This is a 2D side-scrolling pickpocket game built in C++ using the Ubisoft Next API.
You play as a young pickpocket on a moving train set in a Paris\London hybrid city, sneaking up on NPCs, stealing personal items from their pockets using a dice-roll risk system, and hiding from the Robot train patroller. You must trade these items to a Goblin who has taken the train hostage in order to save everyone.

All of the assets and animations are drawn by me using Procreate!

---

## Features

- **Player movement & animations** walk, run (hold Shift), idle, hide, and steal (hide and steal need new sprites) animations using the keyboard arrows
- **NPC system** seven unique NPCs in 3 different cars, each with their own loot table and steal difficulty rating
- **Pickpocket UI** approach an NPC and press enter to open their inventory; navigate slots with arrow keys and press Enter to attempt a steal
- **Dice roll mechanic** stealing rolls a d20 against the NPC's difficulty; failure alerts the NPC and closes the UI
- **Alert system** alerted NPCs display a bobbing exclamation icon and can no longer be stolen from
- **Player inventory** stolen items are added to your inventory, viewable by holding I
- **Level** scrolling train background with parallax cityscape, rain animation, and a roaming background NPC
- **Patroller** a simple enemy that chases the player on a failed roll
- **Campsite** Talk to the goblin to trade goods
- **Archetype System** - Choose between Charismatic, Dexterous, or Lucky profiles, each offering unique gameplay modifiers and special mechanics.
- **Dynamic Quest System** - Interact with the campsite Goblin to receive daily requests; steal specific items to trade for gears to repair the clock and reach the good ending. On a failed attempt, you lose one day and begin the next with the same request.
- **Pickpocket Lore** - Features a randomized "loading screen" system that displays authentic 19th-century criminal terminology and definitions during transitions.
- **Time of Day & Stealth** - A full day/night cycle where sleeping at the campsite resets NPC alertness and advances the world state.
- **Interactive UI** - A grid-based inventory system with detailed item descriptions and profile-specific descriptions.

## Technical Overview

### Architecture
The game is built using a **State-Driven Architecture**. A central `SceneState` controller manages transitions between game modes (Menu, Exploration, Dialogue, and Outro).

### Key Systems
- **Dynamic NPC Manager**: NPCs are dynamically loaded based on the current train car. Each NPC inherits from a base class and contains a unique `LootTable` that dictates item rarity and steal difficulty.
- **Probabilistic Theft Mechanic**: Theft attempts use a d20-based calculation. The result is modified by the player's chosen **Archetype** (e.g., the Lucky archetype applies a +2 flat bonus to the raw roll).
- **Parallax & Environment**: The "moving train" effect is achieved through a three-layer parallax system. Background assets are scaled and moved at varying speeds based on the camera's X-coordinate.
- **Efficient Ownership Transfer**: The Item system utilizes Move Semantics (Move Constructors and Move Assignment Operators) to transfer ownership of item data and strings using std::move. This avoids expensive deep-copying of strings when items are generated or moved into the player's inventory.


## Setup Instructions

1. Open `GameTest.sln` in **Visual Studio 2022**
2. Set the configuration to **Debug x64**
3. Build and run

Also available for download on itch.io: https://f3if.itch.io/cannon

> The project uses OpenGL via GLUT. The required `.dll` files are copied to the 
> output directory automatically on build via the post-build step.

---

## Controls

| Input | Action |
|---|---|
| Keyboard Arrows | Move left / right |
| Hold Shift | Sprint |
| Enter | Open pickpocket UI (near NPC) |
| Arrow Keys | Navigate loot slots |
| Enter | Attempt steal |
| ESC | Close UI |
| I | View your inventory |
| Up | Climp up ladder |

Hide from the patroller by crouching in a crowd clump.

## Attributions / Sources
Train sounds came from ZapSplat.com: Paris, France, ambience, train station, subway, underground, metro train arrive and depart, people talk, announcement
Music from https://freemusicarchive.org/search/?quicksearch=jazz&search-genre=

Pickpocket definitions from: 
1811 DICTIONARY OF THE VULGAR TONGUE.
A DICTIONARY OF BUCKISH SLANG, UNIVERSITY WIT, AND PICKPOCKET ELOQUENCE.
https://www.gutenberg.org/ebooks/5402
https://www.gutenberg.org/cache/epub/76632/pg76632-images.html