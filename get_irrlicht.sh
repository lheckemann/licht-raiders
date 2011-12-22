#!/bin/sh
test -d irrlicht-svn && echo "Warning: irrlicht-svn directory already exists"
svn co https://irrlicht.svn.sourceforge.net/svnroot/irrlicht/trunk irrlicht-svn --depth=immediates > /dev/null || (echo "Couldn't checkout irrlicht" ; exit -1)
cd irrlicht-svn
test -d source || (echo "Error: irrlicht-svn/source seems not to exist." ; exit -1)
test -d lib || (echo "Error: irrlicht-svn/lib seems not to exist." ; exit -1)
test -d include || (echo "Error: irrlicht-svn/include seems not to exist." ; exit -1)
cd source
svn up --set-depth infinity > /dev/null
cd ..
cd lib
svn up --set-depth infinity > /dev/null
cd ..
cd include
svn up --set-depth infinity > /dev/null
cd ..
patch -p0 -i ../irrlicht-mods.diff || (echo "Error: couldn't apply patch to irrlicht sources." ; exit -1)
cd ..
