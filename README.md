# BM@N Magnetic field visializer
This is a library for visualizing measured magnetic field.

## Installation
### Requirements:
* Installed ROOT of version 6.18 or higher
* CMake version 3.0 or higher

### Building from the sources
````
git clone https://github.com/mam-mih-val/bmn_magnetic_field
cd bmn_magnetic_field
mkdir build
cd build
source /path/to/your/thisroot.sh
cmake ..
make -j2
````

## Usage
### Built-in macro
There already are several macros for magnetic field visualization:
#### draw_2dgraph.cc
is capable of drawing 2-dimensinal pictures for each component as well 1-dimensional projections on X-axis. It dumps all of the results into root-file.
#### draw_2dgraph.cc
is used for visualization field vectors as arrows. The output is picture.
#### Any custom macro you wish
can be written and processed by the executable. You can implement your own analysis using the library classes. 

### Running the macro
To run the macro is really simple:
````
/path/to/your/build/directory/process any_macro.cc
````
For example to perform the visualization of the measured data:
````
/path/to/your/build/directory/process draw_2dgraph.cc
````
The output is the *.root file, which you can open with
````
root -l output.root
````