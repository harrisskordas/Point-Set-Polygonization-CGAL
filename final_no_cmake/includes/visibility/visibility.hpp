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
    Would be better named difference, as the function, given two sets of points (points and outter), returns
    the points that belong to points and not to CH.
    */
    static void find_inner_points(const Points& points, const Polygon_2& outter, Points& inner_points) {
        // for each point
        for (const Point_2& p:points) {
            bool check=true;
            // check if it is equal to one of the points on the CH
            for (const Point_2& p_out:outter) {
                if (p==p_out) {
                    check=false;
                    break;
                }
            } 
            // if not, check remains true, so add it to inner points
            if (check)
                inner_points.push_back(p);
        }
    }
    // same with as above but returns the inner points
    static Points find_inner_points(const Points& points, const Polygon_2& outter) {
        Points inner_points;
        // for each point
        for (const Point_2& p:points) {
            bool check=true;
            // check if it is equal to one of the points on the CH
            for (const Point_2& p_out:outter) {
                if (p==p_out) {
                    check=false;
                    break;
                }
            } 
            // if not, check remains true, so add it to inner points
            if (check)
                inner_points.push_back(p);
        }
        return inner_points;
    }

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
    // overloaded version to call with edges
    bool is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Edges& edges);
    // overloaded version to call with extra edges and edges to skip
    bool is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Polygon_2& poly_line, const Edges& skip_edges, const Edges& extra_edges);

    // assignment 2
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
    Checks if a segment is intersecting with a polygon.
    */ 
    bool are_intersecting(const Segment_2& seg_check, const Polygon_2& poly_line);

    /*
    Checks if two polygons can be connected and return possible bridges
    */ 
    bool can_connect_polys_simple(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges);

    /*
    Checks if two polygons can be connected and return possible bridges
    */ 
    bool can_connect_polys(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges, bool just_one=false);

    /*
    get a polygon from a starting point to a final point - the long way around
    */
    Polygon_2 get_from_to(const Polygon_2& poly, const Point_2& from, const Point_2& to);

    /*
    connect two polygons at bridge (bridge is from poly 1 to poly 2)
    */ 
    Polygon_2 connect_polys_at_bridge(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges);


}

#endif
