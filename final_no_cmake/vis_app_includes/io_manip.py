# functions for getting points or edges from files

# get points from a file on path
def points_from_file(path):      
    file1 = open(path, 'r')
    Lines = file1.readlines()

    count = 0
    # Strips the newline character
    points=[]
    for line in Lines:
        try:
            x,y = line.split()
            points+=[[float(x),float(y)]]
        except:
            points+=["NOT_VISIBLE"]

    file1.close()
    
    return points

# get edges and visibility from file on path
def edges_vis_from_file(path):
    file1 = open(path, 'r')
    Lines = file1.readlines()

    count = 0
    # Strips the newline character
    edges_vis=[]
    for line in Lines:
        x1,y1,x2,y2,vis = line.split()
        edge=[[float(x1),float(y1)],[float(x2),float(y2)]]
        edges_vis+=[[edge,vis]]

    file1.close()
    
    return edges_vis    

# get edges from file on path
def edges_from_file(path):
    file1 = open(path, 'r')
    Lines = file1.readlines()

    count = 0
    # Strips the newline character
    edges=[]
    for line in Lines:
        x1,y1,x2,y2 = line.split()
        edge=[[float(x1),float(y1)],[float(x2),float(y2)]]
        edges+=[edge]

    file1.close()
    
    return edges  