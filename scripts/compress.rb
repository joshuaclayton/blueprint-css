#!/usr/bin/env ruby
require 'blueprint.rb'
require 'compress/compressor'

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
# 
# **Custom Settings
#   
#   To use custom settings, the file should be stored in settings.yml within this directory.
#   
#   Another ability is to use YAML (spec is at http://www.yaml.org/spec/1.1/) for project settings in a predefined structure and 
#   store all pertinent information there.  The YAML file has multiple keys (usually a named project) with a set of data that defines 
#   that project.  A sample structure can be found in settings.example.yml.
#   
#   The basic structure is this:
#   
#     Root nodes are project names.  You use these when calling compress.rb as such:
#     
#       PROJECT=projectname ruby compress.rb
#       
#     A sample YAML with only roots and output paths would look like this:
#     
#       project1:
#         path: /path/to/my/project/stylesheets
#       project2:
#         path: /path/to/different/stylesheets
#       project3:
#         path: /path/to/another/projects/styles
#       
#     You can then call
#     
#       PROJECT=project1 ruby compress.rb
#       
#       PROJECT=project3 ruby compress.rb
#     
#     This would compress and export Blueprints CSS to the respective directory, checking for my-(ie|print|screen).css and 
#     appending it if present
#     
#   A more advanced structure would look like this:
#   
#       project1:
#         path: /path/to/my/project/stylesheets
#         namespace: custom-namespace-1-
#         custom_css:
#           ie.css:
#             - custom-ie.css
#           print.css:
#             - docs.css
#             - my-print-styles.css
#           screen.css:
#             - subfolder-of-stylesheets/sub_css.css
#       project2:
#         path: /path/to/different/stylesheets
#         namespace: different-namespace-
#         custom_css:
#           screen.css
#             - custom_screen.css
#       project3:
#         path: /path/to/another/projects/styles
# 
#     In a structure like this, a lot more assignment is occurring.  Custom namespaces are being set for two projects, while 
#     the third (project3) is just a simple path setting.
#     
#     In Ruby, the structure would look like this:
#     
#       {
#         'project1' => {
#           'path' => '/path/to/my/project/stylesheets',
#           'namespace' => 'custom-namespace-1-',
#           'custom_css' => {
#             'ie.css' => ['custom-ie.css'],
#             'print.css' => ['docs.css', 'my-print-styles.css'],
#             'screen.css' => ['subfolder-of-stylesheets/sub_css.css']
#           }
#         },
#         'project2' => {
#           'path' => '/path/to/different/stylesheets',
#           'namespace' => 'different-namespace-',
#           'custom_css' => {
#             'screen.css' => ['custom_screen.css']
#           }
#         },
#         'project3' => {
#           'path' => '/path/to/another/projects/styles'
#         }
#       }

# instantiate new Compressor, passing in variables set in call
c = Compressor.new(:destination => ENV['LOCATION'], :namespace => ENV['NAMESPACE'], :project => ENV['PROJECT'])

# the meat-and-potatoes method that handles generation of all the files and saves them in the correct location
c.generate!