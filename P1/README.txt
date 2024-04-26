******EXTRA POINTS******
My project has FOUR classes, up to 8 objects which could interact with each
other and the generation of one object could be controlled by user input, which
is a more complex game logic using two separate barriers.
My project also has objects rendered in different shapes and colors to
distinguish them and make this game content understandable visually.

GAME CONTROLS
Fire Cannon: Press the space key to shoot a cannonball.
Terminate Game: Press the q key to quit the game.

INTERACTING OBJECTS
MovingObj & Monster: Move across the screen within a range, they can interact
with each other to kill MovingObj by Monster
Cannon: Stays at the bottom of the screen. It shoots cannonballs when the space
key is pressed.
Cannonball: Travels upward from the cannon. If it hits reaches a specific
vertical limit, it stops. (Further logic could be added, like when cannonball
hit other objects, it will stopped with that object.)

HOW TO PLAY
Start the game in a terminal environment.
Press the space key to fire the cannonball. Ensure the previous cannonball stops
before firing another.
Press q to quit the game if there is no active cannonball.