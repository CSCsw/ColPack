#include "PD2SMPGCColoring.h"
#include <cstring>
using namespace ColPack;
void usage();

int main(int argc, char* argv[]) {
    vector<string> fnames;
    vector<string> orders(1,"RANDOM");
    vector<string> methds(1,"PD2_OMP_GM3P");
    bool   bVerbose(false);
    vector<int> nTs(1);
    vector<string> options(1,"PD2_OMP_GM3P");
    vector<string> pf_names(1,".");
    int   bCheck(0);

    for(int i=1; i<argc; i++){
        if(     !strcmp(argv[i], "-f")) {
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                fnames.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "-o")) {
            orders.clear();
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                orders.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "--m")||!strcmp(argv[i],"-m")) {
            methds.clear();
            for(int j=i+1; j<argc; j++, i++) {
                if(argv[j][0]=='-') break;
                methds.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "--nT")||!strcmp(argv[i],"-nT")) {
            nTs.clear(); 
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                nTs.push_back( atoi(argv[j]));
            }
        }
        else if(!strcmp(argv[i], "-v")) bVerbose = true;
        else if(!strcmp(argv[i], "-opt")||!strcmp(argv[i],"--opt")) {
            options.clear(); 
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                options.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-checkd1")||!strcmp(argv[i],"--checkd1")) bCheck|=1;
        else if(!strcmp(argv[i],"-checkd2")||!strcmp(argv[i],"--checkd2")) bCheck|=2;
        
        else printf("Waringing, unused argument %s",argv[i]);
    }   

    if(fnames.empty()) {usage(); exit(0); }
   
    for(auto & fname : fnames){
        double iotime,ordtime;
        PD2SMPGCColoring *g = new PD2SMPGCColoring(fname,"MM",bVerbose?(&iotime):nullptr, PD2SMPGC::L, "RANDOM", nullptr);
        if(bVerbose) {
                printf("%s\n",fname.c_str());
                printf("iotime"); if(iotime>60) { printf(" %d min",((int)iotime)/60); iotime= ((int)(iotime)%60)+(iotime- (int)(iotime)); }  printf(" %g sec\n",iotime);  
        }

        for(auto & o : orders){
            g->global_ordering(PD2SMPGC::L, o, bVerbose?(&ordtime):nullptr);
            if(bVerbose)  {
                printf("global order %s ordtime",o.c_str()); if(ordtime>60) { printf(" %d min",((int)ordtime)/60); ordtime= ((int)(ordtime)%60)+(ordtime- (int)(ordtime)); }  printf(" %g sec\n",ordtime);  
            }

            for(auto & m : methds) {
                for(auto nT : nTs) {
                    g->Coloring(PD2SMPGC::L, nT, m);
                }
            }//end for methods
        }//end for orders
        delete g;
    }//end for files
    return 0;
}

void usage(){
    fprintf(stderr, "\n\nUSAGE:\n"
            " $./ColPack -f <list of gname> -o <list of orderings> -m <list of methods> [-v] --nT <list of number threads>\n"
            "\n"
            " -f <gnames>  :  Input file names\n"
            " -o <orders>  :  NATURAL\n"
            "                 RANDOM\n"
            " -m <methods> :  PD2_OMP_GM3P\n" 
            "                 PD2_OMP_GMMP\n"
            "                 PD2_OMP_GM3P_LOLF\n"
            "                 PD2_OMP_GMMP_LOLF\n"
            "                 PD2_SERIAL\n"
            "\n"
            " -nT <threads>:  list of number threads, --nT also accept.\n"
            " -v           :  verbose for debug infomation\n"
            "\n"
            "\n"
            "\n"
           ); 
}


