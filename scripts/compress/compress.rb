#!/usr/bin/env ruby

# 
# Blueprint CSS Compressor
# Copyright (c) Olav Bjorkoy 2007. 
# See docs/License.txt for more info.
# 
# This script creates up-to-date compressed files from
# the 'blueprint/source' directory. Each source file belongs 
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
files = {
  'screen.css'  => ['reset.css', 'typography.css', 'grid.css', 'forms.css'],
  'print.css'   => ['print.css'],
  'ie.css'      => ['ie.css']
}

# -------------------------------------------------------- #

require 'lib/parsecss.rb'

# directories
destination = '../../blueprint/'
source = destination + 'src/'

# compressed file header
header = File.new('lib/header.txt').read

puts "** Blueprint CSS Compressor"
puts "** Builds compressed files from the source directory."

# start parsing and compressing
files.each do |name, sources|
  puts "\nAssembling #{name}:"
  css = header
  
  # parse and compress each source file in this group
  sources.each do |file|
    puts "+ src/#{file}"
    css += "/* #{file} */\n" if sources.length > 1
    css += ParseCSS.new(source + file).to_s
    css += "\n"
  end
  css.rstrip! # remove unnecessary linebreaks 
  
  # find original and dermine if anything changed
  puts "(no changes made)" if css == File.new(destination + name).read
  
  # write compressed css to destination file
  File.open(destination + name, 'w') do |f|
    f << css
  end
end

puts "\n** Done!"
puts "** Your compressed files are now up-to-date."