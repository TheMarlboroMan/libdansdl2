# libdansdl2
C++ Wrappers for SDL2 and OpenGL.

## TODO: 

- Think again about the renderer part... 
- Remove the renderer from the render functions: it doesn't do anything there anymore.
- Remove the two different rendering functions (with and without camera): use a default camera value.
- Integrate the zooming better.
- Maybe get rid of most of the hierarchy: it's way too convoluted and the whole position and clipping is a bit rancid.
- Get rid of filled vs wireframe classes?. The only difference is at rendering and doesn't make much sense.
- Integrate box with polygon in the primitives?????.
- Integrate "patterned bitmap" with "bitmap"?
- Get rid of old text interface for bitmap fonts.
- In the future, fork for OPENGL 1.4 and the rest.

## DONE:
- Remove the need a return value from the rendering.
- Get rid of static vs dynamic.
- Re-think the RGBA part, see how we can integrate it with the alpha thing.
