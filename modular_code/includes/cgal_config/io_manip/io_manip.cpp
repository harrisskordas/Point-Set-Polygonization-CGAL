#include "io_manip.hpp"

/*
Reads points and convex hull area from path. Expected format of input is:
    <line describing point set>
    parameters "convex_hull": {"area": "x"} // where x is the ch area
    0 x0 y0
    1 x1 y1
    ...
    n-1 xn yn
*/ 
void io_manip::read_data(const std::string& path, Points& points, NUM& ch_area) {
    // open file to read
    std::ifstream input_file(path);
    
    if (!input_file.good()) {
        std::cout<<"input file: "<<path<<" does not exist.\n";
        exit(1);
    }

    // read file line by line
    std::string line;
    int i=0;
    while (std::getline(input_file, line))
    {
        // 2nd line has ch_area
        if (i==1) {
            // convert read string to stringstream
            std::stringstream convestsion_stream;
            std::string ch_area_str = line.substr(38,line.size()-2-38);
            convestsion_stream << ch_area_str;
            // then convert the stringstream to NUM to handle greater values
            convestsion_stream >> ch_area;
        }
        // starting from the 3d line
        else if (i>=2) {
            std::string item;            
            std::stringstream ss_line(line);
            std::vector<std::string> vals;

            // separate values by tab
            while (std::getline(ss_line, item, '\t'))    
                vals.push_back(item);

            // insert new point from vals[1] and vals[2] (vals[0] is an index)
            points.push_back(Point_2(stoi(vals[1]),stoi(vals[2])));
        }
        // increase line counter number
        i++;
    }

    // close read file
    input_file.close();
}


// prints proper use of the program
void print_correct_use() {
    std::cout<<"\ngeneral use: ./to_polygon -i <input_file> -o <output_file> -algorithm <algorithm> -edge_selection <edge_selection> -initialization <initialization> -onion_initialization <onion_initialization> \n";
    std::cout<<"use1: ./to_polygon -i <input_file> -o <output_file> -algorithm incremental -edge_selection <1/2/3> -initialization <1a/1b/2a/2b>\n";
    std::cout<<"use2: ./to_polygon -i <input_file> -o <output_file> -algorithm convex_hull -edge_selection <1/2/3> \n";
    std::cout<<"use3: ./to_polygon -i <input_file> -o <output_file> -algorithm onion -onion_initialization <1/2/3/4/5> \n";
    std::cout<<"\n";
    std::cout<<"parameters: \n";
    std::cout<<"edge_selection: 1/2/3 -> random, min area, max area\n";
    std::cout<<"initialization: 1a/1b/2a/2b -> sort x dec, x inc, y dec, y inc\n";
    std::cout<<"onion_initialization: 1/2/3/4/5 -> ch construction random pick, min x, max x, min y, max y\n\n";
    std::cout<<"Optionally add -vis <1/2> for full or minimal visualisation.\n\n";
}

// search an array of strings for str
// return position of str in strings, or -1 if not found
int find(const std::vector<std::string>& strings,const std::string& str) {
    for (int i=0; i<strings.size(); i++) 
        if (strings[i].compare(str)==0) return i;
    return -1;
}

