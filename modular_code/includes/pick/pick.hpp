#ifndef pick_hpp
#define pick_hpp

/*
This header file contains an implementation of an algorithm
to calculate area by Pick's formula.
*/

#include "../cgal_config/cgal_config.hpp" // CGAL types
#include <boost/optional/optional_io.hpp> // for intersection

namespace pick {
        
    /*
    Find points inside and on poly line and use Pick's 
    formula to calculate are of poly line.

    Pick's formula: A = I + B/2 - 1
        A: area of poly
        I: number of latice points inside poly
        B: number of latice points on poly
    */
    double run(Polygon_2& poly_line);

}

#endif
