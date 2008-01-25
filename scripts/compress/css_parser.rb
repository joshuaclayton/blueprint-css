class CSSParser < Blueprint
  # attributes
  attr_accessor :namespace
  attr_reader   :css_output, :raw_data
  
  # constructor
  def initialize(options = {})
    @raw_data     = options[:file_path] ? File.path_to_string(options[:file_path]) : options[:css_string] || ""
    @namespace    = options[:namespace] || ""
    compress(@raw_data)
  end
  
  # instance methods
  def to_s
    @css_output
  end
  
  private
  
  def compress(data)
    @css_output = ""
    data.strip_side_space!.strip_space!
    
    data.split('}').each do |assignments|
      tags, styles = assignments.split('{').map{|a| a.strip_side_space!}

      tags.strip_selector_space!
      tags.gsub!(/\./, ".#{namespace}") unless namespace.blank?
      
      rules = []
      styles.split(';').each do |key_val_pair|
        unless key_val_pair.nil?
          property, value = key_val_pair.split(':').map{|kv| kv.strip_side_space!}
          break unless property && value
          rules << "#{property}:#{value};"
        end
      end
      
      @css_output += "#{tags} {#{rules}}\n"
    end
  end
end