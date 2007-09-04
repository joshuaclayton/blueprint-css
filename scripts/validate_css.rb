#!/usr/bin/env ruby
#
#  Created by Glenn Rempe on 2007-08-29.
#  Copyright (c) 2007. All rights reserved.

# Run this script to call the W3C css validator Java binary which we have locally installed.
# The script will iterate over all CSS files in the framework and report any validator warnings
# or errors. 

# ===========================================================================================
# BEGIN USER EDITABLE CONFIG

# What command will we run to test each file?
VALIDATOR_BIN = "./css-validator/css-validator.jar"

# What is the main CSS dir for the framework?  (Where are screen.css and print.css?)
CSS_DIR = "../blueprint"

# Where is the CSS lib dir (contains grid.css, reset.css, etc.)
CSS_LIB_DIR = "#{CSS_DIR}/lib"

# What directory are the original CSS files stored in? (No trailing slash)
CSS_COMPRESSED_DIR = "#{CSS_DIR}/compressed"

# Which CSS files should we test?  '.css' extension is not required or desired.
CSS_MAIN_FILES = %w( screen print )
CSS_CORE_FILES = %w( reset typography grid ie )
CSS_COMPRESSED_FILES = %w( screen print )

# END USER EDITABLE CONFIG
# ===========================================================================================


# ===========================================================================================
# REALLY!  NO NEED TO CHANGE ANYTHING BELOW HERE!

CSS_MAIN_FILES.each do |file|

  puts "TESTING MAIN FILE : #{CSS_DIR}/#{file}.css"
  puts "-------------------------------------------------------"
  puts ""
  
  begin
    system("java -jar #{VALIDATOR_BIN} #{CSS_DIR}/#{file}.css")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end

end


CSS_CORE_FILES.each do |file|

  puts "TESTING CORE FILE : #{CSS_LIB_DIR}/#{file}.css"
  puts "-------------------------------------------------------"
  puts ""
  
  begin
    system("java -jar #{VALIDATOR_BIN} #{CSS_LIB_DIR}/#{file}.css")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end

end


CSS_COMPRESSED_FILES.each do |file|

  puts "TESTING COMPRESSED FILE : #{CSS_COMPRESSED_DIR}/#{file}.css"
  puts "-------------------------------------------------------"
  puts ""
  
  begin
    system("java -jar #{VALIDATOR_BIN} #{CSS_COMPRESSED_DIR}/#{file}.css")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end

end

