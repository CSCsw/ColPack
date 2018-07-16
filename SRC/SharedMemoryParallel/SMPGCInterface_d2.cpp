/*************************************************************************
    > File Name: SMPGCD2Interface.cpp
    > Author: xc
    > Descriptions: 
    > Created Time: Wed 13 Jun 2018 01:14:59 PM EDT
 ************************************************************************/

#include "SMPGCInterface.h"
#include <unordered_set>
#include <unordered_map>
using namespace std;
using namespace ColPack;



// ============================================================================
// check the graph validation
// ----------------------------------------------------------------------------
// uncolored vertex will not conflict with any other vertex
// ============================================================================
int SMPGCInterface::cnt_d2conflict(const vector<INT>&vtxColor) {
    //do it serial
    if(0)
    {

        const int N = nodes();
        const vector<int>& vtxPtr = CSRia();
        const vector<int>& vtxVal = CSRja();
        int   conflicts = 0;
        
        vector<int> profile_uc_nodes;
        unordered_map<int, unordered_set<int>> profile_confs;

        for(int v=0; v<N; v++){
            const auto vc = vtxColor[v];
            if(vc==-1) { profile_uc_nodes.push_back(v); continue; }
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++){  // check d1 neighbors
                const auto d1w = vtxVal[d1wit];
                if( vc==vtxColor[d1w] ) {
                    profile_confs[min(v,d1w)].insert(max(v,d1w));
                    conflicts++;
                }
            }
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++) { 
                const auto d1w = vtxVal[d1wit];
                for(int d2wit=vtxPtr[d1w]; d2wit!=vtxPtr[d1w+1]; d2wit++){  // check d2 neighbors
                    const auto d2w = vtxVal[d2wit];
                    if(v==d2w) continue;
                    if( vc == vtxColor[d2w]) { 
                        conflicts++;
                        profile_confs[min(v,d2w)].insert(max(v,d2w));
                    }
                }
            }
        }
        if(profile_uc_nodes.size()!=0) printf("\nCNTCONF uncolored_nodes %d\n",(int)profile_uc_nodes.size());
        if(profile_confs.size()!=0) { 
            printf("\nConflicts:\n");
            int cnt_lines=0;
            for(const auto& x : profile_confs){
                printf("[%d(%d)]:",x.first, vtxColor[x.first]);
                int cnt_cols=0;
                for(const auto &y : x.second){
                    printf("\t%d(%d)", y, vtxColor[y]);
                    if(cnt_cols++>10) {printf("..."); break; }
                }
                if(cnt_lines++>10) { printf("\n...\n"); break; }
                else printf("\n");
            }
        }
        
        return conflicts;
    }
    // do it in parallel
    const int N = nodes();
    const vector<int>& vtxPtr = CSRia();
    const vector<int>& vtxVal = CSRja();
    int   conflicts = 0;
    int   uc_num_nodes=0;
    
    #pragma omp parallel reduction(+:conflicts), reduction(+: uc_num_nodes)
    {
        unordered_map<int, unordered_set<int>> profile_confs;
        #pragma omp for
        for(int v=0; v<N; v++){
            const auto vc = vtxColor[v];
            if(vc==-1) { uc_num_nodes++;  continue; }
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++){  // check d1 neighbors
                const auto d1w = vtxVal[d1wit];
                if( vc==vtxColor[d1w] ) { profile_confs[min(v,d1w)].insert(max(v,d1w)); }
            }
            for(int d1wit=vtxPtr[v]; d1wit!=vtxPtr[v+1]; d1wit++) { 
                const auto d1w = vtxVal[d1wit];
                for(int d2wit=vtxPtr[d1w]; d2wit!=vtxPtr[d1w+1]; d2wit++){  // check d2 neighbors
                    const auto d2w = vtxVal[d2wit];
                    if(v==d2w) continue;
                    if( vc == vtxColor[d2w]) profile_confs[min(v,d2w)].insert(max(v,d2w)); 
                }
            }
        }
        for(const auto &p : profile_confs)
            conflicts+= (p.second).size();
    }
    if(uc_num_nodes) printf("Warning, uncolored vertices %d\n", uc_num_nodes);
    return conflicts;
}



