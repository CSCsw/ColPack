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

    std::string gname(""),order(""),color("");
    while(1){
        static struct option long_options[]={
            {"graph", required_argument, 0, 'g'},
            {"file" , required_argument, 0, 'f'},
            {"order", required_argument, 0, 'o'},
            {"color", required_argument, 0, 'c'},
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
            case 'c':
                color=optarg;
                break;
            default:
                std::cout<<"using --help or -h for more details"<<std::endl;
                exit(1);
        }
    }

    if(gname.empty() && order.empty() && color.empty() && optind<argc && argc-optind<=3)
        switch(argc-optind){
            case 3:
                color=argv[optind+2];
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
    else if(order.empty() || color.empty())
        official_example(gname);
    else{
        GraphColoringInterface *g = new GraphColoringInterface(SRC_FILE, gname.c_str(), "AUTO_DETECTED");
        g->Coloring(order.c_str(), color.c_str());
        std::cout<<"graph :"<<gname<<std::endl;
        std::cout<<"order :"<<order<<std::endl;
        std::cout<<"color :"<<color<<std::endl;
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
    std::cerr<<"$"<<appname<<" <GraphName> [order_option] [coloring_option]"<<std::endl;
    std::cerr<<"$"<<appname<<" --graph <GraphName> [--order <order_option>] [--color <coloring_option>]"<<std::endl;
    std::cerr<<"$"<<appname<<" -g <GraphName> [-o <order_option>] [-c <coloring_option>]"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"-g or --graph can be changed to -f --file in case needed."<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"### HELP "<<std::endl;
    std::cerr<<"$"<<appname<<" -h"<<std::endl;
    std::cerr<<"$"<<appname<<" --help"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"### OPTIONs "<<std::endl;
    std::cerr<<"order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM"<<std::endl;
    std::cerr<<"color: DISTANCE_ONE, ACYCLIC, ACYCLIC_FOR_INDIRECT_RECOVERY, STAR, RESTRICTED_STAR, DISTANCE_TWO"<<std::endl;
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
    string orderpool[6]={"NATURAL", "LARGEST_FIRST", "DYNAMIC_LARGEST_FIRST", "SMALLEST_LAST", "INCIDENCE_DEGREE", "RANDOM"};
    string colorpool[6]={"DISTANCE_ONE", "ACYCLIC", "ACYCLIC_FOR_INDIRECT_RECOVERY", "STAR", "RESTRICTED_STAR", "DISTANCE_TWO"};
    std::vector<std::string> order(orderpool,orderpool+6);
    std::vector<std::string> color(colorpool,colorpool+6);
    std::cout<<std::endl;    
    for(std::vector<std::string>::const_iterator itc=color.begin(); itc!=color.end(); itc++){
        std::cout<<"#"<<*itc<<" Result: "<<std::endl;
        for(std::vector<std::string>::const_iterator ito=order.begin(); ito!=order.end(); ito++){
            g->Coloring(ito->c_str(), itc->c_str());
            std::cout<<g->GetVertexColorCount()<<"  : ("<<*ito<<")"<<std::endl;
        }
        std::cout<<std::endl;
    }
    delete g;
    return;
}


