require 'erb'
class CustomLayout < Blueprint
  # class constants
  CSS_ERB_FILE = File.join(Blueprint::LIB_PATH, 'compress', 'grid.css.erb')
  
  # properties
  attr_writer :column_count, :column_width, :gutter_width
  
  def column_count
    (@column_count || Blueprint::COLUMN_COUNT).to_i
  end
  
  def column_width
    (@column_width || Blueprint::COLUMN_WIDTH).to_i
  end
  
  def gutter_width
    (@gutter_width || Blueprint::GUTTER_WIDTH).to_i
  end
  
  def page_width
    column_count * (column_width + gutter_width) - gutter_width
  end
  
  # constructor
  def initialize(options = {})
    self.column_count = options[:column_count]
    self.column_width = options[:column_width]
    self.gutter_width = options[:gutter_width]
  end
  
  # instance methods
  def generate_grid_css
    css = ERB::new(File.path_to_string(CustomLayout::CSS_ERB_FILE))
    css.result(binding)
  end
end