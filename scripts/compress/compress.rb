#!/usr/bin/env ruby
# 
# Blueprint CSS Compressor
# 
# This script creates up-to-date compressed files from
# the 'blueprint/src' directory. Each source file belongs 
# to a certain compressed file, as defined below. 
# 
# The newly compressed files are placed in the
# 'blueprint' directory.
# 
# To use this script, Ruby must be installed. 
# To use the script, simply run this command (without the $):
#
#   $ ruby compress.rb
# 

require 'lib/parse.rb'

# directories
blueprint = '../../blueprint/'
source = blueprint + 'src/'

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
  
  for file in files
    puts "+ src/#{file}.."
    css += Parse.new().path_to_string(source + file)    
  end
  File.open(blueprint + name, 'w') do |f|
    f << css
  end
end
puts "\n** Done! Your compressed files are now up-to-date! :)"

