Caustics
John F Novak- <firstname>.franc.<lastname> <at> gmail . com
January 12, 2012

What this is:
This code is for generating random caustics by first generating a random (liquid) surface, then calculating the refraction of planar monotomic light as it passes through that surface.

Currently this code requires ROOT (available at http://root.cern.ch/drupal/) to run because it uses it for generating the output. It is also written in C++, I use the GNU GCC compiler.

One problem the code currently has is that it does not correctly map the light intensities when generating the image of the "bottom". I may or may not ever fix that, this project was just for fun.

I've considered rewriting this using gnuplot, and/or python. Root can be very painful to work with, and I prefer python to C++, but this project was just for fun.

This code was actually pounded out over a weekend in July of 2011, I am only fidling with it again right now to learn how to create git repositories.
