Program makes use of the Eigen C++ library.

To compile, type: 
make
To run, type:
./raytracer <driverName> <outputName>
where <driverName> is the name of a driver file, and <outputName> is the file you want the image to be written to


here are optional arguments to override some parameters without editing the driver file:
./raytracer <driverName> <outputName> <width> <height> <index of refraction (set all spheres, doesn't matter for the opaque ones)> 

./raytracer <driverName> <outputName> <width> <height>



./raytracer <driverName> <outputName> <recursion level> 

./raytracer <driverName> <outputName> <width> <height> <eye x> <eye y> <eye z> <look x> <look y> <look z>

./raytracer <driverName> <outputName> <eye x> <eye y> <eye z> <look x> <look y> <look z>

