# mmcurses

A small c++11 library to somewhat ease ncurses programming

# Documentation

The library is small enough to provide the documentation in the header files in src/mmcurses/ or ($PREFIX/include/mmcurses/ if installed).

For convenience there is also a Doxyfile. So you can do this:

<pre>
mkdir docs
doxygen
</pre>

to generate documentation for the library.

# Building and Installation

Change into the root of the source distribution and then perform these steps:

<pre>
mkdir bld
cd bld
cmake ..
make
make install
</pre>

NOTE: The last step might have to be performed as superuser.

# Examples

* mmcurses_application_example1: A simple application that prints the current window size, counts key presses and quits on key press q or Q.
* mmcurses_application_example2: An application that displays a very simple animation using the system's wallclock time.

# Author

Florian Paul Schmidt (mista.tapas@gmx.net)
