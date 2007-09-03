#!/usr/bin/env ruby
#
#  Created by Glenn Rempe on 2007-08-29.
#  Copyright (c) 2007. All rights reserved.


# ===========================================================================================
# BEGIN USER EDITABLE CONFIG

# Where is the csstidy executable?
# By default this is an OS X executable.  If you need it on another platform you need to get
# a binary for 'csstidy' or compile it yourself from source.
CSSTIDY_BIN = "bin-osx/csstidy"

# What directory are the original CSS files stored in? (No trailing slash)
CSS_LIB_DIR = "../blueprint/lib"

# Which CSS files should we add to the compressed final version?  '.css' extension is not required or desired.
CSS_INPUT_FILES = %w( reset typography grid )

# where are our output files?
TEMP_FILE = "temp.css"
OUTPUT_FILE = "#{CSS_LIB_DIR}/compressed.css"

# start flags off with a nice safe empty value which we can append to if needed
flags = ""

# Which config options shall we pass to csstidy?  Uncomment an option to use it.
# Each of these is set to override the default behavior.

#flags += '--preserve_css=true '

#flags += '--remove_bslash=false '

#flags += '--compress_colors=false '

#flags += '--lowercase_s=true '

#flags += '--timestamp=true '

#flags += '--optimise_shorthands=1 '
#flags += '--optimise_shorthands=2 '
#flags += '--optimise_shorthands=0 '

# had to change '--remove_last_;=true' to escape the semi-colon.  Really bad command option name...
#flags += '--remove_last_\;=true'

#flags += '--sort_properties=true'

#flags += '--sort_selectors=true'

#flags += '--merge_selectors=2 '
#flags += '--merge_selectors=1 '
#flags += '--merge_selectors=0 '

#flags += '--compress_font-weight=false'

#flags += '--allow_html_in_templates=false'

#flags += '--silent=true'

#flags += '--case_properties=0 '
#flags += '--case_properties=1 '
#flags += '--case_properties=2 '

#flags += '--template=default '
#flags += '--template=filename '
#flags += '--template=low '
#flags += '--template=high '
#flags += '--template=highest '

# END USER EDITABLE CONFIG
# ===========================================================================================


# ===========================================================================================
# REALLY!  NO NEED TO CHANGE ANYTHING BELOW HERE!

# Delete any pre-existing output files.  We'll re-generate them.
[TEMP_FILE, OUTPUT_FILE].each do |file|
  begin
    File.delete(file)
  # Errno::ENOENT represents case where you try to delete a file that doesn't exist.
  # We don't need to do or display anything in this case.
  rescue Errno::ENOENT
    # no need to do anything here
  # However this is a real exception (like maybe permission denied) that we should look at.
  rescue Exception => e
    puts "File deletion failed with exception: " + e
  end
end


# Create the temp file
File.open(TEMP_FILE,"w") do |outfile|
   
   # Add the contents (in order) of each of the CSS files we are concatenating
   CSS_INPUT_FILES.each do |file|
     File.open("#{CSS_LIB_DIR}/#{file}.css", "r+") do |oldfile|
          oldfile.each_line { |line| outfile.puts line}
     end
   end
   
end


# Do some csstidy magic on the temp file and send the results to the output file.
begin
  system("./#{CSSTIDY_BIN} #{TEMP_FILE} #{flags} #{OUTPUT_FILE}")
rescue Exception => e
  puts "Calling CSS Tidy executable failed with exception: " + e
end


# clean up the unneeded temp file
begin
  File.delete(TEMP_FILE)
rescue Errno::ENOENT
  # no need to do anything here.  No file found to delete.
rescue Exception => e
  # However this is a real exception (like maybe permission denied) that we should look at.
  puts "Temp file deletion failed with exception: " + e
end
