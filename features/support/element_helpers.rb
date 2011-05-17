module ElementHelpers
  def element(selector)
    DocumentElement.new(page, selector)
  end
end

World(ElementHelpers)
