import os

def filesEndingWith(sufix) :
	findCommand = 'cd src; find * -name \'%s\'; cd ..'%sufix
	files = []
	for file in os.popen(findCommand) :
		files.append(file.strip())
	return files

exports = {}
exports['sources'] = filesEndingWith('*.cxx')
exports['includePaths'] = [
	'-Isrc/Utils',
	'-Isrc/Organismes'
	'-Isrc/Topologies'
]


SConscript('src/SConscript', build_dir='build', duplicate=0, exports=exports)
       

