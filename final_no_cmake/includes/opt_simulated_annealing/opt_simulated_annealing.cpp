#include "opt_simulated_annealing.hpp"

bool debug=false;

/*
Definition of static variables.
*/

// Algorithm parameters below.
int opt_simulated_annealing::LL; 
int opt_simulated_annealing::minmax; 
int opt_simulated_annealing::local_global_subdiv;           
bool opt_simulated_annealing::vis;          
bool opt_simulated_annealing::vis_min;

// Set visualisation paths.
int opt_simulated_annealing::vis_counter;
std::string opt_simulated_annealing::vis_points="visualisation/points";
std::string opt_simulated_annealing::vis_poly_line="visualisation/opt_poly_line_";
std::string opt_simulated_annealing::vis_sub_points="visualisation/opt_sub_points_";

// run simulated annealing with local search
void opt_simulated_annealing::local(const Points& points, 
                                    const int& LL, 
                                    const int& minmax, 
                                    const int& local_global_subdiv,
                                    const int& poly_algorithm,
                                    Polygon_2& poly_line,
                                    NUM& poly_area,
                                    const NUM& ch_area,
                                    const bool& vis,
                                    const bool& vis_min)
{

    opt_simulated_annealing::vis_counter=0;
    opt_simulated_annealing::vis=vis; 
    opt_simulated_annealing::vis_min=vis_min;
    opt_simulated_annealing::LL=LL; 
    opt_simulated_annealing::minmax=minmax; 
    opt_simulated_annealing::local_global_subdiv=local_global_subdiv;  

    // generate a random number for mitropolis
    double R=get_uniform_R();

    // size of polygon
    int size=poly_line.size();
    //if(vis || vis_min) mkdir("visualisation",0777);
    //if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);

    double T=1;

    // create a kd-tree and insert point of the poly line
    Tree tree;
    for(int i = 0; i < size; i++)
        tree.insert(poly_line[i]);
    
    // local step
    while (T>=0) {

        // get a random point and the one after it
        int ind = get_r(size);
        //ind=0;
        
        Point_2 rand_point=poly_line[ind];
        Point_2 rand_point_next=poly_line[(ind+1)%size];

        // calculate removed edge and new edges
        Segment_2 removed_edge(rand_point,rand_point_next);
        Segment_2 new_edge1(poly_line[(size+ind-1)%size],rand_point_next);
        Segment_2 new_edge2(rand_point,poly_line[(ind+2)%size]);

        // find the bounding box of the points 
        // and the one after them and the one before them
        Polygon_2 bbox_poly;

        bbox_poly.push_back(poly_line[(size+ind-1)%size]);
        bbox_poly.push_back(poly_line[ind]);
        bbox_poly.push_back(poly_line[(ind+1)%size]);
        bbox_poly.push_back(poly_line[(ind+2)%size]);

        CGAL::Bbox_2 bbox=bbox_poly.bbox();

        // define range query
        Point_2 bot_left(bbox.xmin(), bbox.ymin());
        Point_2 top_right(bbox.xmax(), bbox.xmax());
        Fuzzy_iso_box exact_range(bot_left,top_right);

        // make kd-tree query
        Points query_result;
        tree.search( std::back_inserter(query_result), exact_range);

        // find possibly intersecting edges
        Edges edges_check;

        for (const Segment_2& edge:poly_line.edges()) {
            if(edge==removed_edge) continue;

            bool found_target=false;
            bool found_source=false;

            for(const Point_2& p:query_result) {
                if (!found_source) found_source=edge.source()==p;
                if (!found_target) found_target=edge.target()==p;
            }

            if (found_source||found_target){
                edges_check.push_back(edge);
            }
        }

        // check intersections
        bool can_make_swap=true;
        if (visibility::are_intersecting(new_edge1,new_edge2)) can_make_swap=false;

        //for (const Segment_2& edge:edges_check) {
        for (const Segment_2& edge:poly_line.edges()) {
            if (visibility::are_intersecting(new_edge1,edge)) can_make_swap=false;
            if (visibility::are_intersecting(new_edge2,edge)) can_make_swap=false;
        }

        if (!can_make_swap)  continue;

        // caculate are after change
        int poly_ori = poly_line.orientation();

        Triangle_2 tri1(poly_line[(size+ind-1)%size],poly_line[ind],poly_line[(ind+1)%size]);
        Triangle_2 tri2(poly_line[ind],poly_line[(ind+1)%size],poly_line[(ind+2)%size]);
        NUM area1=-poly_ori*tri1.area();
        NUM area2=-poly_ori*tri2.area();

        NUM area_change = area1+area2;
        NUM new_poly_area=poly_area+area_change;

        // caclculate energy and change of energy
        double energy = get_energy(minmax,size,poly_area,ch_area);
        double new_energy = get_energy(minmax,size,new_poly_area,ch_area);

        double energy_change=new_energy-energy;

        // decide to make change or not
        if (take_it_or_leave_it(energy_change,T,R)) {
        //if(minmax==1 && area_change<0 || minmax==2 && area_change>0) {
            // apply change
            // remove random point
            poly_line.erase(poly_line.vertices_begin() + ind);
            // insert it after its successor
            poly_line.insert(poly_line.vertices_begin() + (ind+1)%size,rand_point);
            // update area
            poly_area=new_poly_area;
            // vis
            if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));
        }

        // update temprature
        T-=1.0/LL;



        // debuging and stats
        if (debug){
            NUM cgal_poly_area = CGAL::abs(poly_line.area());
            bool simple = poly_line.is_simple();

            std::cout<<poly_area<<","
            <<cgal_poly_area<<","
            <<std::to_string(poly_area*1.0/ch_area)<<","
            <<simple<<"\n";
        }
    }

    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}

