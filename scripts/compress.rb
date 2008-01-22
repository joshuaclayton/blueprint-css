#!/usr/bin/env ruby
require 'blueprint.rb'
require 'compress/compressor.rb'

# **Basic
# 
#   Calling this file by itself will pull files from blueprint/src and concatenate them into three files; ie.css, print.css, and screen.css.
# 
#     ruby compress.rb
# 
#   However, variables can be set to change how this works.
# 
# **Custom Locations
# 
#   Passing LOCATION will save the files to a designated location; for example, calling
# 
#     LOCATION='/Users/me/Sites/application_name/public/stylesheets' ruby compress.rb
#   
#   will output the three CSS files to that specified directory (if permissions allow).
#   
# **Custom CSS Files
# 
#   If you pass a custom location, the generation code will look for three files:
#     
#     LOCATION/my-ie.css
#     LOCATION/my-screen.css
#     LOCATION/my-print.css
#     
#   If any of these exist, their contents will be compressed and appended to the end of the CSS generated from Blueprint
# 
# **Custom Namespace
# 
#   If you would like a custom namespace to be appended to all the classes, that too can be passed in to the call; for example, calling
#   
#     NAMESPACE='my-custom-namespace-' ruby compress.rb
#     
#   will prepend all Blueprint classes with 'my-custom-namespace-'
  

# instantiate new Compressor, passing in variables set in call
c = Compressor.new(:destination => ENV['LOCATION'], :namespace => ENV['NAMESPACE'])

# the meat-and-potatoes method that handles generation of all the files and saves them in the correct location
c.generate!