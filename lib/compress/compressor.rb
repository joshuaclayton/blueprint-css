class Compressor < Blueprint
  # class constants
  CSS_FILES = {
    'screen.css'   => ['reset.css', 'typography.css', 'grid.css', 'forms.css'],
    'print.css'    => ['print.css'],
    'ie.css'       => ['ie.css']
  }
  
  TEST_FILES = [
    'index.html', 
    'parts/elements.html', 
    'parts/forms.html', 
    'parts/grid.html', 
    'parts/sample.html'
  ]
  
  # properties
  attr_accessor :namespace, :destination_path
  attr_reader   :custom_path
  
  # constructor
  def initialize(options = {})
    @namespace =          options[:namespace]   || ""
    @destination_path =   options[:destination] || Blueprint::BLUEPRINT_ROOT_PATH
    @custom_path =        @destination_path != Blueprint::BLUEPRINT_ROOT_PATH

    # iterates through lib/compress folder
    Dir["#{File.join(Blueprint::ROOT_PATH, "lib", "compress")}/*"].each do |file|
      # require ruby files not including compressor.rb
      require "#{file}" if file =~ /.rb$/ && file !~ /^compressor/
    end
  end
  
  # instance methods
  def generate!
    output_header
    
    Compressor::CSS_FILES.each do |output_file_name, css_source_file_names|
      css_output_path = File.join(destination_path, output_file_name)
      puts "\n    Assembling to #{custom_path ? css_output_path : "default blueprint path"}"

      # CSS file generation
      css_output = css_file_header # header included on all three Blueprint-generated files
      
      # Iterate through src/ .css files and compile to individual core compressed file
      css_source_file_names.each do |css_source_file|
        puts "      + src/#{css_source_file}"
        css_output += "/* #{css_source_file} */\n" if css_source_file_names.any?
        css_output += CSSParser.new(:file_path => File.join(Blueprint::SOURCE_PATH, css_source_file), :namespace => namespace).to_s
        css_output += "\n"
      end

      # check to see if a custom (non-default) location was used for output files
      # if custom path is used, handle custom CSS, if any
      if custom_path
        overwrite_path = File.join(destination_path, "my-#{output_file_name}")
        overwrite_css = File.exists?(overwrite_path) ? File.path_to_string(overwrite_path) : ""
        
        # if there's CSS present, add it to the CSS output
        unless overwrite_css.blank?
          puts "      + custom styles\n"
          css_output += CSSParser.new(:css_string => overwrite_css, :namespace => namespace).to_s
        end
      end
      
      #save CSS to correct path
      File.string_to_file(css_output, css_output_path)
    end

    
    # TODO: have tests kick out to custom location
    generate_tests
    output_footer
  end

  private 

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

----------------------------------------------------------------------- */
    )
  end
end