#ifndef visibility_hpp
#define visibility_hpp

/*
This header file contains functions related to visibility, for example
if an edge if visible from a point given a polygonal line to block its line of sight.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include <boost/optional/optional_io.hpp> // for are_intersecting(...)
#include <ctime>                    	  // for random seed

namespace visibility {
    
    /*
    When we want to add an edge or point to the poly line we are concerned:
        - wether it is visible
        - the area of the triangle created by the point and its paired edge
    
    This function chooses from a set of points, an index of a point that is visibile
    and has min or max area. The choice can also be made at random. 

    edge_selection = 1,2,3 -> random, min area, max area
    */ 
    int choose_index(const std::vector<bool>& visibility_vec,const std::vector<NUM>& areas,const int& edge_selection);

    /*
    Checks if two segments are intersecting. Used to check visibility.
    */ 
    bool are_intersecting(const Segment_2& seg1, const Segment_2& seg2);

    /*
    Checks if edge is visible from point, given a poly_line.

    The edge is supposed to be part of the poly line. 

    The check is made by checking if any edge of the poly line intersects with three
    visibility segments strating from point and ending at the edge.

    The three visibility segments checked are, if edge=(p1,p2):
        - (point,p1)
        - (point,p2)
        - (point,midpoint of edge)
    */ 
    bool is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Polygon_2& poly_line);

}

#endif
