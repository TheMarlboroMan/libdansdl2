# libdansdl2

C++ Wrappers for SDL2 and OpenGL.

As of July 2016 it has been overhauled to completely skew the SDL2 rendering so everything is rendered now with OpenGL. SDL2 continues in use in the audio, input and window management systems.

As of mid July 2016 all the library has been translated into english. All public interfaces have been translated and most of them have been retooled (hopefully it's all a little easier to use). As a result of these changes, all compatibility with previous versions has been broken. Copies of the old versions are to be found in the "master-es" and "classic" branches, but these will be developed no more.

## Changelog

- August 2017: 
	- added doxygen support and comments.
	- removed unused features.
	- normalised a few functions that got two integers instead of a point class.


## TODO: 

- Revise uppercase constants.
- In the future, fork for OPENGL 1.4 and the rest.
