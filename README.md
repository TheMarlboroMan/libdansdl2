# libdansdl2
C++ Wrappers for SDL2 and OpenGL.

## TODO: 

- Revise uppercase constants.
- In the future, fork for OPENGL 1.4 and the rest.

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
- Do real camera checking. Now it's fucked up.
- Restore rotation calculation for stuff, so we can do better camera work.
- Remove the two different rendering functions (with and without camera): use a default camera value.
- Integrate the zooming better.
- What about "establecer_posicion" on primitives?. Does it behave?. Should it?.
	- It should move the origin and recalculate the position. 
	- Width and height could be ignored or could scale the thing.
	- Thus, establecer_posicion should be virtual and be reimplemented on these classes :(.
- Maybe get rid of most of the hierarchy: it's way too convoluted and the whole position and clipping is a bit rancid.
- Not libdansdl2 but fix the screen changes in the framework: they don't really work.
- Get rid of filled vs wireframe classes?. The only difference is at rendering and doesn't make much sense. 
	- Yes... Ad an enum class there and go with it.
- Setup representación agrupada.
- Check screen boundaries for rendering.
- Fix groups and camera errors: the items are not in focus since they're not displaced.
	- Either skip the check if the whole group is in position...
	- Or add the distance to the check across the calls.
- Add rotation to groups
	- After various tries I think it's better to skip the screen check for each single element: it's very problematic with rotations and leads to dirty code.
- Cleanup warnings.
- Alpha in groups.
- Test in Windows...
- Keep clearing old SDL_Rect functions.
	- At least on the video part.
- Add vector constructor and insert for points.
- Remove mapa_colision_bitmap
- Clear OLD functions with no use. Keep the interface simpler. 
	- Delete old functions using SDL_Texture.
	- Take a look at "herramientas" and clear it. There's a lot of things belonging in herramientas_proyecto now.
	- We should actually take a look at the whole code... There's a lot of things that are only defined and never used.
- Put the logger with the herramientas_proyecto. Just throw when something fails.
	- I can't: there are real use cases in the input systems.
- Revise TODOs.
