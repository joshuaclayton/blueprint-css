#!/usr/bin/env ruby
require 'lib/parse.rb'

# directories
source  = '../../blueprint/src/'
dest    = '../../blueprint/'

# files
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
puts "** Blueprint CSS Framework Compressor **"
puts "See docs/Readme.txt for further instructions.\n\n"

header = File.new('lib/header.txt').read
groups.each do |name, files|
  puts "Creating #{name}:"
  
  css = header
  for file in files
    puts "* Compressing /src/#{file}.."
    css += Parse.new().path_to_string(source + file)    
  end
  File.open(dest + name, 'w') { |f|
    f << css
  }
end
puts "Done!"

