require "./lib/blueprint/blueprint"

module BlueprintHelpers
  def blueprint_command(options = nil)
    %{ruby #{compress_path} -o custom_path --custom_tests_path custom_tests_path #{options}}
  end

  def html_page_content
    %{
      <!DOCTYPE html>
      <html lang="en">
        <head>
          <style type="text/css">
            #{blueprint}
          </style>
          <title>Blueprint Test HTML</title>
          <meta charset="utf-8">
          <script type="text/javascript">
            #{jquery}
          </script>
        </head>
        <body>
          #{yield}
        </body>
      </html>
    }
  end

  private

  def compress_path
    File.join(Blueprint::ROOT_PATH, "lib", "compress.rb")
  end

  def jquery
    File.open("features/support/jquery.js", "r").read
  end

  def blueprint
    File.open("tmp/aruba/custom_path/screen.css", "r").read
  end
end

World(BlueprintHelpers)
