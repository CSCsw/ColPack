/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "SMPGCInterface.h"
#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;


// ============================================================================
// Interface
// ============================================================================
int SMPGCInterface::Coloring(int nT, const string& method){
    
    if     (method.compare("DISTANCE_ONE_OMP_GM")==0) { return D1_OMP_GM(nT, num_colors_, vertex_color_);}
    else if(method.compare("DISTANCE_ONE_OMP_IP")==0) return D1_OMP_IP(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_IP_1")==0) return D1_OMP_IP_1(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_IP_2")==0) return D1_OMP_IP_2(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_IP_3")==0) return D1_OMP_IP_3(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_JP")==0) return D1_OMP_JP(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_LB")==0) return D1_OMP_LB(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_JP_AW_LF")==0) return D1_OMP_JP_AW_LF(nT, num_colors_, vertex_color_);
    else if(method.compare("DISTANCE_ONE_OMP_JP_AW_SL")==0) return D1_OMP_JP_AW_SL(nT, num_colors_, vertex_color_);
    else { fprintf(stdout, "Unknow method %s\n",method.c_str()); exit(1); }   
    return _TRUE;
}

// ============================================================================
// Construction
// ============================================================================
SMPGCInterface::SMPGCInterface(const string& graph_name)
: SMPGCOrdering(graph_name, "MM", nullptr, "NATURAL",nullptr) {
    INT N = SMPGCCore::nodes();
    vertex_color_.assign(N,-1);
    num_colors_=0;
}

// ============================================================================
// Construction
// ============================================================================
SMPGCInterface::SMPGCInterface(const string& graph_name, const string& fmt, double* iotime, const string& glb_order="NATURAL", double *ordtime=nullptr) 
: SMPGCOrdering(graph_name, fmt, iotime, glb_order, ordtime){
    INT N = SMPGCCore::nodes();
    vertex_color_.assign(N,-1);
    num_colors_=0;
}

// ============================================================================
// dump information
// ============================================================================
void SMPGCInterface::dump(){
    ;
}



// ============================================================================
// check if the graph is correct colored
// ============================================================================
SMPGCInterface::INT SMPGCInterface::cnt_conflict(INT colors, const vector<INT>& vtxColor){
    const INT N         = nodes();
    vector<INT>& vtxPtr = CSRia();
    vector<INT>& vtxVal = CSRja();
    INT Conflicts=0;
#pragma omp parallel reduction(+:Conflicts)
    {
#pragma omp for
    for (INT v=0; v<N; v++) {
        INT vc=vtxColor[v];
        for(INT wit=vtxPtr[v],witEnd=vtxPtr[v+1]; wit!=witEnd; wit++ ) {
            if ( vc == vtxColor[vtxVal[wit]] ) {
                if(v>vtxVal[wit])
                    Conflicts++; //sync_fetch_and_add(&Conflicts, 1); //increment the counter
            }
        }
    }

    }
    return Conflicts;
}



// ============================================================================
// based on Gebremedhin and Manne's GM algorithm [1]
// ============================================================================
int SMPGCInterface::D1_OMP_GM(int nT, INT&colors, vector<INT>&vtxColor) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);




    double tim_color=0,tim_detect=0, tim_recolor=0;    //run time
    double tim_Tot=0;                            //run time
    INT    nConflicts = 0;                       //Number of conflicts 
    
    const INT N = nodes();   //number of vertex
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verInd = CSRja();
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    const vector<INT>& Q = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);
    

    vector<vector<INT>> QQ(nT);
    const INT qtnt = N/nT;
    const INT rmnd = N%nT;
    for(INT tid=0; tid<nT; tid++){
        const INT Nloc = qtnt + (tid<rmnd?1:0);
        QQ[tid].reserve(Nloc);
    }

    tim_color =- omp_get_wtime();
