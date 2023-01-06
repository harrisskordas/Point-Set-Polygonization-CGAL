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
Would be better named difference, as the function, given two sets of points (points and outter), returns
the points that belong to points and not to CH.
*/
void poly_from_ch::find_inner_points(const Points& points, const Polygon_2& outter, Points& inner_points) {
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

/*
Given an edge and a set of points, find the point closest to the edge and its index.
*/
void poly_from_ch::point_closest_to_edge(const Segment_2& e,const Points& points, Point_2& closest_point) {
    // set closest as first point
    closest_point = points[0];
    NUM min_distance = CGAL::squared_distance(e,points[0]); 

    // run through all points and find min distance and point
    int i=0;
    for (const Point_2& p:points) {
        NUM distance = CGAL::squared_distance(e,p);
        if ( distance < min_distance ){
            // if distance is less than min, update closest
            min_distance = distance;
            closest_point=p;
        }
        i++;
    }
}

/*
Removes a point from a list of points, if found.
*/
void poly_from_ch::remove_point(Points& points,const Point_2& point) {
    for (int i=0;i<points.size();i++){
        if(points[i]==point){
            points.erase(points.begin() + i);
            break;
        }
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

    // at each iteration the variables below will hold:
    Points closest_points;              // closest point to each edge
    std::vector<NUM> areas;             // if it is visible
    std::vector<bool> visibility_vec;   // area of created triangle
    int chosen_index;                   // the index of insertion
    Point_2 chosen_point;               // the point to be inserted

    // inwards addition loop - one step for each inner point
    int max_reps = points.size()-poly_line.size();
    for (int reps=0; reps<max_reps; reps++) {

        // for each edge on the polygonal line:
        int i=0;
        for(const Segment_2& e : poly_line.edges()) {

            // only update values for points that had as closest point the chosen point
            if(reps==0 || closest_points[i] == chosen_point || i==chosen_index) {
                //on first iteration always enter and push back instead of updating

                // update closest point
                Point_2 closest_point;
                point_closest_to_edge(e,inner_points,closest_point);
                if(reps==0) closest_points.push_back(closest_point);
                else closest_points[i]=closest_point;

                // update triangle area
                NUM area = Triangle_2(e.source(),e.target(),closest_point).area();
                if(reps==0) areas.push_back(area);
                else areas[i] = area;

                // update visibility check
                if(reps==0) visibility_vec.push_back(visibility::is_visible_p_from_e(closest_point,e,poly_line));
                else visibility_vec[i]=visibility::is_visible_p_from_e(closest_point,e,poly_line);

            }
            i++;
        }

        // vis - save closest points
        if(vis) io_manip::save_points_to_file(closest_points,visibility_vec,vis_closest+std::to_string(vis_counter));

        // choose next point to insert - c = 1,2,3 -> random, min, max
        chosen_index = visibility::choose_index(visibility_vec,areas,edge_selection);
        chosen_point = closest_points[chosen_index];

        // update area
        poly_area-=areas[chosen_index];

        // insert new point in polygon
        if (poly_line.vertices_begin()+chosen_index == poly_line.vertices_end()) 
            poly_line.push_back(chosen_point);
        else
            poly_line.insert(poly_line.vertices_begin()+chosen_index+1,chosen_point);

        // insert dummy values for new point at closest_points,areas,visibility
        closest_points.insert(closest_points.begin()+chosen_index,Point_2(-1,-1));
        areas.insert(areas.begin()+chosen_index,-1);
        visibility_vec.insert(visibility_vec.begin()+chosen_index,false);


        // update inner_points, remove added point
        remove_point(inner_points,chosen_point);

        // vis - increase vis_counter and save new poly line
        if(vis) vis_counter++;
        if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));
    }

    // vis - save final poly line points
    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}
