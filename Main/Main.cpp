#include "ColPackHeaders.h"
#include <getopt.h>
using namespace ColPack;
void official_example(std::string gname);
void usage(std::string appname);

int main(int argc, char* argv[]) {
    if(argc==1){
        std::cout<<"using --help or -h for more details"<<endl;
        exit(1);
    }

    std::string gname(""),order(""),distance("");
    while(1){
        static struct option long_options[]={
            {"graph", required_argument, 0, 'g'},
            {"file" , required_argument, 0, 'f'},
            {"order", required_argument, 0, 'o'},
            {"distance", required_argument, 0, 'd'},
            {"help",  no_argument,       0, 'h'},
            {0,0,0,0}
        };
        int c,option_index=0;
        c=getopt_long(argc,argv,"g:f:o:d:h",
                    long_options, &option_index);
        if(c==-1)
            break;
        switch(c) {
            case 'h':
                usage(argv[0]);
                exit(1);
            case 'g':
            case 'f':
                gname=optarg;
                break;
            case 'o':
                order=optarg;
                break;
            case 'd':
                distance=optarg;
                break;
            default:
                std::cout<<"using --help or -h for more details"<<std::endl;
                exit(1);
        }
    }

    if(gname.empty() && order.empty() && distance.empty() && optind<argc && argc-optind<=3)
        switch(argc-optind){
            case 3:
                distance=argv[optind+2];
            case 2:
                order=argv[optind+1];
            case 1:
                gname=argv[optind];
                break;
            default:
                std::cout<<"using --help or -h for more details"<<std::endl;
                exit(1);
        }
    if(gname.empty())
        official_example("Graphs/bcsstk01.mtx");
    else if(order.empty() || distance.empty())
        official_example(gname);
    else{
        GraphColoringInterface *g = new GraphColoringInterface(SRC_FILE, gname.c_str(), "AUTO_DETECTED");
        g->Coloring(order.c_str(), distance.c_str());
        std::cout<<"graph :"<<gname<<std::endl;
        std::cout<<"order :"<<order<<std::endl;
        std::cout<<"dists :"<<distance<<std::endl;
        std::cout<<"result:"<<g->GetVertexColorCount()<<std::endl;
        delete g;
    }
    return 0;
}

void usage(std::string appname){
    std::cerr<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"Welcome to use ColPack!"<<std::endl;
    std::cerr<<"### USAGE "<<std::endl;
    std::cerr<<"$"<<appname<<" <GraphName> [order_option] [distance_option]"<<std::endl;
    std::cerr<<"$"<<appname<<" --graph <GraphName> [--order <order_option>] [--distance <distance_option>]"<<std::endl;
    std::cerr<<"$"<<appname<<" -g <GraphName> [-o <order_option>] [-d <distance_option>]"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"-g or --graph can be changed to -f --file in case needed."<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"### HELP "<<std::endl;
    std::cerr<<"$"<<appname<<" -h"<<std::endl;
    std::cerr<<"$"<<appname<<" --help"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"### OPTIONs "<<std::endl;
    std::cerr<<"order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM"<<std::endl;
    std::cerr<<"distance:DISTANCE_ONE, DISTANCE_TWO"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"### EXAMPLE "<<std::endl;
    std::cerr<<"$"<<appname<<" Graphs/bcsstk01.mtx"<<std::endl; 
    std::cerr<<"$"<<appname<<" Graphs/bcsstk01.mtx LARGEST_FIRST DISTANCE_ONE"<<std::endl; 
    std::cerr<<"$"<<appname<<" --graph Graphs/bcsstk01.mtx --order SMALLEST_LAST -d DISTANCE_TWO"<<std::endl; 
    std::cerr<<"$"<<appname<<" -f Graphs/bcsstk01.mtx -o RANDOM -distance DISTANCE_ONE"<<std::endl; 
    std::cerr<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<std::endl;
    return;
}

void official_example(std::string gname){

    std::cout<<"List common coloring result for example graph:"<<gname<<std::endl;
    std::cout<<"using --help or -h for more details"<<std::endl;
    GraphColoringInterface * g = new GraphColoringInterface(SRC_FILE, gname.c_str(), "AUTO_DETECTED");
    std::cout<< "DISTANCE_ONE COLOR Result:" <<std::endl;
    g->Coloring("NATURAL", "DISTANCE_ONE");
    std::cout<<"  NATURAL              : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("LARGEST_FIRST", "DISTANCE_ONE");
    std::cout<<"  LARGEST_FIRST        : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("DYNAMIC_LARGEST_FIRST", "DISTANCE_ONE");
    std::cout<<"  DYNAMIC_LARGEST_FIRST: " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("SMALLEST_LAST", "DISTANCE_ONE");
    std::cout<<"  SMALLEST_LAST        : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("INCIDENCE_DEGREE", "DISTANCE_ONE");
    std::cout<<"  INCIDENCE_DEGREE     : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("RANDOM", "DISTANCE_ONE");
    std::cout<<"  RANDOM               : " << g->GetVertexColorCount()<<std::endl;
    std::cout<<endl;
    std::cout<< "DISTANCE_TWO COLOR Result:" << endl;
    g->Coloring("NATURAL", "DISTANCE_TWO");
    std::cout<<"  NATURAL              : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("LARGEST_FIRST", "DISTANCE_ONE");
    std::cout<<"  LARGEST_FIRST        : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("DYNAMIC_LARGEST_FIRST", "DISTANCE_ONE");
    std::cout<<"  DYNAMIC_LARGEST_FIRST: " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("SMALLEST_LAST", "DISTANCE_ONE");
    std::cout<<"  SMALLEST_LAST        : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("INCIDENCE_DEGREE", "DISTANCE_ONE");
    std::cout<<"  INCIDENCE_DEGREE     : " << g->GetVertexColorCount()<<std::endl;
    g->Coloring("RANDOM", "DISTANCE_ONE");
    std::cout<<"  RANDOM               : " << g->GetVertexColorCount()<<std::endl;
    delete g;
    return;
}


