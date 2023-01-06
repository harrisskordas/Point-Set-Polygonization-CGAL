#include "poly_onion.hpp"

/*
Definition of static variables.
*/

// Algorithm parameters below.
int poly_onion::initialization; 
bool poly_onion::vis;           
bool poly_onion::vis_min;       

// Set visualisation paths.
int poly_onion::vis_counter;
std::string poly_onion::vis_points="visualisation/points";
std::string poly_onion::vis_ch="visualisation/ch_";
std::string poly_onion::vis_poly_line="visualisation/poly_line_";
std::string poly_onion::vis_bridge="visualisation/bridge_";

/*
Given an edge and a polygon, find the index of the point closest to the edge.
*/
int poly_onion::point_closest_to_edge(const Segment_2& e,const Polygon_2& polygon) {
    // set closest as first point
    Point_2 closest_point = polygon[0];
    int closest_index = 0;
    NUM min_distance = CGAL::squared_distance(e,polygon[0]); 

    // run through all points and find min distance and point
    int i=0;
    for (const Point_2& p:polygon) {
        NUM distance = CGAL::squared_distance(e,p);
        if ( distance < min_distance ){
            // if distance is less than min, update closest
            min_distance = distance;
            closest_point=p;
            closest_index=i;
        }
        i++;
    }

    return closest_index;
}

/*
Runs the onion algorithm on a set of points, with given initialization strategy. 

poly_line and poly_area hold the calculated polygonal line and its area after
completion of execution.

Also takes visualisation options.
*/  
void poly_onion::run(const Points& points, 
                     const int& initialization, 
                     Polygon_2& poly_line,
                     NUM& poly_area,
                     const bool& vis,
                     const bool& vis_min) {

    // initialize parameters
    poly_onion::vis_counter=0;
    poly_onion::vis=vis; 
    poly_onion::vis_min=vis_min;
    poly_onion::initialization = initialization; 

    // vis - save points
    if(vis || vis_min) mkdir("visualisation",0777);
    if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);

    // copy given points
    Points inner_points;
    for (const Point_2& p:points)
        inner_points.push_back(p);

    /*
    initialise inner points by sorting
    */
    // initialization = 1: no sorting
    if (initialization==2 || initialization==3){
        // initialization = 2: sorted at x
        std::stable_sort(inner_points.begin(), inner_points.end());
        if(initialization==2)
            // initialization = 3: sorted at x, reversed
            std::reverse(inner_points.begin(), inner_points.end());
    }
    if (initialization==4 || initialization==5){
        // initialization = 4: sorted at y
        struct YCopmare {
            bool operator() (Point_2 p1, Point_2 p2) { return (p1.y() < p2.y());}
        } yCompare;
        std::stable_sort(inner_points.begin(), inner_points.end(), yCompare);
        if(initialization==4)
            // initialization = 5: sorted at y reversed
            std::reverse(inner_points.begin(), inner_points.end());
    }

    // find onion convex hulls moving inwards
    std::vector<Polygon_2> CHs;
    int i=0;
    while (inner_points.size()>0) {
        // find and insert CHi
        Polygon_2 CHi;
        CGAL::convex_hull_2( inner_points.begin(), inner_points.end(), std::back_inserter(CHi) );
        CHs.push_back(CHi);
        // remove points on CHi from inner points
        inner_points = visibility::find_inner_points(inner_points,CHi);

        // vis - save CHi    
        if(vis) io_manip::save_points_to_file(CHi,vis_ch+std::to_string(i));
        i++;
    }

    // init poly line as the outermost ch
    for (const Point_2& p:CHs[0])
        poly_line.push_back(p);

    // vis - save poly line
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));

    // initialisation of outer CH 
    Polygon_2& outer_CH = CHs[0];
    poly_line=outer_CH;

    
    // vis
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));
    // inwards combination loop - one step for each inner CH
    int max_reps = CHs.size();
    for (int i=1;i<max_reps;i++) {
        // find possible bridges of poly_line and next inner ch
        Edges bridges;
        visibility::can_connect_polys(poly_line,CHs[i],bridges,true);

        if(bridges.size()==0) break;

        // pick a bridge
        Edges bridge;
        bridge.push_back(bridges[0]);
        bridge.push_back(bridges[1]);
        
        // connect poly_line with next point set on bridge
        poly_line=visibility::connect_polys_at_bridge(poly_line,CHs[i],bridge);

        // vis
        if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));
    }

    // calculate area
    poly_area=CGAL::abs(poly_line.area());

    // vis - save final poly line points
    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);

}

