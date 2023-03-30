#include "pick.hpp"

/*
Find points inside and on poly line and use Pick's 
formula to calculate are of poly line.

Pick's formula: A = I + B/2 - 1
    A: area of poly
    I: number of latice points inside poly
    B: number of latice points on poly
*/
double pick::run(Polygon_2& poly_line) {

    // find bounding box
    CGAL::Bbox_2 bb = poly_line.bbox();

    // for picks formula
    NUM I=0,B=0;

    // check all points insde bounding box
    for (NUM x=bb.xmin(); x<=bb.xmax(); x++){
        for (NUM y=bb.ymin(); y<=bb.ymax(); y++){
            CGAL::Bounded_side side = poly_line.bounded_side(Point_2(x,y));
            switch(side) {
                // inside poly_line -> increase I
                case CGAL::ON_BOUNDED_SIDE: I++; break;
                // on poly_line -> increase B
                case CGAL::ON_BOUNDARY: B++; break;
                // outside poly_line -> do nothing
                case CGAL::ON_UNBOUNDED_SIDE: break;                
            }
        }
    }

    // return formula of A
    return I+B/2.0-1;
}