#ifndef MODULE2_HPP
#define MODULE2_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;

using std::string;

namespace module2 {
    string module2_fun();
}

#endif
