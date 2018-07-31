/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "SMPGCColoring.h"
#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;

// ============================================================================
// based on Gebremedhin and Manne's GM algorithm [1]
// ============================================================================
int SMPGCColoring::D1_OMP_GM3P(int nT, int&colors, vector<int>&vtxColor) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_partition = 0;
    double tim_color  = 0;                     // run time
    double tim_detect = 0;                     // run time
    double tim_recolor= 0;                     // run time
    double tim_total  =0;                          // run time
    
    int    n_conflicts = 0;                     // Number of conflicts 
    
    const int N = num_nodes();   //number of vertex
    const vector<int>& vtxPtr = get_CSR_ia();
    const vector<int>& vtxVal = get_CSR_ja();
    const vector<int>& const_ordered_vertex = global_ordered_vertex(); 
    const int BufSize         = max_degree()+1;
    colors=0;                       
    vtxColor.assign(N, -1);

    vector<vector<int>> QQ(nT); 
    for(int i=0; i<nT; i++)
        QQ[i].reserve(N/nT+1+16); //1-odd/even, 16-bus wides
    

    // pre-partition the graph
    tim_partition =- omp_get_wtime();
    {
        vector<int> lens(nT, N/nT); for(int i=0; i<N%nT; i++) lens[i]++;
        vector<int> disps(nT+1, 0); for(int i=1; i<nT+1; i++) disps[i]=disp[i-1]+lens[i-1];
        for(int i=0; i<nT; i++)
            QQ[i].insert(QQ[i].end(), const_ordered_vertex.begin()+disps[i], 
                    const_ordered_vertex.begin()+disps[i+1]);
    }
    tim_partition += omp_get_wtime();

    // phase pseudo color
    tim_color =- omp_get_wtime();
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        const vector<int>& Q = QQ[tid];
        vector<int> Mask; Mask.assign(BufSize,-1);
        for(const auto v : Q){
            const auto vc = vtxColor[v];
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++) {
                const auto wc=vtxColor[vtxVal[iw]];
                if( wc >= 0) 
                    Mask[wc] = v;
            } 
            int c=0;
            for (; c!=BufSize; c++)
                if(Mask[c]!=v)
                    break;
            vtxColor[v] = c;
        }
    } //end omp parallel
    tim_color  += omp_get_wtime();    

    // phase conflicts detection
    tim_detect =- omp_get_wtime();
    #pragma omp parallel
    {
        int qsize = 0;
        const int tid=omp_get_thread_num();
        vector<int>& Q = QQ[tid];
        for(int iv=0; iv<(signed)Q.size(); iv++) {
            const auto v  = Q[iv];
            const auto vc = vtxColor[v];
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++){ 
                const auto w = vtxVal[iw];
                if(v<w && vc == vtxColor[w]) {
                    Q[qsize++] = v;
                    vtxColor[v] = -1;  //Will prevent v from being in conflict in another pairing
                    break;
                } 
            } 
        }
        Q.resize(qsize);
    } //end omp parallel
    tim_detect  += omp_get_wtime();
    
    // phase serial coloring remain part
    tim_recolor =- omp_get_wtime();
    {
        vector<bool> Mark; Mark.assign(BufSize, -1);
        for(int tid=0; tid<nT; tid++){
            for(const auto v : QQ[tid]){
                for(auto iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++) {
                    const auto wc = vtxColor[vtxVal[iw]];
                    if(wc>0) Mark[wc]=v;
                }
                int c=0;
                for(; c!=BufSize; c++)
                    if( Mark[c]==v)
                        break;
                vtxColor[v]=c;
            }
        }
    }
    tim_recolor += omp_get_wtime();

    // get number of colors
    double tim_maxc = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_maxc += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_recolor+tim_maxc;



    printf("@GM3P_nT_c_T_Tcolor_Tdetect_Trecolor_TmaxC_nCnf_Tpart");
    printf("\t%d",  nT);    
    printf("\t%d",  colors);    
    printf("\t%lf", tim_total);
    printf("\t%lf", tim_color);
    printf("\t%lf", tim_detect);
    printf("\t%lf", tim_recolor);
    printf("\t%lf", tim_maxc);
    for(int i=0; i<nT; i++) n_conflicts+=QQ[i].size();
    printf("\t%d", n_conflicts);
    printf("\t%lf", tim_partition);
    printf("\n");
    return true;   
}




// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_GMMP(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT); 

    double tim_total   =.0;
    double tim_color   =.0;
    double tim_detect  =.0;
    double tim_max_c   =.0;                     // run time
    int    nLoops      = 0;                     // number of iteration 
    
    const INT N   = nodes();                    // number of vertex
    const INT MaxDegreeP1 = maxDeg()+1;         // maxDegree
    
    INT const * const verPtr  = CSRiaPtr();     // ia of csr
    INT const * const verInd  = CSRjaPtr();     // ja of csr
    
    INT    nConflicts = 0;                      // number of conflicts 
    
    colors=0;
    vtxColors.clear(); vtxColors.assign(N, -1);

    vector<INT> readyQ;    readyQ.resize(N);
    vector<INT> conflictQ; conflictQ.resize(N,-1);
    INT Q_len=N;
    
    #pragma omp parallel for
    for (INT i=0; i<N; i++) {
        readyQ[i]  = ordered_vertex()[i];
        conflictQ[i]= -1; //Empty queue
    }
    vector<double> profile_tim_colors;
    vector<double> profile_tim_detects;
    vector<INT> profile_conflicts;
    do {
        const INT num_nodes_remains = Q_len;
        Q_len = 0;

        auto tim = omp_get_wtime();
        tim_color -= tim;
        profile_tim_colors.push_back(-tim);
        // phase psedue color
        #pragma omp parallel
        {
            vector<INT> Mark; Mark.reserve(MaxDegreeP1);
            #pragma omp for
            for (INT i=0; i<num_nodes_remains; i++) {
                const auto v = readyQ[i]; 
                Mark.assign(MaxDegreeP1, -1);
                for(INT wit = verPtr[v], witEnd=verPtr[v+1]; wit !=witEnd; wit++ ) {
                    const auto w = verInd[wit];
                    const auto nbColor=vtxColors[w];
                    if(nbColor>= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            } //End of omp for
        }
        
        tim = omp_get_wtime();
        tim_color += tim;
        profile_tim_colors.back()+=tim;
        tim_detect -= tim;
        profile_tim_detects.push_back(-tim);
        //phase Detect Conflicts:
        #pragma omp parallel for
        for (INT i=0; i<num_nodes_remains; i++) {
            const auto v = readyQ[i]; 
            for(INT wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++) {
                const INT w = verInd[wit];
                if(v>w && vtxColors[v] == vtxColors[w]) {
                    INT whereInQ = __sync_fetch_and_add(&Q_len, 1);
                    conflictQ[whereInQ] = v;//Add to the queue
                    vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                    break;
                } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
            } //End of inner for loop: w in adj(v)
        }//End of outer for loop: for each vertex
        
        nConflicts += Q_len;
        nLoops++;
        conflictQ.resize(Q_len);
        readyQ.resize(Q_len);
        readyQ.swap(conflictQ);
        tim = omp_get_wtime();
        tim_detect += tim;
        profile_tim_detects.back()+=tim;
        profile_conflicts.push_back(Q_len);
    } while (Q_len > 0);


    // get number of colors
    tim_max_c = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1
    tim_max_c += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_max_c;

    printf("@GMmp_nT_c_T_TColor_TDetect_TMaxC_nCnf_nLoop____profiles\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_max_c);
    printf("%d\t", nConflicts);  
    printf("%d\t\t\t",  nLoops);      
    for(size_t i=0; i<profile_tim_colors.size(); i++) printf("\t%g\t%g\t%d\t",profile_tim_colors[i], profile_tim_detects[i], profile_conflicts[i]);
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
    printf("%d\t",  colors);    
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

    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator(12345);
    std::uniform_real_distribution<double> distribution(0.0,1.0);
tim_Wgt =-omp_get_wtime();
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
    }while( QTail!=0);

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
    printf("@JP_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);
    return _TRUE;
}

// ============================================================================
// based on Allwright's LF JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_LF(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_Wgt_Rnd=0;    //run time
    double tim_Wgt_LF=0;    //run time
    double tim_MIS=0;    //run time
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

    std::uniform_real_distribution<double> distribution(0.0,1.0);
tim_Wgt_Rnd =-omp_get_wtime();
#pragma omp parallel
{
    int pid = omp_get_thread_num();
    std::default_random_engine generator(pid);
#pragma omp for
    for(INT i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
}
tim_Wgt_Rnd += omp_get_wtime();


tim_Wgt_LF =- omp_get_wtime();
#pragma omp parallel for
    for(INT v=0; v<N; v++)
        WeightDeg[v] = verPtr[v+1]-verPtr[v];
tim_Wgt_LF +=omp_get_wtime();
    
    colors=0;
    do{

        //phase 0: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            INT tid = omp_get_thread_num();
            vector<INT> candi;
            vector<INT> MASK(MaxDegreeP1,-1);
            
            #pragma omp for
            for(INT vit=0; vit<QTail; vit++){
                INT v = Q[vit];
                double vwt = WeightDeg[v];
                bool bDomain=true;
                for(auto wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++){
                    auto w = verInd[wit];
                    if(vtxColors[w]>=0) continue;
                    double nbwt = WeightDeg[w];
                    if(vwt<nbwt || (vwt==nbwt && WeightRnd[v]>WeightRnd[w])){
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
                MASK.assign(MaxDegreeP1, -1);
                for(auto wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++){
                    auto w = verInd[wit];
                    int nbcolor=vtxColors[w];
                    if( nbcolor>=0 ){
                        MASK[nbcolor]=w;
                    }
                }
                INT c;
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
            QQ[tid].clear();
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
    colors++;
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Wgt_Rnd+tim_Wgt_LF+tim_MIS+ tim_ReG+tim_MxC;

    printf("@AJPLF_nT_c_T_TWgtRnd_TWgtLF_TMISC_TReG_TMxC_nLoops\t");
    printf("%d\t",  nT);
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt_Rnd);
    printf("%lf\t", tim_Wgt_LF);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);
    return _TRUE;
}

// ============================================================================
// based on Allwright's SL JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_SL(int nT, INT&colors, vector<INT>&vtxColors) {
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
        
        //INT mxDeg = MaxDegreeP1-1;
        //int iMin = 0;
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
    colors++;
    tim_MxC += omp_get_wtime();

    tim_Tot = tim_Wgt+tim_MISReG+tim_MxC;

    printf("@AJPSL_np_c_T_TWgt_TMISReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MISReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n",nLoops);
    return _TRUE;
}






