class Validator < Blueprint
  # class constants
  CSS_FILES_TO_TEST = ['screen.css', 'print.css', 'ie.css']
  VALIDATOR_PATH    = File.join(Blueprint::ROOT_PATH, 'scripts', 'validate', 'css-validator.jar')
  
  # instance variables
  attr_reader :error_count
    
  # constructor
  def initialize(options = {})
    @error_count = 0
  end

  # instance methods
  def validate
    raise "You do not have a Java installed, but it is required." if `which java`.blank?
    
    output_header
    
    Validator::CSS_FILES_TO_TEST.each do |file_name|
      css_output_path = File.join(Blueprint::BLUEPRINT_ROOT_PATH, file_name)
      puts "\n\n  Testing #{css_output_path}"
      puts "  Output ============================================================\n\n"
      @error_count += 1 if !system("java -jar '#{Validator::VALIDATOR_PATH}' -e '#{css_output_path}'")
    end
    
    output_footer
  end

  def output_header
    puts "\n\n"
    puts "  ************************************************************"
    puts "  **"
    puts "  **   Blueprint CSS Validator"
    puts "  **   Validates output CSS files"
    puts "  **"
    puts "  ************************************************************"
  end

  def output_footer
    puts "\n\n"
    puts "  ************************************************************"
    puts "  **"
    puts "  **   Done!"
    puts "  **   Your CSS files are#{" not" if error_count > 0} valid.#{"  You had #{error_count} error(s) within your files" if error_count > 0}"
    puts "  **"
    puts "  ************************************************************"
  end
end