Bundler.require(:test)

require "aruba/cucumber"
require "capybara/cucumber"

require "./features/support/blueprint_app"

Capybara.app = BlueprintApp.new
Capybara.javascript_driver = :webkit
