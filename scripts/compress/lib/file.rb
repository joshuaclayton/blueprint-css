# add a few useful methods to the default File class
class File
  
  # reads a file at path into a string
  def self.path_to_string(path)
    File.new(path).read    
  end  
  
  # writes a string to file at given path
  def self.string_to_file(path, string)
    File.open(path, 'w') do |f|
      f << string
    end
  end  

end  