#!/bin/sh

svn co https://irrlicht.svn.sourceforge.net/svnroot/irrlicht/trunk irrlicht-svn || (echo "Couldn't checkout irrlicht" ; exit)
cd irrlicht-svn
patch -p0 -i ../irrlicht-mods.diff
cd ..
