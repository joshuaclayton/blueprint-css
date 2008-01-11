#!/usr/bin/env ruby

# 
# Blueprint CSS Validator
# Created by Glenn Rempe on 2007-08-29.
# 
# Copyright (c) 2007. All rights reserved.
# Grabbed from http://illumit.com/css-validator
# 
# This script will iterate over all CSS files in 
# the framework and report any validator errors. 
# 
# Ruby and Java must be installed for this to work.
# You can then run this command:
# $ ruby validate.rb
# 

validator   = "lib/css-validator.jar"     # location of validator
blueprint   = "../../blueprint"           # main css directory
files       = ['screen', 'print', 'ie']   # files to test

# ------------------------------------------------------------------------ #

# start testing files
files.each do |file|
  puts "*** Testing #{blueprint}/#{file}.css"
  begin
    # the "-e" suppresses warnings, as they are quite useless.
    system("java -jar #{validator} -e #{blueprint}/#{file}.css")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end
end
puts "*** CSS testing complete!"