// ============================================================================
// distance two coloring GM 3 phase
// ============================================================================
int SMPGCInterface::D2_OMP_GM3P(int nT, INT &colors, vector<INT>& vtxColor) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double ttmp = 0;
    ttmp =- omp_get_wtime();

    double tim_Tot=0;                          // run time
    double tim_Color  = 0;                     // run time
    double tim_Detect = 0;                     // run time
    double tim_Recolor= 0;                     // run time
    double tim_MxC    = 0;                     // run time
    INT    nConflicts = 0;                     // Number of conflicts 
    
    const INT N = nodes();                     //number of vertex
    const INT MaxColorCapacity =min( maxDeg()*(maxDeg()-1)+1, N); //maxDegree
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verVal = CSRja();
    const vector<INT>& orig_ordered_vertex = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);

    ttmp += omp_get_wtime();
    printf("dec %g ", ttmp);
    ttmp =- omp_get_wtime();
    
    vector<vector<INT>> QQ(nT);
    {
        vector<int> lens (nT, N/nT);
        for(int i=0; i<N%nT; i++)
            lens[i]++;
        vector<int> begs(nT+1,0);
        for(int i=1; i<=nT; i++)
            begs[i]=begs[i-1]+lens[i-1];
        for(int i=0; i<nT; i++){
            QQ[i].reserve(N/nT+1+16);
            QQ[i].assign(orig_ordered_vertex.begin()+begs[i], orig_ordered_vertex.begin()+begs[i+1]); 
        }
    }
    ttmp += omp_get_wtime();
    printf("ini %g ", ttmp);
   
    // Phase - Pseudo Coloring
    tim_Color =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        vector<INT> &Q = QQ[tid];
        vector<INT> Mask; Mask.assign(MaxColorCapacity, -1);
        const INT Nloc = Q.size();
        for(INT vit=0; vit<Nloc; vit++) {
            const auto v=Q[vit];
            //unordered_set<INT> d012_neighbors={v};
            //Mask.assign(MaxColorCapacity, -1);
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {  // d1 neighbors
                const auto d1w  = verVal[d1wit];
                const auto d1wc = vtxColor[d1w];
                if(d1wc!=-1) Mask[d1wc] = v;
                //d012_neighbors.insert(d1w);
            }
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    const auto d2w  = verVal[d2wit];
                    if(v==d2w) continue;
                    //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                    const auto d2wc = vtxColor[d2w];
                    if(d2wc!=-1) Mask[d2wc] = v;
                }
            }
            
            INT c;
            for (c=0; c!=MaxColorCapacity; c++)
                if(Mask[c]!=v)
                    break;
            vtxColor[v] = c;
        } //end for
    }//end of omp parallel
    tim_Color  += omp_get_wtime();    

    // Phase - Detect Conflicts
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
            //unordered_set<INT> d012_neighbors={v};
            bool b_find_conflict=false;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                const auto d1w = verVal[d1wit];
                if(v>=d1w) continue;   // check conflict is little brother's job
                if(vc == vtxColor[d1w]) {
                    Q[num_leftover++]=v;
                    vtxColor[v]=-1;
                    b_find_conflict=true;
                    break;
                }
                //d012_neighbors.insert(d1w);
            }
            if(b_find_conflict) continue;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    const auto d2w = verVal[d2wit];
                    if(v>=d2w) continue; // check conflict is little brother's job
                    //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
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
   
    // Phase - Resolve Conflicts
    tim_Recolor =- omp_get_wtime();
    vector<INT> Mark; Mark.assign(MaxColorCapacity,-1);
    for(int tid=0;tid<nT; tid++){
        nConflicts+=QQ[tid].size();
        for(const auto v: QQ[tid]){
            //Mark.assign(MaxColorCapacity, -1);
            //unordered_set<INT> b012_neighbors={v};
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                const auto d1w =verVal[d1wit];
                const auto d1wc=vtxColor[d1w];
                if(d1wc!=-1) Mark[d1wc]=v;
                //b012_neighbors.insert(d1w);
            }
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { 
                const auto d1w = verVal[d1wit];
                for(auto d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    
                    const auto d2w =verVal[d2wit];
                    if(v==d2w) continue;
                    //if(b012_neighbors.find(d2w)!=b012_neighbors.end()) continue;
                    const auto d2wc=vtxColor[d2w];
                    if(d2wc!=-1) Mark[d2wc]=v;
                    //b012_neighbors.insert(d2w);
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



// ============================================================================
// Distance Two Openmp Multiple Phase Coloring
// ============================================================================
int SMPGCInterface::D2_OMP_GMMP(int nT, INT &colors, vector<INT>&vtxColor){
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
   
    double ttmp = 0;
    ttmp = - omp_get_wtime();

    double tim_Tot=0;                          // run time
    double tim_Color  = 0;                     // run time
    double tim_Detect = 0;                     // run time
    double tim_Sumconf= 0;                     // run time
    double tim_MxC    = 0;                     // run time
    INT    nConflicts = 0;                     // Number of conflicts 
    int    nLoops     = 0;

    const INT N = nodes();                     //number of vertex
    const INT MaxColorCapacity =min( maxDeg()*(maxDeg()-1)+1, N); //maxDegree
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verVal = CSRja();
    const vector<INT>& orig_ordered_vertex = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);

    ttmp += omp_get_wtime();
    printf("dec %g ", ttmp);
    ttmp = -omp_get_wtime();

    vector<vector<INT>> QQ(nT);
    {
        vector<int> lens (nT, N/nT);
        for(int i=0; i<N%nT; i++)
            lens[i]++;
        vector<int> begs(nT+1,0);
        for(int i=1; i<=nT; i++)
            begs[i]=begs[i-1]+lens[i-1];
        for(int i=0; i<nT; i++){
            QQ[i].reserve(N/nT+1+16);
            QQ[i].assign(orig_ordered_vertex.begin()+begs[i], orig_ordered_vertex.begin()+begs[i+1]); 
        }
    }

    ttmp += omp_get_wtime();
    printf("ini %g ", ttmp);
    
    INT num_un_colored_vertices=N;
    while(num_un_colored_vertices!=0){
        // Phase - Pseudo Coloring
        tim_Color -= omp_get_wtime();
        #pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            vector<INT> &Q = QQ[tid];
            vector<INT> Mask; Mask.assign(MaxColorCapacity,-1);
            const INT Nloc = Q.size();
            for(INT vit=0; vit<Nloc; vit++) {
                const auto v=Q[vit];
                //unordered_set<INT> d012_neighbors={v};
                //Mask.assign(MaxColorCapacity, -1);
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {  // d1 neighbors
                    const auto d1w  = verVal[d1wit];
                    const auto d1wc = vtxColor[d1w];
                    if(d1wc!=-1) Mask[d1wc] = v;
                    //d012_neighbors.insert(d1w);
                }
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                    const auto d1w = verVal[d1wit];
                    for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                        const auto d2w  = verVal[d2wit];
                        //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                        const auto d2wc = vtxColor[d2w];
                        if(d2wc!=-1) Mask[d2wc] = v;
                    }
                }
                INT c;
                for (c=0; c!=MaxColorCapacity; c++)
                    if(Mask[c]!=v)
                        break;
                vtxColor[v] = c;
            } //end for
        }//end of omp parallel
        tim_Color  += omp_get_wtime();    

        // Phase - Detect Conflicts
        tim_Detect -= omp_get_wtime();
        #pragma omp parallel
        {
            const int tid=omp_get_thread_num();
            vector<INT>& Q = QQ[tid];
            INT num_leftover = 0;
            const INT Nloc = Q.size();
            for(INT vit=0; vit<Nloc; vit++){
                const auto v  = Q[vit];
                const auto vc = vtxColor[v];
                //unordered_set<INT> d012_neighbors={v};
                bool b_find_conflict=false;
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                    const auto d1w = verVal[d1wit];
                    if(v>=d1w) continue;   // check conflict is little brother's job
                    if(vc == vtxColor[d1w]) {
                        Q[num_leftover++]=v;
                        vtxColor[v]=-1;
                        b_find_conflict=true;
                        break;
                    }
                    //d012_neighbors.insert(d1w);
                }
                if(b_find_conflict) continue;
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                    const auto d1w = verVal[d1wit];
                    for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                        const auto d2w = verVal[d2wit];
                        if(v>=d2w) continue; // check conflict is little brother's job
                        //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
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
    

        tim_Sumconf -= omp_get_wtime();
        num_un_colored_vertices=0;
        for(int tid=0;tid<nT; tid++)
            num_un_colored_vertices+=QQ[tid].size();
        nConflicts+=num_un_colored_vertices;
        nLoops++;
        tim_Sumconf += omp_get_wtime();
    }

    // get number of colors
    tim_MxC = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Color+tim_Detect+tim_Sumconf+tim_MxC;

    printf("@D2GMMP_nT_c_T_TColor_TDetect_TSumconf_TMxC_nCnf_nLoop\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Color);
    printf("%lf\t", tim_Detect);
    printf("%lf\t", tim_Sumconf);
    printf("%lf\t", tim_MxC);
    printf("%d\t", nConflicts);
    printf("%d\n" , nLoops);
    return _TRUE;   
}




// ============================================================================
// distance two coloring GM 3 phase Local Ordering
// ============================================================================
int SMPGCInterface::D2_OMP_GM3P_LO(int nT, INT &colors, vector<INT>& vtxColor, const string& local_ordering) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    const int DEF_ORDER_LF = 1;
    const int DEF_ORDER_SL = 2;
    const int DEF_ORDER_SL1 = 21;
    //const int DEF_ORDER_DY = 3;
    //const int DEF_ORDER_ID = 4;
    //const int DEF_ORDER_NT = 5;
    //const int DEF_ORDER_RD = 6;
    const int DEF_ORDER_ERR=-1;


    int order_option = DEF_ORDER_ERR;
    string order_short_tag="Non";
    if(local_ordering=="LARGEST_FIRST"){
        order_short_tag = "LF";
        order_option    = DEF_ORDER_LF; 
    }
    else if(local_ordering=="SMALLEST_LAST"){
        order_short_tag = "SL";
        order_option    = DEF_ORDER_SL;
    }
    else if(local_ordering=="SMALLEST_LAST1"){
        order_short_tag = "SL1";
        order_option    = DEF_ORDER_SL1;
    }

    double tim_Tot=0;                          // run time
    double tim_Color  = 0;                     // run time
    double tim_Detect = 0;                     // run time
    double tim_Recolor= 0;                     // run time
    double tim_MxC    = 0;                     // run time
    INT    nConflicts = 0;                     // Number of conflicts 
    
    const INT N = nodes();                     //number of vertex
    const INT MaxColorCapacity =min( maxDeg()*(maxDeg()-1)+1, N); //maxDegree
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verVal = CSRja();
    const vector<INT>& orig_ordered_vertex = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);

    vector<vector<INT>> QQ(nT);
    {
        vector<int> lens (nT, N/nT);
        for(int i=0; i<N%nT; i++)
            lens[i]++;
        vector<int> begs(nT+1,0);
        for(int i=1; i<=nT; i++)
            begs[i]=begs[i-1]+lens[i-1];
        for(int i=0; i<nT; i++){
            QQ[i].reserve(N/nT+1+16);
            QQ[i].assign(orig_ordered_vertex.begin()+begs[i], orig_ordered_vertex.begin()+begs[i+1]); 
        }
    }

    // Phase - Pseudo Coloring
    tim_Color =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        vector<INT> &Q = QQ[tid];
        // Phase - local order
        if(order_option == DEF_ORDER_LF)
            local_largest_degree_first_ordering(Q);
        else if(order_option == DEF_ORDER_SL)
            local_smallest_degree_last_ordering(Q);
        else if(order_option == DEF_ORDER_SL1)
            local_smallest_degree_last_ordering_B1a(Q);

        vector<INT> Mask; Mask.assign(MaxColorCapacity, -1);
        const INT Nloc = Q.size();
        for(INT vit=0; vit<Nloc; vit++) {
            const auto v=Q[vit];
            //unordered_set<INT> d012_neighbors={v};
            //Mask.assign(MaxColorCapacity, -1);
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {  // d1 neighbors
                const auto d1w  = verVal[d1wit];
                const auto d1wc = vtxColor[d1w];
                if(d1wc!=-1) Mask[d1wc] = v;
                //d012_neighbors.insert(d1w);
            }
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    const auto d2w  = verVal[d2wit];
                    //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                    const auto d2wc = vtxColor[d2w];
                    if(d2wc!=-1) Mask[d2wc] = v;
                }
            }
            
            INT c;
            for (c=0; c!=MaxColorCapacity; c++)
                if(Mask[c]!=v)
                    break;
            vtxColor[v] = c;
        } //end for
    }//end of omp parallel
    tim_Color  += omp_get_wtime();    

    // Phase - Detect conflicts
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
            //unordered_set<INT> d012_neighbors={v};
            bool b_find_conflict=false;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                const auto d1w = verVal[d1wit];
                if(v>=d1w) continue;   // check conflict is little brother's job
                if(vc == vtxColor[d1w]) {
                    Q[num_leftover++]=v;
                    vtxColor[v]=-1;
                    b_find_conflict=true;
                    break;
                }
                //d012_neighbors.insert(d1w);
            }
            if(b_find_conflict) continue;
            for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                const auto d1w = verVal[d1wit];
                for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    const auto d2w = verVal[d2wit];
                    if(v>=d2w) continue; // check conflict is little brother's job
                    //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
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
   
    // Phase - Resolve conflicts
    tim_Recolor =- omp_get_wtime();
    vector<INT> Mark; Mark.assign(MaxColorCapacity,-1);
    for(int tid=0;tid<nT; tid++){
        nConflicts+=QQ[tid].size();
        for(const auto v: QQ[tid]){
            //Mark.assign(MaxColorCapacity, -1);
            //unordered_set<INT> b012_neighbors={v};
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                const auto d1w =verVal[d1wit];
                const auto d1wc=vtxColor[d1w];
                if(d1wc!=-1) Mark[d1wc]=v;
                //b012_neighbors.insert(d1w);
            }
            for(auto d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { 
                const auto d1w = verVal[d1wit];
                for(auto d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                    const auto d2w =verVal[d2wit];
                    //if(b012_neighbors.find(d2w)!=b012_neighbors.end()) continue;
                    const auto d2wc=vtxColor[d2w];
                    if(d2wc!=-1) Mark[d2wc]=v;
                    //b012_neighbors.insert(d2w);
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

    printf("@D2GM3PLO_%s_nT_c_T_TColor_TDetect_TRecolor_TMxC_nCnf\t", order_short_tag.c_str());
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



// ============================================================================
// Distance Two Coloring Openmp Multiple Phase Coloring Local Ordering
// ============================================================================
int SMPGCInterface::D2_OMP_GMMP_LO(int nT, INT &colors, vector<INT>&vtxColor, const string& local_ordering){
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    const int DEF_ORDER_LF = 1;
    const int DEF_ORDER_SL = 2;
    const int DEF_ORDER_SL1 = 21;
    //const int DEF_ORDER_DY = 3;
    //const int DEF_ORDER_ID = 4;
    //const int DEF_ORDER_NT = 5;
    //const int DEF_ORDER_RD = 6;
    const int DEF_ORDER_ERR=-1;

    int order_option = DEF_ORDER_ERR;
    string order_short_tag="Non";
    if(local_ordering=="LARGEST_FIRST"){
        order_short_tag = "LF";
        order_option    = DEF_ORDER_LF; 
    }
    else if(local_ordering=="SMALLEST_LAST"){
        order_short_tag = "SL";
        order_option    = DEF_ORDER_SL;
    }
    else if(local_ordering=="SMALLEST_LAST1"){
        order_short_tag = "SL1";
        order_option    = DEF_ORDER_SL1;
    }


    double tim_Tot=0;                          // run time
    double tim_Color  = 0;                     // run time
    double tim_Detect = 0;                     // run time
    double tim_Sumconf= 0;                     // run time
    double tim_MxC    = 0;                     // run time
    INT    nConflicts = 0;                     // Number of conflicts 
    int    nLoops     = 0;

    const INT N = nodes();                     //number of vertex
    const INT MaxColorCapacity =min( maxDeg()*(maxDeg()-1)+1, N); //maxDegree
    const vector<INT>& verPtr = CSRia();
    const vector<INT>& verVal = CSRja();
    const vector<INT>& orig_ordered_vertex = ordered_vertex(); 
    
    colors=0;                       
    vtxColor.assign(N, -1);


    vector<vector<INT>> QQ(nT);
    {
        vector<int> lens (nT, N/nT);
        for(int i=0; i<N%nT; i++)
            lens[i]++;
        vector<int> begs(nT+1,0);
        for(int i=1; i<=nT; i++)
            begs[i]=begs[i-1]+lens[i-1];
        for(int i=0; i<nT; i++){
            QQ[i].reserve(N/nT+1+16);
            QQ[i].assign(orig_ordered_vertex.begin()+begs[i], orig_ordered_vertex.begin()+begs[i+1]); 
        }
    }

    INT num_un_colored_vertices=N;
    while(num_un_colored_vertices!=0){
        // Phase - Pseudo Coloring
        tim_Color -= omp_get_wtime();
        #pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            vector<INT> &Q = QQ[tid];
            if(order_option==DEF_ORDER_LF)
                local_largest_degree_first_ordering(Q);
            else if(order_option==DEF_ORDER_SL)
                local_smallest_degree_last_ordering(Q);
            else {
                local_smallest_degree_last_ordering_B1a(Q);
            }
            
            vector<INT> Mask; Mask.assign(MaxColorCapacity,-1);
            const INT Nloc = Q.size();
            for(INT vit=0; vit<Nloc; vit++) {
                const auto v=Q[vit];
                //unordered_set<INT> d012_neighbors={v};
                //Mask.assign(MaxColorCapacity, -1);
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++ ) {  // d1 neighbors
                    const auto d1w  = verVal[d1wit];
                    const auto d1wc = vtxColor[d1w];
                    if(d1wc!=-1) Mask[d1wc] = v;
                    //d012_neighbors.insert(d1w);
                }
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                    const auto d1w = verVal[d1wit];
                    for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                        const auto d2w  = verVal[d2wit];
                        //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
                        const auto d2wc = vtxColor[d2w];
                        if(d2wc!=-1) Mask[d2wc] = v;
                    }
                }
                INT c;
                for (c=0; c!=MaxColorCapacity; c++)
                    if(Mask[c]!=v)
                        break;
                vtxColor[v] = c;
            } //end for
        }//end of omp parallel
        tim_Color  += omp_get_wtime();    

        // Phase - Detect Conflicts
        tim_Detect -= omp_get_wtime();
        #pragma omp parallel
        {
            const int tid=omp_get_thread_num();
            vector<INT>& Q = QQ[tid];
            INT num_leftover = 0;
            const INT Nloc = Q.size();
            for(INT vit=0; vit<Nloc; vit++){
                const auto v  = Q[vit];
                const auto vc = vtxColor[v];
                //unordered_set<INT> d012_neighbors={v};
                bool b_find_conflict=false;
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) { // d1 neighbors
                    const auto d1w = verVal[d1wit];
                    if(v>=d1w) continue;   // check conflict is little brother's job
                    if(vc == vtxColor[d1w]) {
                        Q[num_leftover++]=v;
                        vtxColor[v]=-1;
                        b_find_conflict=true;
                        break;
                    }
                    //d012_neighbors.insert(d1w);
                }
                if(b_find_conflict) continue;
                for(INT d1wit=verPtr[v]; d1wit!=verPtr[v+1]; d1wit++) {
                    const auto d1w = verVal[d1wit];
                    for(INT d2wit=verPtr[d1w]; d2wit!=verPtr[d1w+1]; d2wit++) { // d2 neighbors
                        const auto d2w = verVal[d2wit];
                        if(v>=d2w) continue; // check conflict is little brother's job
                        //if(d012_neighbors.find(d2w)!=d012_neighbors.end()) continue;
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
    

        tim_Sumconf -= omp_get_wtime();
        num_un_colored_vertices=0;
        for(int tid=0;tid<nT; tid++)
            num_un_colored_vertices+=QQ[tid].size();
        nConflicts+=num_un_colored_vertices;
        nLoops++;
        tim_Sumconf += omp_get_wtime();
    }

    // get number of colors
    tim_MxC = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(INT i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Color+tim_Detect+tim_Sumconf+tim_MxC;

    printf("@D2GMMPLO_%s_nT_c_T_TColor_TDetect_TSumconf_TMxC_nCnf_nLoop\t",order_short_tag.c_str());
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Color);
    printf("%lf\t", tim_Detect);
    printf("%lf\t", tim_Sumconf);
    printf("%lf\t", tim_MxC);
    printf("%d\t", nConflicts);
    printf("%d\n" , nLoops);
    return _TRUE;   
}






