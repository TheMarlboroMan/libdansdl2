# libdansdl2
C++ Wrappers for SDL2 and OpenGL.

## TODO: 
- Re-think the RGBA part, see how we can integrate it with the alpha thing.
- Think again about the renderer part...
- Remove the two different rendering functions (with and without camera).
- Integrate the zooming better.
- Remove the need to return something from the rendering.
- Maybe get rid of most of the hierarchy: it's way too convoluted and the whole position and clipping is a bit rancid.
- Get rid of filled vs wireframe classes?. The only difference is at rendering and doesn't make much sense.
- Integrate box with polygon in the primitives?????.
- Integrate "patterned bitmap" with "bitmap"?
- Get rid of old text interface for bitmap fonts.
- In the future, fork for OPENGL 1.4 and the rest.