// run simulated annealing with global search
void opt_simulated_annealing::global(const Points& points, 
                                     const int& LL, 
                                     const int& minmax, 
                                     const int& local_global_subdiv,
                                     const int& poly_algorithm,
                                     Polygon_2& poly_line,
                                     NUM& poly_area,
                                     const NUM& ch_area,
                                     const bool& vis,
                                     const bool& vis_min)
{
    opt_simulated_annealing::vis_counter=0;
    opt_simulated_annealing::vis=vis; 
    opt_simulated_annealing::vis_min=vis_min;
    opt_simulated_annealing::LL=LL; 
    opt_simulated_annealing::minmax=minmax; 
    opt_simulated_annealing::local_global_subdiv=local_global_subdiv;  
    
    // generate a random number for mitropolis
    double R=get_uniform_R();

    // size of polygon
    int size=poly_line.size();
    //if(vis || vis_min) mkdir("visualisation",0777);
    //if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);

    double T=1;

    // orientation of the polygon
    int poly_ori = poly_line.orientation();

    // local step
    while (T>=0) {

        // get a random point
        int ind = get_r(size);
        
        // find point p from index, and edge that will be added by removing the point
        Point_2 p=poly_line[ind];
        Polygon_2 poly_line_no_p;
        Segment_2 new_edge;

        // create a polygon without the point p
        for (const Point_2& q:poly_line)
            if (q!=p) poly_line_no_p.push_back(q);


        // calculate the edge to be added
        new_edge=Segment_2(poly_line[(size+ind-1)%size],poly_line[(ind+1)%size]);
        // the orientation of the point relative to the edge
        int ori  = CGAL::orientation(new_edge.source(),new_edge.target(),poly_line[ind]);
        // the area of the triangle from p and new_edge
        NUM new_edge_area = CGAL::abs(Triangle_2(new_edge.source(),new_edge.target(),p).area());

        // check if the new edge is intersecting with any other edge of the polygon
        bool yoinkz=false;
        for (const Segment_2& edge:poly_line_no_p.edges()) {
            if (edge == new_edge) continue; // skip new edge
            if (visibility::are_intersecting(edge,new_edge))
                yoinkz = true;    
        }
        // if so continue to next random point
        if (yoinkz) continue;

        // if not choose randomly an edge to insert the point
        int ind1;
        Segment_2 edge;

        do {
            ind1=get_r(size);
            edge=Segment_2(poly_line[ind1],poly_line[(ind1+1)%size]);
        } while(ind1==1 || edge==new_edge);

        // check if random point can be moved to random edge        
        if (!visibility::is_visible_p_from_e(p,edge,poly_line_no_p))
            // if not continue
            continue;

        // calculate area with edge
        NUM removed_edge_area = CGAL::abs(Triangle_2(edge.source(),edge.target(),p).area());
        // calculate area change
        NUM area_change = ori*poly_ori*(new_edge_area-removed_edge_area);
        // calculate area after change
        NUM new_poly_area=poly_area+area_change;

        // caclculate energy and change of energy
        double energy = get_energy(minmax,size,poly_area,ch_area);
        double new_energy = get_energy(minmax,size,new_poly_area,ch_area);

        double energy_change=new_energy-energy;

        // decide to apply change or not
        if (take_it_or_leave_it(energy_change,T,R)) {
        //if(minmax==1 && area_change<0 || minmax==2 && area_change>0) {
            // apply change
            // remove point p at ind
            poly_line.erase(poly_line.begin()+ind);
            // insert p at edge
            for (int i=0;i<size;i++){
                if(poly_line[i]==edge.source() && 
                   poly_line[(i+1)%size]==edge.target())
                {
                   poly_line.insert(poly_line.vertices_begin()+(i+1)%size,p);
                }
            }
            //update area
            poly_area=new_poly_area;
            // vis
            if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));
        }

        // update temprature
        T-=1.0/LL;


        // debuging and stats
        // debuging and stats
        if (debug){
            NUM cgal_poly_area = CGAL::abs(poly_line.area());
            bool simple = poly_line.is_simple();

            std::cout<<poly_area<<","
            <<cgal_poly_area<<","
            <<std::to_string(poly_area*1.0/ch_area)<<","
            <<simple<<"\n";
        }
    }

    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}

