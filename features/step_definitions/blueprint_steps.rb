When /^I generate Blueprint stylesheets$/ do
  When %{I run `#{blueprint_command}`}
end

When /^I generate Blueprint stylesheets with the options "([^"]*)"$/ do |options|
  When %{I run `#{blueprint_command(options)}`}
end

When /^I open the HTML page using the styles with the content:$/ do |string|
  BlueprintApp.current_response = html_page_content { string }
  visit("/")
end

Then /^the "([^"]*)" should be below "([^"]*)"$/ do |selector_1, selector_2|
  element(selector_1).height.should >= element(selector_2).top
end

Then /^the "([^"]*)" should be next to "([^"]*)"$/ do |selector_1, selector_2|
  element(selector_1).top.should >= element(selector_2).top &&
    element(selector_1).top.should <= element(selector_2).bottom
end

Then /^the "([^"]*)" should have a width of (\d+)px$/ do |selector, width|
  element(selector).width.should == width.to_i
end

Then /^the "([^"]*)" should have a right margin of (\d+)px$/ do |selector, margin|
  element(selector).margin.right.should == margin.to_i
end

Then /^the "([^"]*)" should have a background image matching "([^"]*)"$/ do |selector, image|
  element(selector).background.image.should =~ %r{#{image}}
end

Then /^the "([^"]*)" should have a background color of "([^"]*)"$/ do |selector, rgb|
  element(selector).background.color.should == rgb
end
