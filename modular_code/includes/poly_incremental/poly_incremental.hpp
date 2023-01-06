#ifndef poly_incremental_hpp
#define poly_incremental_hpp

/*
This header file contains an implementation of the incremental algorithm as discussed
in class. 

    At each step we add a point, calculate the ch_bb (beneath beyond convex hull),
    find the red edges from the the previous ch_bb and poly line, pick an edge based on 
    an edge selection strategy and the point to the poly line based on the chosen edge.

    Two visualisation options are provided:
        - full: at each step we save the points of the ch, the visible points on the ch,
        the points on the poly line and the visible points on the poly line in separate files
        - minimal: save only point set and final poly line.

The files created can then be used on our Python visualisation app where we can see
the algorithm run step by step.

Whenever "vis - ..." is used the code described concerns visualisation.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir() (used to create visualisation folder)

class poly_incremental {

    private:
        /*
        Algorithm parameters below.
        */
        static int edge_selection; // 1,2,3 -> random, min, max
        static int initialization; // 1,2,3,4 -> sort x, srot x reverse, sort y, sort y reverse
        static bool vis;           // visualisation at each step of the algorithm
        static bool vis_min;       // minimal visualisation, points and final poly line

        /*
        If full visualisation is picked, at each step we save information on the files below.
        vis_counter counts the step number and is increased on each step.
        */
        static int vis_counter;              // counts step
        static std::string vis_points;       // saves points
        static std::string vis_ch;           // saves convex hull
        static std::string vis_visible;      // saves points on convex hull, visible from next point
        static std::string vis_poly_line;    // saves poly line
        static std::string vis_visible_poly; // saves points on poly line, visible from next point

        /*
        Runs a step of the algorithm. Given next_point, current poly line, ch and 
        index of last point inserted, calculates the new poly line and ch from adding the next point.
        Also updates last_point_i to the index of the new last point and updates the area of the poy line.
        */
        static void bb_inc_step(const Point_2& next_point,
                                Polygon_2& ch_bb,
                                Polygon_2& poly_line,
                                int& last_point_i,
                                NUM& poly_area);

    public:
        /*
        Runs the incremental algorithm on a set of points, with given initialization and
        edge selection strategy. 

        poly_line and poly_area hold the calculated polygonal line and its area after
        completion of execution.

        Also takes visualisation options.
        */    
        static void run(const Points& points, 
                        const int& initialization, 
                        const int& edge_selection, 
                        Polygon_2& poly_line,
                        NUM& poly_area,
                        const bool& vis=false,
                        const bool& vis_min=false);
};

#endif
