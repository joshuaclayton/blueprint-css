#!/usr/bin/env ruby
require 'blueprint'
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
#   To use custom settings, the file need to be stored in settings.yml within this directory.  An example YAML file has been included.
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
#         custom_layout:
#            column_count: 12
#            column_width: 70
#            gutter_width: 10
#       project2:
#         path: /path/to/different/stylesheets
#         namespace: different-namespace-
#         custom_css:
#           screen.css:
#             - custom_screen.css
#         semantic_classes:
#           "#footer, #header": column span-24 last
#           "#content": column span-18 border
#           "#extra-content": last span-6 column
#           "div#navigation": last span-24 column
#           "div.section, div.entry, .feeds": span-6 column
#       project3:
#         path: /path/to/another/projects/styles
# 
#     In a structure like this, a lot more assignment is occurring.  Custom namespaces are being set for two projects, while 
#     the third (project3) is just a simple path setting.
#     
#     Also, custom CSS is being used that is appended at the end of its respective file.  So, in project1, print.css will have docs.css 
#     and my-print-styles.css instead of the default my-print.css.  Note that these files are relative to the path that you defined above;
#     you can use subdirectories from the default path if you would like.
#     
#     Another thing to note here is the custom_layout; if not defined, your generated CSS will default to the 24 column, 950px wide grid that 
#     has been standard with Blueprint for quite some time.  However, you can specify a custom grid setup if you would like.  The three options 
#     are column_count (the number of columns you want your grid to have), column width (the width in pixels that you want your columns to be), and 
#     gutter_width (the width in pixels you want your gutters - space between columns - to be).  To use the Blueprint default, do not define this 
#     in your settings file.
#     
#     Semantic classes are still in the works within Blueprint, but a simple implementation has been created.
#     
#     Defining semantic_classes, with nodes underneath, will generate a class for each node which has rules of each class assigned to it.  For example,
#     in project2 above, for '#footer, #header', elements with id's of footer and header will be assigned all the rules from the 
#     classes 'span-24, column, and last', while divs with classes either entry or section, as well as any element with class of feed, is 
#     assigned all the rules from 'span-6 and column'.  Although it is a crude way do accomplish this, it keeps the generated CSS separate from the core BP CSS.
#     
#     In Ruby, the structure would look like this:
#     
#     {
#       'project1' => {
#         'path' => '/path/to/my/project/stylesheets',
#         'namespace' => 'custom-namespace-1-',
#         'custom_css' => {
#           'ie.css' => ['custom-ie.css'],
#           'print.css' => ['docs.css', 'my-print-styles.css'],
#           'screen.css' => ['subfolder-of-stylesheets/sub_css.css']
#         },
#         'custom_layout' => {
#           'column_count' => 12,
#           'column_width' => 70,
#           'gutter_width' => 10
#         }
#       },
#       'project2' => {
#         'path' => '/path/to/different/stylesheets',
#         'namespace' => 'different-namespace-',
#         'custom_css' => {
#           'screen.css' => ['custom_screen.css']
#         },
#         'semantic_classes' => {
#           '#footer, #header' => 'column span-24 last',
#           '#content' => 'column span-18 border',
#           '#extra-content' => 'last span-6 column',
#           'div#navigation' => 'last span-24 column',
#           'div.section, div.entry, .feeds' => 'span-6 column'
#         }
#       },
#       'project3' => {
#         'path' => '/path/to/another/projects/styles'
#       }
#     }

# instantiate new Compressor, passing in variables set in call
c = Compressor.new(:destination => ENV['LOCATION'], :namespace => ENV['NAMESPACE'], :project => ENV['PROJECT'])

# the meat-and-potatoes method that handles generation of all the files and saves them in the correct location
c.generate!