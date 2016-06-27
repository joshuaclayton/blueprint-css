@javascript
Feature: Sensible typography
  In order to develop sites that are easy to read
  As a developer
  I should be able to use Blueprint for good typography

  Background:
    When I generate Blueprint stylesheets

  Scenario: Links are underlined
    When I open the HTML page using the styles with the content:
      """
        <a href="http://www.example.com">Hello world</a>
      """
    Then the "a:first" should be underlined

  Scenario: Hiding elements is straightforward
    When I open the HTML page using the styles with the content:
      """
      <div class="container">
        <div class="awesome">Text</div>
        <div class="hide">
          <div>More hidden stuff</div>
        </div>
      </div>
      """
    Then the ".container .hide:first" should be hidden
    And the ".container .hide > div:first" should be hidden
    And the ".container div:first" should be visible

  Scenario: Tables are striped
    When I open the HTML page using the styles with the content:
      """
      <div class="container">
        <table>
          <tr><td>Great</td></tr>
          <tr><td>Great</td></tr>
          <tr><td>Great</td></tr>
          <tr><td>Great</td></tr>
        </table>
      </div>
      """
    Then the ".container table tr:eq(0) td" should have a background color of "transparent"
    And the ".container table tr:eq(1) td" should have a background color of "rgb(229, 236, 249)"
    And the ".container table tr:eq(2) td" should have a background color of "transparent"
    And the ".container table tr:eq(3) td" should have a background color of "rgb(229, 236, 249)"

  Scenario: Tables look good
    When I open the HTML page using the styles with the content:
      """
      <div class="container">
        <table>
          <thead>
            <tr><th>Great</th></tr>
          </thead>
          <tfoot>
            <tr><td>Great</td></tr>
          </tfoot>
          <tbody>
            <tr><td>Great</td></tr>
            <tr><td>Great</td></tr>
            <tr><td>Great</td></tr>
          </tbody>
        </table>
      </div>
      """
    Then the ".container table" should have a width of 950px
    And the ".container table thead th" should be bold
    And the ".container table tfoot td" should be italic

  Scenario: Highlight text
    When I open the HTML page using the styles with the content:
      """
      <div class="container">
        <a href="http://www.example.com" class="highlight">Text</a>
      </div>
      """
    Then the ".container a" should have a background color of "rgb(255, 255, 0)"
