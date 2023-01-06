from vis_app_includes.alg_steps import *
from vis_app_includes.draw_alg import *
# UI stuff
import PySimpleGUI as sg
import matplotlib.pyplot as plt

import os
import sys
import shutil

def remove_dir(dir):
    # Get directory name
    mydir = dir

    # Try to remove the tree; if it fails, throw an error using try...except.
    try:
        shutil.rmtree(mydir)
    except OSError as e:
        print("Error: %s - %s." % (e.filename, e.strerror))

# disable buttons on poly algorithms based on chosen algorithm
def handle_alg_event(window,event):
    vals=[]
    locs=[]

    # handle poly algorithms
    ######################################################
    if event in ['-INC-','-CONV-','-ONION-']:
        for i in range(1,12+1):
            locs.append(i)

    if event=='-INC-':
        for i in range(1,7+1):
            vals+=[True]
        for i in range(8,12+1):
            vals+=[False]
    
    if event=='-CONV-':
        for i in range(1,3+1):
            vals+=[True]
        for i in range(4,12+1):
            vals+=[False]
            
    if event=='-ONION-':
        for i in range(1,7+1):
            vals+=[False]
        for i in range(8,12+1):
            vals+=[True]

    # handle opt algorithms
    ######################################################
    if event in ['-LOCAL-','-SIMAN-']:
        for i in range(18,29+1):
            locs.append(i)
    
    if event=='-LOCAL-':
        window['-R16-'].update(disabled=False)
        window['-R17-'].update(disabled=False)
        for i in range(18,23+1):
            vals+=[True]
        for i in range(24,29+1):
            vals+=[False]

    if event=='-SIMAN-':
        window['-R16-'].update(disabled=False)
        window['-R17-'].update(disabled=False)
        for i in range(18,23+1):
            vals+=[False]
        for i in range(24,29+1):
            vals+=[True]

    # handle opt on/off
    ######################################################
    if event in ['-OPT_ON-','-OPT_OFF-']:
        for i in range(16,29+1):
            locs.append(i)

    if event=='-OPT_ON-':
        window['-LOCAL-'].update(disabled=False)
        window['-SIMAN-'].update(disabled=False)
        return

    if event=='-OPT_OFF-':
        window['-LOCAL-'].update(disabled=True)
        window['-SIMAN-'].update(disabled=True)
        for i in range(16,29+1):
            vals+=[False]

    for i in range(len(locs)):
        window['-R'+str(locs[i])+'-'].update(disabled=not vals[i])
    
# get the value of the chosen radio button
def get_val_radio(vals,bools):
    for i in range(len(vals)):
        if bools[i]:
            return vals[i]


# UI stuff below

# left column, drawing and step counter
col1 = [
    [sg.Canvas(key="-CANVAS-")],
    [sg.Button("START"),sg.Button("<<"),sg.Button("<"),sg.Text("0",key="-CURRENT-"),sg.Text("/ "+str(0),key="-MAX_STEPS-"), sg.Button(">"),sg.Button(">>"),sg.Button("OPT"),sg.Button("FINAL")]
]

