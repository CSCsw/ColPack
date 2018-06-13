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
int SMPGCInterface::cnt_d2coloring_conflict(int colors, const vector<INT>&vtxColor) {
    const int N = nodes();
    const vector<int>& vtxPtr = CSRia();
    const vector<int>& vtxVal = CSRja();
    int   conflicts = 0;
    #pragma omp parallel reduction(+:conflicts)
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
                    if( d012_neighbors.find(d2w)!=d012_neighbors.end() ) continue;
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
int SMPGCInterface::D2_OMP_GM3P(int nT, INT &colors, vector<INT>& vtxColor) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double tim_Tot=0;                          // run time
    double tim_Color  = 0;                     // run time
    double tim_Detect = 0;                     // run time
    double tim_Recolor= 0;                     // run time
    double tim_MxC    = 0;                     // run time
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
            QQ[tid].push_back(orig_ordered_vertex[i]);
        }
    }

    tim_Color =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        vector<INT> &Q = QQ[tid];
        vector<INT> Mask;
        const INT Nloc = Q.size();
        for(INT vit=0; vit<Nloc; vit++) {
            const auto v=Q[vit];
            unordered_set<INT> d012_neighbors={v};
            Mask.assign(MaxColorCapacity, -1);
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {
                const auto d1w  = verVal[d1wit];
                const auto d1wc = vtxColor[d1w];
                if(d1wc!=-1) Mask[d1wc] = v;
                d012_neighbors.insert(d1w);
            }
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) {
                    const auto d2w  = verVal[d2wit];
                    if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                    const auto d2wc = vtxColor[d2w];
                    if(d2wc!=-1) Mask[d2wc] = v;
                }
            }
            
            int c;
            for (c=0; c!=MaxColorCapacity; c++)
                if(Mask[c]!=v)
                    break;
            vtxColor[v] = c;
        } //end for
    }//end of omp parallel
    tim_Color  += omp_get_wtime();    

    // Phase 2: Detect conflicts
    tim_Detect =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid=omp_get_thread_num();
        vector<INT>& Q = QQ[tid];
        INT num_leftover = 0;
        const INT Nloc = Q.size();
        for(INT vit=0; vit<Nloc; vit++){
            const auto v  = Q[vit];
            const auto vc = vtxColor[v];
            unordered_set<INT> d012_neighbors={v};
            bool b_find_conflict=false;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                if(v>=d1w) continue;   // check conflict is little brother's job
                if(vc == vtxColor[d1w]) {
                    Q[num_leftover++]=v;
                    vtxColor[v]=-1;
                    b_find_conflict=true;
                    break;
                }
                d012_neighbors.insert(d1w);
            }
            if(b_find_conflict) continue;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) {
                    const auto d2w = verVal[d2wit];
                    if(v>=d2w) continue; // check conflict is little brother's job
                    if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                    if(vc == vtxColor[d2w]) {
                        Q[num_leftover++]=v;
                        vtxColor[v]=-1;
                        b_find_conflict=true;
                        break;
                    }
                }
                if(b_find_conflict) break;
            } //end for d1wit
        } //end for v
        Q.resize(num_leftover);
    } //end of omp parallel
    tim_Detect  += omp_get_wtime();
    
// Phase 3: Resolve conflicts
    tim_Recolor =- omp_get_wtime();
    vector<bool> Mark;
    for(int tid=0;tid<nT; tid++){
        nConflicts+=QQ[tid].size();
        for(const auto v: QQ[tid]){
            Mark.assign(MaxColorCapacity, -1);
            unordered_set<INT> b012_neighbors={v};
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w =verVal[d1wit];
                const auto d1wc=vtxColor[d1w];
                if(d1wc!=-1) Mark[d1wc]=v;
                b012_neighbors.insert(d1w);
            }
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(auto d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) {
                    const auto d2w =verVal[d2wit];
                    if(b012_neighbors.find(d2w)!=b012_neighbors.end()) continue;
                    const auto d2wc=vtxColor[d2w];
                    if(d2wc!=-1) Mark[d2wc]=v;
                    b012_neighbors.insert(d2w);
                }
            }
            INT c;
            for (c=0; c!=MaxColorCapacity; c++)
                if(Mark[c]!=v)
                    break;
            vtxColor[v] = c;
        }
    }
    tim_Recolor += omp_get_wtime();

    // get number of colors
    tim_MxC = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Color+tim_Detect+tim_Recolor+tim_MxC;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    int debug_conflict=cnt_d2coloring_conflict(colors, vector<INT> &vtxColor);
    if( debug_conflict!=0 )
        printf("fail D2Coloring, there are %d conflict in the graph.\n",debug_conflict);
    else
        printf("correct D2Coloring.\n");
    printf("***********************************************\n");
#endif  

    printf("@D2GM3P_nT_c_T_TColor_TDetect_TRecolor_TMxC_nCnf\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Color);
    printf("%lf\t", tim_Detect);
    printf("%lf\t", tim_Recolor);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nConflicts);
    return _TRUE;   
}




int SMPGCInterface::D2_OMP_GMMP(int nT, INT &colors, vector<INT>&vtxColors){
    printf("under construction!\n");
    return _TRUE;
}







