GitHub link: "https://github.com/BoDeKeersmaeker/Engine"

Game loop pattern:
I use the game loop patern to ensure my code is not frame rate based.
also it the best fit for a game engine because it allows the game to keep running when the player isn't giving inputs.

Update method:
In this engine I use a lot of seperate independent objects.
The update method pattern is perfect for this.

Command:
It makes it easy to bind buttons to commands and makes it user friendly.
It is also configurable so later users of the engine can easily make their own commands.

Observer:
I use the observer pattern to easely check events like player death or player score gain and update their respective counter components.

EventQueue:
The eventQueue in my engine is used to play and queue sounds.
it is perfect for this because a lot of sound can possibly be queued in one frame.

Component:
I use the component system to avoid having to use inheritance.
Composition also helps keep classes cleaner because they can choose what components to add.
It also keeps seperate parts that don't need to be connected decoupled. 
And prefer composition over inheritance!

Service locator:
the service locater is used for the audio system.
This allows users to make and use their own audio systems.



