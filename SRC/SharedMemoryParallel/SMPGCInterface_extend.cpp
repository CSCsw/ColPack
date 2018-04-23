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
// based on Gebremedhin and Manne's GM algorithm [1]
// ============================================================================
int SMPGCInterface::D1_OMP_GM_LO(int nT, INT&colors, vector<INT>&vtxColor, const string& local_ordering) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double tim_Tot     =.0;     // total time
    double tim_loc_ord =.0;     // local ordering time
    double tim_color   =.0;     // pseudo coloring time
    double tim_detect  =.0;     // detect conflicts time
    double tim_recolor =.0;     // recolor remains conflicted graph time

    INT    nConflicts = 0;      // Number of conflicts 
    const INT N = nodes();      // number of vertex
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verInd = CSRja();
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    const vector<INT>& Q = ordered_vertex(); 
   

    colors=0;                       
    vtxColor.assign(N, -1);

    vector<vector<INT>> QQ(nT);
    for(INT tid=0; tid<nT; tid++){
        QQ[tid].reserve(N/nT+1+64/sizeof(INT));
    }
    
    // set up memory
    vector<vector<INT>> MEM_LocalOrderings(nT);
    #pragma omp parallel
    {   
        int tid = omp_get_thread_num();
        MEM_LocalOrderings[tid].reserve(N/nT+1+64/sizeof(INT));  // 64/8 is for false sharing 
        #pragma omp for
        for(INT v=0; v<N; v++){
            MEM_LocalOrderings[tid].push_back(Q[v]);
        }
    }// end of omp parallel


    if(local_ordering=="LARGES_FIRST"){
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_largest_degree_first_ordering(MEM_LocalOrderings[tid]);
        }
        tim_loc_ord += omp_get_wtime();
    }
    else if(local_ordering=="SMALLEST_LAST"){
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_smallest_degree_last_ordering(MEM_LocalOrdering[tid]);
        }
        tim_loc_ord += omp_get_wtime();
    }
    else{
        fprintf(stderr,"Warning! Unknown local ordering: %s, using 'local natural ordering' instead.\n",local_ordering.c_str());
    }



    tim_color =- omp_get_wtime();
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        vector<INT>& LocalOrdering=MEM_LocalOrderings[tid];
        vector<bool> Mask;
        for(auto v : LocalOrdering) {
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
    double tim_maxc = -omp_get_wtime();
#pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_maxc += omp_get_wtime();

    tim_Tot = tim_loc_ord + tim_color+tim_detect+tim_recolor+tim_maxc;

    printf("@GMlo_nT_c_T_TlocOrd_Tcolor_Tdetect_Trecolor_TmaxC_nCnf\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_loc_ord);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_recolor);
    printf("%lf\t", tim_4);
    printf("%lld\n", nConflicts);
    return _TRUE;   
} //end of fun D1_OMP_GM_LO



// ============================================================================
//
// ============================================================================
int SMPGCInterface::D1_OMP_IP(int nT, INT&colors, vector<INT>&vtxColors) {

}


