Blueprint CSS Framework Readme

* URL:    code.google.com/p/blueprintcss
* List:   groups.google.com/group/blueprintcss
* News:   bjorkoy.com

----------------------------------------------------------------

Welcome to Blueprint! This is a CSS framework designed to 
cut down on your CSS development time. It gives you a solid 
foundation to build your own CSS on. Here are some of the 
features BP provides out-of-the-box:

* An easily customizable grid
* Sensible default typography
* A typographic baseline
* Perfected browser CSS reset
* A stylesheet for printing
* Absolutely no bloat!


Setup instructions
----------------------------------------------------------------

Here's how you set up Blueprint on your site. 

1) Upload the "blueprint" folder in this folder to your server, 
   and place it in whatever folder you'd like. A good choice 
   would be your CSS folder.

2) Add the following three lines to every <head> section of your
   site. Make sure the three href paths are correct (here, BP is in my CSS folder):

   <link rel="stylesheet" href="css/blueprint/screen.css" type="text/css" media="screen, projection">
   <link rel="stylesheet" href="css/blueprint/print.css" type="text/css" media="print">	
   <!--[if IE]><link rel="stylesheet" href="css/blueprint/ie.css" type="text/css" media="screen, projection"><![endif]-->

3) For development, add the .showgrid class to any container or column
   to see the underlying grid. Check out the 'plugins' directory for
   more advanced functionality.
   

How to use Blueprint
----------------------------------------------------------------

Here's a quick primer on how to use BP:
http://code.google.com/p/blueprintcss/wiki/Tutorial

Each CSS file is also heavily commented, so you'll 
learn a lot by reading through them.


Files in Blueprint
----------------------------------------------------------------

The framework has a few files you should check out. Every file
in the 'src' directory contains lots of (hopefully) clarifying 
comments.


Compressed files (these go in the HTML):

* blueprint/screen.css
* blueprint/print.css
* blueprint/ie.css


Source files:

* blueprint/src/reset.css
  This file resets CSS values that browsers tend to set for you.

* blueprint/src/grid.css
  This file sets up the grid (it's true). It has a lot of classes
  you apply to divs to set up any sort of column-based grid.

* blueprint/src/typography.css
  This file sets some default typography. It also has a few
  methods for some really fancy stuff to do with your text.

* blueprint/src/forms.css
	Includes some minimal styling of forms.

* blueprint/src/print.css
  This file sets some default print rules, so that printed versions
  of your site looks better than they usually would. It should be 
  included on every page.

* blueprint/src/ie.css
  Includes every hack for our beloved IE6 and 7.


Other:

* blueprint/plugins/
  Contains additional functionality in the form of simple plugins
  for Blueprint. See individual readme files in the directory
  of each plugin for further instructions.

* lib/
  BP comes with two scripts to run: 
  * One for validating the CSS in the core files (validate.rb).
  * One for re-compressing the main CSS files from the 
    source files, if you've made changes to the core (compress.rb).

* tests/
  Contains html files which tests most aspects of Blueprint.
  Open tests/index.html for further instructions.


Credits
----------------------------------------------------------------

Many parts of BP are directly inspired by other peoples work. 
You may thank them for their brilliance. However, *do not* ask 
them for support or any kind of help with BP.

* Jeff Croft                [jeffcroft.com]
* Nathan Borror             [playgroundblues.com]
* Christian Metts           [mintchaos.com]
* Wilson Miner              [wilsonminer.com]
* The Typogrify Project     [code.google.com/p/typogrify]
* Eric Meyer                [meyerweb.com/eric]
* Khoi Vinh                 [subtraction.com]

Questions, comments, suggestions or bug reports go to
olav at bjorkoy dot com. 

Thanks for your interest!

