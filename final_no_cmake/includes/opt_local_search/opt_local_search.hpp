#ifndef opt_local_search_hpp
#define opt_local_search_hpp

/*
This header file contains an implementation of the local search
optimiazation algorithm as discussed in class. 

    Pick L consecutive points, remove them from the polygon and
    add them at a different point of the polygon. 

    Repeat until change of area ratio is below threshold.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir() (used to create visualisation folder)

class opt_local_search {
    private:
        /*
        Algorithm parameters below.
        */
        static int LL;             // 1 to 10 - number of points to be moved
        static int minmax;         // 1,2 -> min or max optimization
        static double threshold;   // threshold to stop
        static bool vis;           // visualisation at each step of the algorithm
        static bool vis_min;       // minimal visualisation, points and final poly line

        /*
        If full visualisation is picked, at each step we save information on the files below.
        vis_counter counts the step number and is increased on each step.
        */
        static int vis_counter;              // counts step
        static std::string vis_points;       // saves points
        static std::string vis_poly_line;    // saves poly line

    public:
    /*
    Runs the local search algorithm on a simple polygon, with given L,
    threshold and min or max option. 

    poly_line and poly_area hold the calculated polygonal line and its area after
    completion of execution.

    Also takes visualisation options.
    */    
    static void run(const Points& points, 
                    const int& LL, 
                    const int& minmax, 
                    const double& threshold,
                    Polygon_2& poly_line,
                    NUM& poly_area,
                    const NUM& ch_area,
                    const bool& vis=false,
                    const bool& vis_min=false);
};

#endif
