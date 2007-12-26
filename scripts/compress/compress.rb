#!/usr/bin/env ruby

# 
# Blueprint CSS Compressor
# By Olav Bjorkoy (www.bjorkoy.com)
# 
# This script creates up-to-date compressed files from
# the 'blueprint/src' directory. Each source file belongs 
# to a certain compressed file, as defined below. 
# 
# The newly compressed files are placed in the
# 'blueprint' directory.
# 
# Ruby has to be installed for this script to work.
# You can then run the following command (without the $): 
# $ ruby compress.rb
# 

require 'lib/parse.rb'

# directories
dest  = '../../blueprint/'
src   = dest + 'src/'

# grouped source files
screen  = ['reset.css', 'typography.css', 'grid.css', 'forms.css']
print   = ['print.css']
ie      = ['ie.css']

# compressed file names and related sources
groups  = {
  'screen.css' => screen,
  'print.css' => print,
  'ie.css' => ie
}

# ------------------------------------------------------------------------ #

# compress each file
puts "** Blueprint CSS Framework Compressor"
puts "** Builds compressed files from the source directory."

header = File.new('lib/header.txt').read
groups.each do |name, files|
  puts "\nAssembling #{name}:"
  css = header
  
  files.each do |f|
    puts "+ src/#{f}.."
    css += Parse.new().path_to_string(src + f)
  end
  File.open(dest + name, 'w') do |f|
    f << css
  end
end

puts "\n** Done!"
puts "** Your compressed files are now up-to-date."
