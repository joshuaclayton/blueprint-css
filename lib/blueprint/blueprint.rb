require 'fileutils'

class Blueprint
  ROOT_PATH =             File.join(File.expand_path(File.dirname(__FILE__)), "../../")
  BLUEPRINT_ROOT_PATH =   File.join(Blueprint::ROOT_PATH, 'blueprint')
  SOURCE_PATH =           File.join(Blueprint::BLUEPRINT_ROOT_PATH, 'src')
  TEST_PATH =             File.join(Blueprint::ROOT_PATH, 'tests')
  LIB_PATH =              File.join(Blueprint::ROOT_PATH, 'lib', 'blueprint')
  PLUGINS_PATH =          File.join(Blueprint::BLUEPRINT_ROOT_PATH, 'plugins')
  SETTINGS_FILE =         File.join(Blueprint::ROOT_PATH, 'lib', 'settings.yml')
  VALIDATOR_FILE =        File.join(Blueprint::LIB_PATH, 'validate', 'css-validator.jar')
  CSS_FILES = {
    'screen.css'   => ['reset.css', 'typography.css', 'grid.css', 'forms.css'],
    'print.css'    => ['print.css'],
    'ie.css'       => ['ie.css']
  }
  
  # Default column layout
  # 24 columns * (30px + 10px) - 10px = 950px width
  COLUMN_COUNT =          24
  COLUMN_WIDTH =          30
  GUTTER_WIDTH =          10
end

Dir["#{File.join(Blueprint::LIB_PATH)}/*"].each do |file|
  require "#{file}" if file =~ /\.rb$/ && file !~ /blueprint.rb/
end