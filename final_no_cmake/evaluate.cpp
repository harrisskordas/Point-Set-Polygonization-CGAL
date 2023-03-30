
#include "includes/evaluate_helpers/evaluate.hpp"

// setup algorithms we will be checking
const int alg_count=8;

std::string alg_names[alg_count] = {
    "inc_ls_min",
    "inc_ls_max",
    "inc_siman_min",
    "inc_siman_max",
    "ch_ls_min",
    "ch_ls_max",
    "ch_siman_min",
    "ch_siman_max"
};

std::string alg_calls[alg_count] = {
    " -o data.out -algorithm incremental -edge_selection 2 -initialization 1a -opt_algorithm local_search -minmax 1 -L 10 -threshold 0.0001",
    " -o data.out -algorithm incremental -edge_selection 3 -initialization 1a -opt_algorithm local_search -minmax 2 -L 10 -threshold 0.0001",
    " -o data.out -algorithm incremental -edge_selection 2 -initialization 1a -opt_algorithm simulated_annealing -minmax 1 -L 10000 -local_global_subdiv 2",
    " -o data.out -algorithm incremental -edge_selection 3 -initialization 1a -opt_algorithm simulated_annealing -minmax 2 -L 10000 -local_global_subdiv 2",
    " -o data.out -algorithm convex_hull -edge_selection 3 -opt_algorithm local_search -minmax 1 -L 10 -threshold 0.0001",
    " -o data.out -algorithm convex_hull -edge_selection 2 -opt_algorithm local_search -minmax 2 -L 10 -threshold 0.0001",
    " -o data.out -algorithm convex_hull -edge_selection 3 -opt_algorithm simulated_annealing -minmax 1 -L 10000 -local_global_subdiv 2",
    " -o data.out -algorithm convex_hull -edge_selection 2 -opt_algorithm simulated_annealing -minmax 2 -L 10000 -local_global_subdiv 2"        
};

// handle cut off
void handle_alarm(int signum){
    std::string score_str = "xxxxxxxxx";

    int n=write(1, score_str.c_str(), sizeof(score_str.c_str()));

    exit(0);
}