#pragma omp parallel
    {
        //// Phase 0: Partition
        //const int Nloc =  qtnt + (tid<rmnd?1:0);
        //const int low  =  tid*qtnt + (tid<rmnd?tid:rmnd);
        //const int high =  low+Nloc;        
        //// Phase 1: Pseudo-color
        //for(int it=low; it<high; it++){
        
        vector<bool> Mask;
#pragma omp for
        for(INT it=0; it<N; it++) {
            INT v=Q[it];
            Mask.assign(MaxDegreeP1, false);
            for(INT jt=verPtr[v], jtEnd=verPtr[v+1]; jt!=jtEnd; jt++ ) {
                INT wColor;
                if( (wColor=vtxColor[verInd[jt]]) >= 0) 
                    Mask[wColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mask[c] == false)
                    break;
            vtxColor[v] = c;
        } //End of omp for
    }//end of omp parallel
    tim_color  += omp_get_wtime();    


// Phase 2: Detect conflicts
    tim_detect =- omp_get_wtime();
    #pragma omp parallel
    {
        int tid=omp_get_thread_num();
        vector<INT>& Qtmp = QQ[tid];
        //const int Nloc =  qtnt + (tid<rmnd?1:0);
        //const int low  =  tid*qtnt + (tid<rmnd?tid:rmnd);
        //const int high =  low+Nloc;
        //for(int qit=low; qit<high; qit++){
#pragma omp for
        for(INT it=0; it<N; it++){
            INT v  = Q[it];
            INT vc = vtxColor[v];
            for(INT jt=verPtr[v],jtEnd=verPtr[v+1]; jt!=jtEnd; jt++) {
                int w = verInd[jt];
                if(v<w && vc == vtxColor[w]) {
                    Qtmp.push_back(v);
                    vtxColor[v] = -1;  //Will prevent v from being in conflict in another pairing
                    break;
                } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
            } //end of inner for loop: w in adj(v)
        } //end of omp for
    } //end of omp parallel
    tim_detect  += omp_get_wtime();
    
// Phase 3: Resolve conflicts
    tim_recolor =- omp_get_wtime();
    vector<bool> Mark;
    for(int tid=0;tid<nT; tid++){
        if(QQ[tid].empty()) continue;
        nConflicts+=QQ[tid].size();
        for(auto v: QQ[tid]){
            Mark.assign(MaxDegreeP1, false);
            for(auto wit=verPtr[v], witEnd=verPtr[v+1]; wit!=witEnd; wit++ ) {
                INT wc;
                if( (wc=vtxColor[verInd[wit]]) >= 0) 
                    Mark[wc] = true; //assert(adjColor<Mark.size())
            } 
            INT c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mark[c] == false)
                    break;
            vtxColor[v] = c;
        }
    }
    tim_recolor += omp_get_wtime();


    // get number of colors
    double tim_4 = -omp_get_wtime();
#pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_4 += omp_get_wtime();

    tim_Tot = tim_color+tim_detect+tim_recolor+tim_4;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %d \n",nConflicts);  
    printf("Total Time                 : %lf sec\n", tim_Tot);
    printf("Phase para color           : %lf sec\n", tim_color);
    printf("Phase detect               : %lf sec\n", tim_detect);
    printf("Phase recolor              : %lf sec\n", tim_recolor);
    printf("Phase find max color       : %lf sec\n", tim_4);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("@GM_nT_c_T_Tcolor_Tdetect_Trecolor_TmaxC_nCnf\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_recolor);
    printf("%lf\t", tim_4);
    printf("%lld\n", nConflicts);
    return _TRUE;   
}




// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_IP(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    

    double tim_color=0, tim_detect=0, tim_total=0;   //run time
    INT   nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    
    const INT N   = nodes(); //number of vertex
    
    INT *verPtr  = CSRiaPtr();      //ia of csr
    INT *verInd  = CSRjaPtr();      //ja of csr
    

    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear();
    vtxColors.assign(N, -1);

    vector<INT> Q;
    Q.resize(N);

    vector<INT> Qtmp;
    Qtmp.resize(N);

    INT QTail=0, QtmpTail=0;
    
    #pragma omp parallel for
    for (INT i=0; i<N; i++) {
        Q[i]  = ordered_vertex()[i];
        Qtmp[i]= -1; //Empty queue
    }
    QTail = N;	//Queue all vertices
