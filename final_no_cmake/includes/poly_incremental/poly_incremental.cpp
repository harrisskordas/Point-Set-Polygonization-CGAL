#include "poly_incremental.hpp"

/*
Definition of static variables.
*/

// Algorithm parameters below.
int poly_incremental::edge_selection; 
int poly_incremental::initialization; 
bool poly_incremental::vis;           
bool poly_incremental::vis_min;       

// Set visualisation paths.
int poly_incremental::vis_counter;
std::string poly_incremental::vis_points="visualisation/points";
std::string poly_incremental::vis_ch="visualisation/ch_";
std::string poly_incremental::vis_visible="visualisation/visible_";
std::string poly_incremental::vis_poly_line="visualisation/poly_line_";
std::string poly_incremental::vis_visible_poly="visualisation/visible_poly_";

/*
Runs a step of the algorithm. Given next_point, current poly line, ch and 
index of last point inserted, calculates the new poly line and ch from adding the next point.
Also updates last_point_i to the index of the new last point and updates the area of the poy line.
*/
int poly_incremental::bb_inc_step(const Point_2& next_point,
                                   Polygon_2& ch_bb,
                                   Polygon_2& poly_line,
                                   int& last_point_i,
                                   NUM& poly_area){
   
    // get size of ch_bb
    int size=ch_bb.size();

    // get last point from given index
    Point_2 last_point=ch_bb[last_point_i];
    
    // check points turning right
    int right_i=last_point_i;
    Segment_2 right_edge(ch_bb[right_i],ch_bb[(right_i+1)%size]);
    
    while (visibility::is_visible_p_from_e(next_point,right_edge,ch_bb)){
        right_i=(right_i+1)%size;
        right_edge=Segment_2(ch_bb[right_i],ch_bb[(right_i+1)%size]);
    }

    // check points turning left
    int left_i=last_point_i;
    Segment_2 left_edge(ch_bb[(left_i-1+size)%size],ch_bb[left_i]);

    while (visibility::is_visible_p_from_e(next_point,left_edge,ch_bb)){
        left_i=(left_i-1+size)%size;
        left_edge=Segment_2(ch_bb[(left_i-1+size)%size],ch_bb[left_i]);
    }

    /*
    at this point all points from left_i to right_i are visible from next_point
    */ 

    // vis - save visible points on ch
    if(vis) {
        Points visible_points;
        int index = left_i;
        while(index != right_i){
            visible_points.push_back(ch_bb[index]);
            index=(index+1)%size;
        }
        visible_points.push_back(ch_bb[right_i]);
        io_manip::save_points_to_file(visible_points,vis_visible+std::to_string(vis_counter));
    }


    /*
    find the "red" edges on the poly line and update poly_line
    */     
    /////////////////////////////////////////////////////////////////////////////////
    
    // first find index of point at poly_line that is equal to ch_bb[left_i]
    int poly_left_i;
    for (int i=0; i<poly_line.size(); i++)
        if (ch_bb[left_i] == poly_line[i]) {
            poly_left_i=i;
            break;
        }

    // traverse poly line until the point equal to ch_bb[right_i] and save edges
    Edges edges;
    int poly_i=poly_left_i;
    do {
        int next_poly_i=(poly_i+1)%poly_line.size();
        Segment_2 edge(poly_line[poly_i],poly_line[next_poly_i]);
        edges.push_back(edge);
        poly_i=next_poly_i;
    } while (poly_line[poly_i] != ch_bb[right_i]);

    // check which edges are visible from next_point and calculate area
    std::vector<bool> visibility_vec;
    std::vector<NUM> areas;

    for(const Segment_2& edge:edges) {
        NUM area = CGAL::abs(Triangle_2(edge.source(),edge.target(),next_point).area());
        areas.push_back(area);
        // only red edges could possibly block visibility
        visibility_vec.push_back(visibility::is_visible_p_from_e(next_point,edge,edges));
        //visibility_vec.push_back(visibility::is_visible_p_from_e(next_point,edge,poly_line));
    }


    // choose next index to insert - edge_selection = 1,2,3 -> random, min, max
    int chosen_index=visibility::choose_index(visibility_vec,areas,edge_selection);

    // if chosen_index is -1, there is no visible to red edge to be selected - return -1
    if (chosen_index==-1) return -1;

    // find acctual index on poly_line
    int poly_index=(poly_left_i+chosen_index+1)%poly_line.size();
    // insert next point in poly_line
    poly_line.insert(poly_line.vertices_begin()+poly_index,next_point);

    // update area
    poly_area+=areas[chosen_index];
    
    // vis - save visible edges on poly line
    if(vis) io_manip::save_points_and_vis_to_file(edges,visibility_vec,vis_visible_poly+std::to_string(vis_counter));

    /////////////////////////////////////////////////////////////////////////////////

    /*
    continue with ch_bb
    */  

    // remove points from ch
    int remove_count=right_i-left_i;
    if (remove_count<0) remove_count+=size;
    remove_count--;
    int erase_pos=left_i;
    int new_size=size;
    for(int reps=0;reps<remove_count;reps++) {
        ch_bb.erase(ch_bb.begin()+(erase_pos+1)%new_size);
        if ((erase_pos+1)%new_size < erase_pos) erase_pos--;
        new_size--;
    }

    // and add new point to ch
    int insert_index=(erase_pos+1)%new_size; 
    ch_bb.insert(ch_bb.vertices_begin()+insert_index,next_point);

    // vis - increase vis_counter and save new ch
    if(vis) vis_counter++;
    if(vis) io_manip::save_points_to_file(ch_bb,vis_ch+std::to_string(vis_counter));
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));

    // set last_point_i as the index where the last point was inserted at ch
    last_point_i=insert_index;

    return 0;
}

