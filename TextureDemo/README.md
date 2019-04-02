# Comp2501 Term Project (RTS/Tower Defense)
Github: https://github.com/SharjeelAliBCS/2501_project

********************Developers********************
David Neudorf #101029913
	Roles:
	Path planning, graph setup, map rendering, enemies, gameplay

Sharjeel Ali #101070889
	Roles:
	Art Director, AI, towers,particles

Raul Rodriguez Azurdia #101058745
	Roles:
	map loading, HUD, main menu,tower selection

********************Game********************
A 2D Tower defense RTS type game. Set in a fictional Sci fi world,
two factions are at war sending legions of beasts to break the opponents
stronghold, each side builds towers to destroy the invaders before they 
reach a very important intel facility. If the aliens reach the base, game over. 

********************Assignment 6 Requirments********************
1. Particles system is incorporated into the game
2. Flame like texture chosen. 
3. Shader modified to produce flame effect
4. Flame particles spawn at tower and move with it
5. color becomes darker, and single particle becomes smaller over time. 
6. Bonus is done, spawns in once an enemy has been destroyed. 

********************Features/Gameplay********************
1. By default, two enemies move across the map to the goal using Dijkstra's algorithim. 
2. Towers can be placed an infinite amount of times in empty grass areas. 
3. Towers will locate the closest enemy and fire at it, and only it. 
4. If there are no enemies, the towers will remain idle. 
5. Once a bullet is fired, it will "home" in on the enemy and explode on hit.
6. If a bullet did not hit anything, it will explode after a certain distance. 
7. Enemies have 60 health, and each bullet does 10 damage. So it takes 6 to take one down. 
8. Enemies can be spawned in an infinite amount of times.
9. The map can be zoomed in or out of. 
10. Once 20 enemies have reached the endpoint, game over. 
11. There is also a flamethrower tower. 
12. Multiplayer component: Two teams, last one standing wins.

********************CODE LOCATIONS********************
Particle system: Uses the shader.cpp/.h file, then is used in the towerobject and enemyobject classes alongside
		 the particle class. 

********************NOTES/USEFUL INFO********************
-There are two videos in the texturedemo videos folder which showcases the assignment requirements. 
-Any buttons with an exit symbol (orange x) is non functional and should be assumed to be.
-Cannot yet buy creeps from left shop.
-Line 294 on main can be changed to any int from 1-3 for a different map.
-Game does not end when hp<=0, again for testing purposes.
-For now, both towers and creep are free, this will be changed soon.


********************BUGS/GLITCHES********************
-hud turns red when at a certain horizantal coordinate with the camera
-flames use the explosion vertex shader for the enemies when in close range to an exploding enemy
-tower dps based on fps, more zoom = more dmg. must fix.
-scroll speed varies a little too much for my liking with zoom.

********************Controls********************s
MAIN MENU:
Start: P

IN GAME:
Scroll up: W
Scroll down: S
Scroll right: D
Scroll left: A

Zoom in: Z
Zoom out: X

Switch Team: T (all enemies must be dead, can switch without starting wave for ease of testing)
Start Wave: B

Place a tower: LEFT-MOUSE click
Spawn an enemy: SPACE
Deselect tower: SHIFT

********************HOW TO PLAY********************
1. Once the game has started, press the P key to begin the game. 
2. You will see two sets of the same map (one above, the other below). This is meant for the team
   based gameplay. To begin, place some towers.
3. To place a tower, click on the tower icon on the hud (bottom right section) then, place it on the map. 
   The gray one is the base default, and the red one is the flamethrower. 
4. Once placed, press the space bar to place one enemy per spawn (for map3, 3 enemies). They will be added to the queue to be spawned. 
5. Press T to change teams
6. Place towers for player 2. 
7. Press B to start the enemy wave. 
8. rinse repeat.



