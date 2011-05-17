class BlueprintApp
  def self.current_response=(resp)
    @@current_response = resp
  end

  def self.current_response
    @@current_response || ""
  end

  def call(env)
    [200, { "Content-Type" => "text/html" }, self.class.current_response]
  end
end
