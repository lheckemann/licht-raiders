#!/usr/bin/python -c "pass"
# ^ Trick for syntax highlighting.

env = Environment()
conf = Configure(env)

colours = {}
colours['cyan']   = '\033[96m'
colours['purple'] = '\033[95m'
colours['blue']   = '\033[94m'
colours['green']  = '\033[92m'
colours['yellow'] = '\033[93m'
colours['red']    = '\033[91m'
colours['end']    = '\033[0m'

LIBS=['boost_filesystem', 'boost_system', 'GL', 'X11']

SConscript("scons_exglob.py")

for lib in LIBS:
    if not conf.CheckLib(lib):
        print 'Could not find lib ' + lib
        Exit(1)

env = conf.Finish()

#env.Append(CXXFLAGS=['-O3', '-fexpensive-optimizations']) # Optimised builds
env.Append(CXXFLAGS=['-ggdb', '-g3', '-Wall']) # Debug builds


# Loads of stuff for irrlicht
import os
# 1. Check if it's downloaded
if not os.path.isdir("./irrlicht-svn"):
	print colours['cyan'] + "Irrlicht seems not to be present. Run get_irrlicht.sh if possible." + colours['end']
	Exit(1)

# 2. Compile it if necessary
print colours['cyan'] + "Running make to recompile Irrlicht if necessary." + colours['end']
os.chdir("irrlicht-svn/source/Irrlicht")
os.system("make -j%d" % (env.GetOption("num_jobs")))
os.chdir("../../../")
if not os.path.exists("./irrlicht-svn/lib/Linux/libIrrlicht.a"): # TODO adapt to work on non-linux systems
	print colours['cyan'] + "Compiling irrlicht seems to have failed." + colours['end']
	Exit(1)
else:
	print colours['cyan'] + "Irrlicht found. Compiling Licht Raiders." + colours['end']

# 3. Add it to the path.
env.Append(LIBPATH=[os.getcwd() + "/irrlicht-svn/lib/Linux"])
env.Prepend(LIBS=["Irrlicht"]) # must be prepended because it depends on GL and X11
env.Append(CPPPATH=[os.getcwd() + "/irrlicht-svn/include"])
# That's it for Irrlicht


env.Append(CPPFLAGS=['-DBE_POLITE'])
#env.Replace(CXX=['clang++'])

Export('env')

SConscript("src/SConscript")
