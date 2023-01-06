from vis_app_includes.draw import *
from vis_app_includes.io_manip import *
from vis_app_includes.alg_steps import *


# interface for using the draws
def draw_alg(alg,vis_counter,f,vis_min):
    if alg == 'incremental':
        draw_inc(vis_counter,f,vis_min)
    if alg == 'convex_hull':
        draw_ch(vis_counter,f,vis_min)
    if alg == 'onion':
        draw_onion(vis_counter,f,vis_min)
    if alg in ['local_search','an_local','an_global']:
        draw_local_search(vis_counter,f,vis_min)
    if alg == "opt_subdiv":
        draw_subdiv(vis_counter,f,vis_min)  

# convex hull algorithm
##############################################################################################################################

def draw_min_ch(fig):
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    poly_line=points_from_file("visualisation/poly_line_")
    draw_poly(poly_line,fig)

def draw_ch(vis_counter,fig,vis_min):
    fig.clear()
    fig.axis("equal")
    
    if(vis_min):
        draw_min_ch(fig)
        return
    
    steps=get_steps_ch()
    
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    if vis_counter<steps-2:
        closest_points=points_from_file("visualisation/closest_"+str(vis_counter))    
        draw_points(closest_points,fig,c='r')

    if vis_counter>0 and vis_counter<steps-1:
        prev_poly_line=points_from_file("visualisation/poly_line_"+str(vis_counter-1))
        draw_poly(prev_poly_line,fig,c='r')
    
    if vis_counter==steps-1:
        vis_counter-=1
        
    poly_line=points_from_file("visualisation/poly_line_"+str(vis_counter))
    draw_poly(poly_line,fig)
    if vis_counter<steps-2:    
        draw_closest_connections(poly_line,closest_points,fig)

# incremental algorithm
##############################################################################################################################

def draw_min_inc(fig):
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    ch=points_from_file("visualisation/ch_")
    draw_poly(ch,fig,ls='--')
    
    poly_line=points_from_file("visualisation/poly_line_")
    draw_poly(poly_line,fig)

def draw_inc(vis_counter,fig,vis_min):
    fig.clear()
    fig.axis("equal")
    
    if(vis_min):
        draw_min_inc(fig)
        return
    
    steps=get_steps_inc()
    
    points=points_from_file("visualisation/points")
    draw_points(points,fig)

    ch=points_from_file("visualisation/ch_"+str(vis_counter))
    draw_poly(ch,fig,ls='--')

    poly_line=points_from_file("visualisation/poly_line_"+str(vis_counter))
    draw_poly(poly_line,fig)

    if vis_counter<steps-1:
        next_point=points[vis_counter+3]
        draw_point(next_point,fig,c='r')

        visible_points=points_from_file("visualisation/visible_"+str(vis_counter))
        draw_visibility_ch(visible_points,next_point,fig)

        edges_vis=edges_vis_from_file("visualisation/visible_poly_"+str(vis_counter))
        draw_visible_edges_poly(edges_vis,next_point,fig,c1='r',c2='b')
    return

# onion algorithm
##############################################################################################################################

def draw_min_onion(fig):
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    poly_line=points_from_file("visualisation/poly_line_")
    draw_poly(poly_line,fig)

def draw_onion(vis_counter,fig,vis_min):
    fig.clear()
    fig.axis("equal")
    
    if(vis_min):
        draw_min_onion(fig)
        return
    
    steps=get_steps_onion()
    
    points=points_from_file("visualisation/points")
    draw_points(points,fig)

    CHs=[]
    for i in range(steps):
        CHi=points_from_file("visualisation/ch_"+str(i))
        CHs+=[CHi]

    if vis_counter>0:
        for i in [vis_counter]:
            draw_poly(CHs[i],fig,c='r')

    for i in range(vis_counter+1,steps):
        draw_poly(CHs[i],fig,c='b')

  
    poly_line=points_from_file("visualisation/poly_line_"+str(vis_counter))
    draw_poly(poly_line,fig)

    return

# local search algorithm
##############################################################################################################################

def draw_min_local_search(fig):
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    poly_line=points_from_file("visualisation/opt_poly_line_")
    draw_poly(poly_line,fig)

def draw_local_search(vis_counter,fig,vis_min):
    fig.clear()
    fig.axis("equal")
    
    if(vis_min):
        draw_min_local_search(fig)
        return
    
    steps=get_steps_local_search()
    
    points=points_from_file("visualisation/points")
    draw_points(points,fig)

    poly_line=points_from_file("visualisation/opt_poly_line_"+str(vis_counter))
    draw_poly(poly_line,fig)

    return

# local search algorithm
##############################################################################################################################

def draw_min_subdiv(fig):
    points=points_from_file("visualisation/points")
    draw_points(points,fig)
    
    poly_line=points_from_file("visualisation/opt_poly_line_")
    draw_poly(poly_line,fig)

def draw_subdiv(vis_counter,fig,vis_min):
    fig.clear()
    fig.axis("equal")
    
    if(vis_min):
        draw_min_subdiv(fig)
        return
    
    steps=get_steps_subdiv()
    
    points=points_from_file("visualisation/points")
    draw_points(points,fig)

    sub_polys=[]
    for i in range(vis_counter+1,steps+1):
        sub_poly_i=points_from_file("visualisation/opt_sub_points_"+str(i))
        sub_polys+=[sub_poly_i]

    for i in range(len(sub_polys)):
        draw_poly(sub_polys[i],fig,c='b')

    poly_line=points_from_file("visualisation/opt_poly_line_"+str(vis_counter))
    draw_poly(poly_line,fig)
    

    return