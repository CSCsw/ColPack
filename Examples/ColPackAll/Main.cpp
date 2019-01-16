#include "ColPackHeaders.h"
#include "PD2SMPGCColoring.h"
#include "SMPGCColoring.h"
#include <cstring>
#include <unordered_set>
using namespace ColPack;
void usage();
void general_coloring(int argc, char* argv[]);
void partial_coloring(int argc, char* argv[]);
void bicoloring(int argc, char* argv[]);
void smpg_general_coloring(int argc, char* argv[]);
void smpg_partial_coloring(int argc, char* argv[]);


const unordered_set<string> GENERAL_COLORING({
        "DISTANCE_ONE", 
        "ACYCLIC", 
        "ACYCLIC_FOR_INDIRECT_RECOVERY", 
        "STAR", 
        "RESTRICTED_STAR", 
        "DISTANCE_TWO"});
const unordered_set<string> BICOLORING({
        "IMPLICIT_COVERING__STAR_BICOLORING",
        "EXPLICIT_COVERING__STAR_BICOLORING",
        "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING",
        "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"});
const unordered_set<string> PARTIAL_COLORING({
        "COLUMN_PARTIAL_DISTANCE_TWO",
        "ROW_PARTIAL_DISTANCE_TWO"});


int main(int argc, char* argv[]) {
    string method="";
    for(int i=1; i<argc; i++){
        if( !strcmp(argv[i],"-m")) {
            method = argv[++i]; break;
        }
    }
    if(      GENERAL_COLORING.count(method) ) 
        general_coloring(argc, argv);
    else if( PARTIAL_COLORING.count(method) ) 
        partial_coloring(argc, argv);
    else if( BICOLORING.count(method)       ) 
        bicoloring(argc,argv);
    else if( method.size()>4 && method.substr(0,3)=="D1_")
        smpg_general_coloring(argc,argv);
    else if( method.size()>4 && method.substr(0,4)=="PD2_")
        smpg_partial_coloring(argc,argv);
    else{
        usage();
        exit(1);
    }
    return 0;
}



void general_coloring(int argc, char* argv[]){
    vector<string> fnames;
    vector<string> methds;
    vector<string> orders(1,"RANDOM"); 
    bool   bVerbose(false);
    for(int i=1; i<argc; i++){
        if( !strcmp(argv[i], "-f") ){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                fnames.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-m")){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                if(GENERAL_COLORING.count(argv[j])) 
                    methds.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "-o")){
            orders.clear();
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                orders.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-v")){
            bVerbose=true;
        }
        else{
            printf("\nWarning: unknown input argument\"%s\".\n", argv[i]);
        }
    }

    if (fnames.empty()) { usage(); exit(1); }
    for(auto fname : fnames){
        GraphColoringInterface *g = new GraphColoringInterface(SRC_FILE, fname.c_str(), "AUTO_DETECTED");
        for(auto m : methds){
            for(auto o : orders){
                if(bVerbose) printf("\ngraph: %s\norder: %s\nmethd: %s\nGeneral Graph Coloring\n",fname.c_str(), o.c_str(), m.c_str());
                g->Coloring(o.c_str(), m.c_str());
                if(bVerbose) {
                    double t1 = g->GetVertexOrderingTime();
                    double t2 = g->GetVertexColoringTime();
                    printf("order+color time = %f = %f+%f\n",t1+t2, t1,t2);
                    printf("number of colors: ");
                    printf("%d\n",g->GetVertexColorCount());
                }
                else {
                    printf("%d\n",g->GetVertexColorCount());
                }
            }
        }
        delete g;
    }
}


