#ifndef poly_from_ch_hpp
#define poly_from_ch_hpp

/*
This header file contains an implementation of the poly line from convex hull algorithm
as discussed in class. 

    First we calculate the convex hull and inner points. Then for each edge, we 
    find the closest point. From these pairs of edges and closest points we pick 
    one that the edge is visible from the point. The choise is made at random or from
    min/max area of the adge and closest point.

    Two visualisation options are provided:
        - full: at each step we save the points on the poly line, the closest point
        to each edge and if the edgse is visible from the point
        - minimal: save only point set and final poly line.

The files created can then be used on our Python visualisation app where we can see
the algorithm run step by step.

Whenever "vis - ..." is used the code concerns visualisation.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir (for vis)
#include <CGAL/convex_hull_2.h>           // CGAL::convex_hull_2


class poly_from_ch {

    private:
        /*
        Algorithm parameters below.
        */
        static int edge_selection; // 1,2,3 -> random, min, max
        static bool vis;           // visualisation
        static bool vis_min;       // minimal visualisation, points and final poly line

        /*
        If full visualisation is picked, at each step we save information on the files below.
        vis_counter counts the step number and is increased on each step.
        */
        static int vis_counter;           // counts step
        static std::string vis_points;    // saves points
        static std::string vis_poly_line; // saves poly line
        static std::string vis_closest;   // saves points on poly line, visible from next point

        /*
        Would be better named difference, as the function, given two sets of points (points and CH), returns
        the points that belong to points and not to CH.

        Implemented here to use template.
        */
        template <typename Point_container> // Points or Polygon_2
        static void find_inner_points(const Points& points, const Point_container& CH, Points& inner_points) {
            // for each point
            for (const Point_2& p:points) {
                bool check=true;
                // check if it is equal to one of the points on the CH
                for (const Point_2& p_ch:CH) {
                    if (p==p_ch) {
                        check=false;
                        break;
                    }
                } 
                // if not, check remains true, so add it to inner points
                if (check)
                    inner_points.push_back(p);
            }
        }

        /*
        Given an edge and a set of points, find the point closest to the edge and its index.
        */
        static void point_closest_to_edge(const Segment_2& e,const Points& points, Point_2& closest_point, int& closest_i);

    public:
        /*
        Runs the poly line from convex hull algorithm, with given edge selection strategy. 

        poly_line and poly_area hold the calculated polygonal line and its area after
        completion of execution.

        Also takes visualisation options.
        */ 
        static void run(const Points& points, 
                        const int& edge_selection, 
                        Polygon_2& poly_line,
                        NUM& poly_area,
                        const NUM& ch_area,
                        const bool& vis=false,
                        const bool& vis_min=false);
};

#endif