/*
check and get input from argc, argv
general use: ./to_polygon -i <input_file> -o <output_file> -algorithm <algorithm> -edge_selection <edge_selection> -initialization <initialization> -onion_initialization <onion_initialization>
check if -i, -o, -algorithm, -edge_selection, -initialisation, -onion_initialization are present 
and extract the corresponding values
the paramaters do not need to be in that order
specific parameters need to be present based on given algorithm
*/ 
void io_manip::process_input(int argc, 
                             char** argv, 
                             std::string& inpout_file,
                             std::string& output_file,
                             int& algorithm,
                             int& edge_selection,
                             int& init_inc,
                             int& init_onion,
                             bool& vis,
                             bool& vis_min) {

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
        inpout_file = arg;
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

    // algorithm   
    loc = find(arguments,"-algorithm");
    if (loc != -1) {
        arg = arguments[loc+1];
        if (arg.compare("incremental")==0) algorithm=1;
        else if (arg.compare("convex_hull")==0) algorithm=2;
        else if (arg.compare("onion")==0) algorithm=3;
        else {
            std::cout<<"\n\nInput Error: -algorithm = <incremental/convex_hull/onion>\n\n";
            print_correct_use();
            exit(1);
        }
    } else {
        std::cout<<"\n\nInput Error: -algorithm not found.\n\n";
        print_correct_use();
        exit(1);
    }

    // edge_selection, initialization, onion_initialization
    switch(algorithm) {
        case 1: // incremental only
            // initialization
            loc = find(arguments,"-initialization");
            if (loc != -1) {
                arg = arguments[loc+1];
                if (arg.compare("1a")==0) init_inc=2;
                else if (arg.compare("1b")==0) init_inc=1;
                else if (arg.compare("2a")==0) init_inc=4;
                else if (arg.compare("2b")==0) init_inc=3;
                else {
                    std::cout<<"\n\nInput Error: -initialization = <1a/ab/2a/2b>\n\n";
                    print_correct_use();
                    exit(1);
                }
            } else {
                std::cout<<"\n\nInput Error: -initialization not found.\n\n";
                print_correct_use();
                exit(1);
            }
            // no break here
        case 2: // convex hull and incremental
            // edge_selection
            loc = find(arguments,"-edge_selection");
            if (loc != -1) {
                arg = arguments[loc+1];
                if (arg.compare("1")==0) edge_selection=1;
                else if (arg.compare("2")==0) edge_selection=2;
                else if (arg.compare("3")==0) edge_selection=3;
                else {
                    std::cout<<"\n\nInput Error: -edge_selection = <1/2/3>\n\n";
                    print_correct_use();
                    exit(1);
                }
            } else {
                std::cout<<"\n\nInput Error: -edge_selection not found.\n\n";
                print_correct_use();
                exit(1);
            }
            break;
        case 3: // onion
        // onion_initialization
            loc = find(arguments,"-onion_initialization");
            if (loc != -1) { 
                arg = arguments[loc+1];
                if (arg.compare("1")==0) init_onion=1;
                else if (arg.compare("2")==0) init_onion=2;
                else if (arg.compare("3")==0) init_onion=3;
                else if (arg.compare("4")==0) init_onion=4;
                else if (arg.compare("5")==0) init_onion=5;
                else {
                    std::cout<<"\n\nInput Error: -initialization = <1a/ab/2a/2b>\n\n";
                    print_correct_use();
                    exit(1);
                }
            } else {
                std::cout<<"\n\nInput Error: -onion_initialization not found.\n\n";
                print_correct_use();
                exit(1);
            }
    }
    
    // vis
    loc = find(arguments,"-vis");
    if (loc != -1) {
        arg = arguments[loc+1];
        if (arg.compare("1")==0) vis=true;
        else if (arg.compare("2")==0) vis_min=true;
        else {
            std::cout<<"\n\nInput Error: -vis = <1/2>\n\n";
            print_correct_use();
            exit(1);
        }
    }

}

/*
Creates an output string with the format below:
    Polygonization
    <points>
    <edges>
    Algorithm: <incremental or convex_hull or onion>_edge_selection<1 or 2 or 3
                where applicable>_initialization<incremental and onion options accordingly>
    area: <area calculated during algorithm>
    cgal_area: <area calculated from built in cgal function>
    pick_calculated_area: <pick area (-1 not implemented)>
    ratio: <poly_area / ch_area>
    construction time: <milliseconds as integer> 
*/ 
std::string io_manip::create_output(const Polygon_2& poly_line,
                                    const NUM& poly_area,
                                    const NUM& cgal_poly_area, 
                                    const NUM& pick_area,
                                    const NUM& ch_area,
                                    const double& time,
                                    const int& algorithm,
                                    const int& edge_selection,
                                    const int& init_inc,
                                    const int& init_onion,
                                    const int& simple,
                                    const int& points_count,
                                    const int& points_poly_count) {                                    

    std::string output_string;

    output_string += "Polygonization\n";

    // write points
    for (const Point_2& p:poly_line) 
       output_string += std::to_string((NUM)p.x())+" "+std::to_string((NUM)p.y())+"\n";
    
    // write edges
    for (const Segment_2& e:poly_line.edges()) {
        Point_2 p1=e.source(), p2=e.target();
        output_string +=  std::to_string((NUM)p1.x())+" "+std::to_string((NUM)p1.y())+" "+std::to_string((NUM)p2.x())+" "+std::to_string((NUM)p2.y())+"\n";
    }

    // write algorithm
    output_string += "Algorithm: ";
    switch(algorithm) {
        case 1:
            output_string += "incremental";
            //write edges
            output_string += "_edge_selection_" + std::to_string(edge_selection);
            output_string += "_initialization_";
            //write init_inc
            switch(init_inc) {
                case 1: output_string += "1b"; break;
                case 2: output_string += "1a"; break;
                case 3: output_string += "2b"; break;
                case 4: output_string += "2a"; break;
            }
            break;
        case 2:
            output_string += "convex_hull";
            //write edges
            output_string += "_edge_selection_" + std::to_string(edge_selection);
            break;
        case 3:
            output_string += "onion";
            //write init_onion
            output_string += "_initialization_" + std::to_string(init_onion);
            break;
    }
    output_string += "\n";

    // write area, pick, ratio, time
    output_string += "area: " + std::to_string(poly_area) + "\n";
    output_string += "cgal_area: " + std::to_string(cgal_poly_area) + "\n";
    output_string += "pick_calculated_area: " + std::to_string(pick_area) + "\n";
    output_string += "ratio: " + std::to_string(poly_area*1.0/ch_area) + "\n";
    output_string += "construction time: " + std::to_string(time) + " ms\n";
    output_string += "is simple: " + std::to_string(simple) + "\n";
    output_string += "points: " + std::to_string(points_count) + "\n";
    output_string += "points on poly: " + std::to_string(points_poly_count) + "\n";    

    return output_string;
}                            