int main(int argc, char** argv) {

    // set alarm handle for cut-off
    signal( SIGALRM, handle_alarm );

    std::string input_dir;
    std::string output_file;

    // get input from argc, argv
    process_input(argc, argv, input_dir, output_file);

    // get all files in input dir and save at files vector
    DIR *dir; struct dirent *diread;
    std::vector<std::string> files;


    if ((dir = opendir(input_dir.c_str())) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            std::string dir_str(diread->d_name);
            // keep only .instace files (skip . and ..)
            if (dir_str.find(".instance") != std::string::npos) {
                files.push_back(dir_str);
            }
        }
        closedir (dir);
    } else {
        perror ("opendir");
        return EXIT_FAILURE;
    }


    // create a map with sizes and files
    std::unordered_map< int, std::vector<std::string> > files_by_size;

    // insert files in map
    for (auto file : files) {   
        // get file size
        int size=get_size_of_file(file);
        // if size not in keys
        if (files_by_size.find(size) == files_by_size.end()) {
            // create key and insert
            std::vector<std::string> files_of_size;
            files_of_size.push_back(file);
            std::pair< int, std::vector<std::string> > size_files_pair (size,files_of_size);
            files_by_size.insert (size_files_pair);   
        } else {
            // just insert
            files_by_size.at(size).push_back(file);
        }
    }

    // order keys/sizes
    std::vector<int> keys;
    for(auto kv : files_by_size) {
        keys.push_back(kv.first);
    } 
    std::sort (keys.begin(), keys.end());

    // create a map for sizes and scores
    std::unordered_map< int, std::vector<double> > scores_by_size;

    // run algorithms
    // for each size
    for (auto& it : keys) {
        int size = (int)it;
        std::cout<<"running files of size: "<<it<<"\n";
        std::cout<<"------------------------------------------------------------\n";
        // for each file of given size
        for (auto file : files_by_size[it]) {
            std::cout<<file<<"\n";
            // for each algorithm
            for (int i=0;i<alg_count;i++) {
                
                std::string score_str;

                // set up some variables
                int MAX=9;
                char arr[MAX];
                int fd[2];
                int p=pipe(fd);
                pid_t pid = fork();

                // Create a child process using fork
                if (pid == 0) {
                    // no need to use the read end of pipe here so close it
                    close(fd[0]); 
                    // closing the standard output
                    close(1);    
                    // duplicating fd[0] with standard output 1
                    p=dup(fd[1]);  

                    // cut-off time;
                    int c=500;
                    ualarm(c*size*1000,0); // cut off = 500*n (*1000 cause ualrm uses micro seconds so turn to mili)

                    // run to_polygon
                    std::string run_command = "./to_polygon -i " + input_dir+"/"+file + alg_calls[i];
                    std::string score_str = exec( (run_command+"| cut -d'\n' -f2 | cut -d',' -f9").c_str() );

                    // send score to parent
                    p=write(1, score_str.c_str(), sizeof(score_str.c_str()));

                    exit(0);
                } else if (pid > 0) {
                    // This is the parent process
                    wait(NULL);

                    // closing the standard input 
                    close(0);
                    // no need to use the write end of pipe here so close it
                    close(fd[1]); 
                    // duplicating fd[0] with standard input 0
                    p=dup(fd[0]); 
                
                    // read score from child
                    int n = read(fd[0], arr, MAX);
                } else {
                    // fork failed
                    perror("fork");
                }

                // convert score to string
                score_str="";
                for (int i =0;i<MAX;i++) score_str+=arr[i];

                // convert score to double
                double score;                
                try {
                    // Block of code to try
                    score = std::stod(score_str);
                }
                catch (const std::invalid_argument&) {
                    // if score can't be converted to double 
                    // timoute cutoff happened
                    // score = 1 for min
                    // score = 0 for max
                    score = 1-(i%2); // i%2==0 -> min, i%2==1 -> max
                }
                
                // print score
                std::cout << alg_names[i] << ": " << score << "\n";

                // insert score on map
                if (scores_by_size.find(size) == scores_by_size.end()) {
                    // key not found - create key and insert
                    std::vector<double> scores_of_size;
                    scores_of_size.push_back(score);
                    std::pair< int, std::vector<double> > size_scores_pair (size,scores_of_size);
                    scores_by_size.insert (size_scores_pair);   
                } else {
                    // key found -just insert
                    scores_by_size.at(size).push_back(score);
                }

                continue;
            }
            std::cout<<"\n";
        }
        std::cout<<"\n";
    }  

    // write output on file
    std::ofstream output_file_stream(output_file);
    
    output_file_stream << "        ||            incremental-local_search           ||        incremental-simulated_annealing        ||            convex_hull-local_search           ||        convex_hull-simulated_annealing        ||\n";
    output_file_stream << "  Size  || min score | max score | min bound | max bound || min score | max score | min bound | max bound || min score | max score | min bound | max bound || min score | max score | min bound | max bound ||\n";
    std::cout.precision(5);
    for (auto& it : keys) {
        int size = (int)it;
        output_file_stream << std::setw(7) << size << " || ";

        for (int alg_id=0;alg_id<alg_count;alg_id+=2) {
            std::vector<double> min_scores = get_alg_scores(alg_id,alg_count,scores_by_size[it]);
            std::vector<double> max_scores = get_alg_scores(alg_id+1,alg_count,scores_by_size[it]); 

            output_file_stream << std::setprecision (5) << std::setw(9) << vec_sum(min_scores) << " | ";
            output_file_stream << std::setprecision (5) << std::setw(9) << vec_sum(max_scores) << " | ";
            output_file_stream << std::setprecision (5) << std::setw(9) << vec_min(min_scores) << " | ";
            output_file_stream << std::setprecision (5) << std::setw(9) << vec_max(max_scores) << " || ";
        }

        output_file_stream << "\n";
    }

    return 0;
}
