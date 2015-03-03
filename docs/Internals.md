How does it work?
=================

The program takes a very simple approach like the following:

-    Read a frame from the video
-    Binary threshold (convert it to black and white)
-    Detect the shape of the droplet in an area close to where it touches 
     the surface
-    Fit a parabola for each side and take the contact angle as the slope 
     of the function at the point where it touches the surface.
-    Repeat for all frames in image sequence

This simple approach is robust and fast, but may not be as precise as
other algorithms that user a larger part of the image. However, it is
accurate enough for most purposes.