vector<double> wocaoTimes1;
vector<double> wocaoTimes2;
vector<INT> wocaoRemains;
double tim_maozhong =-omp_get_wtime();
    do {

#pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> Mark(MaxDegreeP1, -1);
            double t_color=0, t_detect=0;
            // phase psedue color
            if(tid==0) t_color -= omp_get_wtime();
            if(tid==0) wocaoTimes1.push_back(-omp_get_wtime());
#pragma omp for
            for (INT i=0; i<QTail; i++) {
                INT v = Q[i]; 
                Mark.assign(MaxDegreeP1, -1);

                for(INT wit = verPtr[v], witEnd=verPtr[v+1], nbColor; wit !=witEnd; wit++ ) {
                    INT w = verInd[wit];
                    if( (nbColor=vtxColors[w]) >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            } //End of omp for
            if(tid==0) t_color  += omp_get_wtime();
if(tid==0) wocaoTimes1[wocaoTimes1.size()-1]+=omp_get_wtime();
if(tid==0) wocaoTimes2.push_back(-omp_get_wtime());
            //phase Detect Conflicts:
            if(tid==0) t_detect -= omp_get_wtime();
#pragma omp for
            for (INT i=0; i<QTail; i++) {
                INT v = Q[i]; 
                for(INT wit=verPtr[v],witEnd=verPtr[v+1],w; wit!=witEnd; wit++) {
                    w = verInd[wit];
                    if(v>w && vtxColors[v] == vtxColors[w]) {
                        INT whereInQ = __sync_fetch_and_add(&QtmpTail, 1);
                        Qtmp[whereInQ] = v;//Add to the queue
                        vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                        break;
                    } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
                } //End of inner for loop: w in adj(v)
            }//End of outer for loop: for each vertex
            if(tid==0) t_detect  += omp_get_wtime();
            
            if(tid==0) {
                tim_color+=t_color;
                tim_detect+=t_detect;
            }
        
if(tid==0) wocaoTimes2[wocaoTimes2.size()-1]+=omp_get_wtime();
        } //end of omp parallel

wocaoRemains.push_back(QTail);
        nConflicts += QtmpTail;
        nLoops++;
        Q.swap(Qtmp);
        QTail=QtmpTail;
        QtmpTail=0;


    } while (QTail > 0);


    // get number of colors
    double tim_maxColor = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_maxColor += omp_get_wtime();

    
    tim_maozhong += omp_get_wtime();


    tim_total = tim_color+tim_detect+tim_maxColor;

#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %lld \n", nT);    
    printf("Total number of colors used: %lld \n", colors);    
    printf("Number of conflicts overall: %lld \n",nConflicts);  
    printf("Number of rounds           : %d \n", nLoops);      
    printf("Total Time                 : %lf sec\n", tim_total);
    printf("Time color                 : %lf sec\n", tim_color);
    printf("Time detect                : %lf sec\n", tim_detect);
    printf("Time max color             : %lf sec\n", tim_maxColor);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("@AGM_nT_c_T_Tcolor_Tdetect_TmaxC_Tmaozhong_nCnf_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_maxColor);
    printf("%lf\t", tim_maozhong);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    
    for(auto x:wocaoTimes1) printf("\t%g",x);
    for(auto x:wocaoTimes2) printf("\t%g",x);
    for(auto x:wocaoRemains) printf("\t%lld",x);

    printf("\n");      
    return _TRUE;
}


// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_IP_1(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_color=0, tim_detect=0, tim_total=0;   //run time
    INT   nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    
    const INT N   = nodes(); //number of vertex
    
    INT *verPtr  = CSRiaPtr();      //ia of csr
    INT *verInd  = CSRjaPtr();      //ja of csr
    
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear();
    vtxColors.assign(N, -1);

    vector<INT> Q;
    Q.resize(N);
    vector<INT> Qtmp;
    Qtmp.resize(N);

    INT QTail=0, QtmpTail=0;
    
    #pragma omp parallel for
    for (INT i=0; i<N; i++) {
        Q[i]  = ordered_vertex()[i];
        Qtmp[i]= -1; //Empty queue
    }
    QTail = N;	//Queue all vertices

    double tim_maozhong = -omp_get_wtime();
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        vector<INT> Mark(MaxDegreeP1, -1);
        double t_color=0, t_detect=0;
        INT QTailloc = QTail;
        do {
            // phase psedue color
            t_color -= omp_get_wtime();
#pragma omp for
            for (INT i=0; i<QTailloc; i++) {
                INT v = Q[i]; 
                Mark.assign(MaxDegreeP1, -1);

                for(INT wit = verPtr[v], witEnd=verPtr[v+1], nbColor; wit !=witEnd; wit++ ) {
                    INT w = verInd[wit];
                    if( (nbColor=vtxColors[w]) >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            } //End of omp for
            t_color  += omp_get_wtime();

            //phase Detect Conflicts:
            t_detect -= omp_get_wtime();
#pragma omp for
            for (INT i=0; i<QTailloc; i++) {
                INT v = Q[i]; 
                for(INT wit=verPtr[v],witEnd=verPtr[v+1],w; wit!=witEnd; wit++) {
                    w = verInd[wit];
                    if(v>w && vtxColors[v] == vtxColors[w]) {
                        INT whereInQ = __sync_fetch_and_add(&QtmpTail, 1);
                        Qtmp[whereInQ] = v;//Add to the queue
                        vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                        break;
                    } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
                } //End of inner for loop: w in adj(v)
            }//End of outer for loop: for each vertex
            t_detect  += omp_get_wtime();

#pragma omp single copyprivate(QTailloc)
            {
                nConflicts += QtmpTail;
                nLoops++;
                Q.swap(Qtmp);
                QTailloc=QtmpTail;
                QtmpTail=0;
            }
        } while (QTailloc > 0);
        
        if(tid==0) {
            tim_color+=t_color;
            tim_detect+=t_detect;
        }

    } //end of omp parallel

    double tim_maxColor = -omp_get_wtime();
    // get number of colors
#pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_maxColor += omp_get_wtime();

    tim_maozhong += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_maxColor;

#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %lld \n", nT);    
    printf("Total number of colors used: %lld \n", colors);    
    printf("Number of conflicts overall: %lld \n",nConflicts);  
    printf("Number of rounds           : %d \n", nLoops);      
    printf("Total Time                 : %lf sec\n", tim_total);
    printf("Time color                 : %lf sec\n", tim_color);
    printf("Time detect                : %lf sec\n", tim_detect);
    printf("Time max color             : %lf sec\n", tim_maxColor);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("@AGM1_nT_c_T_Tcolor_Tdetect_TmaxC_Tmaozhong_nCnf_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_maxColor);
    printf("%lf\t", tim_maozhong);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    printf("\n");      
    return _TRUE;
}


// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_IP_2(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_color=0, tim_detect=0, tim_total=0;   //run time
    INT   nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    
    const INT N   = nodes(); //number of vertex
    
    INT *verPtr  = CSRiaPtr();      //ia of csr
    INT *verInd  = CSRjaPtr();      //ja of csr
    
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear();
    vtxColors.assign(N, -1);

    vector<INT> Q;
    Q.resize(N);
    vector<INT> Qtmp;
    Qtmp.resize(N);

    INT QTail=0, QtmpTail=0;
    
    #pragma omp parallel for
    for (INT i=0; i<N; i++) {
        Q[i]  = ordered_vertex()[i];
        Qtmp[i]= -1; //Empty queue
    }
    QTail = N;	//Queue all vertices

    double tim_maozhong = -omp_get_wtime();
    do
    {

        tim_color -= omp_get_wtime();
#pragma omp parallel for
        for (INT i=0; i<QTail; i++) {
            INT v = Q[i]; 
            vector<INT> Mark(MaxDegreeP1, -1);

            for(INT wit = verPtr[v], witEnd=verPtr[v+1], nbColor; wit !=witEnd; wit++ ) {
                INT w = verInd[wit];
                if( (nbColor=vtxColors[w]) >= 0) 
                    Mark[nbColor] = w; //assert(adjColor<Mark.size())
            } 
            INT c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mark[c] == -1)
                    break;
            vtxColors[v] = c;
        } //End of omp for
        tim_color  += omp_get_wtime();

        //phase Detect Conflicts:
        tim_detect -= omp_get_wtime();
#pragma omp parallel for
        for (INT i=0; i<QTail; i++) {
            INT v = Q[i]; 
            for(INT wit=verPtr[v],witEnd=verPtr[v+1],w; wit!=witEnd; wit++) {
                w = verInd[wit];
                if(v>w && vtxColors[v] == vtxColors[w]) {
                    INT whereInQ = __sync_fetch_and_add(&QtmpTail, 1);
                    Qtmp[whereInQ] = v;//Add to the queue
                    vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                    break;
                } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
            } //End of inner for loop: w in adj(v)
        }//End of outer for loop: for each vertex
        tim_detect  += omp_get_wtime();

        nConflicts += QtmpTail;
        nLoops++;
        Q.swap(Qtmp);
        QTail=QtmpTail;
        QtmpTail=0;
    } while (QTail > 0);

    double tim_maxColor = -omp_get_wtime();
    // get number of colors
#pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_maxColor += omp_get_wtime();

    tim_maozhong += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_maxColor;

#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %lld \n", nT);    
    printf("Total number of colors used: %lld \n", colors);    
    printf("Number of conflicts overall: %lld \n",nConflicts);  
    printf("Number of rounds           : %d \n", nLoops);      
    printf("Total Time                 : %lf sec\n", tim_total);
    printf("Time color                 : %lf sec\n", tim_color);
    printf("Time detect                : %lf sec\n", tim_detect);
    printf("Time max color             : %lf sec\n", tim_maxColor);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("@AGM2_nT_c_T_Tcolor_Tdetect_TmaxC_Tmaozhong_nCnf_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_maxColor);
    printf("%lf\t", tim_maozhong);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    printf("\n");      
    return _TRUE;
}


// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_IP_3(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_color=0, tim_detect=0, tim_total=0;   //run time
    INT   nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    
    const INT N   = nodes(); //number of vertex
    
    INT const  * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear();
    vtxColors.assign(N, -1);

    vector<INT> Q;
    Q.resize(N);
    vector<INT> Qtmp;
    Qtmp.resize(N);

    vector<vector<INT>> MEM_QQ(nT);
    vector<vector<INT>> MEM_QQTail(nT);
    for(INT i=0; i<nT; i++)
        MEM_QQ[i].assign(N/nT+1+64, -1);  //+64 for sack of false shareing
    for(INT i=0; i<nT; i++)
        MEM_QQTail[i].assign(1+64, -1);  //+64 for sack of false shareing

    INT QTail=0, QtmpTail=0;
    
    #pragma omp parallel for
    for (INT i=0; i<N; i++) {
        Q[i]  = ordered_vertex()[i];
        Qtmp[i]= -1; //Empty queue
    }
    QTail = N;	//Queue all vertices

    double tim_maozhong = -omp_get_wtime();
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        vector<INT> Mark(MaxDegreeP1, -1);
        
        INT* QQ=&(MEM_QQ[tid][0]);
        INT* QQTail=&(MEM_QQTail[tid][0]);
        INT  QQTailloc=0;
        double t_color=0, t_detect=0;
        INT QTailloc = QTail;
        do {
            // phase psedue color
            t_color -= omp_get_wtime();
#pragma omp for
            for (INT i=0; i<QTailloc; i++) {
                INT v = Q[i]; 
                Mark.assign(MaxDegreeP1, -1);

                for(INT wit = verPtr[v], witEnd=verPtr[v+1], nbColor; wit !=witEnd; wit++ ) {
                    INT w = verInd[wit];
                    if( (nbColor=vtxColors[w]) >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            } //End of omp for
            t_color  += omp_get_wtime();

            //phase Detect Conflicts:
            t_detect -= omp_get_wtime();
            QQTailloc=0;
#pragma omp for
            for (INT i=0; i<QTailloc; i++) {
                INT v = Q[i]; 
                for(INT wit=verPtr[v],witEnd=verPtr[v+1],w; wit!=witEnd; wit++) {
                    w = verInd[wit];
                    if(v>w && vtxColors[v] == vtxColors[w]) {
                        QQ[QQTailloc++]=v;
                        vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                        break;
                    } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
                } //End of inner for loop: w in adj(v)
            }//End of outer for loop: for each vertex
            *QQTail=QQTailloc;
            t_detect  += omp_get_wtime();

#pragma omp single copyprivate(QTailloc)
            {
                INT nConfloc = 0;
                for(int i=0; i<nT; i++){
                    INT tmpN = MEM_QQTail[i][0];
                    nConfloc += tmpN;
                    for(int j=0; j<tmpN; j++)
                        Qtmp[QtmpTail++]=MEM_QQ[i][j];//TODO
                }
                nConflicts += nConfloc;
                nLoops++;
                Q.swap(Qtmp);
                QTailloc=QtmpTail;
                QtmpTail=0;
            }
        } while (QTailloc > 0);
        
        if(tid==0) {
            tim_color+=t_color;
            tim_detect+=t_detect;
        }

    } //end of omp parallel

    double tim_maxColor = -omp_get_wtime();
    // get number of colors
#pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_maxColor += omp_get_wtime();

    tim_maozhong += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_maxColor;

#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %lld \n", nT);    
    printf("Total number of colors used: %lld \n", colors);    
    printf("Number of conflicts overall: %lld \n",nConflicts);  
    printf("Number of rounds           : %d \n", nLoops);      
    printf("Total Time                 : %lf sec\n", tim_total);
    printf("Time color                 : %lf sec\n", tim_color);
    printf("Time detect                : %lf sec\n", tim_detect);
    printf("Time max color             : %lf sec\n", tim_maxColor);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("@AGM3_nT_c_T_Tcolor_Tdetect_TmaxC_Tmaozhong_nCnf_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_maxColor);
    printf("%lf\t", tim_maozhong);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    printf("\n");      
    return _TRUE;
}




// ============================================================================
// based on Luby's algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_LB(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_Wgt=0;    //run time
    double tim_MIS=0;    //run time
    double tim_ReG=0;    //run time
    double tim_Tot=0;               //run time
    

    const INT N = nodes(); //number of vertex
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();         //ja of csr
    
    colors=0;
    vtxColors.clear();
    vtxColors.resize(N, -1);

    //const int qtnt = N/nT;
    //const int rmnd = N%nT;
    
    vector<INT> Q; Q.resize(N,-1);
    INT QTail=0;
    
    vector<vector<int>>  QQ;
    QQ.resize(nT);
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1);
    }

    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }
    QTail = N;


    // generate rando m numbers
