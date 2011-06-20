#!/usr/bin/python -c "pass"

env = Environment()
conf = Configure(env)

LIBS=['Irrlicht', 'z', 'tinyxml']

for lib in LIBS:
    if not conf.CheckLib(lib):
        print 'Could not find lib ' + lib
        Exit(1)

env = conf.Finish()

env.ParseConfig('pkg-config tinyxml --cflags')
env.ParseConfig('pkg-config tinyxml --libs')
env.ParseConfig('pkg-config zlib --cflags')
env.ParseConfig('pkg-config zlib --libs')

import getpass
if getpass.getuser() == "linus" or getpass.getuser() == "marius":
	env.Append(CPPFLAGS=['-I/usr/local/include/irrlicht'])

env.Append(CXXFLAGS=['-g', '-Wall'])
env.Replace(CXX=['clang++'])

Export('env')
SConscript('TmxParser/SConscript')

ConfigFile = env.Object('ConfigFile.cpp')
event = env.Object("event.cpp")

env.Program('IrrRR', ['main.cpp'] + event + ConfigFile)
