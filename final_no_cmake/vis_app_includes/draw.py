import numpy as np
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
matplotlib.use("TkAgg")

# functions for drawing on PySimpleGUI using plt

# used to draw a figure on our UI
def draw_figure(canvas, figure):
    figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
    figure_canvas_agg.draw()
    figure_canvas_agg.get_tk_widget().pack(side="top", fill="both", expand=1)
    return figure_canvas_agg

# necessary to reset the figure on the UI, without this each draw happens below the previous one
def delete_figure_agg(figure_agg):
    figure_agg.get_tk_widget().forget()

# functions for drawing points, lines, polygons, visibility on figures from plt

# draws a line
def draw_line(p1,p2,fig,c='k',lw=1.5,ls='-'):
    fig.plot([p1[0],p2[0]], [p1[1],p2[1]], c, linestyle=ls,linewidth=lw)

# draws an arrow
def draw_arrow(p1,p2,fig,c='k'):
    x=p1[0]
    y=p1[1]
    dx=p2[0]-p1[0]
    dy=p2[1]-p1[1]
    size=np.sqrt(dx*dx+dy*dy)
    hl=200
    hw=200
    hlx=hl*dx/size
    hly=hl*dy/size
    fig.arrow(x,y,dx-hlx,dy-hly, head_width=hw, head_length=hl, fc=c, ec=c)

# draws a point, uses visibility
def draw_point(p,fig,c='k'):
    if(p=="NOT_VISIBLE"):
        return
    fig.plot([p[0]], [p[1]], c+'o',markersize=4)

# draw a list of points
def draw_points(points,fig,c='k'):
    for point in points:
        draw_point(point,fig,c)

# draws a polygon
def draw_poly(CH,fig,c='k',lw=1.5,ls='-'):
    n=len(CH)
    for i in range(n):
        draw_line(CH[i],CH[(i+1)%n],fig,c,lw,ls)

# draws a polygon with arrows
def draw_poly_dir(CH,fig,c='k'):
    n=len(CH)
    for i in range(n):
        draw_arrow(CH[i],CH[(i+1)%n],fig,c)

# draws edges represnting visiblity (used for ch)
def draw_closest_connections(CH,closest_points,fig):
    n=len(CH)
    for i in range(n):
        if closest_points[i]=="NOT_VISIBLE":
            continue        
        draw_line(CH[i],closest_points[i],fig,c='b',ls='--')
        draw_line(closest_points[i],CH[(i+1)%n],fig,c='b',ls='--')

# draws a polygon - skips last edge
def draw_poly_open(CH,fig,c='k',lw=1.5,ls='-'):
    n=len(CH)
    for i in range(n-1):
        draw_line(CH[i],CH[(i+1)%n],fig,c,lw,ls)

# draws edges representing visibilty
def draw_visibility(visible_points,point,fig):
    n=len(visible_points)
    for i in range(n):      
        draw_line(visible_points[i],point,fig,c='b',ls='--')

# draws 2 edges represnting the visibilty of the new point on incremental 
# towards the last ch
def draw_visibility_ch(visible_points,point,fig,c='k'):    
    draw_line(visible_points[0],point,fig,c,ls='--')
    draw_line(visible_points[-1],point,fig,c,ls='--')

# draws all visibility lines towards previous ch (similar to function above)
def draw_visibility_ch_full(visible_points,point,fig):
    n=len(visible_points)
    for i in range(n):      
        draw_line(visible_points[i],point,fig,c='b',ls='--')
        if i+1<n:
            draw_line(visible_points[i],visible_points[i+1],fig,c='r',ls='--')

# draws vibility lines on visible edges - used for incremental
def draw_visible_edges_poly(edges_vis,point,fig,c1='r',c2='b'):
    for edge_vis in edges_vis:
        p1,p2=edge_vis[0]
        vis=edge_vis[1]
        draw_line(p1,p2,fig,c1)
        
        if vis=="1":
            draw_line(p1,point,fig,c2,ls='--')
            draw_line(p2,point,fig,c2,ls='--')

# draws a list of edges
def draw_edges(edges,fig,c='r'):
    for edge in edges:
        p1,p2=edge
        draw_line(p1,p2,fig,c)