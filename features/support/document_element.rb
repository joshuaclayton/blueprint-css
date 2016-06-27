class DocumentElement
  class Dimension  < Struct.new(:top, :right, :bottom, :left);       end
  class Background < Struct.new(:color, :image, :repeat, :position); end
  class Text       < Struct.new(:decoration);                        end
  class Font       < Struct.new(:weight, :style);                    end

  def initialize(page, selector)
    @page     = page
    @selector = selector
  end

  def visible?
    @page.evaluate_script(%{$("#{@selector}").is(":visible")})
  end

  def height
    @page.evaluate_script(%{$("#{@selector}").height()}).to_i
  end

  def width
    @page.evaluate_script(%{$("#{@selector}").width()}).to_i
  end

  def top
    @page.evaluate_script(%{$("#{@selector}").offset().top}).to_i
  end

  def bottom
    top + height
  end

  def margin
    margins = Dimension.new
    margins.top    = @page.evaluate_script(%{$("#{@selector}").css("marginTop")}).to_i
    margins.right  = @page.evaluate_script(%{$("#{@selector}").css("marginRight")}).to_i
    margins.bottom = @page.evaluate_script(%{$("#{@selector}").css("marginBottom")}).to_i
    margins.left   = @page.evaluate_script(%{$("#{@selector}").css("marginLeft")}).to_i

    margins
  end

  def background
    background = Background.new
    background.color    = @page.evaluate_script(%{$("#{@selector}").css("backgroundColor")})
    background.image    = @page.evaluate_script(%{$("#{@selector}").css("backgroundImage")})
    background.repeat   = @page.evaluate_script(%{$("#{@selector}").css("backgroundRepeat")})
    background.position = @page.evaluate_script(%{$("#{@selector}").css("backgroundPosition")})
    background
  end

  def font
    font = Font.new
    font.weight = @page.evaluate_script(%{$("#{@selector}").css("fontWeight")})
    font.style  = @page.evaluate_script(%{$("#{@selector}").css("fontStyle")})
    font
  end

  def text
    text = Text.new
    text.decoration = @page.evaluate_script(%{$("#{@selector}").css("textDecoration")})
    text
  end
end
