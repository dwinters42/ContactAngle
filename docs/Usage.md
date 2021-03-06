How to use ContactAngle
=======================

Here is how to use the program in a nutshell:

Load a file
-----------

Press the "Load" button (or Ctrl-O) to open a movie file in a video
format support by OpenCV (many will work). A suitable [sample
file](../samples/test.avi) comes with the distribution.

Set the parameters
------------------

Next, you have to set a couple of parameters with the sliders below
the image.

First, adjust the threshold slider so that you get the best possible
shape with sharp edges of the droplet. For the sample movie, a value
of 110 is good. This parameter is usually not critical.

Second, adjust the baseline (the horizon): Move the "base left" slider
and see how the red edge of the droplet at the left side goes up an
down. To the right of the image there is a plot window in which you
see the edge points of the droplet in blue (corresponding to the red
line in the image). The thin red line in the plot window is the best
parabolic fit through the points. The goal is to adjust the slider so
that the point where the droplet touches the surface is the lowest
blue point in the plot window. The correct value for the sample is
about 263.

Repeat the same for the "Base right" slider and the right plot
window, a good value for the sample is 261.

Adjust the "NumofPoints" slider to change the number of points (as
shown in the plot windows) to get a good fit, meaning that the thin
red lines in the plot windows do not deviate from the blue edge
points. In general, the more points, the more precise the fit, but if
the line deviates from the points, you will get the wrong number. In
reality this is usually not critical and you do not need too many
points. For the sample, 80 points is fine.

Finally, by moving the "Frame number" slider, check that the
values you have selected are good for all frames in the
movie. The left and right contact angles and tangents are shown in
the image.

Run the analysis
----------------

Press the "Analyze movie" button and the program will go through
all the frames and write the corresponding left and right contact
angle to a comma-separated text file with the extension ".txt",
for the sample "sample.avi" this will be "sample.txt".
