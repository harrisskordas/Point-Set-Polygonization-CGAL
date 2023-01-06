#include "poly_from_ch.hpp"

/*    
Definition of static variables.
*/

// Algorithm parameters below.
int poly_from_ch::edge_selection; 
bool poly_from_ch::vis;           
bool poly_from_ch::vis_min;       

// Set visualisation paths.
int poly_from_ch::vis_counter;
std::string poly_from_ch::vis_points="visualisation/points";
std::string poly_from_ch::vis_poly_line="visualisation/poly_line_";
std::string poly_from_ch::vis_closest="visualisation/closest_";

/*
Given an edge and a set of points, find the point closest to the edge and its index.
*/
void poly_from_ch::point_closest_to_edge(const Segment_2& e,const Points& points, Point_2& closest_point, int& closest_i) {
    // set closest as first point
    closest_point = points[0];
    closest_i = 0;
    NUM min_distance = CGAL::squared_distance(e,points[0]); 

    // run through all points and find min distance and point
    int i=0;
    for (const Point_2& p:points) {
        NUM distance = CGAL::squared_distance(e,p);
        if ( distance < min_distance ){
            // if distance is less than min, update closest
            min_distance = distance;
            closest_point=p;
            closest_i=i;
        }
        i++;
    }
}

/*
Runs the poly line from convex hull algorithm, with given edge selection strategy. 

poly_line and poly_area hold the calculated polygonal line and its area after
completion of execution.

Also takes visualisation options.
*/ 
void poly_from_ch::run(const Points& points, 
                           const int& edge_selection, 
                           Polygon_2& poly_line,
                           NUM& poly_area,
                           const NUM& ch_area,
                           const bool& vis,
                           const bool& vis_min) {

    // set poly_area to ch_area
    poly_area = ch_area;

    // initialize parameters
    poly_from_ch::vis_counter=0;
    poly_from_ch::vis=vis; 
    poly_from_ch::vis_min=vis_min;
    poly_from_ch::edge_selection = edge_selection;  

    // vis - save points
    if(vis || vis_min) mkdir("visualisation",0777);
    if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);

    // create poly line - starts as the CH of the points
    CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(poly_line) );

    // vis - save poly line
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));

    // initialise inner points
    Points inner_points;
    find_inner_points(points,poly_line,inner_points);
    
    // inwards addition loop - one step for each inner point
    int max_reps = points.size()-poly_line.size();
    for (int reps=0; reps<max_reps; reps++) {
        // if(reps%10==0) std::cout<<reps<<std::endl;
        
        Points closest_points;              // find closest point
        std::vector<NUM> areas;             // if it is visible
        std::vector<bool> visibility_vec;   // area of created triangle
        std::vector<int> inner_points_i;    // index of closest point at inner_points

        // for each edge on the polygonal line:
        for(const Segment_2& e : poly_line.edges()) {
            // closest point
            Point_2 closest_point;
            int inner_i;
            point_closest_to_edge(e,inner_points,closest_point,inner_i);
            closest_points.push_back(closest_point);
            inner_points_i.push_back(inner_i);

            // triangle area
            NUM area = Triangle_2(e.source(),e.target(),closest_point).area();
            areas.push_back(area);

            // visibility check
            visibility_vec.push_back(visibility::is_visible_p_from_e(closest_point,e,poly_line));
        }

        // vis - save closest points
        if(vis) io_manip::save_points_to_file(closest_points,visibility_vec,vis_closest+std::to_string(vis_counter));

        // choose next point to insert - c = 1,2,3 -> random, min, max
        int chosen_index=visibility::choose_index(visibility_vec,areas,edge_selection);

        // insert new point in polygon
        if (poly_line.vertices_begin()+chosen_index == poly_line.vertices_end()) 
            poly_line.push_back(closest_points[chosen_index]);
        else
            poly_line.insert(poly_line.vertices_begin()+chosen_index+1,closest_points[chosen_index]);

        // update area
        poly_area-=areas[chosen_index];

        // update inner_points, remove added point
        inner_points.erase(inner_points.begin() + inner_points_i[chosen_index]);

        // vis - increase vis_counter and save new poly line
        if(vis) vis_counter++;
        if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));
    }

    // vis - save final poly line points
    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}