# right sub column, radio buttons for poly options
sub_col21 = [
    [sg.Text("alg:       "),
     sg.Radio('inc', "RAD_ALG", default=True, enable_events=True, key='-INC-'),
     sg.Radio('ch', "RAD_ALG", default=False, enable_events=True, key='-CONV-'),
     sg.Radio('onion', "RAD_ALG", default=False, enable_events=True, key='-ONION-')],
    
    [sg.Text("edge_sel:  "),
     sg.Radio('rand', "RAD_EDGE", default=True,key='-R1-'), 
     sg.Radio('min', "RAD_EDGE", default=False,key='-R2-'), 
     sg.Radio('max', "RAD_EDGE", default=False,key='-R3-')],

    [sg.Text("inc_init:  "),
     sg.Radio('x↓', "RAD_INIT_INC", default=True,key='-R4-'), 
     sg.Radio('x↑', "RAD_INIT_INC", default=False,key='-R5-'), 
     sg.Radio('y↓', "RAD_INIT_INC", default=False,key='-R6-'), 
     sg.Radio('y↑', "RAD_INIT_INC", default=False,key='-R7-')],
    
    [sg.Text("onion_init:"),
     sg.Radio('1', "RAD_INIT_ONION", default=True,key='-R8-'), 
     sg.Radio('2', "RAD_INIT_ONION", default=False,key='-R9-'), 
     sg.Radio('3', "RAD_INIT_ONION", default=False,key='-R10-'), 
     sg.Radio('4', "RAD_INIT_ONION", default=False,key='-R11-'), 
     sg.Radio('5', "RAD_INIT_ONION", default=False,key='-R12-')]    
]

# right sub column, radio buttons for optimization options
sub_col22 = [
    [sg.Text("alg:       "),
     sg.Radio('local', "RAD_OPT_ALG", default=True, enable_events=True, key='-LOCAL-'),
     sg.Radio('sim_an', "RAD_OPT_ALG", default=False, enable_events=True, key='-SIMAN-')],

    [sg.Text("minmax:    "),
     sg.Radio('min', "RAD_OPT_MINMAX", default=True,key='-R16-'), 
     sg.Radio('max', "RAD_OPT_MINMAX", default=False,key='-R17-')],

    [sg.Text("L:         "),
     sg.Radio('1', "RAD_OPT_L_LOCAL", default=True,key='-R18-'), 
     sg.Radio('5', "RAD_OPT_L_LOCAL", default=False,key='-R19-'), 
     sg.Radio('10', "RAD_OPT_L_LOCAL", default=False,key='-R20-'),],

    [sg.Text("threshold: "),
     sg.Radio('0.1', "RAD_OPT_THRESH", default=True,key='-R21-'), 
     sg.Radio('0.01', "RAD_OPT_THRESH", default=False,key='-R22-'), 
     sg.Radio('0.001', "RAD_OPT_THRESH", default=False,key='-R23-'),],

    [sg.Text("L:         "),
     sg.Radio('1000', "RAD_OPT_L_SIM_AN", default=True,key='-R24-'), 
     sg.Radio('5000', "RAD_OPT_L_SIM_AN", default=False,key='-R25-'), 
     sg.Radio('10000', "RAD_OPT_L_SIM_AN", default=False,key='-R26-'),],
    
    [sg.Text("lgs:       "),
     sg.Radio('local', "RAD_OPT_LGS", default=True,key='-R27-'), 
     sg.Radio('global', "RAD_OPT_LGS", default=False,key='-R28-'), 
     sg.Radio('subdiv', "RAD_OPT_LGS", default=False,key='-R29-')]    
]

# righ sub column, vis options
sub_col23 = [
    [sg.Text("vis:       "),
     sg.Radio('0', "RAD_VIS",key='-R13-', default=False), 
     sg.Radio('1', "RAD_VIS",key='-R14-', default=True), 
     sg.Radio('2', "RAD_VIS",key='-R15-', default=False)]
]

