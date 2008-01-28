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
  
  def parse(data = nil)
    data ||= @raw_data
    
    # wrapper array holding hashes of css tags/rules
    css_out = []
    # clear initial spaces
    data.strip_side_space!.strip_space!
    
    # split on end of assignments
    data.split('}').each_with_index do |assignments, index|
      # split again to separate tags from rules
      tags, styles = assignments.split('{').map{|a| a.strip_side_space!}
      
      # clean up tags and apply namespaces as needed
      tags.strip_selector_space!
      tags.gsub!(/\./, ".#{namespace}") unless namespace.blank?
      
      # split on semicolon to iterate through each rule
      rules = []
      styles.split(';').each do |key_val_pair|
        unless key_val_pair.nil?
          # split by property/val and append to rules array with correct declaration
          property, value = key_val_pair.split(':').map{|kv| kv.strip_side_space!}
          break unless property && value
          rules << "#{property}:#{value};"
        end
      end
      # now keeps track of index as hashes don't keep track of position (which will be fixed in Ruby 1.9)
      css_out << {:tags => tags, :rules => rules.to_s, :idx => index} unless tags.blank? || rules.to_s.blank?
    end
    css_out
  end
  
  private
  
  def compress(data)
    @css_output = ""
    parse(data).flatten.sort_by {|i| i[:idx]}.each do |line|
      @css_output += "#{line[:tags]} {#{line[:rules]}}\n"
    end
  end
end