tim_Wgt =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
tim_Wgt +=omp_get_wtime();


    colors=0;
    do{
tim_MIS -= omp_get_wtime();
#pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> candi;
#pragma omp for
        for(INT vit=0; vit<QTail; vit++){
            INT v = Q[vit];
            double vwt = WeightRnd[v];
            bool bDomain=true;
            for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                INT nb = verInd[k];
                if(vtxColors[nb]!=-1) {
                    continue;
                }
                double nbwt = WeightRnd[nb];
                if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) {
                candi.push_back(v);//vtxColors[v]=colors;
            }
            else{
                QQ[tid].push_back(v);
            }
        } //end of omp for
        
        for(auto v : candi) vtxColors[v]=colors;
        } //end of omp parallel

        colors++;
tim_MIS += omp_get_wtime();
tim_ReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].clear(); //resize(0);
        }
        //printf("color, Qtail = %d, %d\n",colors, QTail);
tim_ReG += omp_get_wtime();
    }while(QTail!=0);

tim_Tot = tim_Wgt+tim_MIS+tim_ReG;
    printf("@LB_nT_c_T_Twt_TPse_TReG\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf", tim_ReG);
    printf("\n");
    return _TRUE;
}

// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_Wgt  =0;    //run time
    double tim_MIS =0;
    double tim_ReG =0;    //run time
    double tim_MxC =0;    //run time
    double tim_Tot =0;               //run time
    INT    nLoops = 0;                         //Number of rounds 
 
    const INT N = nodes(); //number of vertex
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();         //ja of csr
    
    colors=0;
    vtxColors.clear();
    vtxColors.resize(N, -1);
   
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    vector<INT> Q; Q.resize(N,-1);
    INT QTail=0;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
#pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }
    QTail = N;

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    vector<double> WeightDeg;
    WeightDeg.resize(N,-1);

tim_Wgt =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
tim_Wgt +=omp_get_wtime();
    
    colors=0;
    do{
        //printf("color%d_Qtail%d\n",colors,QTail);
        //phase 0: find maximal indenpenent set, and color it
tim_MIS -= omp_get_wtime();
#pragma omp parallel
{
    int tid = omp_get_thread_num();
    vector<INT> candi;
#pragma omp for
        for(INT vit=0; vit<QTail; vit++){
            INT v = Q[vit];
            double vwt = WeightRnd[v];
            bool bDomain=true;
            for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                INT nb = verInd[k];
                if(vtxColors[nb]!=-1) continue;
                double nbwt = WeightRnd[nb];
                if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) {
                candi.push_back(v);
            }
            else
                QQ[tid].push_back(v);
        } //end of omp for

        for(auto v : candi) {
            vector<INT> MASK(MaxDegreeP1,-1);
            for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                INT nb = verInd[k];
                INT nbcolor=vtxColors[nb];
                if( nbcolor>=0 ){
                    MASK[nbcolor]=nb;
                }
            }
            int c;
            for(c=0; c<MaxDegreeP1; c++)
                if(MASK[c]==-1) break;
            vtxColors[v]=c;
        }
}   //end of omp parallel
tim_MIS += omp_get_wtime();
tim_ReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].clear();
        }
