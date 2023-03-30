#ifndef poly_onion_hpp
#define poly_onion_hpp    

/*
This header file contains an implementation of the onion algorithm as discussed
in class. 

    - Find CH and inner points
    - Find CH of inner points and new inner points
    - Continue until no points are left
    - Connect CHs moving inwards
    
    Two visualisation options are provided:
        - full: save all CHs and new poly line at each step
        - minimal: save only point set and final poly line.

The files created can then be used on our Python visualisation app where we can see
the algorithm run step by step.

Whenever "vis - ..." is used the code described concerns visualisation.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir() (used to create visualisation folder)
#include <CGAL/convex_hull_2.h>           // CGAL::convex_hull_2

class poly_onion {

    private:
        /*
        Algorithm parameters below.
        */
        static int initialization; // 1,2,3,4,5 -> random, sort x asc, sort x desc, sort y asc, sort y desc
        static bool vis;           // visualisation
        static bool vis_min;       // minimal visualisation, points and final poly line

        /*
        If full visualisation is picked, at each step we save information on the files below.
        vis_counter counts the step number and is increased on each step.
        */
        static int vis_counter;             // counts step
        static std::string vis_points;      // saves points
        static std::string vis_ch;          // saves layered convex hulls of onion
        static std::string vis_poly_line;   // saves poly line
        static std::string vis_bridge;      // saves points on convex hull, visible from next point

        /*
        Given an edge and a set of points, find the point closest to the edge and its index.
        */
        static int point_closest_to_edge(const Segment_2& e,const Polygon_2& polygon);
        
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
