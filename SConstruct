import glob
import os

options = Variables('options.cache', ARGUMENTS)
options.Add(BoolVariable('verbose', 'Display the full command line instead a short command description', 'no') )
options.Add(BoolVariable('qtunit', 'Use the qt based test runner', 'no') )
options.Add('cross', 'Cross compiling tools prefix, ie: x86_64-w64-mingw32-', '')

env = DefaultEnvironment(ENV=os.environ, tools=['default'], options=options)
options.Save('../options.cache', env)
Help(options.GenerateHelpText(env))
env.SConsignFile() # Single signature file

def buildDir():
	if not env['cross']: return 'build/'
	subdir = env['cross']
	if subdir.endswith('-'): subdir=subdir[:-1]
	return 'build/' + subdir + "/"

def built(s):
	if s.startswith("src/"): s = s[4:]
	return buildDir()+s

def ScanSources(pattern) :
	return [ built(a) for a in glob.glob('src/'+pattern) ]
	pass

allSources = ([]
	+ ScanSources("*.cxx")
	+ ScanSources("*.cpp")
	+ ScanSources("*.c")
	+ ScanSources("*/*.cxx")
	+ ScanSources("*/*.cpp")
	)
tests = ScanSources("*/*.test.cxx") + ScanSources("*.test.cxx")
mainFiles = [built("MainBio1.cpp"), built("RegressionTest.cxx")]
commonSources = [s for s in allSources if s not in tests and s not in mainFiles ]

print(commonSources)

sources = commonSources + [built("MainBio1.cpp")]
testSources = commonSources + tests + [built("RegressionTest.cxx")]
includePath = [ dir.strip() for dir in os.popen('find src -type d') ]
env.Append(CPPPATH = includePath)

print('# Sources:')
print('\n'.join(sources))
print('# Tests:')
print('\n'.join(testSources))
print('# IncludePath')
print(includePath)

#SConscript('src/SConscript', duplicate=0, variant_dir='build')

env.VariantDir(buildDir(), 'src', duplicate=0)

cross_prefix = env['cross']
env.Replace(
	CC    = cross_prefix+"gcc",
	CXX   = cross_prefix+"g++",
	LD    = cross_prefix+"g++",
	AR    = cross_prefix+"ar",
	STRIP = cross_prefix+"strip",
)
env.Append(BUILDERS = {'Test' :
	Builder(action = "wine64 ./$SOURCE")
	if cross_prefix=='x86_64-w64-mingw32-' else
	Builder(action = "./$SOURCE")
	})
if cross_prefix:
	env['ENV']['PKG_CONFIG_LIBDIR'] = 'dependencies/usr/lib/pkgconfig'
	env.Replace(PROGSUFFIX = '.exe')




env.Tool('qt4', toolpath=['.'])


env['CXXFILESUFFIX'] = '.cxx'
env['QT4_UICDECLSUFFIX'] = '.hxx'
env['QT4_MOCHPREFIX'] = os.path.join('moc_')
env['QT4_UICDECLPREFIX'] = os.path.join('uic_')
env['QT4_QRCCXXPREFIX'] = os.path.join('qrc_')
def blue(str):
    return u'\033[34m== '+str+u'\033[0m'
if not env['verbose']:
    env['CCCOMSTR'] = blue('Compiling $SOURCE')
    env['CXXCOMSTR'] = blue('Compiling $SOURCE')
    env['LINKCOMSTR'] = blue('Linking $TARGET')
    env['SHLINKCOMSTR'] = blue('Linking library $TARGET')
    env['QT4_RCCCOMSTR'] = blue('Embeding resources $SOURCE')
    env['QT4_UICCOMSTR'] = blue('Compiling interface $SOURCE')
    env['QT4_LRELEASECOMSTR'] = blue('Compiling translation $TARGET')
    env['QT4_MOCFROMHCOMSTR'] = blue('Generating metaobjects for $SOURCE')
    env['QT4_MOCFROMCXXCOMSTR'] = blue('Generating metaobjects for $SOURCE')

if False:
	Env.EnableQt4Modules([
		'QtCore',
		'QtGui',
		'QtOpenGL',
	], debug=False)

env.ParseConfig('pkg-config cppunit --cflags --libs')


env.Program('#bioscenatests', testSources),
env.Default( [
	env.Program('#bioscena', sources),
	env.Test('#bioscenatests.passed', '#bioscenatests' ),
])


# vim: syntax=python
