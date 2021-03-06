# Open Rock Raiders: Irr

(C) 2011 Linus Heckemann
All rights reserved.

## About

A fork of the Open Rock Raiders project, created by the community Rock Raiders United (RRU) at http://rockraidersunited.org. I no longer visit this site, and am not, other than being a member, affiliated with it in any way.

Note that it uses a MODIFIED version of the irrlicht engine - this means that you can *not* use the standard distributions of it.

## Compiling

Building in any non-GNU/Linux system is probably a huge pain. In future, SDKs will be provided for compiling on other OSes.

You need:
 - C++ compiler etc. compatible with SCons (gcc works just fine)
 - SCons (for SCons, you need python)
 - OpenGL header files (mesa-common-dev in debian and ubuntu)
 - X11 headers (libx11-dev and libxxf86vm-dev in debian and ubuntu)
 - SVN commandline client
 - Boost headers

The build process is currently not very refined; This will be improved later on as development progresses. In a nicely configured system, you should be able to build using the following commands:
$ ./get_irrlicht.sh
<wait as Irrlicht is downloaded and patched>
$ scons
<wait as it gets compiled>

The resulting executable assumes it is run in the same directory as the data/ directory containing all the game data.
