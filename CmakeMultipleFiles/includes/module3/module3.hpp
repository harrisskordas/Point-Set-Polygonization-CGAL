#ifndef MODULE3_HPP
#define MODULE3_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;

using std::string;

namespace module3 {
    string module3_fun();
}

#endif