/*
Runs the incremental algorithm on a set of points, with given initialization and
edge selection strategy. 

poly_line and poly_area hold the calculated polygonal line and its area after
completion of execution.

Also takes visualisation options.
*/    
void poly_incremental::run(const Points& points, 
                           const int& initialization, 
                           const int& edge_selection, 
                           Polygon_2& poly_line,
                           NUM& poly_area,
                           const bool& vis,
                           const bool& vis_min) {
    // initialize parameters
    poly_incremental::vis_counter=0;
    poly_incremental::vis=vis; 
    poly_incremental::vis_min=vis_min;
    poly_incremental::edge_selection = edge_selection;  
    poly_incremental::initialization = initialization; 

    // initialize rest of points to be added to poly_line, as the given points
    Points rest_of_points;
    for (const Point_2& p:points)
        rest_of_points.push_back(p);

    // initialise rest of points by sorting
    if (initialization==1 || initialization==2){
        // initialization = 1: sorted at x
        std::stable_sort(rest_of_points.begin(), rest_of_points.end());
        if(initialization==2)
            // initialization = 2: sorted at x, reversed
            std::reverse(rest_of_points.begin(), rest_of_points.end());
    }
    if (initialization==3 || initialization==4){
        // initialization = 3: sorted at y
        struct YCopmare {
            bool operator() (Point_2 p1, Point_2 p2) { return (p1.y() < p2.y());}
        } yCompare;
        std::stable_sort(rest_of_points.begin(), rest_of_points.end(), yCompare);
        if(initialization==4)
            // initialization = 4: sorted at y reversed
            std::reverse(rest_of_points.begin(), rest_of_points.end());
    }

    // vis - create visualisation directory and save points
    if(vis || vis_min) mkdir("visualisation",0777);
    if(vis || vis_min) io_manip::save_points_to_file(rest_of_points,vis_points);

    // create poly line, ch_bb - both start as first triangle
    Polygon_2 ch_bb;
    for (int i=0;i<3;i++) {
        poly_line.push_back(rest_of_points[0]);
        ch_bb.push_back(rest_of_points[0]);
        // also remove added points from rest_of_points
        rest_of_points.erase(rest_of_points.begin());
    }

    // calculate area of first triangle
    poly_area = CGAL::abs(poly_line.area());

    // vis - save ch_bb
    if(vis) io_manip::save_points_to_file(ch_bb,vis_ch+std::to_string(vis_counter));
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter));

    // index of last inserted point - always te third point (points are sorted)
    int last_point_i=2;

    // incremental algorithm loop - one step for each point in rest_of_points
    int max_reps = rest_of_points.size();
    for (int reps=0; reps<max_reps; reps++) {
        //std::cout<<reps<<"\n";
        // get next point
        Point_2 next_point = rest_of_points[0];      

        // calculate next poly_line, ch, last added point, poly area
        if(
        bb_inc_step(next_point,
                    ch_bb,
                    poly_line,
                    last_point_i,
                    poly_area) == -1
        ) {
            std::cout << "ERROR - NO VISIBLE POINT TO ADD.\n";
            break;
        }

        //std::cout<<"    "<<poly_line.is_simple()<<"\n";
        // if (!poly_line.is_simple()) break;

        // erase added point
        rest_of_points.erase(rest_of_points.begin());
    }
    
    // vis - save final poly_line and ch
    if(vis_min) io_manip::save_points_to_file(ch_bb,vis_ch);
    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}

