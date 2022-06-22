# Citadel 2

I've decided to write a sequel inspired by the original Citadel created by Martin Walker. I spoke to Mr. Walker a while ago and asked him for his blessing to do a game inspired by the original and he said OK!
Way back in 1992 I had attempted this kind of project (See http://www.gamesthatwerent.com/gtw64/citadel-2/ ) but this time I want to really do it properly.
This time around I will be writing most of the code first and using placeholder graphics, music and SFX. This means the initial demos will be technical demos designed to show working code first.
Only when I'm sure the game code is mostly complete will I start adding the graphics/sound and doing game balancing and polish.

# Build and run

Use Maven to build the pom.xml test configuration.
It is setup to use JUnit automatically.

Also BuildIt*.bat will produce C64 ready prg files suitable for the normal BASIC load and run in an emulator or the real computer.

# Instructions

The CRT file will need to be mounted as a cartridge in a suitable emulator or written to a compatible GMod2 ROM.

## Title screen

Press down on the joystick to choose between Aracade or Strategy. By default Arcade mode is selected.
Press left/right/up to choose between  SFX/SFX+Music/Music.

### Arcade mode

The player moves around the map with eight-way control not aligned to the map blocks.
Pickups are automatically added instead of waiting to be installed by accessing a terminal.

### Strategy mode

The player moves around the map aligned to the map blocks and with four-way movement.
Pickups are stored in the inventory and can be installed when accessing a terminal.
A terminal sub-game needs to be completed before an inventory item is installed or door is activated.


## Computer terminals and switches

To access a computer terminal in a wall, stop the player, hold down fire while pushing the joystick in the direction of the terminal until the screen changes.
A switch in a wall looks a bit like a computer terminal, using it is the same as a computer terminal except that it will just toggle its associated door.

### In Arcade mode
	Terminals are like switches, there is no sub-game.

### In Strategy mode
	The first default option for the terminal sub-game is: Exit back to the game.
	The next option is: Open/close (toggle) the associated door.
	The next options are any collected powerups.
	Choose the option with left/right and use fire to select it. The sub-game will start.


## Teleporters

To use a teleporter stop the player and hold down fire.

## Capturing enemies

While stationary double click and hold the fire button to activate the capture probe. Move the probe over an enemy to capture it, if you have enough energy to do so. Once captured, the enemy status will be displayed on the status panel.

It is possible to capture and control up to three enemies at once. The currently active captured enemy will quickly pulse grey, other captured enemies will slowly pulse grey. Cycle through the captured enemies with by pressing the "space bar".

The currently captured enemy can be ordered to do different actions with the function keys:

	* F1 - Orders Stay or continue moving, it toggles the order
	* F3 - Follow the player, the point to be followed is two squares along the last direction the player fired.
	* F5 - Hunt mode, shoots at other enemies
	* F7 - Assimilate, depending on the enemy health and the capture cost an amount of energy is returned to the player and the target is destroyed


## Dropped pickups

	* Flashing weapon pod - Upgrades weapons. Bullet range, rear shot, side shot etc
	* CPU icon - Reduce sub-game difficulty
	* Battery - Immediate health increase. Also adds up to two batteries above and below energy bar.
				Constantly moving slowly recharges the batteries.
				When health is critically low and you're damaged a battery will immediately discharge and replenish your energy, saving you from destruction.



# Diary of a game

## Technical review

Most of the technical challenges for a four way scrolling top down shooter have been solved.

* Sprite multiplexor
* Multi-directional scroller
* Sound effects
* Music
* Tape and disk loaders
* Cartridge code

Reusing the Scroller game library ( https://github.com/martinpiper/C64Public/tree/master/Scroller ) which is used for all the SEUCK redux games will help me save some significant dev time.
I'm also going to use BDD6502 ( https://github.com/martinpiper/BDD6502 ) which is a java based 6502 code testing framework utilising Cucumber. This lets me write behavioural/functional test feature files and easily execute the tests as I write code.

The only challenge left is to see if I can write a suitable routine in 6502 for improved enemy AI.
This improved AI should be capable of finding alternate routes around temporarily blocked corridors or avoiding corridors where the player has been killing fellow units.

When units need to route around the maze it will be a simple matter for them to calculate a list of moves they need to accomplish to get to certain places.
If they get stuck or react to changing events (like being attacked or seeing the player) then they can request new routes.

Units would have goals, such as wanting to get to power recharge stations, going to waypoints at certain times, checking doors are locked, etc.
Depending on the aggressive nature (or passive) of the unit it might choose to investigate sounds of combat or to try to find safety by going to a position behind a higher ranked unit away from the player or combat sounds.
 
The idea for the enemy units is to allow for behaviour to be emergent, this is accomplished by having several variables for each unit that will affect behaviour and change depending on in game events.

The computer terminals can be used to unlock certain doors leading to different sections of the map.
This unlocking process leads to a circuit based sub-game where depending on the relative upgrade status of the player the path through the circuit is harder or easier.
Probably some kind of sliding block based puzzle with a certain number of moves/time possible before the attempt is rejected.
The player can find energy/weapon/computer upgrades by destroying and collecting parts from enemies.
These parts can have levels, so the more difficult enemies drop higher level weapons.
The computer terminal sub-game can be used to attempt installation of these dropped upgrades.
The computer upgrades make the sub-game easier by raising the level of the player.
A failed door open will cause the player to lose energy as the attempt is rejected.
Doors can have levels, like the upgrades.

Due to all this extra code with some non-trivial complexity, I estimate it will take about fifteen days to a point where it should be possible to play a mostly code complete first level of the game (minus a ton of polish, title screen, score entry etc).


## Day 1

So what I've done is to write some testable requirements in gherkin script (the *.feature files) to describe exactly the behaviour I want to see.

The next stage is to implement the requirements in some easily prototyped language. I'll probably choose java since I want to be able to utilise the BDD6502 tool I wrote earlier to test any final 6502 code.

The rest of today is setup and futzing around day, making sure the java IDE (IntelliJ) is setup correctly and I can remember how to test simple 6502 and java based code with the test framework.

The feature files are executed with JUnit and Cucumber included with the BDD6502 project. The algorithms have been implemented in Java to satisfy the feature file requirements. The classes are also unit tested with JUnit.

Thoughts:
    * It might be better to create a stack of points to process while expanding for the routes instead of doing a sweep of the whole level every time.
    It's more memory, but less iterations.


## Day 2

The code was updated to include a stack of points while expanding the routes.
It now takes many less iterations, of course.

Almost time to start writing the 6502 implementation.

Thinking about the Solve implementation a tad, I can use a simple linear array with a start and end that just wraps around.
A simple circular FIFO buffer will suffice.
FIFO.feature, FIFO.java (and added to Solve.java instead of IndexList) and FIFO.a now works.


## Day 3 - Demo

Started work on the maze solving asm code.
SolveRunner.a has been created as a standalone simple test of maze solving that displays information on the screen and flashes the border to show timing.
MazeSolve.a was created.
MazeSolve_init takes the input Maze data and calculates the Node array entries (cost, aversion etc) used by Solver.Solve
This code is validated against the Java code implementation and data it calculates. This means while running C64 tests the Java version of the code is executed as well. Most of this common code is located in MazeCommon.java

All in all, a productive day where lots of 6502 code worked first time. This model of testing really helps reduce undetected mistakes creeping into code.


## Day 4 - Demo

The main game loop code has now been started which means Citadel2Entry.a and BuildIt.bat have been created. This uses the Scroller common framework code.
The options have been set for split screen multi-directional operation with multiplexor options chosen to optimise for this game type.

Created EnemyProcessActivation. This regularly processes the enemy DB and decides when the activate and deactivate those enemies from the on-screen animation routines.
	* When enemies are off-screen they use coarse movement.
	* When they're on-screen then they use the full sprite animation code that also can fire bullets etc
This saves a lot of processor time and helps to spread the enemy update work across multiple frames.

Citadel2Entry - UpdateSprites : This updates on-screen sprites either from the enemy DB positions with respect to the max position; Or if it is a simple animated sprite it moves them according to the screen movement.
	Simple animated sprites are going to be used for bullets and explosions, where we don't need to have more expensive enemy DB update checks.


Adding animation code to animate some example enemy sprites.
EnemyMoveFineUpdate and EnemyMoveCoarseUpdate use some hand generated movements sequences so I can test the interaction between on-screen and off-screen movement code. These paths do not take into account the walls of the map, so enemies will appear to move through walls. This is expected.

Basically, enemies use fine movement every frame for when they are on-screen and coarse movement less often when they are off-screen.
The test here is that enemies will get to their exact specified end points even if they remain visible, or only partially visible.

Now the hand generated route test works it's time to add some code to process the graphics map data the MazeSolve code data structures.
Then start calling the MazeSolve route calculation to see if an enemy can use the calculated route to move around the map.

After a few minutes of plumbing all the bits together the first enemy sprite is able to move with a calculated route.
This is a big step forward, it tests that all the code is able to coexist in memory.


## Day 5 - Demo

Now the dynamic memory (DynamicMemoryData) code (DynamicMemory.a) has been written (Memory.feature) enemies request memory when their route calculation is finished.
This means multiple enemies can simultaneously remember their requested routes.
To demonstrate this I added 12 enemies moving around the maze and calculating their own routes.

Added a simple player control routine HandleUserInput. The player is actually a special case enemy that moves under joystick control.
Added quick collision detection for the player and the map.

Now I have to decide if I'm going to use characters for the player weapons or sprites, or maybe a combination of both.


## Day 6 - Demo

Added a quick test of player bullet firing. It uses characters and these are scrolled with the background and updated with the direction the player is moving.
When they reach the edges of the screen or encounter solid screen characters then they are removed.
You can now glide around the map shooting the enemies.
Enemies will notice they are blocked and route around the problem. Enemies will also stop if a map square is blocked by something else.


## Day 7 - 2 Demos!

Enemies can now fire sprite bullets that hit the player. Also the player can collide with enemies.
The player input code now also allows movement and firing in different directions by holding down the fire button to lock a movement direction. This is shown in the first demo (Day7.prg file).
The second demo, Day7Aligned.prg, has a different user input method enabled that aligns the player to map blocks like in the original Citadel.
It's a bit buggy at the moment so I made it the second demo just so we can see how it (kind of) works.
I'll probably leave the code disabled in future builds until I can figure out the bugs.
Also the free roaming player movement code, in the first demo, lets me test the scrolling code can handle difficult movements and changes of direction more easily.


## Day 8 - Demo

Starting on the sub-game code, which is like a sliding block puzzle.
Holding down fire will display the sub-game screen, first a circuit layout is calculated.
Then the current flows from the left to the right.
This populates the target current colours on the right hand side.
At this point the target computer "rejects the hacking attempt", so some animation will be needed at this point.
	What is planned is some kind of animation where the circuit is scrambled by the target computer.
The objective of the sub-game is to mend the circuit, so that it matches the target output state again.
The user can then move the cursor left and right, then slide the sections up and down.
The current will flow differently depending on the connections.

This actually turned out to be a lot of code. The game state must be saved correctly, the screen swapped while scrolling and all manner of little tweaks were needed.
Then the code that calculates the circuit and then gets the current flowing needed quite a chunk of work.
The player wins the sub-game if the output state matches the target end state for a certain number of frames, likely to be the time taken for a complete current flow calculation.


## Day 9 - Demo

Now during the sub-game the target computer rejects the hacking attempt and and shuffles the circuit.

Added the code to display a horizontal energy time, effectively a time limit for the sub-game.
When this timer reaches zero the sub-game is lost.

Also added a vertical energy bar that increases when the circuit indicates a connection with the correct output is made.
When this bar is full then the sub-game has been won.

To activate the sub-game the player must be stationary, near a computer terminal, hold down fire and then push and hold towards a computer terminal.

Doors can be opened and closed by accessing the terminals and winning the sub-game. Each computer terminal is paired with particular sets of doors.

The teleporter pads, the tiles with a circle pattern on them, can also be activated when stationary by holding down fire. The player must be centred inside the tile however.

The radar is also working now so you can see the enemies move around the map, even when they're not on screen.

At the moment an idle enemy will pick a waypoint and try to route to it, but if the waypoint is blocked by a locked door the route will fail and the enemy will not move.
So now before the level starts at memory location $9c00 a zone lookup map is calculated from the processed maze map.
This zone map flags all the map tiles that are within a certain area bounded by walls and closed doors.
This extra zone map will allow an enemy to pick waypoints that are within the current zone and this will improve the enemy mobility.


## Day 10 - Demo

The enemies now use the zone map lookup to find waypoints for the map tile zone they are currently on.
This means enemies now move around a lot more than yesterday since they can find waypoints they can route to a lot easier.
Placeholder sound effects (while the tune plays) have also been added. They're just there to technically test the music/SFX routine.

When the player is aggressive (firing or killing enemies) the enemies will gradually start trying to hunt down the player depending on how alert/courageous they are.
Some enemies however will always hunt down the player, no matter what aggressive actions the player has taken.
Enemies will also consider several different map tiles near the player, this has a nice effect of making the enemies shuffle around the player instead of just sitting there.
It adds a nice bit of variation that looks like more intelligent movement.
If the player stays out of trouble then the enemies will gradually get bored and go back to patrolling.

Lastly, debugging code is important during code development. In this case some debug memory heap checking code was added with DynamicMemory_HeapCorruptHook.
When allocating/freeing or scanning the heap for free blocks to merge if the heap status isn't kDynamicMemory_free/kDynamicMemory_allocated then DynamicMemory_HeapCorruptHook is called which rapidly changes the border colour.
This allows me to set a breakpoint and trace back (using VICE monitor CPU history command chis) to find the code that is abusing the memory heap.
It found an error where memory was sometimes being freed twice when a blocked enemy was being shot just has it was re-calculating a route.

## Day 11 - Demo

Now enemies sometimes drop upgrades that the player can pickup. These are different colours and are stored in the player's inventory.
When entering the sub-game with inventory items the player is given a choice of accessing the door control or activating an inventory object.
If there is nothing in the inventory then the default option is to access the door control.
If the player sub-game succeeds the door is opened/closed or the inventory object is used to upgrade the player's robot.
At the moment the only upgrade is increased fire rate. Later on different upgrades will be available.

To prepare for title screen and extra level data the memory map has also been shuffled around. There is currently 6K free in the first three banks, which should be ample.
A placeholder title screen has been added to test the game flow code.
Lastly, the player's health meter is now updated. When it reaches zero be very careful not to get hit by anything, otherwise kaboom!

## Day 12 - Demo

The game has now moved to a cartridge image, it's usable in VICE. Also the map tile aligned player movement code has been enabled again.
The scroller code memory has also been reduced, by about 1K, by restricting the scroll to four way instead of eight way.
Enemies now have some extra health and some are therefore harder to kill than others. This is not restricted to graphical type, so this means the same looking enemies can have different starting health.
Enemies also have four different movement speeds now, instead of the two they had before.

Enemy generators now detect when the player is near and animate open.
When the player is somewhat near an open generator then there is a chance of generating a random enemy.

# Code complete milestone

Most of the code for a playable first level demo is now complete and there is rough placeholder code for a title screen to main game and back again flow in there.
So now it's effectively reached the code complete milestone mentioned in the "Technical review" above. So now this needs a ton of polish.

As promised the code is at: https://github.com/martinpiper/C64Public

If you want to try adding your own graphics then use CharPad 2.0 to open the file C64\Citadel2\Level1.ctm
After saving the file (with the original map size) then run C64\Citadel2\BuildIt.bat to build the C64\Citadel2\Citadel2Cart.crt file

Most of the level data configuration is in the C64\Citadel2\asm\DataLevel1.a file.
The interesting map point coordinates are in the InterestingPlacesX and InterestingPlacesY arrays.
So if you change the map layout significantly then you will need to add extra points.

If the computer terminal locations are changed then alter the coordinates found in MapTerminalToDoor

The pairs of teleporter coordinates are in TeleportPairs

To change the starting enemy types and locations use EnemyType, EnemyPositionsX and EnemyPositionsY

If you want to add map tiles, then alter the number for numMapUsedTiles = 32 to match the number of tiles used in the Level1.ctm file.

If you want to enable a full colour scroll then in C64\Citadel2\asm\DataDefs.a enable the line ";CharColoursForEveryCharacter = 1" by removing the ";"
This will allow you to mix hi-res and multi-colour graphics for each character in CharPad.
This is a bit buggy at the moment, the sub-game switch needs to be handled correctly. Which will be done if there is a good graphical design that needs it.



## Day 13 - Demo

Enemies now fire at the player. The player bullet range and firing speed has been tweaked so at the start of the game.
Charger points have been added to various locations. Stay static over them for a short period and charge will be transferred to you.

When the player is destroyed the enemies now stop firing and start to patrol as usual.

When installing upgrades in the sub-game the player will progressively receive rear firing followed by side fire.
During the sub-game the first option is to exit without opening/closing a door or using any inventory item.
The second option is to open/close a door.
The remaining options will be to use a collected inventory item.

Enemy types are now defined by templates so that consistent enemies are spawned in the generators.
Some enemy types try to ram you.



## Day 14 - 

Added ScrollerDemo_OverrunCounter to allow the code to detect when the frame is overrunning and stop executing certain non-critical code to recover the time in the next frame.
Enemy generators limit the number of enemies they create on screen at the same time.
Different doors types have been added that will open for the player, or enemies. The timing of the doors as well as their animations can also be set.
A quick teleport fade effect has been added, to test the position changing code and check the rest of the game logic can handle the update.



## Day 15 -

Activated generators can now be destroyed by repeatedly destroying enemies whilst over the open generator. The explosion damage eventually destroys the generator.
Switches have been added that will open/close certain linked doors. They are activated like the computer terminals.



## Day 16 - Demo

The sub-game now starts in easy mode and gets progressively harder with each successful attempt.
The switch between title screen and game can use different music. Graphics still need to be switched.
New dropped pickup tytpes:
	Flashing weapon pod
	CPU icon - Reduce sub-game difficulty
	Battery - Health increase

Doors now cause damage if they close on an unsuspecting enemy/player.



## Day 17 - Demo

A day of optimisations, mostly by moving lots of arrays, especially for the multiplexor, into zero page.
Speed code, massive unrolled block of code, are now used for the char and colour RAM copy/scroll. These are stored in cartridge banks and only enabled when a scroll is needed. This means a lot of memory during normal runtime can be freed. I think this is the first cartridge game using a multiplexor to swapping in and out large chunks of cartridge executed speed code while scrolling. Not only does this save a lot of RAM, it also saves a lot of CPU time because we no longer need to used absolute indexed addressing.

Also the code for the title screen and the game code has been split into separate chunks. This means the common code, for the multiplexor/input/sound effects, is kept in memory while the game can utilise the space left by the title screen.

Glowing enemies now always drop a pickup. A nice hint for the player.
The sequence of enemies can be set for each level and also the precise pattern of pickups can be defined. This allows level design to be more strategic instead of just random.



## Day 18 - Demo

To really test the title screen to game to title screen switching code I resurrected the graphics and design from the old game attempt from 1992 (on the GTW site https://www.gamesthatwerent.com/gtw64/citadel-2/ ). The code I wrote from scratch however, there's quite a lot of timing work in there for the various splits with sprites moving around over them.

The player's radar has also been improved. It now shows a more "zoomed in" view than before and moves around with the player.

At this point the slow part of the title screen to game transition was where the C64 had to calculate the level zone map. This zone map needed a quick tool to be generated that would use the PC building the game to calculate it in much quicker time than the C64 even could.
the MakeCart tool was updated to allow simple addition of different file data in the cartridge and pack it all into contiguous banks. This means it is much easier to build complex data structures in the cart ready for the game code to swap them in quickly.

Decompression has been optimised for speed and memory, so the game now starts quicker than before. It has also freed memory at $200-$3ff so I have an extra .5K of code to play with.



# Day 19 - Demo

Up to three enemies can now be captured with the player's capture probe. Pressing the function keys will order them around to stay or follow the player.
Captured enemies are immune to the player's bullets, but can still be destroyed if the player rams them, or they ram the player.



# Day 20 - Demo

Captured enemies can now be told to use Destroy mode, where they will shoot at other enemies.
Installable batteries, slowly recharge as the player moves.

The title screen can be used to change the game mode and sound configuration.
	Press down on the joystick to choose between Aracade or Strategy. By default Arcade mode is selected.
	Press left/right/up to choose between  SFX/SFX+Music/Music.

### Arcade mode

The player moves around the map with eight-way control not aligned to the map blocks.
Pickups are automatically added instead of waiting to be installed by accessing a terminal.

### Strategy mode

The player moves around the map aligned to the map blocks and with four-way movement.
Pickups are stored in the inventory and can be installed when accessing a terminal.
A terminal sub-game needs to be completed before an inventory item is installed or door is activated.



# Day 21

Added a quick tutorial/hint display in ther score panel. At certain places in the map and with certain game events, a hint will be displayed.
This is mostly to help the player learn the controls for the capture probe, how to open doors, or use power recharge stations.
