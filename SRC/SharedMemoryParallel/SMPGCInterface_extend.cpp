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
// Local Ordering version of GM alg 
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

    string order_short_tag=local_ordering;
    if(local_ordering=="LARGEST_FIRST"){
        order_short_tag="LF";
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_largest_degree_first_ordering(MEM_LocalOrderings[tid]);
        }
        tim_loc_ord += omp_get_wtime();
    }
    else if(local_ordering=="SMALLEST_LAST"){
        order_short_tag="SM";
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_smallest_degree_last_ordering(MEM_LocalOrderings[tid]);
        }
        tim_loc_ord += omp_get_wtime();
    }
    else if(local_ordering=="SMALLEST_LAST1"){
        order_short_tag="SM1";
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_smallest_degree_last_ordering_B1a(MEM_LocalOrderings[tid]);
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

    printf("@GM3pLO%s_nT_c_T_TlocOrd_Tcolor_Tdetect_Trecolor_TmaxC_nCnf\t",order_short_tag.c_str());
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_loc_ord);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_recolor);
    printf("%lf\t", tim_maxc);
    printf("%lld\n", nConflicts);
    return _TRUE;   
} //end of fun D1_OMP_GM_LO



// ============================================================================
// Local Ordering
// ============================================================================
int SMPGCInterface::D1_OMP_IP_LO_perloop(int nT, INT&colors, vector<INT>&vtxColors, const string& local_ordering) {
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
    string order_short_tag=local_ordering;
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




    double tim_total    =.0;    
    double tim_loc_ord=.0;
    double tim_color  =.0;
    double tim_detect =.0;
    double tim_max_c  =.0;

    INT    nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    const INT N   = nodes(); //number of vertex
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    const vector<INT>& const_orig_Q = ordered_vertex();

    colors=0;
    vtxColors.assign(N, -1);

    // set up memory
    INT global_queue_len = N;
    vector<INT>        global_queue(N,-1);
    vector<vector<INT>> local_queues(nT);
    #pragma omp parallel
    {   
        int tid = omp_get_thread_num();
        local_queues[tid].reserve(N/nT+1+64/sizeof(INT));  // 64/8 is for false sharing 
        #pragma omp for
        for(INT i=0; i<N; i++){
            global_queue[i] = const_orig_Q[i];
        }
    } //end omp parallel

    while(global_queue_len){ //local order, pseudo color and resolve conflict
        const INT num_nodes_remains = global_queue_len; //global_queue.size();
        // local ordering
        tim_loc_ord = -omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            local_queues[tid].clear();
            #pragma omp for nowait
            for(INT i=0; i<num_nodes_remains; i++) {
                local_queues[tid].push_back(global_queue[i]);
            }

            if(order_option == DEF_ORDER_LF)
                local_largest_degree_first_ordering(local_queues[tid]);
            else if(order_option == DEF_ORDER_SL)
                local_smallest_degree_last_ordering(local_queues[tid]);
            else if(order_option == DEF_ORDER_SL1)
                local_smallest_degree_last_ordering_B1a(local_queues[tid]);
            else
                fprintf(stderr, "Warning! unknow local ordering %s, using 'local natural ordering instead.\n", local_ordering.c_str());
        }// end omp parallel
        global_queue_len = 0;
        tim_loc_ord += omp_get_wtime();

        tim_color -= omp_get_wtime();
        // phase psedue color
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> Mark;  Mark.reserve(MaxDegreeP1);
            for(const auto v : local_queues[tid]) {
                Mark.assign(MaxDegreeP1, -1);
                for(INT wit = verPtr[v], witEnd=verPtr[v+1]; wit !=witEnd; wit++ ) {
                    INT w = verInd[wit];
                    INT nbColor = vtxColors[w];
                    if( nbColor >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            }
        } //end omp parallel
        tim_color  += omp_get_wtime();

        tim_detect -= omp_get_wtime();
        //phase Detect Conflicts
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            for(const auto v: local_queues[tid]) {
                for(INT wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++) {
                    const auto w = verInd[wit];
                    if(v>w && vtxColors[v] == vtxColors[w]) {
                        INT whereInQ = __sync_fetch_and_add(&global_queue_len, 1);
                        global_queue[whereInQ] = v;
                        vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                        break;
                    } // end if( vtxColor[v] == vtxColor[verInd[k]] )
                } //end for neighbors 
            } //end for each v
        }
        nConflicts += global_queue_len;
        global_queue.resize(global_queue_len);
        nLoops++;
        tim_detect  += omp_get_wtime();
    }//end while

    tim_max_c = -omp_get_wtime();
    // get number of colors
#pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_max_c += omp_get_wtime();

    tim_total = tim_loc_ord + tim_color + tim_detect + tim_max_c;

    
    printf("@GMmpPerloopLO%s_nT_c_T_TLocOrd_TColor_TDetect_TmaxC_nCnf_nLoop\t",order_short_tag.c_str());
    printf("%d\t",  nT);    
    printf("%lld\t",colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_loc_ord);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_max_c);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    printf("\n");      
    return _TRUE;
} //end D1_OMP_IP_LO_perloop 


// ============================================================================
// Local Ordering
// ============================================================================
int SMPGCInterface::D1_OMP_IP_LO_once(int nT, INT&colors, vector<INT>&vtxColors, const string& local_ordering) {
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
    string order_short_tag=local_ordering;
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




    double tim_total    =.0;    
    double tim_loc_ord=.0;
    double tim_color  =.0;
    double tim_detect =.0;
    double tim_max_c  =.0;
    INT    nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    const INT N   = nodes(); //number of vertex
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    const vector<INT>& const_orig_Q = ordered_vertex();

    colors=0;
    vtxColors.assign(N, -1);

    // set up memory
    INT global_queue_len = N;
    vector<INT>        global_queue(N,-1);
    vector<vector<INT>> local_queues(nT);
    #pragma omp parallel
    {   
        int tid = omp_get_thread_num();
        local_queues[tid].reserve(N/nT+1+64/sizeof(INT));  // 64/8 is for false sharing 
        #pragma omp for
        for(INT i=0; i<N; i++){
            global_queue[i] = const_orig_Q[i];
        }
    } //end omp parallel

    tim_loc_ord = -omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        local_queues[tid].clear();
        #pragma omp for nowait
        for(INT i=0; i<global_queue_len; i++) {
            local_queues[tid].push_back(global_queue[i]);
        }

        if(order_option == DEF_ORDER_LF)
            local_largest_degree_first_ordering(local_queues[tid]);
        else if(order_option == DEF_ORDER_SL)
            local_smallest_degree_last_ordering(local_queues[tid]);
        else if(order_option == DEF_ORDER_SL1)
            local_smallest_degree_last_ordering_B1a(local_queues[tid]);
        else
            fprintf(stderr, "Warning! unknow local ordering %s, using 'local natural ordering instead.\n", local_ordering.c_str());
    }// end omp parallel
    tim_loc_ord += omp_get_wtime();






    while(global_queue_len){ //local order, pseudo color and resolve conflict
        global_queue_len=0;
        
        tim_color -= omp_get_wtime();
        // phase psedue color
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> Mark;  Mark.reserve(MaxDegreeP1);
            for(const auto v : local_queues[tid]) {
                Mark.assign(MaxDegreeP1, -1);
                for(INT wit = verPtr[v], witEnd=verPtr[v+1]; wit !=witEnd; wit++ ) {
                    INT w = verInd[wit];
                    INT nbColor = vtxColors[w];
                    if( nbColor >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            }
        } //end omp parallel
        tim_color  += omp_get_wtime();

        tim_detect -= omp_get_wtime();
        //phase Detect Conflicts
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            for(const auto v: local_queues[tid]) {
                for(INT wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++) {
                    const auto w = verInd[wit];
                    if(v>w && vtxColors[v] == vtxColors[w]) {
                        INT whereInQ = __sync_fetch_and_add(&global_queue_len, 1);
                        global_queue[whereInQ] = v;
                        vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                        break;
                    } // end if( vtxColor[v] == vtxColor[verInd[k]] )
                } //end for neighbors 
            } //end for each v
        }
        nConflicts += global_queue_len;
        global_queue.resize(global_queue_len);
        nLoops++;
        tim_detect  += omp_get_wtime();
    }//end while

    tim_max_c = -omp_get_wtime();
    // get number of colors
#pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_max_c += omp_get_wtime();

    tim_total = tim_loc_ord + tim_color + tim_detect + tim_max_c;

    
    printf("@GMmpSingleLO%s_nT_c_T_TLocOrd_TColor_TDetect_TmaxC_nCnf_nLoop\t",order_short_tag.c_str());
    printf("%d\t",  nT);    
    printf("%lld\t",colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_loc_ord);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_max_c);
    printf("%lld\t", nConflicts);  
    printf("%d",  nLoops);      
    printf("\n");      
    return _TRUE;
} //end D1_OMP_IP_LO_once



// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_adv(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    //double tim_Wgt  =0;    //run time
    double tim_INIT_MEM =0;
    double tim_MIS =0;
    double tim_MxC =0;    //run time
    double tim_Tot =0;               //run time
    INT    nLoops = 0;                         //Number of rounds 

    INT nConflicts = 0;
    const INT N = nodes(); //number of vertex
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();         //ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
   
    vector<INT> readyQ; readyQ.resize(N,-1);
    INT readyQlen=0;
    
    vector<INT> unDomQ; unDomQ.resize(N,-1);
    INT unDomQlen=0;

    vector<vector<INT>> conflictQs(nT);
    for(int i=0; i<nT; i++){
        conflictQs[i].reserve(N/nT+1+64/sizeof(INT));
    }


    // weight 
    vector<INT> WeightRnd; WeightRnd.reserve(N);

tim_INIT_MEM = -omp_get_wtime();
    for(int i=0; i<N; i++)
        WeightRnd.push_back(i);
    std::random_shuffle ( WeightRnd.begin(), WeightRnd.end() );
tim_INIT_MEM += omp_get_wtime();

tim_INIT_MEM = -omp_get_wtime();
    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        auto v = ordered_vertex()[i];
        readyQ[i]  = v;
    }
    readyQlen = N;
tim_INIT_MEM += omp_get_wtime();


//tim_pernode_ord = -omp_get_wtime();
//    #pragma omp parallel for
//    for(INT i=0; i<N; i++){
//        auto v = readQ[i];
//        sort(G[v].begin(), G[v].end(), [WeightRnd&](const auto& n1, const auto& n2){ return WeightRnd[n1]>WeightRnd[n2]; } );
//    }
//tim_pernode_ord += omp_get_wtime();

    colors=0;
    do{
        //printf("color%d_Qtail%d\n",colors,QTail);
        //phase 0: find maximal indenpenent set, and color it
tim_MIS -= omp_get_wtime();
#pragma omp parallel
{
    //int tid = omp_get_thread_num();
    vector<INT> candi_LG;
    vector<INT> candi_SM;
    vector<INT> MASK; MASK.reserve(MaxDegreeP1);
#pragma omp for
        for(INT vit=0; vit<readyQlen; vit++){
            const INT v = readyQ[vit];
            const INT vwt = WeightRnd[v];
            int  bDomainLG=true;
            bool bDomainSM=true;
            for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                const INT nb = verInd[k];
                if(vtxColors[nb]!=-1) continue;
                const INT nbwt = WeightRnd[nb];
                if(bDomainLG && vwt<nbwt){
                    bDomainLG=false;
                }
                if(bDomainSM && vwt>nbwt){
                    bDomainSM=false;
                }
                if(!bDomainLG && !bDomainSM) {
                    INT whereInQ = __sync_fetch_and_add(&unDomQlen, 1);
                    unDomQ[whereInQ]=v;
                    break;
                }
            }
            if(bDomainLG) {
                vtxColors[v]=nLoops*2;
                //candi_LG.push_back(v);
            }
            if(bDomainSM) {
                vtxColors[v]=nLoops*2+1;
                //candi_SM.push_back(v);
            }
        } //end of omp for

}   //end of omp parallel
    readyQ.swap(unDomQ);
    readyQlen = unDomQlen;
    unDomQlen = 0;
tim_MIS += omp_get_wtime();
    nLoops++;
    }while(nLoops<=3 && readyQlen!=0);


// Greedy Part, using GM
#pragma omp parallel
{
    vector<bool>Mask;
#pragma omp for
    for(INT it=0; it<readyQlen; it++) {
        const auto v = readyQ[it];
        Mask.assign(MaxDegreeP1, false);
        for(INT jt=verPtr[v], jtEnd=verPtr[v+1]; jt<jtEnd; jt++) {
            const auto wColor = vtxColors[verInd[jt]];
            if(wColor>=0)
                Mask[wColor] = true;
        }
        int c;
        for(c=0; c!=MaxDegreeP1; c++)
            if(Mask[c] == false)
                break;
        vtxColors[v]=c;
    }
}

