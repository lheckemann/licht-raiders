#!/bin/sh

svn co https://irrlicht.svn.sourceforge.net/svnroot/irrlicht/trunk irrlicht-svn --depth=immediates || (echo "Couldn't checkout irrlicht" ; exit)
cd irrlicht-svn
cd source
svn up --depth=files
cd ../lib
svn up --depth=files
cd ../include
svn up --depth=files
cd ..
patch -p0 -i ../irrlicht-mods.diff
cd ..
