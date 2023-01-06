#ifndef opt_simulated_annealing_hpp
#define opt_simulated_annealing_hpp

/*
This header file contains an implementation of the simulated annealing
optimiazation algorithm as discussed in class. We have 3 options:

    1 - local steps:
    make L local steps and apply them or not based on mitropolis criterion

    2 - global steps:
    make L global single point steps and apply them or not based on mitropolis criterion

    3 - subdivision
    split points and for each set:
        - polygonize with given algorithm
        - optimze with global single point steps
            -> make L global single point steps and apply them or not based on mitropolis criterion
    connect the sets of points
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include "../visibility/visibility.hpp"   // visibility checks
#include "../io_manip/io_manip.hpp"       // write points to files (used mainly for visualisation)
#include <sys/stat.h>                     // mkdir() (used to create visualisation folder)
#include <CGAL/convex_hull_2.h>           // needed for subdiv with poly from ch
// polygonization algorithms and local search for subdiv
#include "../poly_incremental/poly_incremental.hpp" // incremental algorithm
#include "../poly_from_ch/poly_from_ch.hpp"         // convex hull algorithm
#include "../poly_onion/poly_onion.hpp"             // onion algorithm
#include "../opt_local_search/opt_local_search.hpp" // local search opt algorithm

// for kd-tree search
#include <CGAL/Kd_tree.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Bbox_2.h>
typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;
// for e and pow - used for metropolis
#include <cmath>
#define _USE_MATH_DEFINES

class opt_simulated_annealing {
    private:
        /*
        Algorithm parameters below.
        */
        
        static int LL;                    // number of changes
        static int minmax;                // 1,2 -> min or max optimization
        static int local_global_subdiv;   // 1 for local, 2 for global, 3 for subdiv
        static bool vis;                  // visualisation at each step of the algorithm
        static bool vis_min;              // minimal visualisation, points and final poly line

        /*
        If full visualisation is picked, at each step we save information on the files below.
        vis_counter counts the step number and is increased on each step.
        */
        static int vis_counter;              // counts step
        static std::string vis_points;       // saves points
        static std::string vis_poly_line;    // saves poly line
        static std::string vis_sub_points;   // saves point subsets

        // Get a random number
        static int get_r(int max) {
            int random = rand()%max;
            return random;
        }

        // get a random number in [0,1]
        static double get_uniform_R() {
            return ((double)(rand())/(double)(RAND_MAX));
        }

        // calculate energy from the formulas given in class
        static double get_energy(int minmax, int size, NUM poly_area, NUM ch_area) {
            if (minmax==2) return size*poly_area*1.0/ch_area; //max
            // minmax==1
            else return size*(1-poly_area*1.0/ch_area); //min
        }

        // caculate mitropolis e ^ -DE/T >= R
        static bool get_mitropolis(double energy_change,double T,double R) {
            return pow(M_E,energy_change/T) >= get_uniform_R();
            //return pow(M_E,-energy_change/T) >= R;
        }

        // decide to make a change or not on the polygon
        static bool take_it_or_leave_it(double energy_change,double T,double R){
            // keep better poly
            if(energy_change>0) return true;
            return false;

            // decide with mitropolis to keep change or not
            // caculate mitropolis e ^ -DE/T >= R
            return get_mitropolis(energy_change,T,R); 
            
            double r=get_uniform_R();
            //std::cout<<pow(T-0.04,20)<<" "<<r<<"\n";           
            //return pow(T-0.04,20) > r;
        }

        // run simulated annealing with local search
        static void local(const Points& points, 
                          const int& LL, 
                          const int& minmax, 
                          const int& local_global_subdiv,
                          const int& poly_algorithm,
                          Polygon_2& poly_line,
                          NUM& poly_area,
                          const NUM& ch_area,
                          const bool& vis=false,
                          const bool& vis_min=false);

        // run simulated annealing with global search
        static void global(const Points& points, 
                           const int& LL, 
                           const int& minmax, 
                           const int& local_global_subdiv,
                           const int& poly_algorithm,
                           Polygon_2& poly_line,
                           NUM& poly_area,
                           const NUM& ch_area,
                           const bool& vis=false,
                           const bool& vis_min=false);

        // run simulated annealing with subdivision
        static void subdiv(const Points& points, 
                           const int& LL, 
                           const int& minmax, 
                           const int& local_global_subdiv,
                           const int& poly_algorithm,
                           Polygon_2& poly_line,
                           NUM& poly_area,
                           const NUM& ch_area,
                           const bool& vis=false,
                           const bool& vis_min=false);                                        

    public:
    /*
    Runs the simulated annealing algorithm on a simple polygon, with given L,
    min or max option and local, global or subdiv option. 

    poly_line and poly_area hold the calculated polygonal line and its area after
    completion of execution.

    Also takes visualisation options.
    */    
    static void run(const Points& points, 
                    const int& LL, 
                    const int& minmax, 
                    const int& local_global_subdiv,
                    const int& poly_algorithm,
                    Polygon_2& poly_line,
                    NUM& poly_area,
                    const NUM& ch_area,
                    const bool& vis=false,
                    const bool& vis_min=false);
};

#endif
