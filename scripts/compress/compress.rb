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
# Ruby has to be installed for this script to work.
# You can then run the following command: 
# $ ruby compress.rb
# 


# compressed file names and related sources, in order
files  = {
  'screen.css'  => ['reset.css', 'typography.css', 'grid.css', 'forms.css'],
  'print.css'   => ['print.css'],
  'ie.css'      => ['ie.css']
}

# -------------------------------------------------------- #

require 'lib/parse.rb' # for parsing the css

dest    = '../../blueprint/' # destionation directory
src     = dest + 'src/' # source files directory
header  = File.new('lib/header.txt').read # compressed file header

puts "** Blueprint CSS Framework Compressor"
puts "** Builds compressed files from the source directory."

# start parsing and compressing
files.each do |name, sources|
  puts "\nAssembling #{name}:"
  css = header
  
  # parse and compress each source file in this group
  sources.each do |file|
    puts "+ src/#{file}"
    css += Parse.new(src + file).to_s
  end
  
  # write compressed css to destination file
  File.open(dest + name, 'w') do |f|
    f << css
  end
end

puts "\n** Done!"
puts "** Your compressed files are now up-to-date."
