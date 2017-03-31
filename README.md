# MandelbrotSet
A C++ program for looking at Mandelbrot and Julia set fractals.

![Mandelbrot Fractal](http://i.imgur.com/rZmSD6T.png)

The bulk of the code is in `src/main.cpp`. A few important variables to edit to alter the output are:

Variable | Description
---|---
`width`/`height` | Window width and height
`scaleFactor` | Initial zoom level of the display
`magnificationSpeed` | Amount to magnify by each time a fractal is finished drawing
`offsetX`/`offsetY` | Mandelbrot coordinates on which to zoom

## Controls
Press space to pause and escape to quit.

## Colors
Changing the colors is very ... well, it's not very fun. To make it a bit easier, it's recommended to utilize the HSL to RGB conversion. I really just toy around with it to get it to something I think looks okay; there's not a real method to my madness there.
