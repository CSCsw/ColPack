/*************************************************************************
    > File Name: SMPGCD2Interface.cpp
    > Author: xc
    > Descriptions: 
    > Created Time: Wed 13 Jun 2018 01:14:59 PM EDT
 ************************************************************************/

#include "SMPGCInterface.h"
#include <unordered_set>
using namespace std;
using namespace ColPack;



// ============================================================================
// check the graph validation
// ----------------------------------------------------------------------------
// uncolored vertex will not conflict with any other vertex
// ============================================================================
int SMPGCInterface::cnt_conflict(int colors, const verctor<INT>&vtxColor) {
    const int N = nodes();
    const vector<int>& vtxPtr = CSRia();
    const vector<int>& vtxVal = CSRja();
    int   conflits = 0;
    #pragma omp prallel reduction(+:conflicts)
    {
        #pragma omp for
        for(int v=0; v<N; v++){
            const auto vc = vtxColor[v];
            if(vc==-1) continue;
            unordered_set<INT> d012_neighbors={v};
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++){
                const auto d1w = vtxVal[d1wit];
                if( vc==vtxColor[d1w] ) conflicts++;
                d012_neighbors.insert(d1w);
            }
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++) {
                const auto d1w = vtxVal[d1wit];
                for(int d2wit=vtxPtr[d1w]; d2wit!=vtxPtr[d1w+1]; d2wit++){
                    const auto d2w = vtxVal[d2wit];
                    if( d0123_neighbors.find(d2w)!=d0123_neighbors.end() ) continue;
                    if( vc == vtxColor[d2w]) conflicts++;
                    d012_neighbors.insert(d2w);
                }
            }
        }
    }
    return conflicts;
}



// ============================================================================
// distance two coloring GM 3 phase
// ============================================================================
int SMPGCInterface::D2_OMP_GM3P(const int nT, INT &colors, vector<INT>& vtxColor) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double tim_color  = 0;                     // run time
    double tim_detect = 0;                     // run time
    double tim_recolor= 0;                     // run time
    double tim_Tot=0;                          // run time
    INT    nConflicts = 0;                     // Number of conflicts 
    
    const INT N = nodes();   //number of vertex
    const INT MaxColorCapacity = min(2*maxDeg()+2, N); //maxDegree
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verVal = CSRja();
    const vector<INT>& orig_ordered_vertex = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);

    vector<vector<INT>> QQ(nT);
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        QQ[tid].reserve(N/nT+1+16);
        #pragma omp for
        for(INT i=0; i<N; i++){
            QQ[tid].push_back(orig_ordered_vertex[i])
        }
    }

    tim_color =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        vector<INT> &Q = QQ[tid];
        vector<INT> Mask;
        const INT Nloc = Q.size();
        for(INT vit=0; vit<Nloc; vit++) {
            const auto v=Q[vit];
            unordered_set<INT> d012_neighbors;
            Mask.assign(MaxColorCapacity, -1);
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {
                const auto d1w  = verVal[d1wit];
                const auto d1wc = verColor[d1w];
                if(d1wc!=-1) Mask[d1wc] = v;
                d012_neighbors.insert(d1w);
            }
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[w]; d2wit!=verPtr[w+1]; d2wit++) {
                    const auto d2w  = verVal[d2wit];
                    if(d012_neighbors.find(d2w)!=d012_neighbor.end()) continue;
                    const auto d2wc = verColor[d2w];
                    if(d2wc!=-1) Mask[d2wc] = v;
                }
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
            const auto v  = Q[it];
            const auto vc = vtxColor[v];
            for(INT jt=verPtr[v],jtEnd=verPtr[v+1]; jt!=jtEnd; jt++) {
                const auto w = verInd[jt];
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
                const auto wc=vtxColor[verInd[wit]];
                if(wc >= 0) 
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