#pragma omp parallel
{
    int tid = omp_get_wtime();
    vector<INT>& conflictQ = conflictQs[tid];
#pragma omp for
    for(INT it=0; it<N; it++) {
        const auto v = readyQ[it];
        const auto vc = vtxColors[v];
        for(INT jt=verPtr[v], jtEnd=verPtr[v+1]; jt!=jtEnd; jt++) {
            const auto w = verInd[jt];
            if(v<w && vc==vtxColors[w]) {
                conflictQ.push_back(v);
                vtxColors[v]=-1;
                break;
            }
        }
    }
}

    vector<bool> Mark; Mark.reserve(MaxDegreeP1);
    for(int tid=0; tid<nT; tid++) {
        if(conflictQs[tid].empty()) continue;
        nConflicts += conflictQs[tid].size();
        for(const auto v : conflictQs[tid]) {
            Mark.assign(MaxDegreeP1,false);
            for(auto wit=verPtr[v], witEnd=verPtr[v+1]; wit!=witEnd; wit++) {
                const INT wc = vtxColors[verInd[wit]];
                if(wc>=0) {
                    Mark[wc]=true;
                }
            }
            INT c;
            for(c=0; c!=MaxDegreeP1; c++) {
                if(Mark[c]==false)
                    break;
            }
            vtxColors[v]=c;
        }
    }


tim_MxC = -omp_get_wtime();
//for(int i=0; i<N; i++) printf("%d-%d\n",i,vtxColors[i]);
colors=0;
#pragma omp parallel for reduction(max:colors)
for(int i=0; i<N; i++){
    auto c = vtxColors[i];
    if(c>colors) colors=c;
}
colors++;
tim_MxC += omp_get_wtime();

tim_Tot =tim_INIT_MEM + tim_MIS+tim_MxC;
    printf("@JPHylp3_nT_c_T_TINIT_TMISC_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_INIT_MEM);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);
    return _TRUE;
}



// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_profile(int nT, INT&colors, vector<INT>&vtxColors) {
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
    
    vector<int> profile_num_nodes;
    vector<int> profile_num_colors;



    vector<INT> Q; Q.resize(N,-1);
    INT QTail=0;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
#pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = i; //ordered_vertex()[i];
    }
    QTail = N;

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);

tim_Wgt =-omp_get_wtime();
    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator(9234);
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
tim_Wgt +=omp_get_wtime();

   // ofstream out("wocao_rand_20by1.mat");
   // for(auto x : WeightRnd)
   //     out<<x<<endl;
   // out.close();




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
                printf("%lld with %g is domain with neighbor:",v+1, WeightRnd[v]);
                for(INT k=verPtr[v]; k<verPtr[v+1]; k++){
                    printf("%lld, %g\t", verInd[k]+1, WeightRnd[verInd[k]]);
                }
                printf("\n");
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

    { //for profile
        INT colors=0;
        #pragma omp parallel for reduction(max:colors)
        for(int i=0; i<N; i++){
            auto c = vtxColors[i];
            if(c>colors) colors=c;
        }
        colors++;
        profile_num_nodes.push_back(QTail);
        profile_num_colors.push_back(colors);
    }//end for profile
    printf("loop %lld num_colored %lld remains %lld\n", nLoops, (profile_num_nodes.size()==1)?(N-profile_num_nodes.back()):(profile_num_nodes[profile_num_nodes.size()-2]-profile_num_nodes.back()),  QTail);

    }while(QTail!=0);

tim_MxC = -omp_get_wtime();
//for(int i=0; i<N; i++) printf("%d-%d\n",i,vtxColors[i]);
colors=0;
#pragma omp parallel for reduction(max:colors)
for(int i=0; i<N; i++){
    auto c = vtxColors[i];
    if(c>colors) colors=c;
}
colors++;
tim_MxC += omp_get_wtime();

tim_Tot =tim_Wgt+ tim_MIS+tim_ReG+tim_MxC;
    printf("@JPProfile_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);
    
   
    stringstream ss;
    ss<<"JP_Profile_num_nodes_nT"<<nT<<".log";
    ofstream of1(ss.str().c_str());
    for(auto x : profile_num_nodes)
        of1<<x<<endl;
    of1.close();

    ss.str("");
    ss<<"JP_Profile_num_colors_nT"<<nT<<".log";
    ofstream of2(ss.str().c_str());
    for(auto x : profile_num_colors)
        of2<<x<<endl;//fprintf(of2, "%d\n",x);
    of2.close();
    return _TRUE;
}















