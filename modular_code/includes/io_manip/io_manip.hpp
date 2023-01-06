#ifndef io_manip_hpp
#define io_manip_hpp

/*
This header file contains functions for manipulating input and output of CGAL types,
points, polygons, segments, from and to files or streams.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include <sstream>                        // std::ifstream

namespace io_manip{

    /*
    Reads points and convex hull area from path. Expected format of input is:
        <line describing point set>
        parameters "convex_hull": {"area": "x"} // where x is the ch area
        0 x0 y0
        1 x1 y1
        ...
        n-1 xn yn
    */ 
    void read_data(const std::string& path, Points& points, NUM& ch_area);


    /*
    check and get input from argc, argv
    general use: ./to_polygon -i <input_file> -o <output_file> -algorithm <algorithm> -edge_selection <edge_selection> -initialization <initialization> -onion_initialization <onion_initialization>
    check if -i, -o, -algorithm, -edge_selection, -initialisation, -onion_initialization are present 
    and extract the corresponding values
    the paramaters do not need to be in that order
    specific parameters need to be present based on given algorithm
    */                             
    void process_input(int argc, 
                       char** argv, 
                       std::string& inpout_file,
                       std::string& output_file,
                       int& algorithm,
                       int& edge_selection,
                       int& init_inc,
                       int& init_onion,
                       bool& vis,
                       bool& vis_min);
                       
    /*
    Creates an output string with the format below:
        Polygonization
        <points>
        <edges>
        Algorithm: <incremental or convex_hull or onion>_edge_selection<1 or 2 or 3
                   where applicable>_initialization<incremental and onion options accordingly>
        area: <area calculated during algorithm>
        cgal_area: <area calculated from built in cgal function>
        pick_calculated_area: <pick area (-1 not implemented)>
        ratio: <poly_area / ch_area>
        construction time: <milliseconds as integer> 
    */ 
    std::string create_output(const Polygon_2& poly_line,
                              const NUM& poly_area,
                              const NUM& cgal_poly_area, 
                              const NUM& pick_area,
                              const NUM& ch_area,
                              const double& time,
                              const int& algorithm,
                              const int& edge_selection,
                              const int& init_inc,
                              const int& init_onion,
                              const int& simple,
                              const int& points_count,
                              const int& points_poly_count);                            

    /*
    Functions below use templates so they are defined here in .hpp.
    */

    /* 
    Save points or segments in container to file at path.

    Used for visualisation with Python.
    */
    template <typename Container> // Points or Polygon_2 or Segments
    void save_points_to_file(const Container& container, const std::string& path){
        std::ofstream outfile(path);

        for (int i=0;i<container.size();i++)
            outfile << container[i] << std::endl;

        outfile.close();
    }

    /* 
    Save points or segments in container to file at path.
    Uses a visibility vector with a value for each point.
    If visibility[i]==False for a point, then instead of the point, "NOT_VISIBLE" is written.

    Used for visualisation with Python.
    */
    template <typename Container> // Points or Polygon_2 or Segments
    void save_points_to_file(const Container& container,const std::vector<bool>& visibility_vec, const std::string& path){
        std::ofstream outfile(path);

        for (int i=0; i<container.size(); i++){
            if (visibility_vec[i])
                outfile << container[i] << std::endl;
            else
                outfile << "NOT_VISIBLE" << std::endl;
        } 

        outfile.close();
    }

    /*
    Save points or segments in container and visibility to file at path.
    
    Used for visualisation with Python.
    */
    template <typename Container> 
    void save_points_and_vis_to_file(const Container& container, const std::vector<bool> visibility_vec, const std::string& path) {
       std::ofstream outfile(path);

        for (int i=0;i<container.size();i++)
            outfile << container[i] <<" "<<visibility_vec[i]<< std::endl;

        outfile.close();
    }

    // print points or segments in container to cout
    template <typename Container>    
    void print_points(const Container& container){
        for (int i=0;i<container.size();i++)
            std::cout << container[i] << std::endl; 
    }

}

#endif
