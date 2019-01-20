#include "PD2SMPGCColoring.h"
#include <cstring>
using namespace ColPack;
void usage();


#ifdef DEBUG_JP_PROFILE
string profile_name="profile_";
#endif

int main(int argc, char* argv[]) {
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
    return 0;
}

void usage(){
    fprintf(stderr, "\n\nUSAGE:\n"
            " $./ColPack -f <list of gname> -o <list of orderings> -m <list of methods> [-v] --nT <list of number threads>\n"
            "\n"
            " -f <gnames>  :  Input file names\n"
            " -o <orders>  :  NATURAL\n"
            "                 RANDOM\n"
            " -m <methods> :  PD2_<OMP/SERIAL>[_GMMP/_GM3P/_SERIAL][_BIT][_LOLF/LORD]\n" 
            "                 PD2_SERIAL\n"
            "                 PD2_OMP_SERIAL\n"
            "                 PD2_OMP_GMMP\n"
            "                 PD2_OMP_GM3P\n"
            "                 PD2_OMP_GMMP_LOLF\n"
            "                 PD2_OMP_GMMP_LOLR\n"
            "                 PD2_OMP_GMMP_BIT_LOLR\n"
            "                    ...            \n"
            "                 PD2_OMP_GM3P_VBBIT\n"
            "                 PD2_OMP_GM3P_VBBIT_LOLR\n"
            "\n"
            " -nT <threads>:  list of number threads, --nT also accept.\n"
            " -v           :  verbose for debug infomation\n"
            "\n"
            " -side        :  L/R , set coloring side. Default L\n"
            " -low         :  print low bound of number of colors\n"
            " -check       :  check coloring is correct or not\n"
            " -fmt         :  default \"MM\" is matrix market\n"
            "              :  \"SQRT\" is trans orignal graph into Bipartite graph s.t. each edge been broken by an extral vertex, which are Right Vertex. (Left Vertex are original vertex)\n"
            "\n"
            " -save        :  print if using BIT how much memory saved\n"
            "\n"
           ); 
}


