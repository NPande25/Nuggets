# Nuggets

This repository contains the code for the Nuggets game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
The server can accomodate up to 26 players and one spectator.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
Each player's current nugget count is printed to them, as well as the total amount remaining.
When all gold nuggets are collected, the game ends and a summary is printed.

## Documentation
See the [Implementation Spec](IMPLEMENTATION.md) for implementation details, including in-depth pseudocode for relevant functions

See the [Design Spec](DESIGN.md) for design details, including overall functional decomposition and major data structures.

## Materials
To assist with this game, we used support libraries from Dartmouth's *CS50: Software Design & Implementation*. These include the [Support](support/) library and the [libcs50](libcs50/) library.
