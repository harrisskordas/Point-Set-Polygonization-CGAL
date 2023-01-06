# runing to_polygon can be tidious so we can use this script
# to more easily handle the arguments we call to_polygon with

# <input_file> 
input_file="input_data/euro-night-0001000.instance "
# <output_file>
output_file="data.out"
# <incremental/convex_hull/onion> 
algorithm="incremental"
# <1,2,3> (rand,min,max)
edge_selection="3"
# <1a/1b/2a/2b> (sort x dec, x inc, y dec, y inc for incremental)
initialization="1a"
# <1/2/3/4/5> (random pick, min x, max x, min y, max y)
onion_initialization="1"
# <local_search/simulated_annealing>
opt_algorithm="local_search"
# <1/2> (1 for min, 2 for max)
minmax="2"
# <int> (local - num points, sim_an - num steps)
L="10"
# <double> (threshold for local)
threshold="0.0001"
# <1/2/3> (local, global, subdiv for simulated annealing>
lgs="1"
# <1/2> (full, minimal - optional argument)
vis="1"

# run with parameters given above
echo
echo ./to_polygon -i $input_file -o $output_file -algorithm $algorithm -edge_selection $edge_selection -initialization $initialization -onion_initialization $onion_initialization -opt_algorithm $opt_algorithm -minmax $minmax -L $L -threshold $threshold -local_global_subdiv $lgs -vis $vis
echo
./to_polygon -i $input_file -o $output_file -algorithm $algorithm -edge_selection $edge_selection -initialization $initialization -onion_initialization $onion_initialization -opt_algorithm $opt_algorithm -minmax $minmax -L $L -threshold $threshold -local_global_subdiv $lgs -vis $vis
echo

exit

# some example to copy-paste below

# incremental no opt
./to_polygon -i input_data/euro-night-0000200.instance -o data.out -algorithm incremental -edge_selection 3 -initialization 1a 

# convex_hull and local_search
./to_polygon -i input_data/euro-night-0000050.instance -o data.out -algorithm convex_hull -edge_selection 2 -opt_algorithm local_search -minmax 2 -L 10 -threshold 0.0001

# convex_hull and simulated_annealing_local
./to_polygon -i input_data/euro-night-0000100.instance -o data.out -algorithm convex_hull -edge_selection 2 -initialization 1a -onion_initialization 1 -opt_algorithm simulated_annealing -minmax 2 -L 5000 -threshold 0.0001 -local_global_subdiv 1 -vis -1