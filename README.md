**TIE-02408 Programming 3-Techniques Project Assignment
Author:**

Name: Jetro Sihvonen, Sijan Pandey

Student number: H291986, H293831

**Game rules and controls:
Game story:**

In this game the player is a superhero who has been called to Tampere to stop a rampaging murderer. The murderer has escaped from a secret government facility and was part of a weapons program. He is now moving around the city centre in the buses and killing passengers on them. The player's task is to chase and capture the murderer using the buses (or running) before it’s too late. The government cannot give support to the hero, because this catastrophe must remain to secret. Capture the murderer before the public notices and there is a widespread panic. 

**Game points:**
Points keep track of the game situation. Every passenger murdered decreases the points (by 100), but so does every game minute passed (by 10) and the murderer wins if the points go to zero. The points start at 5000. To win the player must capture the murderer by getting their character on top of the murderer character before the points go to zero.

**Player control:**
The game has two modes of play: single player and multiplayer.

The hero (player 1) is controlled with WASD-keys and can walk around the map freely. To get on a bus the player must be on a stop and when the bus comes, using the key E, the player enters the bus. Exiting buses is possible only on stops and again using the key E. 

There is a multiplayer option, where the second player gets to play as the murder. The second player is controlled with IJKL and can enter and exit buses similarly to the first player with the key O. Again, the murderer wins if the points go to zero and the player wins if they capture the murderer.

**Keys:**

Player 1: 

W – Up	A - Left 	S-Down	D-Right 	E-Enter/Exit Bus

Player 2:

I – Up	J - Left 	K-Down	L-Right 	O-Enter/Exit Bus

In the start window the game mode (multiplayer or single player) can be chosen. When in single player, a username can be provided for the leader board and the top 10 list is only maintained for single player mode. 

**Additional Features implemented:**
1.	Even screen updates. A timer in the mainwindow updates the screen at even intervals and the mainwindow calls for information updates before drawing.
2.	Scrollable map. The game area is the larger city area and scrollable. The window follows player 1 in singleplayer as they move around keeping the player centred on the screen. In multiplayer the map does not follow the players but rather can be moved with the scrolls and the arrow keys.
3.	Even movement of the playable figure. The players move evenly with the key controls through the map. Also, the automated murder moves smoothly between stops when walking.
4.	Passenger amounts. The amounts of passengers on the buses and the bus stops is shown on the map as numbers on their graphics items.
5.	Following the game state. The points and the current time are shown in the gamewindow and are updated throughout the game.
6.	Top10-list. There is a top10 single player list that is dependent on the points left as the game ends. This data is saved between games and is visible in the start dialogue. 
7.	Local multiplayer. There is an option for two players, where the second player plays with the murderer. This can be chosen from the start dialog.
8.	Simple AI: In single player mode, the murderer makes choices on which buses to get on, which buses to get off and which directions to walk to, depending on the location of the murderer and the hero by trying to move to the opposite direction of the hero.
9.	Buses routes are designed to go even outside the map. In that case, if player is inside a bus that goes outside the map, the player to teleported back to the last stop it was in.
10.	Players are notified if they press invalid keys. For example, a player can only get into a bus at a stop. So, if a player tries to get into a bus outside a stop, notification is provided.
