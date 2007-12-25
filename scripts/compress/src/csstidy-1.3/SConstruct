#This file is adapted from http://www.scons.org/wiki/SconstructMultiple

#get the mode flag from the command line
#default to 'release' if the user didn't specify
mymode = ARGUMENTS.get('mode', 'release')   #holds current mode

#check if the user has been naughty: only 'debug' or 'release' allowed
if not (mymode in ['debug', 'release']):
    print "Error: expected 'debug' or 'release', found: " + mymode
    Exit(1)

#tell the user what we're doing
print '**** Compiling in ' + mymode + ' mode...'

debugcflags = ['-Wall', '-g', '-pedantic']   #extra compile flags for debug
releasecflags = ['-O2']         #extra compile flags for release

env = Environment()

#make sure the sconscripts can get to the variables
Export('env', 'mymode', 'debugcflags', 'releasecflags')

#put all .sconsign files in one place
env.SConsignFile()

#specify the sconscript for CSSTidy
project = 'csstidy'
SConscript(project + '/SConscript', exports=['project'])

