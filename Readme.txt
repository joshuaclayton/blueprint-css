Blueprint CSS framework 0.5 (http://bjorkoy.com/blueprint)
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
* Absolutely no bloat


Setup instructions
----------------------------------------------------------------

Here's how you set up Blueprint on your site. 

1) Upload BP to your server, and place it in whatever folder
   you'd like. A good choice would be your CSS folder.

2) Add the following lines to every <head> section of your
   site. Make sure the link path is correct (here, BP is in my CSS folder):

   <link rel="stylesheet" href="css/blueprint/screen.css" type="text/css" media="screen, projection">
   <link rel="stylesheet" href="css/blueprint/print.css" type="text/css" media="print">	

3) That's it! Blueprint is now ready to shine.


How to use Blueprint
----------------------------------------------------------------

Here's a quick primer on how to use BP:
http://code.google.com/p/blueprintcss/wiki/Tutorial

Each file is also heavily commented, so you'll 
learn a lot by reading through them.


Files in Blueprint
----------------------------------------------------------------

The framework has a few files you should check out. Every file
contains lots of (hopefully) clarifying comments.

* screen.css
  This is the main file of the framework. It imports other CSS 
  files from the "lib" directory, and should be included on 
  every page.

* print.css
  This file sets some default print rules, so that printed versions
  of your site looks better than they usually would. It should be
  included on every page.

* lib/grid.css
  This file sets up the grid (it's true). It has a lot of classes
  you apply to divs to set up any sort of column-based grid.

* lib/typography.css
  This file sets some default typography. It also has a few
  methods for some really fancy stuff to do with your text.

* lib/reset.css
  This file resets CSS values that browsers tend to set for you.

* lib/buttons.css
  Provides some great CSS-only buttons.

* lib/compressed.css
  A compressed version of the core files. Use this on every live site.
  See screen.css for instructions.


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
* Angus Turnbull            [twinhelix.com]
* Khoi Vinh                 [subtraction.com]

Questions, comments, suggestions or bug reports all go to
olav at bjorkoy dot com. Thanks for your interest!


== By Olav Bjorkoy
== http://bjorkoy.com
