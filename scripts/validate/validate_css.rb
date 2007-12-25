#!/usr/bin/env ruby
#
#  Created by Glenn Rempe on 2007-08-29.
#  Copyright (c) 2007. All rights reserved.

# Run this script to call the W3C css validator Java binary which we have locally installed.
# The script will iterate over all CSS files in the framework and report any validator warnings
# or errors. 

# Grabbed from : http://www.illumit.com/css-validator/

# ===========================================================================================
# BEGIN USER EDITABLE CONFIG

# What command will we run to test each file?
VALIDATOR_BIN = "./css-validator/css-validator.jar"

# What is the main CSS dir for the framework?  (Where are screen.css and print.css?)
CSS_DIR = "../../blueprint"

# Which CSS files should we test?  '.css' extension is not required or desired.
CSS_FILES = %w( screen print ie )

# END USER EDITABLE CONFIG
# ===========================================================================================


# ===========================================================================================
# REALLY!  NO NEED TO CHANGE ANYTHING BELOW HERE!


CSS_FILES.each do |file|

  puts "TESTING CORE FILE : #{CSS_DIR}/#{file}.css"
  puts "-------------------------------------------------------"
  puts ""
  
  begin
    system("java -jar #{VALIDATOR_BIN} #{CSS_DIR}/#{file}.css")
  rescue Exception => e
    puts "Calling W3C validator failed with exception: " + e
  end

end


puts "-------------------------------------------------------"
puts "-------------------------------------------------------"
puts "DONE TESTING!"
