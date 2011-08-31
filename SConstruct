#!/usr/bin/python -c "pass"

env = Environment()
conf = Configure(env)

LIBS=['Irrlicht', 'z']

for lib in LIBS:
    if not conf.CheckLib(lib):
        print 'Could not find lib ' + lib
        Exit(1)

env = conf.Finish()

env.ParseConfig('pkg-config zlib --cflags')
env.ParseConfig('pkg-config zlib --libs')

import os
if os.path.isdir("/usr/local/include/irrlicht"):
	env.Append(CPPFLAGS=['-I/usr/local/include/irrlicht'])
elif os.path.isdir("/usr/include/irrlicht"):
	env.Append(CPPFLAGS=['-I/usr/include/irrlicht'])

env.Append(CXXFLAGS=['-g', '-Wall'])
#env.Replace(CXX=['clang++'])

ConfigFile = env.Object('ConfigFile.cpp')
event = env.Object("event.cpp")
enttype = env.Object("enttype.cpp")

env.Program('IrrRR', ['main.cpp'] + event + ConfigFile + enttype)
