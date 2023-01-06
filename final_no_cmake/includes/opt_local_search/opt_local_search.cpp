#include "opt_local_search.hpp"

/*
Definition of static variables.
*/

// Algorithm parameters below.
int opt_local_search::LL; 
int opt_local_search::minmax; 
double opt_local_search::threshold;           
bool opt_local_search::vis;          
bool opt_local_search::vis_min;

// Set visualisation paths.
int opt_local_search::vis_counter;
std::string opt_local_search::vis_points="visualisation/points";
std::string opt_local_search::vis_poly_line="visualisation/opt_poly_line_";

void opt_local_search::run(const Points& points, 
                           const int& LL, 
                           const int& minmax, 
                           const double& threshold,
                           Polygon_2& poly_line,
                           NUM& poly_area,
                           const NUM& ch_area,
                           const bool& vis,
                           const bool& vis_min)
{

    opt_local_search::vis_counter=0;
    opt_local_search::vis=vis; 
    opt_local_search::vis_min=vis_min;
    opt_local_search::LL=LL; 
    opt_local_search::minmax=minmax; 
    opt_local_search::threshold=threshold;  

    // size of polygon
    int size=poly_line.size();
    //if(vis || vis_min) mkdir("visualisation",0777);
    //if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);

    double ratio_change=1;

    //for (int k=0;k<10;k++) {
    while (CGAL::abs(ratio_change)>threshold) {

        // we will find a maximal or minimal step on the current polygon
        NUM chosen_area_change = ch_area;  // min init
        if(minmax==2) chosen_area_change = -ch_area; // max init
        Polygon_2 chosen_new_poly;
        int chosen_L;

        // try all Ls
        for (int L=1;L<=LL;L++){
            // check all points and find a minimal or maximal step
            for (int ii=0;ii<size;ii++) {
                int ind = ii;
                // get L consecutive points
                Polygon_2 v_points;
                for (int i=0;i<L;i++){
                    v_points.push_back(poly_line[(ind+i)%size]);
                }

                // calculate the edge to be added
                Segment_2 new_edge=Segment_2(poly_line[(size+ind-1)%size],poly_line[(ind+L)%size]);         

                // create a polygon without the points in v_points
                Polygon_2 poly_line_no_p;
                for (const Point_2& q:poly_line) {
                    bool can_insert=true;
                    for (const Point_2& p:v_points)
                        if (q==p) {
                            can_insert=false;
                            break;
                        }
                    if (!can_insert) continue;
                    poly_line_no_p.push_back(q);
                }

                // check if the two polygons v_points and poly_line_no_p are not simple, or are intersecting
                if (   visibility::are_intersecting(new_edge,v_points) 
                    || visibility::are_intersecting(new_edge,poly_line_no_p)
                    || visibility::are_intersecting(Segment_2(v_points[0],v_points[L-1]),poly_line_no_p)
                    ) continue;

                // for small datasets we are checking the same things with wrong ori
                if (v_points.size()>poly_line_no_p.size()) continue;

                // find possible ways to connect the polygons
                Edges bridges;
                visibility::can_connect_polys(v_points,poly_line_no_p,bridges);

                for (int i=0;i<bridges.size()/2;i++) {
                    // caclculate new connected polygon
                    Edges current_bridge;
                    current_bridge.push_back(bridges[2*i]);
                    current_bridge.push_back(bridges[2*i+1]);
                    Polygon_2 new_poly=visibility::connect_polys_at_bridge(v_points,poly_line_no_p,current_bridge);
                    
                    // calculate area change
                    NUM area_change = CGAL::abs(new_poly.area()) - CGAL::abs(poly_line.area());
                    if (area_change==0) continue;

                    // update values
                    if (
                        (minmax==1 && area_change < chosen_area_change) // min
                        ||
                        (minmax==2 && area_change > chosen_area_change) // max
                        ) {
                        chosen_area_change = area_change;
                        chosen_new_poly = new_poly;
                        chosen_L=L;
                    }
                }
            }
        }

        // check if minimal polygon is found
        if (minmax == 1 && chosen_area_change>=0) {
            //std::cout << "Min change is positive: "<<chosen_area_change<<"\n";
            break;
        }
        
        // check if maximal polygon is found
        if (minmax == 2 && chosen_area_change<=0) {
            //std::cout << "Max change is negative: "<<chosen_area_change<<"\n";
            break;
        }

        // update poly_line
        poly_line=chosen_new_poly;

        // calcuclate ratio change
        NUM new_poly_area = poly_area+chosen_area_change;
        ratio_change=(new_poly_area*1.0/ch_area)-(poly_area*1.0/ch_area);
        
        // update poly_area
        poly_area=new_poly_area;


        // vis
        if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));

        // debuging and stats
        /*
        NUM cgal_poly_area = CGAL::abs(poly_line.area());
        bool simple = poly_line.is_simple();

        std::cout<<poly_area<<","
        <<cgal_poly_area<<","
        <<std::to_string(poly_area*1.0/ch_area)<<","
        <<simple<<","
        <<chosen_L<<"\n";
        */
    }

    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}