# right column i/o files, radio buttons, run button
col2 = [
    [sg.Text("Input file:  ",font="Courier 12")],
    [sg.Input("input_data/euro-night-0000200.instance",key="-INPUT_FILE-",size=(35),change_submits=True)],
    [sg.FileBrowse(key="-BROWSE-",initial_folder = os.getcwd(), enable_events=True)],
    [sg.Text("")],
    
    [sg.Text("Output file: ",font="Courier 12")],
    [sg.Input("data.out",key="-OUTPUT_FILE-",size=(35))],
    [sg.Text("")],
    
    [sg.Text("Polygonization Algorithm: ",font="Courier 12")],
    [sg.HorizontalSeparator()],
    [sg.Column(sub_col21,element_justification="middle")],
    
    [sg.Text("")],
    [sg.Text("Optimization Algorithm: ",font="Courier 12"),
     sg.Radio('on', "RAD_OPT_ONOFF",key='-OPT_ON-', default=True, enable_events=True), 
     sg.Radio('off', "RAD_OPT_ONOFF",key='-OPT_OFF-', default=False, enable_events=True), 
    ],
    [sg.HorizontalSeparator()],
    [sg.Column(sub_col22,element_justification="middle")],
    [sg.Text("")],

    [sg.Text("Visualisation ",font="Courier 12")],
    [sg.HorizontalSeparator()],
    [sg.Column(sub_col23,element_justification="middle")],
    [sg.Text("")],

    [sg.Text("              "),sg.Button("Run",font="Helvetica 18")]
]

# create the layout, col1, seperator, col2
layout = [
    [
        sg.Column(col1,element_justification="center"),
        sg.VSeperator(),
        sg.Column(col2,element_justification="left"),
    ]
]

# Create the window 
window = sg.Window(
    "Visualisation App",
    layout,
    return_keyboard_events=True,
    relative_location=(-300,-300),
    finalize=True,
    keep_on_top=True,
    element_justification="center",
    font="Courier 10",
)

# create the matplotlib figure for drawing
fig = plt.figure(figsize=(8, 8), dpi=100)
f = fig.add_subplot(111)

# Draw the plot and add the figure to the window
figure_agg=draw_figure(window["-CANVAS-"].TKCanvas, fig)

# only allow buttons for default chosen algorithm (spoilers it's incremental)
handle_alg_event(window,'-INC-')
# only allow buttons for default chosen opt algorithm (spoilers it's local search)
handle_alg_event(window,'-LOCAL-')

# set stuff
vis='0'
vis_counter=0
steps=1
alg='incremental'

