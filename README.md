# Gradient-Lines-for-tft_espi

This is a simple demo of the gradient line code I wrote.  It is targeted to the tft_espi library.  It will write to a sprite or (I think) the base tft screen.  The sprite is named in the call.  It will work on any display because I used tft.width() and tft.height() for the sizing.

It draws two kinds of gradient lines.  One will go through the gradient from one color to the other no matter how long the line is.  The other will do the same gradient for however many lines you specify (normally, the size of the chart you are drawing).  

Please send bugs or enhancement requests to me.  This is the first pass on this code but it seems to work well.  It is in use in two of my projects.  It will only do orthogonal lines at present.  CoPilot promises that if I ask, he will tell me how to do diagonal lines, too.  I might test him on that soon.  

Enjoy,
Mikey the Midnight Coder