void partial_coloring(int argc, char* argv[]){
    vector<string> fnames;
    vector<string> methds;
    vector<string> orders; 
    bool   bVerbose(false);

    for(int i=1; i<argc; i++){
        if( !strcmp(argv[i], "-f") ){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                fnames.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-m")){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                if(PARTIAL_COLORING.count(argv[j])) 
                    methds.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "-o")){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                orders.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-v")){
            bVerbose=true;
        }
        else{
            printf("\nWarning: unknown input argument\"%s\".\n", argv[i]);
        }
    }   

    for(auto fname : fnames){
        BipartiteGraphPartialColoringInterface *g = new BipartiteGraphPartialColoringInterface(0, fname.c_str(), "AUTO_DETECTED");
        for(auto m : methds){
            for(auto o : orders){
                if(bVerbose) printf("\ngraph: %s\norder: %s\nmethd: %s\nPartial Distance Two Bipartite Graph Coloring\n",fname.c_str(), o.c_str(), m.c_str());
                g->PartialDistanceTwoColoring(o.c_str(), m.c_str());
                if(bVerbose) {
                    double t1 = g->GetVertexOrderingTime();
                    double t2 = g->GetVertexColoringTime();
                    printf("order+color time = %f = %f+%f\n",t1+t2, t1,t2);
                    printf("number of colors: ");
                    printf("%d\n",g->GetVertexColorCount());
                }
                else {
                    printf("%d\n",g->GetVertexColorCount());
                }
            }
        }
        delete g;
    }
    return;
}
 


void bicoloring(int argc, char* argv[]){
    vector<string> fnames;
    vector<string> methds;
    vector<string> orders; 
    bool   bVerbose(false);

    for(int i=1; i<argc; i++){
        if( !strcmp(argv[i], "-f") ){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                fnames.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-m")){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                if(BICOLORING.count(argv[j])) 
                    methds.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i], "-o")){
            for(int j=i+1; j<argc; j++, i++){
                if(argv[j][0]=='-') break;
                orders.push_back( argv[j]);
            }
        }
        else if(!strcmp(argv[i],"-v")){
            bVerbose=true;
        }
        else{
            printf("\nWarning: unknown input argument\"%s\".\n", argv[i]);
        }
    }   

    for(auto fname : fnames){
        BipartiteGraphBicoloringInterface *g = new BipartiteGraphBicoloringInterface(0, fname.c_str(), "AUTO_DETECTED");

        for(auto m : methds){
            for(auto o : orders){
                if(bVerbose) printf("\ngraph: %s\norder: %s\nmethd: %s\nBiColoring Bipartite Graph Coloring\n",fname.c_str(), o.c_str(), m.c_str());
                g->Bicoloring(o.c_str(), m.c_str());
                if(bVerbose) {
                    double t1 = g->GetVertexOrderingTime();
                    double t2 = g->GetVertexColoringTime();
                    printf("order+color time = %f = %f+%f\n",t1+t2, t1,t2);
                    printf("number of colors: ");
                    printf("%d\n",g->GetVertexColorCount());
                }
                else {
                    printf("%d\n",g->GetVertexColorCount());
                }
            }
        }
        delete g;
    }
    return;
}
 


void smpg_general_coloring(int argc, char* argv[]){
    vector<string> fnames;
    vector<string> orders(1,"RANDOM");
    vector<string> methds(1,"D1_OMP_GM3P");
    bool   bVerbose(false);
    vector<int> nTs(1);
    vector<string> options(1,"GM3P");
    vector<int>    switch_iters(1,0);
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
}

