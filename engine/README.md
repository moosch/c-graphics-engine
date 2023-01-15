# Engine notes

## Maths

Use GLM. No need to reinvent the wheel.

Mainly used to perform 3D to 2D, and move the world around the camera (or the other way around...TBD).

## Physics

The engine will expose a physics module that the game code can use to manipulate game data.


## Rendering

Vulkan is the rendering engine.

Set up a "backend" which is Vulkan specific, and a "frontend" API that the game can use.
