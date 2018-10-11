#include "SMPGCColoring.h"
#include <cstring>
using namespace ColPack;
void usage();


int main(int argc, char* argv[]) {
    vector<string> fnames;
    vector<string> orders(1,"RANDOM");
    vector<string> methds(1,"DISTANCE_ONE_OMP_GM3P");
    bool   bVerbose(false);
    vector<int> nTs(1);
    vector<string> options(1,"GM3P");
    vector<int>    switch_iters(1,0);
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
        else if(!strcmp(argv[i], "-sit")||!strcmp(argv[i],"--sit")) {
            switch_iters.clear(); 
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                switch_iters.push_back( atoi(argv[j]));
            }
        }
        else if(!strcmp(argv[i],"-checkd1")||!strcmp(argv[i],"--checkd1")) bCheck|=1;
        else if(!strcmp(argv[i],"-checkd2")||!strcmp(argv[i],"--checkd2")) bCheck|=2;

        
        else printf("Waringing, unused argument %s",argv[i]);
    }   

    if(fnames.empty()) {usage(); exit(0); }
   
    for(auto & fname : fnames){
        double iotime,ordtime;
        SMPGCColoring *g = new SMPGCColoring(fname,"MM",bVerbose?(&iotime):nullptr, "NATURAL", nullptr);
        if(bVerbose) {
                printf("%s\n",fname.c_str());
                printf("iotime"); if(iotime>60) { printf(" %d min",((int)iotime)/60); iotime= ((int)(iotime)%60)+(iotime- (int)(iotime)); }  printf(" %g sec\n",iotime);  
        }

        for(auto & o : orders){
            g->global_ordering(o, bVerbose?(&ordtime):nullptr);
            if(bVerbose)  {
                printf("global order %s ordtime",o.c_str()); if(ordtime>60) { printf(" %d min",((int)ordtime)/60); ordtime= ((int)(ordtime)%60)+(ordtime- (int)(ordtime)); }  printf(" %g sec\n",ordtime);  
            }

            for(auto& m : methds) {
                for(auto nT : nTs) 
                    for(auto switch_iter : switch_iters)
                        g->Coloring(nT, m, switch_iter);
            }//end for methods
        }//end for orders
        delete g;
    }//end for files
    return 0;
}

void usage(){
    fprintf(stderr, "\n\n\nUSAGE:\n $./ColPack -f <list of gname> -o <list of orderings> -m <list of methods> [-v] --nT <list of number threads>\n"
            "\n"
            "-f <gnames>  :  Input file names\n"
            "-o <orders>  :  NATURAL\n"
            "                RANDOM\n"
            "                LARGEST_FIRST\n"
            "                SMALLEST_LAST\n"
            "-m <methods> :  DISTANCE_ONE_OMP_<GM3P/GMMP/SERIAL/JP/MTJP>[_<LF/SL/NT/RD/NONE>]\n" 
            "                DISTANCE_ONE_OMP_HB[MT]JP_<GM3P/GMMP/SERIAL>[_<LF/SL/NT/RD/NONE>]\n"
            "                DISTANCE_TWO_OMP_<GM3P/GMMP/SERIAL>[_<LF/SL/NT/RD/NONE>]\n"
            "\n"
            "-nT <threads>:  list of number threads, --nT is also accept.\n"
            "-v           :  verbose for debug infomation\n"
            "(only for HYBRID METHOD)\n"
            "  -sit <switch at the number of iteration,  0 means direct switch>\n"
            "\n"
            "List of Method:\n"
            " DISTANCE_ONE_OMP_GM3P    DISTANCE_ONE_OMP_GM3P_LF     DISTANCE_ONE_OMP_GM3P_..  \n"
            " DISTANCE_ONE_OMP_GMMP    DISTANCE_ONE_OMP_GMMP_LF     DISTANCE_ONE_OMP_GMMP_..  \n"
            " DISTANCE_ONE_OMP_SERIAL  DISTANCE_ONE_OMP_GMMP_SERIAL DISTANCE_ONE_OMP_SERIAL_..\n"
            " DISTANCE_ONE_OMP_JP      DISTANCE_ONE_OMP_GMMP_JP     DISTANCE_ONE_OMP_JP_..    \n"
            " DISTANCE_ONE_OMP_MTJP    DISTANCE_ONE_OMP_GMMP_MTJP   DISTANCE_ONE_OMP_MTJP_..  \n"
            "\n"
            " DISTANCE_ONE_OMP_HBJP_GM3P   DISTANCE_ONE_OMP_HBJP_GM3P_..   DISTANCE_ONE_OMP_HBJP_GMMP..   DISTANCE_ONE_OMP_HBJP_....  \n"
            " DISTANCE_ONE_OMP_HBMTP_GM3P  DISTANCE_ONE_OMP_HBMTJP_GM3P_.. DISTANCE_ONE_OMP_HBMTJP_GMMP.. DISTANCE_ONE_OMP_HBMTJP_....\n"
            "\n"
            " DISTANCE_TWO_OMP_GM3P    DISTANCE_TWO_OMP_GM3P_LF     DISTANCE_TWO_OMP_GM3P_..  \n"
            " DISTANCE_TWO_OMP_GMMP    DISTANCE_TWO_OMP_GMMP_LF     DISTANCE_TWO_OMP_GMMP_..  \n"
            " DISTANCE_TWO_OMP_SERIAL  DISTANCE_TWO_OMP_SERIAL_LF   DISTANCE_TWO_OMP_SERIAL_..\n"
           "\n"
            "Example:\n"
            " $./ColPack -f mc10.mtx mc15.mtx -o RANDOM -m DISTANCE_ONE_OMP_GM3P DISTANCE_TWO_GMMP_LF -v --nT 1 2 4 8\n" 
            " $./ColPack -f bcsstk01.mtx -o RANDOM -m DISTANCE_ONE_OMP_HBJP_SERIAL -v -nT 10 -sit 4\n"
            " $./ColPack -f bcsstk01.mtx mc10.mtx -o RANDOM -m DISTANCE_TWO_OMP_GMMP DISTANCE_TWO_OMP_GMMP_LF -v -nT 10\n"
            "\n\n\n"
            ); 
}


