# import required module
import os

def get_sample_size(sample):
    if sample.find("mona") != -1:
        size = sample[sample.find('1'):sample.find('.')]
    else:
        size = sample[sample.find('0'):sample.find('.')]
    if len(size) != 7:
        size=size[:7]
    return int(size)

def get_files():

    # assign directory
    directory = 'input_data'
    input_files = {}

    # iterate over files in
    # that directory
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        # checking if it is a file
        if os.path.isfile(f):
            sample = f[len(directory)+1:]
            sample_size = get_sample_size(sample)
            if sample_size in input_files:
                input_files[sample_size] += [sample]
            else:
                input_files[sample_size] = [sample]
                
    return input_files

def make_call(call,in_file):
    # make call
    stream = os.popen(call)
    output = stream.read()

    run_data = output.split('\n')[0]+","+in_file.split(".")[0]
    #os.popen("echo " + run_data + " >> data.csv")
    os.popen("echo " + run_data + " >> data.csv")

input_files = get_files()
sorted_keys = sorted(input_files)

# create call       
alg_l = ['incremental','convex_hull','onion']
edge_sel_l = ['1','2','3']
init_inc_l = ['1a','1b','2a','2b']
onion_init_l = ['1','2','3','4','5']

# optimization parameters
opt_algorithm_l=["local_search","simulated_annealing"]
minmax_l=["1","2"]
L_l=["1","5","10"]
LL_l=["1000","5000","10000"]
threshold_l=["0.01","0.001","0.0001","0.00001"]
lgs_l=["1","2","3"]

out_file = "dum_dum.out"
inp_dir = "input_data/"

for inp_size in sorted_keys:
    print(inp_size)
    if inp_size < 2000:
        continue
    if inp_size > 2000:
        break
    for in_file in input_files[inp_size]:
        alg = alg_l[0]
        for edge_sel in edge_sel_l:
            for init_inc in init_inc_l:
                call = "./to_polygon -i " + inp_dir + in_file+ " -o " + out_file + " -algorithm " + alg + " -edge_selection " + edge_sel + " -initialization " + init_inc
                make_call(call,in_file)

        alg = alg_l[1]
        for edge_sel in edge_sel_l:
            call = "./to_polygon -i " + inp_dir + in_file+ " -o " + out_file + " -algorithm " + alg + " -edge_selection " + edge_sel
            make_call(call,in_file)

        alg = alg_l[2]
        call = "./to_polygon -i " + inp_dir + in_file+ " -o " + out_file + " -algorithm " + alg + " -onion_initialization " + "1" 
        print(call)
        make_call(call,in_file)          
