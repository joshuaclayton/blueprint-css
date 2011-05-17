Feature: Generate default Blueprint stylesheets from the command line
  In order to use Blueprint on a project
  As a developer
  I should be able to generate Blueprint stylesheets from the command line

  Scenario: Prints out helpful information
    When I generate Blueprint stylesheets
    Then the output should contain "Grid Settings"
    And the output should contain "Column Count: 24"
    And the output should contain "Column Width: 30px"
    And the output should contain "Gutter Width: 10px"
    And the output should contain "Total Width : 950px"
    And the output should not contain "Loaded from settings.yml"
    And the output should not contain "Namespace:"

  Scenario: Prints out helpful information with custom settings
    Given a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          custom_layout:
            column_count: 12
            column_width: 50
            gutter_width: 20
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    Then the output should contain "Column Count: 12"
    And the output should contain "Column Width: 50px"
    And the output should contain "Gutter Width: 20px"
    And the output should contain "Total Width : 820px"
    And the output should contain "Loaded from settings.yml"

  @javascript
  Scenario: Generates the correct default styles
    When I generate Blueprint stylesheets
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <section class="standard">
          <div class="span-12"></div>
          <div class="span-12 last"></div>
        </section>
        <section class="overflowed">
          <div class="span-12"></div>
          <div class="span-12"></div>
        </section>
      </div>
      """
    Then the ".overflowed .span-12:last" should be below ".overflowed .span-12:first"
    And the ".overflowed .span-12:last" should have a right margin of 10px
    And the ".standard .span-12:first" should have a width of 470px
    And the ".standard .span-12:first" should have a right margin of 10px
    And the ".standard .span-12:last" should have a right margin of 0px

  @javascript
  Scenario: Generates the correct custom styles
    Given a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          custom_layout:
            column_count: 12
            column_width: 50
            gutter_width: 10
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <section class="standard">
          <div class="span-6"></div>
          <div class="span-6 last"></div>
        </section>
        <section class="overflowed">
          <div class="span-6"></div>
          <div class="span-6"></div>
        </section>
      </div>
      """
    Then the ".overflowed .span-6:last" should be below ".overflowed .span-6:first"
    And the ".overflowed .span-6:last" should have a right margin of 10px
    And the ".standard .span-6:first" should have a width of 350px
    And the ".standard .span-6:first" should have a right margin of 10px
    And the ".standard .span-6:last" should have a right margin of 0px

  @javascript
  Scenario: Generates the custom styles with a namespace
    Given a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          namespace: custom-namespace-
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <section class="non-namespaced">
          <div class="span-12"></div>
          <div class="span-12 last"></div>
        </section>
        <section class="namespaced">
          <div class="custom-namespace-span-12"></div>
          <div class="custom-namespace-span-12 custom-namespace-last"></div>
        </section>
      </div>
      """
    Then the ".non-namespaced .span-12:first" should have a width of 0px
    And the ".non-namespaced .span-12:last" should have a width of 0px
    And the ".namespaced .custom-namespace-span-12:first" should have a width of 470px
    And the ".namespaced .custom-namespace-span-12:first" should have a right margin of 10px
    And the ".namespaced .custom-namespace-span-12:last" should have a right margin of 0px

  @javascript
  Scenario: Includes plugins when generating custom styles
    Given a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          plugins:
            - link-icons
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <a href="mailto:person@example.com">Send an email</a>
      </div>
      """
    Then the ".container a:first" should have a background image matching "email.png"

  @javascript
  Scenario: Includes custom CSS when generating custom styles
    Given a file named "custom_path/my-custom-css.css" with:
      """
        .awesome { background-color: #000; }
      """
    And a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          custom_css:
            screen.css:
              - my-custom-css.css
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <div class="awesome"></div>
      </div>
      """
    Then the ".awesome:first" should have a background color of "rgb(0, 0, 0)"

  @javascript
  Scenario: Includes default-named custom CSS when generating custom styles
    Given a file named "custom_path/my-screen.css" with:
      """
        .awesome { background-color: #000; }
      """
    When I generate Blueprint stylesheets
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <div class="awesome"></div>
      </div>
      """
    Then the ".awesome:first" should have a background color of "rgb(0, 0, 0)"

  @javascript
  Scenario: Includes default-named custom CSS and additional custom CSS when generating custom styles
    Given a file named "custom_path/my-screen.css" with:
      """
        .awesome { background-color: #000; }
      """
    And a file named "custom_path/my-custom-css.css" with:
      """
        .lame { background-color: #fff; }
      """
    And a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          custom_css:
            screen.css:
              - my-custom-css.css
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <div class="awesome"></div>
        <div class="lame"></div>
      </div>
      """
    Then the ".awesome:first" should have a background color of "rgb(0, 0, 0)"
    And the ".lame:first" should have a background color of "rgb(255, 255, 255)"

  @javascript
  Scenario: Generate semantic selectors
    Given a file named "custom_settings.yml" with:
      """
        my_awesome_project:
          semantic_classes:
            "#footer, #header": ".span-24"
            ".primary-content": ".span-18"
            ".secondary-content": ".span-6 .last"
      """
    When I generate Blueprint stylesheets with the options "-s custom_settings.yml -p my_awesome_project"
    And I open the HTML page using the styles with the content:
      """
      <div class="container">
        <div id="header"></div>
        <div class="primary-content"></div>
        <div class="secondary-content"></div>
        <div id="footer"></div>
      </div>
      """
    Then the "#header" should have a width of 950px
    And the "#footer" should have a width of 950px
    And the ".primary-content:first" should have a width of 710px
    And the ".secondary-content:first" should have a width of 230px
    And the ".primary-content:first" should be next to ".secondary-content:first"
