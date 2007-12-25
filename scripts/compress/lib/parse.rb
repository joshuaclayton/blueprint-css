class Parse
  
  # parses css file to string
  def path_to_string(path)
    css = ''
    
    # read the css file and remove unwanted whitespace
    data = File.new(path).read
    data = strip_sidespace(data) # remove space on sides
    
    data.gsub!(': ', ':') # remove unwanted spaces
    data.gsub!(/\n/, '') # remove newlines
    data.gsub!(/(\s\s)/, ' ') # remove multiple spaces
    data.gsub!(/(\/\*).*?(\*\/)/, '') # remove comments
    
    # find selectors and properties
    data.split('}').each do |var|
      parts = var.split('{') 
      parts.map! { |p| p = strip_sidespace(p) }
      selector = parts[0]
      
      # find all properties for current selectors
      rules = ''
      parts[1].split(';').each do |str|
        split = str.split(':')
        break unless split[0] && split[1]
        
        split.map! { |s| s = strip_sidespace(s) }
        rules += split[0] + ':' + split[1] + '; '
      end
      
      # remove bogus selector whitespace
      selector.gsub!(/(\n)/, '')
      selector.gsub!(',', ', ')
      selector.gsub!(',  ', ', ')
      
      # add properly compressed css to the string
      css += selector + ' { ' + rules + '}' + "\n"
    end
    css
  end
  
  # strips all whitespace on both sides of a string
  def strip_sidespace(str)
    str.gsub!(/^\s+/, "")
    str.gsub!(/\s+$/, $/)
    str
  end
end
