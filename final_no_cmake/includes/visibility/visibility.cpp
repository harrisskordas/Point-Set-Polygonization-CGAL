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

    // return -1 if there are no visible points
    if (visible_indexes.size()==0)
        return -1;

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
            // we dont care if intersection is at the defining points of the edges - most of the times
            if (*p==seg1.source() || *p==seg1.target() || *p==seg2.source() || *p==seg2.target()) {
                // handle the case where an intersection happens at a source/target go one edge
                // and the interior of the other (like a T shape)
                if ( (*p!=seg1.source() && *p!=seg1.target()) || ((*p!=seg2.source() && *p!=seg2.target())) ) 
                    return true;

                // edges are just touching at their source/target
                return false;
            }
            // intersection inside the edges - return true
            else 
                //std::cout<<"VISIBILTY HERE: "<<seg1<<" - "<<seg2<<"\n";
                return true;
        } 
        // intersection is segment - this shouldn't happen - i guess?
        else {
            const Segment_2 *s = boost::get<Segment_2>(&*inter);
            //std::cout << "LOOK AT ME!!! - Intersection is segment: " << *s << "\n";
            //std::cout << "seg1, seg2: " << seg1 <<" "<<seg2 << "\n";
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

    // ./to_polygon -i input_data/uniform-0000060-2.instance -o data.out -algorithm incremental -edge_selection 2 -initialization 1a
    // point is colinear with edge
    if (are_intersecting(e1,e2) || are_intersecting(e2,e3) || are_intersecting(e1,e3))
        return false;

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

// overloaded version to call with edges
bool visibility::is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Edges& edges) {
    // suppose edge is visible
    bool is_visible=true;

    // create e1,e2,e3 - segment from point and edge source/target/midpoint
    Point_2 p1(edge.source()), p2(edge.target());
    Segment_2 e1(point,p1), e2(point,p2), e3(point,CGAL::midpoint(edge));

    // ./to_polygon -i input_data/uniform-0000060-2.instance -o data.out -algorithm incremental -edge_selection 2 -initialization 1a
    // point is colinear with edge
    if (are_intersecting(e1,e2) || are_intersecting(e2,e3) || are_intersecting(e1,e3))
        return false;

    // for each edge e of poly_line
    for(const Segment_2& e : edges){
        // if e is equal to the edge we are checking for visibility -> skip it
        if (e==edge) continue;
        // if e1,e2 or e3 is intersecting with e, return false
        if (are_intersecting(e,e1) || are_intersecting(e,e2) || are_intersecting(e,e3))
            return false;
    }

    // e1,e2 and e3 intersect with no edge of the poly -> return true
    return true;
}

// overloaded version to call with extra edges and edges to skip
bool visibility::is_visible_p_from_e(const Point_2& point, const Segment_2& edge, const Polygon_2& poly_line, const Edges& skip_edges, const Edges& extra_edges) {
    // suppose edge is visible
    bool is_visible=true;

    // create e1,e2,e3 - segment from point and edge source/target/midpoint
    Point_2 p1(edge.source()), p2(edge.target());
    Segment_2 e1(point,p1), e2(point,p2), e3(point,CGAL::midpoint(edge));

    // ./to_polygon -i input_data/uniform-0000060-2.instance -o data.out -algorithm incremental -edge_selection 2 -initialization 1a
    // point is colinear with edge
    if (are_intersecting(e1,e2) || are_intersecting(e2,e3) || are_intersecting(e1,e3))
        return false;

    // for each edge e of poly_line
    for(const Segment_2& e : poly_line.edges()){
        // if e is equal to the edge we are checking for visibility -> skip it
        if (e==edge) continue;
        for (const Segment_2& e_skip : skip_edges)
            if (e == e_skip) continue;
        // if e1,e2 or e3 is intersecting with e, return false
        if (are_intersecting(e,e1) || are_intersecting(e,e2) || are_intersecting(e,e3))
            return false;
    }
    
    // for each edge e in extra edges
    for(const Segment_2& e : extra_edges){
        // if e is equal to the edge we are checking for visibility -> skip it
        if (e==edge) continue;
        for (const Segment_2& e_skip : skip_edges)
            if (e == e_skip) continue;
        // if e1,e2 or e3 is intersecting with e, return false
        if (are_intersecting(e,e1) || are_intersecting(e,e2) || are_intersecting(e,e3))
            return false;
    }

    // e1,e2 and e3 intersect with no edge of the poly -> return true
    return true;
}


// assignment 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Checks if a segment is intersecting with a polygon.
*/ 
bool visibility::are_intersecting(const Segment_2& seg_check, const Polygon_2& poly_line){
    for (const Segment_2& seg:poly_line.edges()) {
        if (seg == seg_check) continue;
        if (visibility::are_intersecting(seg,seg_check)) {
            return true;
        }
    }
    return false;
}