tim_ReG += omp_get_wtime();
    nLoops++;
    }while(QTail!=0);

tim_MxC = -omp_get_wtime();
//for(int i=0; i<N; i++) printf("%d-%d\n",i,vtxColors[i]);
colors=0;
#pragma omp parallel for reduction(max:colors)
for(int i=0; i<N; i++){
    auto c = vtxColors[i];
    if(c>colors) colors=c;
}
tim_MxC += omp_get_wtime();

tim_Tot =tim_Wgt+ tim_MIS+tim_ReG+tim_MxC;
    printf("@JP_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);
    return _TRUE;
}

// ============================================================================
// based on Allwright's LF JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_AW_LF(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_MIS=0;    //run time
    double tim_Wgt=0;    //run time
    double tim_ReG=0;    //run time
    double tim_MxC=0;    //run time
    double tim_Tot=0;               //run time
    INT    nLoops = 0;                         //Number of rounds 
    
    const INT N = nodes(); //GraphCore::GetVertexCount(); //number of vertex
    
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear();
    vtxColors.resize(N, -1);
   
    vector<INT> Q; Q.resize(N,-1);
    int QTail=0;
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }
    QTail = N;
   
    vector<vector<INT>>  QQ; QQ.resize(nT);
#pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    vector<double> WeightDeg;
    WeightDeg.resize(N,-1);

