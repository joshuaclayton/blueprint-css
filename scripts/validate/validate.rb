#!/usr/bin/env ruby

# 
# Blueprint CSS Validator
# By Glenn Rempe & Olav Bjorkoy.
# Grabbed from http://illumit.com/css-validator
# 
# This script will iterate over all CSS files in 
# the framework and report any validator errors. 
# 
# Ruby and Java must be installed for this to work.
# You can then run this command:
# $ ruby validate.rb
# 

# files to test
files = ['screen.css', 'print.css', 'ie.css']

# -------------------------------------------------------- #

directory = "../../blueprint"       # main css directory
validator = "lib/css-validator.jar" # location of validator

# start testing files
files.each do |file|
  puts "*** Testing #{directory}/#{file}"
  begin
    # the "-e" suppresses warnings, as they are quite useless.
    system("java -jar #{validator} -e #{directory}/#{file}")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end
end
puts "*** CSS testing complete!"
