# libdansdl2
C++ Wrappers for SDL2 and OpenGL.

## TODO: 

- Do real camera checking. Now it's fucked up.
- Restore rotation calculation for stuff, so we can do better camera work.
- Integrate the zooming better.
- What about "establecer_posicion" on primitives?. Does it behave?. Should it?.
	- It should move the origin and recalculate the position. 
	- Width and height could be ignored or could scale the thing.
	- Thus, establecer_posicion should be virtual and be reimplemented on these classes :(.
- Clear OLD functions with no use. Keep the interface simpler.
	- We should actually take a look at the whole code... There's a lot of things that are only defined and never used.
- Keep clearing old SDL_Rect functions.
	- At least on the video part.
- Remove the two different rendering functions (with and without camera): use a default camera value.
- Maybe get rid of most of the hierarchy: it's way too convoluted and the whole position and clipping is a bit rancid.
- Get rid of filled vs wireframe classes?. The only difference is at rendering and doesn't make much sense.
- Cleanup warnings.
- In the future, fork for OPENGL 1.4 and the rest.
- Not libdansdl2 but fix the screen changes in the framework: they don't really work.

## DONE:
- Remove the need a return value from the rendering.
- Get rid of static vs dynamic.
- Re-think the RGBA part, see how we can integrate it with the alpha thing.
- Get rid of old text interface for bitmap fonts.
- Think again about the renderer part... 
- Remove the renderer from the render functions: it doesn't do anything there anymore.
- Do over the patterned bitmap thing... GL_REPEAT modes apply to a full texture so there are worthless.
- Integrate "patterned bitmap" with "bitmap"... Could use a new parameter "pincel" that defaults to rect size.
- Integrate box with polygon in the primitives?????. Would remove yet another class. 
- Rotation on a bitmap really fucks things up.
- Add camera clipping using the stencil buffer.
- We could apply rotation to... everything. It should be easy enough now :D.
