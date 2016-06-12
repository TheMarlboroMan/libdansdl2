# libdansdl2
C++ Wrappers for SDL2 and OpenGL.

## TODO: 

- Do over the patterned bitmap thing... GL_REPEAT modes apply to a full texture so there are worthless.
- Clear OLD functions with no use. Keep the interface simpler.
- Keep clearing old SDL_Rect functions.
- Remove the two different rendering functions (with and without camera): use a default camera value.
- Integrate the zooming better.
- Maybe get rid of most of the hierarchy: it's way too convoluted and the whole position and clipping is a bit rancid.
- Get rid of filled vs wireframe classes?. The only difference is at rendering and doesn't make much sense.
- Integrate box with polygon in the primitives?????. Would remove yet another class. 
- Integrate "patterned bitmap" with "bitmap"?
- In the future, fork for OPENGL 1.4 and the rest.

## DONE:
- Remove the need a return value from the rendering.
- Get rid of static vs dynamic.
- Re-think the RGBA part, see how we can integrate it with the alpha thing.
- Get rid of old text interface for bitmap fonts.
- Think again about the renderer part... 
- Remove the renderer from the render functions: it doesn't do anything there anymore.
