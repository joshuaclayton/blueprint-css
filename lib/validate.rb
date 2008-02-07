#!/usr/bin/env ruby
require 'blueprint/blueprint'
require 'blueprint/validate/validator'

# This script will validate the core Blueprint files. 
# 
# The files are not completely valid. This has to do 
# with a small number of CSS hacks needed to ensure 
# consistent rendering across browsers.
#
# To add your own CSS files for validation, see
# /lib/blueprint/validate/validator.rb

v = Validator.new
v.validate