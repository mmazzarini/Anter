# Anter
Github repo for a personal platform game developed in Unreal Engine 4.

Info: 

This is the repo of "Anter", a platform game developed in C++ and Unreal Engine 4.
The game is a classic side-scrolling platform game.
It is currently under development, both for engine exploration purposes and out of personal interest.

Like most of UE projects, and apart from .ini/bat files, all the committed files are mostly C++ source code files and Unreal Blueprint files.
The ones are essentially describing C++ classes using UE formalism, therefore including Uproperty, Ufunction and other UE-provided functionalities wrapping the core C++ code. The latter are files made in Unreal visual scripting language. They are useful for design purposes and for tuning, tweaking and configuring a number of values and properties inside game objects, avoiding to recompile C++ every time.

Currently the game is PH.
It offers
- a PH menu where you can navigate up to the map (other options are not activated)
- a very basic 3D map where you control a pawn (the movable actor) that can navigate between levels
- a test level (LIVELLO 01) where you can experiment some playground solutions to the platforming gameplay experience.

The additional layers and reusable technology I have put into the project, on top of the UE framework, are:
- a Finite State Machine (FSM), UAnterGameFSM, which can be plugged into the Game State inside a level. This FSM is useful to handle a number of cases
- a simple button click system to react to buttons being pressed and quickly navigate through menu pages (PH)
- a collision support component to customize the control over collision events and choose the reaction of the pawn to the geometry imposed by the collision system
- a system of generators put into the maps that create instances of enemies, objects and interacting actors and destroy them on request (DI principle)

ToDos (Oct 2025)
- I am refactoring the collision system. Instead of using UE provided one I am experimenting a raycasting system to achieve something similar to move and slide to handle oblique platforms.
- Code cleanup to polish the final result of the project and product
- Refactor the main pawn class (AAnterPaperCharacter), to evaluate the possible removal of the Paper library and to modularize the pawn structure further.
- Find a good artist to make graphics for me, I am not a 3D artist :)

