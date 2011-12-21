#!/usr/bin/python -c "pass"
# ^ Trick for syntax highlighting.

env = Environment()
conf = Configure(env)

LIBS=['boost_filesystem', 'boost_system', 'GL']

SConscript("scons_exglob.py")

for lib in LIBS:
    if not conf.CheckLib(lib):
        print 'Could not find lib ' + lib
        Exit(1)

env = conf.Finish()

env.Append(CXXFLAGS=['-ggdb', '-g3', '-Wall'])
import os
if not os.path.isdir("./irrlicht-svn"):
	print "Irrlicht seems not to be present. Run get_irrlicht.sh if possible."
	Exit(1)
if not os.path.exists("./irrlicht-svn/lib/Linux/libIrrlicht.a"):
	print "Irrlicht not compiled."
	os.chdir("irrlicht-svn/source/Irrlicht")
	os.system("make -j%d" % (env.GetOption("num_jobs")))
env.Append(LIBPATH=[os.getcwd() + "/irrlicht-svn/lib/Linux"])
env.Append(LIBS=["Irrlicht"])
env.Append(CPPPATH=[os.getcwd() + "/irrlicht-svn/include"])
#env.Append(CPPFLAGS=['-DBE_POLITE'])
#env.Replace(CXX=['clang++'])

Export('env')

SConscript("src/SConscript")
