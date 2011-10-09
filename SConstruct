#!/usr/bin/python -c "pass"
# ^ Trick for syntax highlighting in gedit...

env = Environment()
conf = Configure(env)

LIBS=['Irrlicht', 'boost_filesystem','lua5.1','luabind']

SConscript("scons_exglob.py")

for lib in LIBS:
    if not conf.CheckLib(lib):
        print 'Could not find lib ' + lib
        Exit(1)

env = conf.Finish()

import os
if os.path.isdir("/usr/local/include/irrlicht"):
	env.Append(CPPFLAGS=['-I/usr/local/include/irrlicht'])
elif os.path.isdir("/usr/include/irrlicht"):
	env.Append(CPPFLAGS=['-I/usr/include/irrlicht'])

env.Append(CPPFLAGS=['-I/usr/include/lua5.1/'])
env.Append(CXXFLAGS=['-g', '-Wall'])

#env.Replace(CXX=['clang++'])

Export('env')
Export('Glob')

SConscript("src/SConscript")
