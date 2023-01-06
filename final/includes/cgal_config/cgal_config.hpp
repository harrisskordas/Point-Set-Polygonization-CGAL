#ifndef cgal_config_hpp
#define cgal_config_hpp

/*
This header file contains typedefs for CGAL types and vectors
used throughout the whole Project. It is included in most files.
*/

// kernel setup
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2         Point_2;
typedef K::Segment_2       Segment_2;
typedef K::Triangle_2      Triangle_2;

#include <CGAL/Polygon_2.h>         
typedef CGAL::Polygon_2<K> Polygon_2;

// vectors and iterators
typedef std::vector<Point_2>    Points;
typedef std::vector<Segment_2>  Edges;

// number type for calculated areas
typedef long long int NUM;

#endif
