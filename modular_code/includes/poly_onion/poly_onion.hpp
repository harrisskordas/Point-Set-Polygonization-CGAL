#ifndef poly_onion_hpp
#define poly_onion_hpp    

/*
This header file contains an implementation of the onion algorithm as discussed
in class. 

    onion description here

The files created can then be used on our Python visualisation app where we can see
the algorithm run step by step.

Whenever "vis - ..." is used the code described concerns visualisation.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir() (used to create visualisation folder)

class poly_onion {
    
    public:      
        /*
        Runs the onion algorithm on a set of points, with given initialization strategy. 

        poly_line and poly_area hold the calculated polygonal line and its area after
        completion of execution.

        Also takes visualisation options.
        */                               
        static void run(const Points& points, 
                        const int& initialization, 
                        Polygon_2& poly_line,
                        NUM& poly_area,
                        const bool& vis=false,
                        const bool& vis_min=false);

};

#endif
