#
# CSS Parser
# Parses the css file at the given path 
# into a string of compressed css.
# 
# string = Parse.new(path).to_s
# 

class ParseCSS
  
  # read the file and start compressing
  def initialize(path)
    data = path_to_string(path) # read the css file 
    @css = compress(data)
  end
  
  # compress a string of css
  def compress(data)
    css  = ""
    data = strip_sidespace(data) # remove unwanted sidespace
    data = remove_css_space(data) # remove other whitespace
    
    # find selectors and properties
    data.split('}').each do |var|
      parts = var.split('{')
      parts.map! { |p| p = strip_sidespace(p) }
      selector = parts[0]
      
      # find all properties for current selectors
      rules = ""
      parts[1].split(';').each do |str|
        split = str.split(':') # seperate properties and values
        break unless split[0] && split[1] # something's missing
        
        split.map! { |s| s = strip_sidespace(s) } # strip sidespace
        rules += split[0] + ':' + split[1] + '; ' # add rule to list
      end
      
      # remove bogus selector whitespace
      selector = remove_css_selector_space(selector)
      
      # add properly compressed css to the string
      css += selector + ' { ' + rules + '}' + "\n"
    end
    css
  end
  
  # return the css when class 
  # instance gets turned into string
  def to_s
    @css
  end
  
  # reads a file at path into a string
  def path_to_string(path)
    File.new(path).read    
  end
  
  # remove unwanted space and comments
  # keeps space inside properties
  def remove_css_space(data)
    data.gsub!(': ', ':') # remove unwanted property spaces
    data.gsub!(/\n/, '') # remove newlines
    data.gsub!(/(\s\s)/, ' ') # remove multiple spaces
    data.gsub!(/(\/\*).*?(\*\/)/, '') # remove comments
    data
  end
  
  # remove unwanted whitespace in selector
  def remove_css_selector_space(selector)
    selector.gsub!(/(\n)/, '')
    selector.gsub!(',', ', ')
    selector.gsub!(',  ', ', ')
    selector
  end
  
  # strip all whitespace on both sides of a string
  def strip_sidespace(data)
    data.gsub!(/^\s+/, "")
    data.gsub!(/\s+$/, $/)
    data
  end
end
