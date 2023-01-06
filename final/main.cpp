#include <iostream> // std::cout
#include <ctime>    // std::clock()
#include "includes/io_manip/io_manip.hpp"                         // for read_data()
#include "includes/poly_line_algorithms/poly_line_algorithms.hpp" // polygonization algorithms

void remove_point(Points& points,const Point_2& point) {
    for (int i=0;i<points.size();i++){
        if(points[i]==point){
            points.erase(points.begin() + i);
            break;
        }
    }
}

int main(int argc, char** argv) {

    // input parameters
    std::string inpout_file;
    std::string output_file;
    int algorithm;
    int edge_selection;
    int init_inc;
    int init_onion;
    bool vis=false;    
    bool vis_min=false;  

    // get input from argc, argv
    io_manip::process_input(argc, 
                            argv, 
                            inpout_file, 
                            output_file, 
                            algorithm, 
                            edge_selection, 
                            init_inc, 
                            init_onion, 
                            vis, 
                            vis_min);

    // read points and area of convex hull from input file
    Points points;
    NUM ch_area;
    io_manip::read_data(inpout_file,points,ch_area);

    // these will be set after running an algorithm
    Polygon_2 poly_line;    // the simple polygona line corssing all points
    NUM poly_area;          // area calculated by algorithm
    NUM cgal_poly_area;     // area calculated by Polygon_2::area()
    NUM pick_area;          // area calculated with Pick's formula
    double time;            // time taken to run algorithm
    int simple;             // if poly line is simple
    int points_count;             // number of given points
    int points_poly_count;        // number of points on poly line

    // "start" timer
    std::clock_t start;
    start = std::clock();

    // run algorithm
    switch(algorithm){
        case 1:
            poly_line_algorithms::incremental(points,init_inc,edge_selection,poly_line,poly_area,vis,vis_min);
            break;
        case 2:
            poly_line_algorithms::convex_hull(points,edge_selection,poly_line,poly_area,ch_area,vis,vis_min);
            break;
        case 3:
            poly_line_algorithms::onion(points,init_onion,poly_line,poly_area,vis,vis_min);
            break;
    }

    // "stop" timer
    time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

    // caculate cgal and pick areas./tp
    cgal_poly_area = CGAL::abs(poly_line.area());
    pick_area = -1;//poly_line_algorithms::pick(poly_line);

    // calculate simple, points_count, points_poly_count
    simple = poly_line.is_simple();
    points_count = points.size();
    points_poly_count = poly_line.size();

    // save output
    std::ofstream outfile(output_file);

    outfile << io_manip::create_output(poly_line,
                                       poly_area, 
                                       cgal_poly_area, 
                                       pick_area,
                                       ch_area, 
                                       time, 
                                       algorithm, 
                                       edge_selection, 
                                       init_inc, 
                                       init_onion,
                                       simple,
                                       points_count,
                                       points_poly_count);
    
    outfile.close();

}