tim_Wgt =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(INT i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
    for(auto v: Q)
        WeightDeg[v] = verPtr[v+1]-verPtr[v];
tim_Wgt +=omp_get_wtime();
    
    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
#pragma omp parallel
        {
            INT tid = omp_get_thread_num();
            vector<INT> candi;
#pragma omp for
            for(INT vit=0; vit<QTail; vit++){
                INT v = Q[vit];
                double vwt = WeightDeg[v];
                bool bDomain=true;
                for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    int nb = verInd[k];
                    if(vtxColors[nb]>=0) continue;
                    double nbwt = WeightDeg[nb];
                    if(vwt<nbwt || (vwt==nbwt && WeightRnd[v]>WeightRnd[nb])){
                        bDomain=false;
                        break;
                    }
                }
                if(bDomain) {
                    candi.push_back(v);
                }
                else
                    QQ[tid].push_back(v);
            } //end of omp for

            for(INT v : candi){
                vector<INT> MASK(MaxDegreeP1,-1);
                for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    int nb = verInd[k];
                    int nbcolor=vtxColors[nb];
                    if( nbcolor>=0 ){
                        MASK[nbcolor]=nb;
                    }
                }
                int c;
                for(c=0; c<MaxDegreeP1; c++)
                    if(MASK[c]==-1) break;
                vtxColors[v]=c;
            }
        } //end of omp block
        tim_MIS += omp_get_wtime();
        tim_ReG -= omp_get_wtime();

        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].resize(0);
        }

        tim_ReG += omp_get_wtime();
        nLoops++;
    }while(QTail!=0);

    tim_MxC = -omp_get_wtime();
    colors=0;
    #pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        auto c = vtxColors[i];
        if(c>colors) colors=c;
    }
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Wgt+tim_MIS+ tim_ReG+tim_MxC;

    printf("@AJPLF_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoops\t");
    printf("%d\t",  nT);
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);
    return _TRUE;
}

// ============================================================================
// based on Allwright's SL JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_AW_SL(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_Wgt=0;    //run time
    double tim_MISReG=0;    //run time
    double tim_MxC=0;    //run time
    double tim_Tot=0;               //run time
    INT    nLoops = 0;                         //Number of rounds 
    
    const INT N = nodes(); 
   
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
    
    vector<INT> MEM_Q;  MEM_Q.resize(N,-1);
    INT MEM_QTail=0;
    vector<INT> MEM_Q2;  MEM_Q2.resize(N,-1);
    INT MEM_QTail2=0;
    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        MEM_Q[i]  = ordered_vertex()[i];
    }
    MEM_QTail = N;
    
    vector<vector<INT>>  MEM_QQ; MEM_QQ.resize(nT);
    #pragma omp parallel for
    for(INT i=0;i<nT; i++){
        MEM_QQ[i].resize(N/nT+1+16);
    }

    // weight 
    vector<double> WeightRnd; WeightRnd.resize(N,-1);
    vector<INT> WeightDeg; WeightDeg.resize(N,-1);  //? false shareing potential v.s. cache line benefit

