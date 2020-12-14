# Flesh-cpp
Flesh is a worms-inspired student game where the idea is to blow up happy creatures. It was written using c++ and the cocos2d-x game engine and is unfinished.

[Gameplay video:](http://www.youtube.com/watch?v=DDBhnYuBsPo)

[![Youtube](https://i.ytimg.com/vi/DDBhnYuBsPo/hqdefault.jpg)](http://www.youtube.com/watch?v=DDBhnYuBsPo)

## Server
The game was designed to be multiplayer, so there is a python server that handles accounts (login/register) and a lobby. SQL is used for the database and TCP sockets for real-time comms.

## Game TODO
* Rework physics
	There are a few major issues with the current physics system.
	chipmunk is being used but does not support swept collisions, which
	make players able to tunnel into objects.
	the solution is either to hack around this or just use box2d instead.
	the downsides to box2d is the API is ugly as hell imo and it will
	be time consuming.

* Rework physics contact
	Currently chipmunks physics collision system is being used. whether or not
	chipmunk or box2d is being used, the contact listener system needs
	to be a lot nicer than it is now.

* For optimisation purposes, use a separate thread when logging
	When using log_info, log_print, ect, the strings can be created and put in 
	a queue, but are only put to action (log to console, write to file)
	when they are run by the logging thread.

* For every system that uses a vector of smart ptrs, log the vector size every x seconds
	This is to make sure there are no dangling smart ptrs around. the vector size
	is logged because each system will remove a element if it doesn't exist anymore.

* Clean up button-GUI class

* Vertex destructable terrain

* Implement entity component system

## Server TODO
* Use separate thread for logging
	When using log_info, log_print, ect, the strings can be created and put in a queue, but are only put to action (log to console, write to file) when they are run by the logging thread.

* Console commands
	Add the ability to input commands in the console while all logs are printed to a file.

	* Stats command
		Shows current amount of players connected, in the lobby and in game

	* Accounts command
		Queries the database and returns the amount of registered users as well
		as the amount of players connected.

	* Lobby command
		Shows current amount of players in lobby, the amount of created games
		and the amount of players that are waiting in a game

	* Help command
		Shows details of all available commands
