require 'core_ext'
require 'fileutils'

class Blueprint
  ROOT_PATH =             File.dirname(File.expand_path(File.dirname(__FILE__)))
  BLUEPRINT_ROOT_PATH =   File.join(Blueprint::ROOT_PATH, 'blueprint')
  SOURCE_PATH =           File.join(Blueprint::BLUEPRINT_ROOT_PATH, 'src')
  TEST_PATH =             File.join(Blueprint::ROOT_PATH, 'tests')
  LIB_PATH =              File.join(Blueprint::ROOT_PATH, 'lib')
  SETTINGS_FILE =         File.join(Blueprint::LIB_PATH, 'settings.yml')
  
  # Default column layout
  # 24 columns * (30px + 10px) - 10px = 950px width
  COLUMN_COUNT =          24
  COLUMN_WIDTH =          30
  GUTTER_WIDTH =          10
end