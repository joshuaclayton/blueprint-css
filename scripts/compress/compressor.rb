require 'yaml'
class Compressor < Blueprint
  # class constants
    CSS_FILES = {
      'screen.css'   => ['reset.css', 'typography.css', 'grid.css', 'forms.css'],
      'print.css'    => ['print.css'],
      'ie.css'       => ['ie.css']
    } unless const_defined?("CSS_FILES")
  
  TEST_FILES = [
    'index.html', 
    'parts/elements.html', 
    'parts/forms.html', 
    'parts/grid.html', 
    'parts/sample.html'
  ] unless const_defined?("TEST_FILES")
  
  # properties
  attr_accessor :namespace, :custom_css, :custom_layout, :semantic_classes
  attr_reader   :custom_path, :loaded_from_settings, :destination_path
  
  def destination_path=(path)
    @destination_path = path
    @custom_path = @destination_path != Blueprint::BLUEPRINT_ROOT_PATH
  end
  
  # constructor
  def initialize(options = {})
    process_required_files
    
    @loaded_from_settings = false
    self.custom_css = {}
    self.semantic_classes = {}
    
    initialize_project_from_yaml(options[:project])

    self.namespace =          options[:namespace]   ? options[:namespace]   : self.namespace || ""
    self.destination_path =   options[:destination] ? options[:destination] : self.destination_path || Blueprint::BLUEPRINT_ROOT_PATH
  end
  
  # instance methods
  def generate!
    output_header       # information to the user (in the console) describing custom settings
    generate_css_files  # loops through Compressor::CSS_FILES to generate output CSS
    generate_tests      # updates HTML with custom namespaces in order to test the generated library.  TODO: have tests kick out to custom location
    output_footer       # informs the user that the CSS generation process is complete
  end

  private 
  
  def process_required_files
    # iterates through lib/compress folder and requires ruby files not including compressor.rb
    Dir["#{File.join(Blueprint::LIB_PATH, "compress")}/*"].each do |file|
      require "#{file}" if file =~ /\.rb$/ && file !~ /^compressor/
    end
  end
  
  # attempts to load output settings from settings.yml
  def initialize_project_from_yaml(project_name = nil)
    # ensures project_name is set and settings.yml is present
    return unless (project_name && File.exist?(Blueprint::SETTINGS_FILE))
    
    # loads yaml into hash
    projects = YAML::load(File.path_to_string(Blueprint::SETTINGS_FILE))
    
    if (project = projects[project_name]) # checks to see if project info is present
      self.namespace =        project['namespace']        || ""
      self.destination_path = project['path']             || Blueprint::BLUEPRINT_ROOT_PATH
      self.custom_css =       project['custom_css']       || {}
      self.semantic_classes = project['semantic_classes'] || {}
      if (layout = project['custom_layout'])
        self.custom_layout = CustomLayout.new(:column_count => layout['column_count'], :column_width => layout['column_width'], :gutter_width => layout['gutter_width'])
      end
      @loaded_from_settings = true
    end
  end
  
  def generate_css_files
    Compressor::CSS_FILES.each do |output_file_name, css_source_file_names|
      css_output_path = File.join(destination_path, output_file_name)
      puts "\n    Assembling to #{custom_path ? css_output_path : "default blueprint path"}"

      # CSS file generation
      css_output = css_file_header # header included on all three Blueprint-generated files
      css_output += "\n\n"
      
      # Iterate through src/ .css files and compile to individual core compressed file
      css_source_file_names.each_with_index do |css_source_file, index|
        puts "      + src/#{css_source_file}"
        css_output += "/* #{css_source_file} */\n" if css_source_file_names.any?
        
        css_output += if self.custom_layout && css_source_file == 'grid.css'
          CSSParser.new(:css_string => self.custom_layout.generate_grid_css, :namespace => namespace).to_s
        else
          CSSParser.new(:file_path => File.join(Blueprint::SOURCE_PATH, css_source_file), :namespace => namespace).to_s
        end
        
        css_output += "\n"
      end
      
      css_output = append_custom_css(css_output, output_file_name)
      
      # append semantic class names if set
      css_output += SemanticClassNames.new(:namespace => self.namespace).css_from_assignments(semantic_classes) if output_file_name == 'screen.css'
      
      #save CSS to correct path, stripping out any extra whitespace at the end of the file
      File.string_to_file(css_output.rstrip, css_output_path)
    end
  end
  
  def append_custom_css(css, current_file_name)
    # check to see if a custom (non-default) location was used for output files
    # if custom path is used, handle custom CSS, if any
    if custom_path
      overwrite_path = File.join(destination_path, (custom_css[current_file_name] || "my-#{current_file_name}"))
      overwrite_css = File.exists?(overwrite_path) ? File.path_to_string(overwrite_path) : ""
      
      # if there's CSS present, add it to the CSS output
      unless overwrite_css.blank?
        puts "      + custom styles\n"
        css += CSSParser.new(:css_string => overwrite_css).to_s
      end
    end
    css
  end
  
  def generate_tests
    puts "\n    Updating namespace to \"#{namespace}\" in test files:"
    test_files = Compressor::TEST_FILES.map {|f| File.join(Blueprint::TEST_PATH, f)}
    
    test_files.each do |file|
      puts "      + #{file}"
      Namespace.new(file, namespace)
    end
  end

  def output_header
    puts "\n\n"
    puts "  ************************************************************"
    puts "  **"
    puts "  **   Blueprint CSS Compressor"
    puts "  **   Builds compressed files from the source directory."
    puts "  **   Loaded from settings.yml" if loaded_from_settings
    puts "  **   Namespace: '#{namespace}'" unless namespace.blank?
    puts "  **   Output to: #{destination_path}"
    puts "  **"
    puts "  ************************************************************"
  end

  def output_footer
    puts "\n\n"
    puts "  ************************************************************"
    puts "  **"
    puts "  **   Done!"
    puts "  **   Your compressed files and test files are now up-to-date."
    puts "  **"
    puts "  ************************************************************"
  end
  
  def css_file_header
    %(/* -----------------------------------------------------------------------

   Blueprint CSS Framework 0.7 (Date TBD) 
   http://blueprintcss.googlecode.com

   * Copyright (c) Olav Bjorkoy 2007 - 2008. See docs/license.txt for more info.
   * See docs/readme.txt for instructions on how to use Blueprint.
   * This is a compressed file. See the sources in the 'src' directory.

----------------------------------------------------------------------- */)
  end
end