// run simulated annealing with subdivision
void opt_simulated_annealing::subdiv(const Points& points, 
                                     const int& LL, 
                                     const int& minmax, 
                                     const int& local_global_subdiv,
                                     const int& poly_algorithm,
                                     Polygon_2& poly_line,
                                     NUM& poly_area,
                                     const NUM& ch_area,
                                     const bool& vis,
                                     const bool& vis_min)
{
    opt_simulated_annealing::vis_counter=0;
    opt_simulated_annealing::vis=vis; 
    opt_simulated_annealing::vis_min=vis_min;
    opt_simulated_annealing::LL=LL; 
    opt_simulated_annealing::minmax=minmax; 
    opt_simulated_annealing::local_global_subdiv=local_global_subdiv;  

    if(vis || vis_min) mkdir("visualisation",0777);
    if(vis || vis_min) io_manip::save_points_to_file(points,vis_points);    
    
    // size of polygon
    // sort points on x
    int size=points.size();
    Points sorted_points;
    
    for (const Point_2& p:points)
        sorted_points.push_back(p);

    std::stable_sort(sorted_points.begin(), sorted_points.end());

    // break point set on sets of size m
    int m=100;                          // number of points per set
    int num_of_sets=ceil(size*1.0/m);  // number of sets to be created  
    
    std::vector<Points> point_sets;

    // fill point sets
    for (int i=0;i<num_of_sets;i++) {
        Points point_set;

        int set_size=m;
        // last set will get remaining points
        if ((i==num_of_sets-1)&&(size%m != 0)) set_size=size%m;
        for(int j=0;j<set_size;j++) {
            point_set.push_back(sorted_points[i*m+j]);
        }

        point_sets.push_back(point_set);
    }

    // polygonize each set
    std::vector<Polygon_2> poly_lines;
    std::vector<NUM> poly_areas;
    std::vector<NUM> ch_areas;

    for (int i=0;i<num_of_sets;i++) {
        Polygon_2 poly_line;
        poly_lines.push_back(poly_line);
        NUM poly_area;

        poly_areas.push_back(poly_area);
        Polygon_2 ch;

        CGAL::convex_hull_2(point_sets[i].begin(), point_sets[i].end(), std::back_inserter(ch));
        NUM ch_area=CGAL::abs(ch.area());
        ch_areas.push_back(ch_area);

    }

    // run given polygonization algorithm for each set of points
    for (int i=0;i<num_of_sets;i++) {
        switch(poly_algorithm){
            case 1:{
                int init_inc=1;
                int edge_selection;
                if (minmax==1) edge_selection=2;  // min poly with min edge_sel for inc alg
                else           edge_selection=3; // max poly with max edge_sel for inc alg
                poly_incremental::run(point_sets[i],init_inc,edge_selection,poly_lines[i],poly_areas[i]);
                break;
            }
            case 2: {
                int edge_selection;
                if (minmax==1) edge_selection=3;  // min poly with max edge_sel for ch alg
                else           edge_selection=2;  // max poly with min edge_sel for ch alg
                poly_from_ch::run(point_sets[i],edge_selection,poly_lines[i],poly_areas[i],ch_areas[i]);
                break;
            }
            case 3: {
                int init_onion=1;
                poly_onion::run(point_sets[i],init_onion,poly_lines[i],poly_areas[i]);
                break;
            }
        }
        // print a short string with polygonization info, used for debuging

        if (debug) {
            NUM cgal_poly_area=poly_lines[i].area();
            bool simple=poly_lines[i].is_simple();

            std::cout<<poly_areas[i]<<","
            <<cgal_poly_area<<","
            <<std::to_string(poly_areas[i]*1.0/ch_areas[i])<<","
            <<simple<<"\n";
        }
    
    }

    // optimize each set using local search
    for (int i=0;i<num_of_sets;i++) {
        int LL=1;
        double threshold=0.1;
        opt_local_search::run(point_sets[i],LL,minmax,threshold,poly_lines[i],poly_areas[i],ch_areas[i]);

        // vis
        if(vis) io_manip::save_points_to_file(poly_lines[i],vis_sub_points+std::to_string(i));
    }

    // connect all sets

    // set poly line as leftmost point set
    //poly_line.clear();
    for(int i=0;i<poly_lines[0].size();i++)
        poly_line.push_back(poly_lines[0][i]);
    
    if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));

    // set area of poly line as area of leftmost point set
    poly_area = poly_areas[0];

    // for each next set
    for (int i=1;i<num_of_sets;i++) {
        // find possible bridges with next set
        Edges bridges;
        visibility::can_connect_polys(poly_lines[i-1],poly_lines[i],bridges);

        // pick a bridge
        Edges bridge;
        bridge.push_back(bridges[0]);
        bridge.push_back(bridges[1]);
        
        // connect poly_line with next point set on bridge
        Polygon_2 new_poly_line=visibility::connect_polys_at_bridge(poly_line,poly_lines[i],bridge);
        poly_line=new_poly_line;

        // calculate area of bridge
        Polygon_2 bridge_poly;
        bridge_poly.push_back(bridge[0].source());
        bridge_poly.push_back(bridge[0].target());
        bridge_poly.push_back(bridge[1].target());
        bridge_poly.push_back(bridge[1].source());
        NUM bridge_area=CGAL::abs(bridge_poly.area());

        // calculate new polygon area
        poly_area+=bridge_area+poly_areas[i];

        if(vis) io_manip::save_points_to_file(poly_line,vis_poly_line+std::to_string(vis_counter++));
    }

    if(vis_min) io_manip::save_points_to_file(poly_line,vis_poly_line);
}

/*
Runs the simulated annealing algorithm on a simple polygon, with given L,
min or max option and local, global or subdiv option. 

poly_line and poly_area hold the calculated polygonal line and its area after
completion of execution.

Also takes visualisation options.
*/ 
void opt_simulated_annealing::run(const Points& points, 
                                  const int& LL, 
                                  const int& minmax, 
                                  const int& local_global_subdiv,
                                  const int& poly_algorithm,
                                  Polygon_2& poly_line,
                                  NUM& poly_area,
                                  const NUM& ch_area,
                                  const bool& vis,
                                  const bool& vis_min)
{

    switch(local_global_subdiv) {
        case 1: local(points,LL,minmax,local_global_subdiv,poly_algorithm,poly_line,poly_area,ch_area,vis,vis_min); break;
        case 2: global(points,LL,minmax,local_global_subdiv,poly_algorithm,poly_line,poly_area,ch_area,vis,vis_min); break;
        case 3: subdiv(points,LL,minmax,local_global_subdiv,poly_algorithm,poly_line,poly_area,ch_area,vis,vis_min); break;
    }

}
