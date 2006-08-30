import glob
import os

def ScanSources(pattern) :
	return [ a[4:] for a in glob.glob('src/'+pattern) ]
	pass

allSources = ScanSources("*/*.cxx")
tests = ScanSources("*/*.test.cxx")
interfaceSources = ScanSources("QtInterface/*.cxx")

print allSources

sources = [ s for s in allSources if s not in tests ] + ["MainQt.cxx"]
testSources = [ s for s in allSources if s not in interfaceSources ] + ["RegressionTest.cxx"]
includePath = [ dir.strip()[4:] for dir in os.popen('find src -type d | grep -v CVS') ] 

Export('sources', 'testSources', 'includePath')

print sources
print testSources
print includePath

SConscript('src/SConscript', duplicate=0, build_dir='build')



