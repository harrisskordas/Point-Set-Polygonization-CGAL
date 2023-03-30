#ifndef evaluate_hpp
#define evaluate_hpp

/*
This header file contains functions for evaluation.cpp for assignment 3 and algorithm evaluation.
*/

#include <iostream>         // cout
#include <vector>           // vector
#include <dirent.h>         // direcrory manipulation
#include <cstring>          // strlen
#include <unordered_map>    // map
#include <algorithm>        // sort
#include <fstream>          // ofstream
#include <iomanip>          // text formating
#include <unistd.h>         // pipe, fork, etc
#include <sys/wait.h>       // wait child

// executes a shell command and returns result as string
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// search an array of strings for str
// return position of str in strings, or -1 if not found
int find(const std::vector<std::string>& strings,const std::string& str) {
    for (int i=0; i<strings.size(); i++) 
        if (strings[i].compare(str)==0) return i;
    return -1;
}

// prints proper use of the program
void print_correct_use() {
    std::cout<<""
    "general use:" 
    "./evaluate -i <point_set_path> -o <output_file>  "
    "\n";
}

/*
check and get input from argc, argv
general use: 
    ./evaluate -i <point_set_path> -o <output_file>                
check if -i, -o, are present 
and extract the corresponding values
the paramaters do not need to be in that order
*/       
void process_input(int argc, char** argv, std::string& input_file, std::string& output_file) {

    // get arguments and convert them to strings
    std::vector<std::string> arguments;
    for (int i=1; i<argc; i++) {
        std::string arg;
        arg.assign(argv[i], strlen(argv[i]));
        arguments.push_back(arg);
    }

    /*
    search and set input vals to given variables
    */

    int loc;
    std::string arg;

    // input_file
    loc = find(arguments,"-i");
    if (loc != -1) {
        arg = arguments[loc+1];
        input_file = arg;
    } else {
        std::cout<<"\n\nInput Error: -i not found.\n\n";
        print_correct_use();
        exit(1);
    }

    // output_file
    loc = find(arguments,"-o");
    if (loc != -1) {
        arg = arguments[loc+1];
        output_file = arg;
    } else {
        std::cout<<"\n\nInput Error: -o not found.\n\n";
        print_correct_use();
        exit(1);
    }

}

// get size of file file_name-xxxxxxx.instance
int get_size_of_file ( std::string file ) {
    if (file.find("uniform") == std::string::npos) {
        // find last -             
        size_t found = file.find("-");
        size_t next_found = file.find("-", found+1);
        while (next_found != std::string::npos) {
            found = next_found;
            next_found = file.find("-", found+1);
        }
        
        // return the 7 digits of size
        return stoi(file.substr(found+1,7));
    } else { // file is uniform-xxxxxx-xy.instance
        // find first -             
        size_t first = file.find("-");
        
        // return the 7 digits of size
        return stoi(file.substr(first+1,7));
    }
}

// extract algorithm scores from saved scores of all algorithms
std::vector<double> get_alg_scores(int alg_id, int alg_count, std::vector<double>& scores) {
    std::vector<double> alg_scores;
    
    int size=scores.size();

    for (int i=0;i<size;i+=alg_count) alg_scores.push_back(scores[alg_id+i]);

    return alg_scores;   
}

// sum of vector with doubles
double vec_sum(std::vector<double>& vec) {
    double sum=0;
    for(double val:vec) sum+=val;
    return sum;
}

// min val of vector with doubles
double vec_min(std::vector<double>& vec) {
    double min=vec[0];
    for(double val:vec) if(val<min) min=val;
    return min;
}

// max val of vector with doubles
double vec_max(std::vector<double>& vec) {
    double max=vec[0];
    for(double val:vec) if(val>max) max=val;
    return max;
}

#endif
