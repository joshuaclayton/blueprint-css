require 'core_ext'

class Blueprint
  ROOT_PATH =             "#{File.dirname(File.expand_path(File.dirname(__FILE__)))}"
  BLUEPRINT_ROOT_PATH =   File.join(Blueprint::ROOT_PATH, 'blueprint')
  SOURCE_PATH =           File.join(Blueprint::BLUEPRINT_ROOT_PATH, 'src')
  TEST_PATH =             File.join(Blueprint::ROOT_PATH, 'tests')  
end