/*
Checks if two polygons can be connected and return possible bridges
*/ 
bool visibility::can_connect_polys_simple(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges){
    // both polys are one point - return a trivial bridge
    if (poly1.size()==1 && poly2.size()==1) {
        bridges.push_back(Segment_2(poly1[0],poly2[0]));
        bridges.push_back(Segment_2(poly1[0],poly2[0]));
        return true;
    }

    if (poly1.size()==1) {
        // here we gooo
        Point_2 point=poly1[0];

        for (const Segment_2& seg:poly2.edges()) {
            if (visibility::is_visible_p_from_e(point,seg,poly2)){
                bridges.push_back(Segment_2(point,seg.source()));
                bridges.push_back(Segment_2(point,seg.target()));
            }
        }
        
        return bridges.size()>0;
    }

    if (poly2.size()==1) {
        // here we gooo
        Point_2 point=poly2[0];

        for (const Segment_2& seg:poly1.edges()) {
            if (visibility::is_visible_p_from_e(point,seg,poly1)){
                bridges.push_back(Segment_2(seg.source(),point));
                bridges.push_back(Segment_2(seg.target(),point));
            }
        }
        
        return bridges.size()>0;
    }

    return false;

}

/*
Checks if two polygons can be connected and return possible bridges
*/ 
bool visibility::can_connect_polys(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges,bool just_one){


    // edge case - one or both poly are just a point
    if (poly1.size()==1 || poly2.size()==1)
        return can_connect_polys_simple(poly1, poly2, bridges);

    if (!poly1.is_simple() || !poly2.is_simple()) return false;


    Edges edge_check1;
    for (const Segment_2& seg:poly1.edges())
        edge_check1.push_back(seg);

    Edges edge_check2;
    for (const Segment_2& seg:poly2.edges())
        edge_check2.push_back(seg);

    // only two points so practically one edge
    if (edge_check1.size()==2)
        edge_check1.pop_back();
    if (edge_check2.size()==2)
        edge_check2.pop_back();

    // check all edge combinations of the two polygons
    for (const Segment_2& seg1:edge_check1){ 
        for (const Segment_2& seg2:edge_check2){
            // get points from edges
            Point_2 e11=seg1.source();
            Point_2 e12=seg1.target();

            Point_2 e21=seg2.source();
            Point_2 e22=seg2.target();

            // create the two possible bridges of the edges
            Segment_2 bridge11(e11,e21);
            Segment_2 bridge12(e12,e22);

            Segment_2 bridge21(e11,e22);
            Segment_2 bridge22(e12,e21);

            // check first bridge
            if((   !visibility::are_intersecting(bridge11,bridge12)) // bridges not intersecting
                && !are_intersecting(bridge11,poly1) // bridges not intersecting poly 1
                && !are_intersecting(bridge12,poly1)
                && !are_intersecting(bridge11,poly2) // bridges not intersecting poly 2
                && !are_intersecting(bridge12,poly2)                
                ) 
            {
                bridges.push_back(bridge11);
                bridges.push_back(bridge12);
                if (just_one) return true;    
            }

            // check second bridge
            if((   
                   !visibility::are_intersecting(bridge21,bridge22)) // bridges not intersecting
                && !are_intersecting(bridge21,poly1) // bridges not intersecting poly 1
                && !are_intersecting(bridge22,poly1)
                && !are_intersecting(bridge21,poly2) // bridges not intersecting poly 2
                && !are_intersecting(bridge22,poly2)                
                ) 
            {
                bridges.push_back(bridge21);
                bridges.push_back(bridge22);  
                if (just_one) return true;             
            }

        }
    }

    return bridges.size()>0;
}

/*
get a polygon from a starting point to a final point - the long way around
*/
Polygon_2 visibility::get_from_to(const Polygon_2& poly, const Point_2& from, const Point_2& to) {
    Polygon_2 re_indexed_poly;
    int size=poly.size();

    // edges cases - 1 or 2 points
    if (size==1)
        return poly;
    if (size==2) {
        re_indexed_poly.push_back(from);
        re_indexed_poly.push_back(to);
        return re_indexed_poly;
    }

    // 3 or more points
    int ind_from,ind_to;
    for(int i=0;i<size;i++) {
        if (poly[i]==from) ind_from=i;
        if (poly[i]==to) ind_to=i;
    }

    // if next point of from is to then we have to reverse the points
    // to go the long way around
    int step=1;
    if (((ind_from+1)%size)==ind_to) step=-1;

    // do the reindexing
    for (int i=0;i<size;i++)
        re_indexed_poly.push_back(poly[(size+ind_from+step*i)%size]);

    return re_indexed_poly;
}

/*
connect two polygons at bridge (bridge is from poly 1 to poly 2)
*/ 
Polygon_2 visibility::connect_polys_at_bridge(const Polygon_2& poly1, const Polygon_2& poly2, Edges& bridges) {
    Polygon_2 connected_poly;

    Segment_2 bridge1=bridges[0];
    Segment_2 bridge2=bridges[1];

    Polygon_2 re_indexed_poly1 = get_from_to(poly1,bridge1.source(),bridge2.source());
    Polygon_2 re_indexed_poly2 = get_from_to(poly2,bridge2.target(),bridge1.target());

    for (const Point_2& p:re_indexed_poly1)
        connected_poly.push_back(p);
    for (const Point_2& p:re_indexed_poly2)
        connected_poly.push_back(p);

    return connected_poly;
}