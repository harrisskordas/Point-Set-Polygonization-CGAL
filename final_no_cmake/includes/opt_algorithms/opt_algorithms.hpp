#ifndef opt_algorithms_hpp
#define opt_algorithms_hpp    

/*
This header file contains calls for algorithms that given a simple polygonal line
make optimizations to increase or decrease its area while retaining its simplicity.

Currently contains:
    1 - local search algorithm
    2 - simulated annealing algorithm

This is supposed to be the main interface for the project, for assigment 2.
*/

#include "../cgal_config/cgal_config.hpp"                         // CGAL types
#include "../opt_local_search/opt_local_search.hpp"               // local search algorithm
#include "../opt_simulated_annealing/opt_simulated_annealing.hpp" // simulated annealing algorithm

namespace opt_algorithms {
    
    // calls the local search algorithm
    void local_search(const Points& points, 
                      const int& LL, 
                      const int& minmax, 
                      const double& threshold,
                      Polygon_2& poly_line,
                      NUM& poly_area,
                      const NUM& ch_area,
                      const bool& vis=false,
                      const bool& vis_min=false) {
    
        opt_local_search::run(points,LL,minmax,threshold,poly_line,poly_area,ch_area,vis,vis_min);
    }

    // calls simulated annealing algorithm
    void simulated_annealing(const Points& points, 
                             const int& LL, 
                             const int& minmax, 
                             const int& local_global_subdiv,
                             const int& poly_algorithm,
                             Polygon_2& poly_line,
                             NUM& poly_area,
                             const NUM& ch_area,
                             const bool& vis=false,
                             const bool& vis_min=false) {
    
        opt_simulated_annealing::run(points,LL,minmax,local_global_subdiv,poly_algorithm,poly_line,poly_area,ch_area,vis,vis_min);
    }                               

}

#endif
