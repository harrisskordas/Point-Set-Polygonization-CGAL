#include <iostream>                         // std::cout
#include <ctime>                            // std::clock()
#include "includes/io_manip/io_manip.hpp"   // for read_data()
#include "includes/poly_line_algorithms/poly_line_algorithms.hpp" // polygonization algorithms (ass1)
#include "includes/opt_algorithms/opt_algorithms.hpp"             // optimization algorithms   (ass2)

#include "includes/visibility/visibility.cpp"             // linking fails without this


int main(int argc, char** argv) {
	
    /*
    process input and setup variables
    ------------------------------------------------------------------------------
    */
    
    srand((unsigned) time(NULL));

    // input parameters - ass1 polygonization
    std::string input_file;
    std::string output_file;
    int algorithm;
    int edge_selection=-1;
    int init_inc=-1;
    int init_onion=-1;

    // input parameters - ass2 optimization
    int opt_algorithm=-1;
    int minmax=-1;
    int LL=-1;
    double threshold=-1;
    int local_global_subdiv=-1;

    // vis options
    bool vis=false;    
    bool vis_min=false;  

    // get input from argc, argv
    io_manip::process_input(argc, 
                            argv, 
                            input_file, 
                            output_file, 
                            algorithm, 
                            edge_selection, 
                            init_inc, 
                            init_onion, 
                            opt_algorithm,
                            minmax,
                            LL,
                            threshold,
                            local_global_subdiv,
                            vis, 
                            vis_min);

    // read points and area of convex hull from input file
    Points points;
    NUM ch_area;
    io_manip::read_data(input_file,points,ch_area);
    int points_count = points.size(); // number of given points

    // these will be set after running a polygonization algorithm
    Polygon_2 poly_line;    // the simple polygon line corssing all points
    NUM poly_area;          // area calculated by algorithm
    NUM cgal_poly_area;     // area calculated by Polygon_2::area()
    NUM pick_area;          // area calculated with Pick's formula
    double run_time;        // rime taken to run algorithm
    int simple;             // if poly line is simple
    int points_poly_count;  // number of points on poly line

    // these will be set after running an optimization algorithm, same as above
    Polygon_2 opt_poly_line;
    NUM opt_poly_area;
    NUM opt_cgal_poly_area;
    NUM opt_pick_area;
    double opt_run_time;
    int opt_simple;             
    int opt_points_poly_count;  

    /*
    Assigment 1 - optimization
    ------------------------------------------------------------------------------
    */

    // optimization with simulated annealing and subdivizion is chosen
    // then polygonization happens with chosen parameters for each subset of points
    if (!(opt_algorithm==2 && local_global_subdiv==3)) {

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
        run_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

        // calculate cgal and pick areas
        cgal_poly_area = CGAL::abs(poly_line.area());
        pick_area = -1;//poly_line_algorithms::pick(poly_line);

        // calculate simple, points_count, points_poly_count
        simple = poly_line.is_simple();
        points_poly_count = poly_line.size();

        // print a short string with polygonization info, used for data analysis

        std::string alg_str;
        std::string inc_init_str;

        switch(algorithm) {
        case 1:
            alg_str = "incremental";
            switch(init_inc) {
                case 1: inc_init_str = "1b"; break;
                case 2: inc_init_str = "1a"; break;
                case 3: inc_init_str = "2b"; break;
                case 4: inc_init_str = "2a"; break;
            }
            break;
        case 2:
            alg_str = "convex_hull";
            inc_init_str="-1";
            break;
        case 3:
            alg_str = "onion";
            //write init_onion
            inc_init_str="-1";
            break;
        }

        std::cout<<alg_str<<","
        <<edge_selection<<","
        <<inc_init_str<<","
        <<init_onion<<","
        <<poly_area<<","
        <<cgal_poly_area<<","
        <<pick_area<<","
        <<std::to_string(poly_area*1.0/ch_area)<<","
        <<run_time<<","
        <<simple<<","
        <<points_count<<","
        <<points_poly_count<<"\n";

    }

    /*
    Assigment 2 - optimization
    ------------------------------------------------------------------------------
    */

    // if opt_algorithm==-1 then only polygonization is run
    if (opt_algorithm!=-1) {
        // "start" timer
        std::clock_t start;
        start = std::clock();

        opt_poly_line=poly_line;
        opt_poly_area=poly_area;

        // run optimization algorithm
        switch(opt_algorithm){
            case 1:
                opt_algorithms::local_search(points,LL,minmax,threshold,opt_poly_line,opt_poly_area,ch_area,vis,vis_min);
                break;
            case 2:
                opt_algorithms::simulated_annealing(points,LL,minmax,local_global_subdiv,algorithm,opt_poly_line,opt_poly_area,ch_area,vis,vis_min);
                break;
        }

        // "stop" timer
        opt_run_time= (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

        // print a short string with optimization info, used for data analysis

        opt_cgal_poly_area=CGAL::abs(opt_poly_line.area());
        opt_pick_area = -1;//poly_line_algorithms::pick(opt_poly_line);

        // calculate simple, points_count, points_poly_count
        opt_simple = opt_poly_line.is_simple();
        opt_points_poly_count = opt_poly_line.size();

        std::string opt_alg_str;
        std::string minmax_str;
        std::string local_global_subdiv_str;

        switch(opt_algorithm) {
        case 1:
            opt_alg_str = "local_search";
            break;
        case 2:
            opt_alg_str = "simulated_annealing";
            switch(local_global_subdiv) {
                case 1: local_global_subdiv_str = "local"; break;
                case 2: local_global_subdiv_str = "global"; break;
                case 3: local_global_subdiv_str = "subdiv"; break;
            }
            break;
        }

        switch(minmax) {
        case 1:
            minmax_str = "min";
            break;
        case 2:
            minmax_str = "max";
            break;
        }

        double opt_ratio_change=opt_cgal_poly_area*1.0/ch_area-poly_area*1.0/ch_area;
        if(local_global_subdiv==3) opt_ratio_change=-1;

        std::cout<<opt_alg_str<<","
        <<minmax_str<<","
        <<LL<<","
        <<threshold<<","
        <<local_global_subdiv<<","
        <<opt_poly_area<<","
        <<opt_cgal_poly_area<<","
        <<opt_pick_area<<","
        <<std::to_string(opt_cgal_poly_area*1.0/ch_area)<<","
        <<std::to_string(opt_ratio_change)<<","
        <<opt_run_time<<","
        <<opt_simple<<","
        <<points_count<<","
        <<opt_points_poly_count<<"\n";
    }

    /*
    save output
    ------------------------------------------------------------------------------
    */

    std::ofstream outfile(output_file);

    outfile << io_manip::create_output(poly_line,
                                       poly_area, 
                                       cgal_poly_area, 
                                       pick_area,
                                       ch_area, 
                                       run_time, 
                                       algorithm, 
                                       edge_selection, 
                                       init_inc, 
                                       init_onion,
                                       simple,
                                       points_count,
                                       points_poly_count,
                                       opt_poly_line,
                                       opt_poly_area, 
                                       opt_cgal_poly_area, 
                                       opt_pick_area,
                                       opt_run_time, 
                                       opt_algorithm, 
                                       minmax, 
                                       LL, 
                                       threshold,
                                       local_global_subdiv,
                                       opt_simple,
                                       opt_points_poly_count);
    
    outfile.close();


    return 0;
}


