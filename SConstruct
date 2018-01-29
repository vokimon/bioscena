import glob
import os

def ScanSources(pattern) :
	return [ a[4:] for a in glob.glob('src/'+pattern) ]
	pass

allSources = ScanSources("*.cxx") + ScanSources("*/*.cxx") + ScanSources("*/*.cpp") + ScanSources("*.cpp") + ScanSources("*.c")
tests = ScanSources("*/*.test.cxx") + ScanSources("*.test.cxx")
mainFiles = ["MainBio1.cpp", "RegressionTest.cxx"]
commonSources = [s for s in allSources if s not in tests and s not in mainFiles ]

print commonSources

sources = commonSources + ["MainBio1.cpp"]
testSources = commonSources + tests + ["RegressionTest.cxx"]
includePath = [ dir.strip()[4:] or '.' for dir in os.popen('find src -type d | grep -v CVS') ] 

Export('sources', 'testSources', 'includePath')

print '# Sources:'
print '\n'.join(sources)
print '# Tests:'
print '\n'.join(testSources)
print '# IncludePath'
print includePath

SConscript('src/SConscript', duplicate=0, variant_dir='build')


# vim: syntax=python
