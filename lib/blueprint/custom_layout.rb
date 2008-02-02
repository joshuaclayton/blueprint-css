require 'erb'

class CustomLayout < Blueprint
  # class constants
  CSS_ERB_FILE = File.join(Blueprint::LIB_PATH, 'grid.css.erb') unless const_defined?("CSS_ERB_FILE")
  
  # properties
  # widths/column count default to core BP settings
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
    @column_count = options[:column_count]
    @column_width = options[:column_width]
    @gutter_width = options[:gutter_width]
  end
  
  # instance methods
  
  def default?
    self.column_width == Blueprint::COLUMN_WIDTH && self.column_count == Blueprint::COLUMN_COUNT && self.gutter_width == Blueprint::GUTTER_WIDTH
  end
  
  def generate_grid_css
    # loads up erb template to evaluate custom widths
    css = ERB::new(File.path_to_string(CustomLayout::CSS_ERB_FILE))
    
    # bind it to this instance
    css.result(binding)
  end
end