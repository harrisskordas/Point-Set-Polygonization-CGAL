from vis_app_includes.io_manip import *
import os

# interface for getting step count for each algorithm
def get_steps_alg(alg):
    if alg == 'incremental':
        return get_steps_inc()
    if alg == 'convex_hull':
        return get_steps_ch()
    if alg == 'onion':
        return get_steps_onion()
    if alg in ['local_search','an_local','an_global']:
        return get_steps_local_search()  
    if alg == "subdiv":
        return 0 
    if alg == "opt_subdiv":
        return get_steps_subdiv()   

# incremental
def get_steps_inc():
    points=points_from_file("visualisation/points")    
    steps = len(points)-2
    return steps

# convex hull
def get_steps_ch():
    points=points_from_file("visualisation/points")
    poly_line=points_from_file("visualisation/poly_line_0")
    steps = len(points) - len(poly_line)
    return steps+2

# onion
def get_steps_onion():
    count=0
    for filename in os.listdir("visualisation"):
        if filename.find("ch")!=-1:
            count+=1
    return count

# local search (and simulated annealing local and global)
def get_steps_local_search():
    count=0
    for filename in os.listdir("visualisation"):
        if filename.find("opt_poly_line_")!=-1:
            count+=1
    return count-1

# simulated annealing subdiv
def get_steps_subdiv():
    count=0
    for filename in os.listdir("visualisation"):
        if filename.find("opt_sub_points_")!=-1:
            count+=1
    return count-1