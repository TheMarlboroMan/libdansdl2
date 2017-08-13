# libdansdl2

C++ Wrappers for SDL2 and OpenGL.

As of July 2016 it has been overhauled to completely skew the SDL2 rendering so everything is rendered now with OpenGL. SDL2 continues in use in the audio, input and window management systems.

As of mid July 2016 all the library has been translated into english. All public interfaces have been translated and most of them have been retooled (hopefully it's all a little easier to use). As a result of these changes, all compatibility with previous versions has been broken. Copies of the old versions are to be found in the "master-es" and "classic" branches, but these will be developed no more.

## Changelog

- August 2017: 
	- added doxygen support and comments.
	- removed unused functions (old functions with window parameters...).
	- normalised a few functions that got two integers instead of a point class.
	- removed the sound_queue class. Each application should have its own.
	- draconian compiler flags.
	- camera margin settings.
	- Powers of 2 for text textures.
	- VSync configuration.

## TODO: 

- Revise uppercase constants.
- In the future, fork for OPENGL 1.4 and the rest.
