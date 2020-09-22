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

********************Project Requirments********************
1. Path planning done
2. FSM AI done
3. Game FSM done
4. Particle systems done
5. transformations done. 
6. atleast one instance of hierachiral transformations done
7. gameworld done
8. enemy types: 9
9. tower types: 8
10. Auto defender done
11. game economy: credits/income
12. level design: different maps
13. HUD design: complex and easy to use
14. Power ups: 5

********************Features/Gameplay********************
1. Once 20 enemies have reached the endpoint, game over. 
2. Multiplayer component: Two teams, last one standing wins.

********************CODE LOCATIONS********************
Particle system: Uses the shader.cpp/.h file, then is used in the towerobject and enemyobject classes alongside
		 the particle class. 

********************NOTES/USEFUL INFO********************
-There are two videos in the texturedemo videos folder which showcases the assignment requirements. 
-Any buttons with an exit symbol (orange x) is non functional and should be assumed to be.
-Cannot yet buy creeps from left shop.
-Game does not end when hp<=0, again for testing purposes.
-For now, both towers and creep are free, this will be changed soon.


********************BUGS/GLITCHES********************
-tower dps based on fps, more zoom = more dmg. must fix.
-scroll speed varies a little too much for my liking with zoom.
-radius is not lined up with the tower shooting radius (laser tower)

********************Controls********************s
MAIN MENU:
Start: P

MAP MENU:
MAP SELECTION: 1,2,3,4,5

IN GAME:
Scroll up: UP ARROW
Scroll down: DOWN ARROW
Scroll right: RIGHT ARROW
Scroll left: LEFT ARROW 

Zoom in: PLUS +
Zoom out: MINUS - 

Switch Team: T (all enemies must be dead, can switch without starting wave for ease of testing)
Start Wave: G

CHOOSE TOWERS: U,I,P,J,K,L,N,M
CHOOSE ENEMIES: Q,W,E,A,S,D,Z,X,C
CHOOSE POWERUPS: 1,2,3,4,5
CHOOSE ENEMY UPGRADES: RFV
CHOOSE TOWER UPGRADES: Y,H,B

toggle item discription: CTRL
remove radius/deselect: SHIFT
Place a tower: LEFT-MOUSE click
Spawn an enemy: SPACE
Deselect tower: SHIFT

********************HOW TO PLAY********************
1. Once it has loaded, click the play button or press p. 
2. Choose a map by pressing any key 1,2,3,4,5 (will show a different map depending on which)
   or click on a map to start. 
3. Once loaded, you will see two teams. One at the top and one at the bottom. 
4. To start, keep track of the credits or money you currently have, defined on the middle
   right side (the number at the bottom). This signifies your current credits you have
   The top one siginfies your income per turn. 
5. To begin playing, click on any tower you wish to select on the bottom right hand of the hud
   or by pressing the corrasponding keys. Once this is done, you will see a tower on the mouse
   selection cursor. If you have the credits (read the info panel) you will be able to place it. 
   Do this for all towers you wish to place. 
6. To choose enemies, you can select them from the bottom left panel or by using the keys. Once 
   selected, you will see a red box around them. Now press space to spawn them in. The counter
   on the top should update to show the new enemies (depending on team). 
7. To place powerups, click on them in the top middle or use the keys, the place them if they have 
   a radius in the area you wish for them to be. 
8. To use upgrades, click on them in the bottom right (left to the tower) or bottom left (right to the enemies)
   then press space to place them. 
9. To switch teams, press the green button in the middle. 
10. You can only start a round if you have enemies coming towards you. So a good way is to spawn one 
   enemy on both teams (spawn on player 1, then switch to player 2 and spawn). 
11. Once enemies are ready, press the big red button to begin the round. 
12. You can place towers during rounds. 
13. If 20 enemies reach the base, it's game over. 
14. press escape to pause the game, then click to go back




