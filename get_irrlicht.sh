#!/bin/sh

svn co https://irrlicht.svn.sourceforge.net/svnroot/irrlicht/trunk irrlicht-svn --depth=immediates || (echo "Couldn't checkout irrlicht" ; exit)
cd irrlicht-svn
cd source
svn up
cd ../lib
svn up
cd ../include
svn up
patch -p0 -i ../irrlicht-mods.diff
cd ..
