# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)

project( to_polygon )


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory

add_subdirectory(includes/cgal_config)
list(APPEND EXTRA_LIBS cgal_config)

add_subdirectory(includes/visibility)
list(APPEND EXTRA_LIBS visibility)

add_subdirectory(includes/io_manip)
list(APPEND EXTRA_LIBS io_manip)

add_subdirectory(includes/poly_incremental)
list(APPEND EXTRA_LIBS poly_incremental)

add_subdirectory(includes/poly_from_ch)
list(APPEND EXTRA_LIBS poly_from_ch)

add_subdirectory(includes/poly_line_algorithms)
list(APPEND EXTRA_LIBS poly_line_algorithms)

add_subdirectory(includes/poly_onion)
list(APPEND EXTRA_LIBS poly_onion)

add_subdirectory(includes/pick)
list(APPEND EXTRA_LIBS pick)

add_subdirectory(includes/opt_local_search)
list(APPEND EXTRA_LIBS opt_local_search)

add_subdirectory(includes/opt_algorithms)
list(APPEND EXTRA_LIBS opt_algorithms)

add_subdirectory(includes/opt_simulated_annealing)
list(APPEND EXTRA_LIBS opt_simulated_annealing)

add_subdirectory(includes/evaluate_helpers)
list(APPEND EXTRA_LIBS evaluate_helpers)

# include for local package


# Creating entries for target: to_polygon
# ############################

add_executable( to_polygon  main.cpp )

add_executable( evaluate evaluate.cpp )

add_to_cached_list( CGAL_EXECUTABLE_TARGETS to_polygon )

# Link the executable to CGAL and third-party libraries
target_link_libraries(to_polygon PRIVATE CGAL::CGAL ${EXTRA_LIBS})