void smpg_partial_coloring(int argc, char* argv[]){
    vector<string> fnames;
    vector<string> orders(1,"RANDOM");
    vector<string> methds(1,"PD2_OMP_GM3P");
    bool   bVerbose(false);
    vector<int> nTs(1);
    vector<string> options(1,"PD2_OMP_GM3P");
    bool   bCheck=false; 
    int    side = PD2SMPGC::L;
    bool   bShowLowBound=false; 
    string fmt = "MM";
    
    bool   bSaved=false;

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
        else if(!strcmp(argv[i],"-check")) bCheck = true;
        else if(!strcmp(argv[i],"-low"))   bShowLowBound = true;
        else if(!strcmp(argv[i],"-side")) {
            if(!strcmp(argv[i+1],"R")) side = PD2SMPGC::R;
            i++;
        }
        else if(!strcmp(argv[i], "-fmt") ||!strcmp(argv[i],"-format")){
            fmt = argv[i+1]; 
            i++;
        }
        else if(!strcmp(argv[i],"-save"))  bSaved=true;
        else printf("Waringing, unused argument %s",argv[i]);
    }   

    if(fnames.empty()) {usage(); exit(0); }
   



    for(auto & fname : fnames){
        double iotime,ordtime;
        PD2SMPGCColoring *g = new PD2SMPGCColoring(fname, fmt.c_str(), bVerbose?(&iotime):nullptr, PD2SMPGC::L, "RANDOM", nullptr);
        if(bVerbose) {
                printf("%s\n",fname.c_str());
                printf("iotime"); if(iotime>60) { printf(" %d min",((int)iotime)/60); iotime= ((int)(iotime)%60)+(iotime- (int)(iotime)); }  printf(" %g sec\n",iotime);  
        }

        if(bShowLowBound || bSaved) {
            if(bShowLowBound){
                printf("LowBound of %s coloring is %d\n", (side==PD2SMPGC::L)?"L":"R", g->get_lowbound_coloring(side));
                if(bVerbose){
                    printf("LowBound of %s coloring is %d\n", (side==PD2SMPGC::L)?"R":"L", (side==PD2SMPGC::L)?(g->get_lowbound_coloring(PD2SMPGC::R)):(g->get_lowbound_coloring(PD2SMPGC::L)));
                    printf("|L|,|R|,|E|,Davg,Dstd/Davg,(L)Davg,(L)Dstd/Davg,(L)LowBound,(L)x,(R)Davg,(R)Dstd/Davg,(R)LowBound,(R)x\n");
                    printf("%d,%d,%d,%g,%g,%g,%g,%d,x,%g,%g,%d,x\n",
                            g->GetLeftVertexCount(), g->GetRightVertexCount(), g->GetEdgeCount(), 
                            g->GetAverageVertexDegree(), g->get_std_degree(PD2SMPGC::LR)/g->GetAverageVertexDegree(),
                            g->GetAverageRowVertexDegree(), g->get_std_degree(PD2SMPGC::L)/g->GetAverageRowVertexDegree(), g->GetMaximumColumnVertexDegree(),
                            g->GetAverageColumnVertexDegree(), g->get_std_degree(PD2SMPGC::R)/g->GetAverageColumnVertexDegree(), g->GetMaximumRowVertexDegree());
                }
            }
            
            if(bSaved==true){
                const int  N             = (side==PD2SMPGC::L)?(g->GetLeftVertexCount()):(g->GetRightVertexCount()); 
                const int  srcMaxDegree  = (side==PD2SMPGC::L)?(g->GetMaximumLeftVertexDegree()):(g->GetMaximumRightVertexDegree());
                const int  dstMaxDegree  = (side==PD2SMPGC::L)?(g->GetMaximumRightVertexDegree()):(g->GetMaximumLeftVertexDegree());
                const int  BufSize       = min(N, dstMaxDegree * srcMaxDegree +1);
                printf("BufSize_|L|_|R|_|E|_maxDL_maxDR_std/avg(L) std/avg(R)_std/avg(LR)");
                printf("\t%d\t%d\t%d\t%d\t%d\t%d", BufSize, g->GetLeftVertexCount(), g->GetRightVertexCount(), g->GetEdgeCount(),
                        g->GetMaximumLeftVertexDegree(), g->GetMaximumRightVertexDegree());
                printf("\t%g\t%g\t%g", g->get_std_degree(PD2SMPGC::L), g->get_std_degree(PD2SMPGC::R), g->get_std_degree(PD2SMPGC::LR));
                printf("\n");
            }
            continue;
        }

        for(auto & o : orders){
            g->global_ordering(side, o, bVerbose?(&ordtime):nullptr);
            if(bVerbose)  {
                printf("global order %s ordtime",o.c_str()); if(ordtime>60) { printf(" %d min",((int)ordtime)/60); ordtime= ((int)(ordtime)%60)+(ordtime- (int)(ordtime)); }  printf(" %g sec\n",ordtime);  
            }

            for(auto & m : methds) {
                for(auto nT : nTs) {
                    g->Coloring(side, nT, m);
                    if(bCheck) printf("%s\n", g->cnt_pd2conflict(side, g->get_vertex_colors())?"Failed":"Varified");
                }
            }//end for methods
        }//end for orders
        delete g;
    }//end for files
}


