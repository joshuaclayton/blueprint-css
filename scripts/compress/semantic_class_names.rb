class SemanticClassNames < Blueprint
  # attributes
  attr_accessor :class_assignments
  attr_reader   :namespace
  
  # constructor
  def initialize(options = {})
    @namespace = options[:namespace] || ""
    self.class_assignments = options[:class_assignments] || {}
  end
  
  # instance methods
  def css_from_assignments(assignments = {})
    assignments ||= self.class_assignments
    
    # define a wrapper hash to hold all the new CSS assignments
    output_css = {}
    
    #loads full stylesheet into an array of hashes
    blueprint_assignments = CSSParser.new(:file_path => File.join(Blueprint::BLUEPRINT_ROOT_PATH, 'screen.css'), :namespace => namespace).parse
    
    # iterates through each class assignment ('footer' => 'span-# column last', 'header' => 'span-# column last')
    assignments.each do |semantic_class, blueprint_classes|
      # gathers all BP classes we're going to be mimicing
      blueprint_classes = blueprint_classes.split(/,|\s/).find_all {|c| !c.blank? }
      classes = []
      # loop through each BP class, grabbing the full hash (containing tags, index, and CSS rules)
      blueprint_classes.each do |bp_class|
        classes << blueprint_assignments.find_all {|line| line[:tags] =~ Regexp.new(/^(\w+, ?)*\.#{bp_class}(, \w+)?$/)}.uniq
      end
      # clean up the array
      classes = classes.flatten.uniq
      
      # set the semantic class to the rules gathered in classes, sorted by index
      # this way, the styles will be applied in the correct order from top of file to bottom
      output_css[".#{semantic_class}"] = "#{classes.sort_by {|i| i[:idx]}.map {|i| i[:rules]}}"
    end
    
    # return the css in proper format
    css = ""
    output_css.each do |tags, rules|
      css += "#{tags} {#{rules}}\n"
    end
    css
  end
  
end