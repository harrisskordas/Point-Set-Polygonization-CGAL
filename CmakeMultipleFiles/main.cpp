#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>

#include "includes/module1/utils.hpp"
#include "includes/module2/module2.hpp"
#include "includes/module3/module3.hpp"

typedef K::Point_2                                          Point_2;
typedef K::Segment_2                                        Segment_2;
typedef std::vector<Point_2> Points;

int main()
{
  // create a polygon and put some points in it
  Polygon_2 p, chp;
  p.push_back(Point_2(8,0));
  p.push_back(Point_2(8,4));
  p.push_back(Point_2(4,0));
  p.push_back(Point_2(0,4));
  p.push_back(Point_2(0,0));
  for(const Point_2& p : p.vertices()){
    std::cout << p << std::endl;
  }
  // As the range is not light weight, we have to use a reference
  const Polygon_2::Vertices& range = p.vertices();
  Points result;
  for(auto it = range.begin(); it!= range.end(); ++it){
    std::cout << *it << std::endl;
  }
  for(const Segment_2& e  : p.edges()){
    std::cout << e << std::endl;
  }
  std::cout << p.area() << std::endl;

  CGAL::convex_hull_2(range.begin(), range.end(), std::back_inserter(result));
  for(auto it = result.begin(); it!= result.end();++it)
    chp.push_back(*it);
  std::cout << chp.area() << std::endl;

  std::cout << utils::simple_or_not(p) << std::endl;

  std::cout << module2::module2_fun() << std::endl;

  std::cout << module3::module3_fun() << std::endl;

  return EXIT_SUCCESS;
}

