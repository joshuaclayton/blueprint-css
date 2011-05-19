module Blueprint
  class GridBuilder
    attr_reader :column_width, :gutter_width, :output_path, :able_to_generate

    # ==== Options
    # * <tt>options</tt>
    #   * <tt>:column_width</tt> -- Width (in pixels) of current grid column
    #   * <tt>:gutter_width</tt> -- Width (in pixels) of current grid gutter
    #   * <tt>:output_path</tt> -- Output path of grid.png file
    def initialize(options={})
      @column_width = options[:column_width] || Blueprint::COLUMN_WIDTH
      @gutter_width = options[:gutter_width] || Blueprint::GUTTER_WIDTH
      @output_path  = options[:output_path]  || Blueprint::SOURCE_PATH
    end

    # generates (overwriting if necessary) grid.png image to be tiled in background
    def generate!
      total_width = self.column_width + self.gutter_width
      height      = 18

      white      = ChunkyPNG::Color.from_hex("ffffff")
      background = ChunkyPNG::Color.from_hex("e8effb")
      line       = ChunkyPNG::Color.from_hex("e9e9e9")

      png = ChunkyPNG::Image.new(total_width, height, white)
      png.rect(0, 0, column_width - 1, height, background, background)
      png.rect(0, height - 1, total_width, height - 1, line, line)

      FileUtils.mkdir(self.output_path) unless File.exists?(self.output_path)
      png.save(File.join(self.output_path, "grid.png"))
    end
  end
end
