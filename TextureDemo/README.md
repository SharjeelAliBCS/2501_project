# Comp2501 Term Project (RTS/Tower Defense)
Github: https://github.com/SharjeelAliBCS/2501_project

********************Developers********************
David Neudorf #101029913
	Roles:
	Path planning, map rendering, enemies

Sharjeel Ali #101070889
	Roles:
	Art Director, AI, towers

Raul Rodriguez Azurdia #101058745
	Roles:
	map loading, HUD, menu

********************Game********************
A 2D Tower defense RTS type game. Set in a fictional Sci fi world,
where aliens are attacking Earth. Here, the player must place towers
to destroy the aliens who travel along a safe road they have built to
a very important human base. If the aliens reach the base, game over. 

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
10. Once 5 enemies have reached the endpoint, game over. 

********************Controls********************
Scroll up: W
Scroll down: S
Scroll right: D
Scroll left: A

Zoom in: Z
Zoom out: X

Place a tower: LEFT-MOUSE click
Spawn an enemy: SPACE


