#include "visibility.hpp"

/*
When we want to add an edge or point to the poly line we are concerned:
    - wether it is visible
    - the area of the triangle created by the point and its paired edge

This function chooses from a set of points, an index of a point that is visibile
and has min or max area. The choice can also be made at random. 

edge_selection = 1,2,3 -> random, min area, max area
*/ 
int visibility::choose_index(const std::vector<bool>& visibility_vec,const std::vector<NUM>& areas,const int& edge_selection){
    // only choose from points that are visible from their respective edge
    std::vector<int> visible_indexes;
    std::vector<NUM> visible_areas;

    // select visible indexes and areas
    for (int i=0; i<visibility_vec.size(); i++) {
        if (visibility_vec[i]) {
            visible_indexes.push_back(i);
            visible_areas.push_back(areas[i]);
        }
    }

    // choose an index from the visible options available
    int i;
    switch(edge_selection) {
        case 1: // random pick
            std::srand(std::time(0));
            i = std::rand()%visible_indexes.size();
            break;
        case 2: // min area
            i = std::distance(std::begin(visible_areas), std::min_element(std::begin(visible_areas), std::end(visible_areas)));
            break;
        case 3: // max area
            i = std::distance(std::begin(visible_areas), std::max_element(std::begin(visible_areas), std::end(visible_areas)));
            break;
    }

    // return the actual index mapped to the chosen index i
    return visible_indexes[i];
}


/*
Checks if two segments are intersecting. Used to check visibility.
*/ 
bool visibility::are_intersecting(const Segment_2& seg1, const Segment_2& seg2) {
    auto inter = intersection(seg1,seg2);

    // intersection handling
    if (inter) {
        // intresection is point
        if (const Point_2 *p = boost::get<Point_2>(&*inter)){
            // we dont care if intersection is at the defining points of the edges
            if (*p==seg1.source() || *p==seg1.target() || *p==seg2.source() || *p==seg2.target())
                return false;
            // intersection inside the edges - return true
            else 
                return true;
        } 
        // intersection is segment - this shouldn't happen - i guess?
        else {
            const Segment_2 *s = boost::get<Segment_2>(&*inter);
            std::cout << "LOOK AT ME!!! - Intersection is segment: " << inter << "\n";
            return true;
        }
    // no intersection return false
    } else {
        return false;
    }
}

/*
Checks if edge is visible from point, given a poly_line.

The edge is supposed to be part of the poly line. 

The check is made by checking if any edge of the poly line intersects with three
visibility segments strating from point and ending at the edge.

The three visibility segments checked are, if edge=(p1,p2):
    - e1=(point,p1)
    - e2=(point,p2)
    - e3=(point,midpoint of edge)
*/ 
bool visibility::is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Polygon_2& poly_line) {
    // suppose edge is visible
    bool is_visible=true;

    // create e1,e2,e3 - segment from point and edge source/target/midpoint
    Point_2 p1(edge.source()), p2(edge.target());
    Segment_2 e1(point,p1), e2(point,p2), e3(point,CGAL::midpoint(edge));

    // for each edge e of poly_line
    for(const Segment_2& e : poly_line.edges()){
        // if e is equal to the edge we are checking for visibility -> skip it
        if (e==edge) continue;
        // if e1,e2 or e3 is intersecting with e, return false
        if (are_intersecting(e,e1) || are_intersecting(e,e2) || are_intersecting(e,e3))
            return false;
    }

    // e1,e2 and e3 intersect with no edge of the poly -> return true
    return true;
}