void usage(){
    fprintf(stderr, "\n"
            "NAME\n"
            "       ColPack - do graph coloring\n"
            "\n"
            "SYNOPISIS\n"
            "       ColPack [-f <list of graphs>] [-m <list of methods>] [-o <list of orders>] ...\n"
            "\n"
            "DESCRIPTION\n"
            "       the ColPack application shall take a list of commands and do the graph coloring. And display the results to the screen.\n"
            "       There are some specific commands for different methods. Find more details on 'https://github.com/ProbShin/ColPack'\n"
            "\n"
            "OPTIONS\n"
            "       the following options shall be supported:\n"
            "\n"
            "       -f files    Indicates the graph file path name.\n"
            "       -v          Indicates verbose flag will be truned on and there will display more rich infomration.\n" 
            "       -o orders   Indicates the orderings. Could be 'RANDOM','NATURAL','LARGEST_FIRST','SMALLEST_LAST','DYNAMIC_LARGEST_FIRST','INCIDENCE_DEGREE'... . There could be some specific ordering for specific methods\n"
            "       -m methods  Indicates the methods. Could be 'DISTANCE_ONE','ACYCLIC','STAR','DISTNACE_TWO','ROW_PARTIAL_DISTANCE_TWO','D1_OMP_GMMP','PD2_OMP_GMMP',...\n"
            "       -nT         Indicates number of threads used of parallel graph coloring\n"
            "       -side       Indiecate Row (L) or Column (R) side of coloring for parallel partial colroing.\n"
            "\n"
            "DESCRIPTION\n"
            "       if the method is one of 'DISTANCE_ONE','ACYCLIC','STAR','DISTANCE_TWO','ROW_PARTIAL_DISTANCE_TWO' The method belongs to gereral coloring on general graphs.\n" 
            "       if the method is one of 'ROW_PARTIAL_DISTANCE_TWO','COLUMN_PARTIAL_DISTANCE_TWO' The method belongs to partial coloring on bipartite graphs.\n" 
            "       if the method is one of 'IMPLICIT_COVERING__STAR_BICOLORING','EXPLICIT_COVERING__STAR_BICOLORING',EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING','IMPLICIT_COVERING__GREEDY_STAR_BICOLORING'. The method belongs to bicoloring on bipartite graphs.\n" 
            "       if the method is one of 'D1_OMP_GM3P, D1_OMP_GM3P_LF, D1_OMP_GMMP, D1_OMP_GMMP_LF,D1_OMP_SERIAL, D1_OMP_SERIAL_LF, D1_OMP_JP, D1_OMP_JP_LF ,D1_OMP_MTJP, D1_OMP_MTJP_LF, D1_OMP_HBJP_GM3P, D1_OMP_HBJP_GM3P_.., D1_OMP_HBJP_GMMP.., D1_OMP_HBJP_.... ,D1_OMP_HBMTP_GM3P, D1_OMP_HBMTJP_GM3P_.., D1_OMP_HBMTJP_GMMP.., D1_OMP_HBMTJP_.... ,D2_OMP_GM3P, D2_OMP_GM3P_LF ,D2_OMP_GMMP, D2_OMP_GMMP_LF ,D2_OMP_SERIAL, D2_OMP_SERIAL_LF' the method belongs to parallel general graph coloring\n"
            "       if the method is one of 'D2_OMP_SERIAL, PD2_OMP_GMMP, D2_OMP_GM3P, PD2_OMP_GMMP_LOLF, D2_OMP_GM3P_LOLF, PD2_OMP_GMMP_BIT, D2_OMP_GM3P_BIT, PD2_OMP_GMMP_BIT_LOLF, D2_OMP_GM3P_BIT_LOLF' the method blongs to parallel partial graph coloring on bipartite graph\n"
            "\n"
            "EXAMPLE\n"
            "./ColPack -f ../Graphs/bcsstk01.mtx -o LARGEST_FIRST RANDOM -m DISTANCE_ONE -v\n"
            "./ColPack -f ../Graphs/bcsstk01.mtx -o SMALLEST_LAST LARGEST_FIRST -m ACYCLIC -v\n"
            "./ColPack -f ../Graphs/bcsstk01.mtx -o RANDOM -m D1_OMP_GMMP -v -np 2\n"
            "./ColPack -f ../Graphs/bcsstk01.mtx -o RANDOM -m PD2_OMP_GMMP -v -np 4\n"
            "\n"
           ); 
}