# widnow event loop
while True:
    # get events and values
    event, values = window.read()

    # close if X is pressed
    if event == sg.WIN_CLOSED or event.find("Escape")!=-1:
        break

    # delete previous canvas of the ui
    delete_figure_agg(figure_agg)

    # update input file after browse
    if(event=='-BROWSE-'):
        window['-INPUT_FILE-'].update(values['-BROWSE-'])
    
    # enable or disable buttons based on chosen algorithm
    handle_alg_event(window,event)    
    
    # on run call to_polygon
    if event=='Run':
        
        remove_dir("visualisation")
        stream = os.popen("mkdir visualisation")

        # create call
        call = "./to_polygon -i " + values['-INPUT_FILE-'] + " -o " + values['-OUTPUT_FILE-']
        
        alg = get_val_radio(['incremental','convex_hull','onion'],[values['-INC-'],values['-CONV-'],values['-ONION-']])
        edge = get_val_radio(['1','2','3'],[values['-R1-'],values['-R2-'],values['-R3-']])
        init_inc = get_val_radio(['1a','1b','2a','2b'],[values['-R4-'],values['-R5-'],values['-R6-'],values['-R7-']])
        onion_init = get_val_radio(['1','2','3','4','5'],[values['-R8-'],values['-R9-'],values['-R10-'],values['-R11-'],values['-R12-']])
        vis = get_val_radio(['0','1','2'],[values['-R13-'],values['-R14-'],values['-R15-']])
        
        call += " -algorithm " + alg
        
        if alg == 'incremental':
            call += " -edge_selection "+edge
            call += " -initialization "+init_inc
        if alg == 'convex_hull':
            call += " -edge_selection "+edge
        if alg == 'onion':
            call += " -onion_initialization "+onion_init
    
        # exetnd call for optimization
        opt_on = get_val_radio(['on','off'],[values['-OPT_ON-'],values['-OPT_OFF-']])
        if opt_on=='on':
            opt_alg = get_val_radio(['local_search','simulated_annealing'],[values['-LOCAL-'],values['-SIMAN-']])
            minmax = get_val_radio(['1','2'],[values['-R16-'],values['-R17-']])
            L = get_val_radio(['1','5','10'],[values['-R18-'],values['-R19-'],values['-R20-']])
            threshold = get_val_radio(['0.1','0.01','0.001'],[values['-R21-'],values['-R22-'],values['-R23-']])
            LL = get_val_radio(['1000','5000','10000'],[values['-R24-'],values['-R25-'],values['-R26-']])
            lgs = get_val_radio(['1','2','3'],[values['-R27-'],values['-R28-'],values['-R29-']])
      
            call += " -opt_algorithm " + opt_alg + " -minmax " + minmax
        
            if opt_alg == 'local_search':
                call += " -L "+L
                call += " -threshold "+threshold
            if opt_alg == 'simulated_annealing':
                call += " -L "+LL
                call += " -local_global_subdiv "+lgs

                if lgs=="1":
                    opt_alg="an_local"
                if lgs=="2":
                    opt_alg="an_global"
                if lgs=="3":
                    alg="subdiv"
                    opt_alg="opt_subdiv"
                
            

        if vis!='0':
            call += " -vis "+vis
        
        #remove previous visualisation
        vis_counter=0
        
        # make call
        print(call)
        stream = os.popen(call)
        output = stream.read()
	
        print(output)

    # if < is pressed    
    if event == '<' or event.find("Left")!=-1:
        # decrease vis_counter, draw with new vis_counter
        vis_counter=(vis_counter-1)%steps
    if event == '>' or event.find("Right")!=-1:
        # increase vis_counter, draw with new vis_counter
        vis_counter=(vis_counter+1)%steps
    if event == '<<' or event.find("Down")!=-1:
        # decrease vis_counter, draw with new vis_counter
        vis_counter=max(vis_counter-10,0)%steps
    if event == '>>' or event.find("Up")!=-1:
        # increase vis_counter, draw with new vis_counter
        vis_counter=min(vis_counter+10,steps-1)%steps  
    if event == 'OPT' and vis=='1':
        # increase vis_counter, draw with new vis_counter
        vis_counter=get_steps_alg(alg)-1 
    if event == 'START' and vis=='1':
        # increase vis_counter, draw with new vis_counter
        vis_counter=0 
    if event == 'FINAL' and vis=='1':
        # increase vis_counter, draw with new vis_counter
        vis_counter=get_steps_alg(alg)+get_steps_alg(opt_alg)

    if os.path.exists('visualisation') and (vis=='1' or vis=='2'):
        vis_min = vis=='2'

        if vis=='1':
            steps=get_steps_alg(alg)
            if opt_on=='on':
                steps+=get_steps_alg(opt_alg)+1
                window['-MAX_STEPS-'].update("/ "+str(steps-1)+"("+str(get_steps_alg(alg)-1)+"+"+str(get_steps_alg(opt_alg)+1)+")")
            else:
                window['-MAX_STEPS-'].update("/ "+str(steps-1))
            
            if (vis_counter<get_steps_alg(alg)):
                draw_alg(alg,vis_counter,f,vis_min)
            else:
                draw_alg(opt_alg,vis_counter-get_steps_alg(alg),f,vis_min)   

        if vis=='2':
            if(opt_alg=="opt_subdiv" and lgs=="1"):
                steps=1
                window['-MAX_STEPS-'].update("/ "+str(0))
                draw_alg(opt_alg,vis_counter,f,vis_min)   
            else:
                steps=2
                window['-MAX_STEPS-'].update("/ "+str(1))
                if (vis_counter==0):
                    draw_alg(alg,vis_counter,f,vis_min)
                else:
                    draw_alg(opt_alg,vis_counter,f,vis_min)   
  

    figure_agg = draw_figure(window['-CANVAS-'].TKCanvas, fig)
    # update "page" number
    window['-CURRENT-'].update(str(vis_counter))
        
window.close()





