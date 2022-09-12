# libdansdl2

C++ Wrappers for SDL2 and OpenGL.

As of July 2016 it has been overhauled to completely skew the SDL2 rendering so everything is rendered now with OpenGL. SDL2 continues in use in the audio, input and window management systems.

Given that my hardware has an openGL implementation with version 1.4 expect a lot of deprecated features.

As of mid July 2016 all the library has been translated into english. All public interfaces have been translated and most of them have been retooled (hopefully it's all a little easier to use). As a result of these changes, all compatibility with previous versions has been broken. Copies of the old versions are to be found in the "master-es" and "classic" branches, but these will be developed no more.

As of January 2020 the library has broken all compatibility again. This time, files have been reorganized so the filesystem represents the namespaces and not so many directories are needed. The old "include all video/audio/etc" include files have been removed too, as including only what's necessary is the better way.

## Building on linux:

Use Cmake.

## Dependencies.

Starting in 2018 and for the first time there is an external dependency on the "log" project in github.com/TheMarlboroMan/log. You'll need to link every project against the log.o created when building that project.

## License

See LICENSE. Dependencies of this project are all my own and similarly licensed.

## TODO: 

- Add a test directory where all uses are tested.
- Revise uppercase constants.
- In the future, fork for OPENGL 1.4 and the rest.
