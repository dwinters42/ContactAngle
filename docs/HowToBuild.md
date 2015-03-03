How to build ContactAngle
=========================

Prerequisites
-------------

Apart form the source code, you will need

- a working C++ compiler (e.g. g++ in Unix, Visual C++ in Windows)
- the [OpenCV](http://opencv.org) library
- the [wxWidgets](http://www.wxwidgets.org) toolkit
- the [cmake](http://www.cmake.org) build system

On an e.g. Debian or Ubuntu, the following commands should install the
necessary packages

	  apt-get install build-essential cmake libcv-dev libwxgtk-dev

On Windows, you can get the libraries precompiled from their
corresponding websites and you can use e.g. the free Microsoft Visual
Studio Express.

Get the source code
-------------------

The latest version of the sourcecode can be downloaded either as
[zip](http://www.tydirium.org/cgi-bin/fossil/ContactAngle/zip/ContactAngle.zip?uuid=tip)
or as
[tar.gz](http://www.tydirium.org/cgi-bin/fossil/ContactAngle/zip/ContactAngle.tar.gz?uuid=tip)
archive.

Alternatively, the complete [fossil](http://www.fossil-scm.org) source
code repository can be cloned as following:

     mkdir ContactAngle
     cd ContactAngle
     fossil clone http://www.tydirium.org/cgi-bin/fossil/ContactAngle ContactAngle.fossil
     fossil open ContactAngle.fossil

Build on Linux and BSD
----------------------

In the source directory run the following:

   cmake .
   make
   sudo make install

Build on Windows
----------------

Open cmake-gui, set the source and build directory to the ContacAngle
source dir, press configure (for eg. Visual Studio), press generate.

Open the generated ContactAngle.sln in Visual Studio, build
solution. This will give you the binary in the Release. or Debug/
directory, depending on your build configuration.