tim_Wgt =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(INT i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
  
    vector<INT> groupID(N);
    vector<INT> elementID(N);
    vector<vector<INT>> vv_deg2vs(MaxDegreeP1);
    {
        for(INT v=0; v<N; v++){
            INT deg = verPtr[v+1]-verPtr[v];
            groupID[v]=deg;
            elementID[v]=vv_deg2vs[deg].size();
            vv_deg2vs[deg].push_back(v);   
        }
        
        INT mxDeg = MaxDegreeP1-1;
        int iMin = 0;
        for(INT i=0; i<N; i++){

        }
    }
    
    
    //get SL by parallel
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        INT  myQTail = MEM_QTail;
        vector<INT> candi;
        INT * QQ = &MEM_QQ[tid][0];
        INT QQsize = 0;
        INT k=0;   //SL degree
        while(myQTail!=0){
            candi.clear();
            QQsize=0;
#pragma omp for nowait
            for(INT it=0; it<myQTail; it++){
                INT v = MEM_Q[it];
                INT jt=verPtr[v], jtEnd = verPtr[v+1];
                INT deg = jtEnd-jt;

                for(; jt<jtEnd && deg>k; jt++)
                    if(WeightDeg[verInd[jt]]!=-1) 
                        deg--;
                
                if(deg <= k) 
                    candi.push_back(v);
                else
                    QQ[QQsize++]=v;
            }//end of omp for 

            for(auto v : candi)
                WeightDeg[v] = k;  //?false sharing potential
            
            INT whereInQ =__sync_fetch_and_add(&MEM_QTail2, QQsize);
            
            // Q += QQ
            for(INT i=0; i<QQsize; i++) {
                MEM_Q2[whereInQ++]=QQ[i];
            }
#pragma omp barrier
#pragma omp single
            {
                MEM_Q.swap(MEM_Q2);
                MEM_QTail = MEM_QTail2;
                MEM_QTail2=0;
            }
            myQTail = MEM_QTail;
            k++;
        } //end of while
    }// end of omp parallel to get SL weight

    
tim_Wgt +=omp_get_wtime();
    

#pragma omp parallel for
    for(INT i=0; i<N; i++){
        MEM_Q[i]  = ordered_vertex()[i];
    }
    MEM_QTail = N;


    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
        tim_MISReG -= omp_get_wtime();
#pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> candi;
            INT *QQ = &MEM_QQ[tid][0];
            INT QQsize = 0;
#pragma omp for nowait
            for(INT vit=0; vit<MEM_QTail; vit++){
                INT v = MEM_Q[vit];
                double vwt = WeightDeg[v];
                bool bDomain=true;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    INT nb = verInd[k];
                    if(vtxColors[nb]>=0) continue;
                    double nbwt = WeightDeg[nb];
                    if(vwt<nbwt || (vwt==nbwt && WeightRnd[v]>WeightRnd[nb])){
                        bDomain=false;
                        break;
                    }
                }
                if(bDomain) {
                    candi.push_back(v);
                }
                else
                    MEM_QQ[tid][QQsize++]=v; //.push_back(v);
            } //end of omp for
            
            // safely coloring candidate vertex
            for(INT v : candi){
                vector<INT> MASK(MaxDegreeP1,-1);
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    INT nb = verInd[k];
                    INT nbcolor=vtxColors[nb];
                    if( nbcolor>=0 ){
                        MASK[nbcolor]=nb;
                    }
                }
                INT c;
                for(c=0; c<MaxDegreeP1; c++)
                    if(MASK[c]==-1) break;
                vtxColors[v]=c;
            }
        
        //phase 2: rebuild the graph
        INT whereInQ =__sync_fetch_and_add(&MEM_QTail2, QQsize);
        for(INT i=0; i<QQsize; i++) {
            MEM_Q2[whereInQ++]=QQ[i];
        }
#pragma omp barrier
#pragma omp single
        {
            MEM_Q.swap(MEM_Q2);
            MEM_QTail=MEM_QTail2;
            MEM_QTail2=0;
        }

      }// end of omp parallel
      tim_MISReG += omp_get_wtime();
        nLoops++;
    }while(MEM_QTail!=0);


    tim_MxC = - omp_get_wtime();
    colors=0;
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        auto c = vtxColors[i];
        if(c>colors) colors=c;
    }
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Wgt+tim_MISReG+tim_MxC;

    printf("@AJPSL_np_c_T_TWgt_TMISReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MISReG);
    printf("%lf\t", tim_MxC);
    printf("%lld\n",nLoops);
    return _TRUE;
}






