/*************************************************************************
    > File Name: SMPGCInterface_JP.cpp
    > Author: xc
    > Descriptions: 
    > Created Time: Fri 25 May 2018 02:55:49 PM EDT
 ************************************************************************/

#include "SMPGCInterface.h"
#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;


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



// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_LargeAndSmall(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_Wgt  =0;                      // run time
    double tim_MIS =0;                       // run time
    double tim_ReG =0;                       // run time
    double tim_MxC =0;                       // run time
    double tim_Tot =0;                       // run time
    INT    nLoops = 0;                       // number of iteration 
 
    const INT N = nodes();                   // number of vertex
    //const INT MaxDegreeP1 = maxDeg()+1;    // maxDegree
    
    INT const * const verPtr=CSRiaPtr();     // ia of csr
    INT const * const verInd=CSRjaPtr();     // ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
   
#ifdef JP_PROFILE
    vector<int> profile_num_nodes_L;
    vector<int> profile_num_nodes_S;
    vector<int> profile_num_nodes_remains;
    vector<int> profile_num_colors;
#endif

    vector<INT> Q; Q.resize(N,-1);
    INT QTail=N;
    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }
    QTail = N;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

//    // weight by uniform
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
//    std::default_random_engine generator(12345);
//    std::uniform_real_distribution<double> distribution(0.0,1.0);
//    tim_Wgt =-omp_get_wtime();
//        for(int i=0; i<N; i++)
//            WeightRnd[i] = distribution(generator);
//    tim_Wgt +=omp_get_wtime();

    // weight by shuffle
    vector<INT> WeightRnd; WeightRnd.reserve(N);
    for(int i=0; i<N; i++)
        WeightRnd.push_back(i);
    tim_Wgt =-omp_get_wtime();
        std::random_shuffle ( WeightRnd.begin(), WeightRnd.end() );
    tim_Wgt +=omp_get_wtime();

//    // weight by file
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    {
//        ifstream in('RandomWeight1000.mat');
//        int cnt=0;
//        while(!in.eof && cnt<N)
//            in>>WeightRnd[cnt++];
//        if(cnt<N) { printf("Error! only read %d random numers expect %d\n",cnt, N); exit(1); }
//    }

    colors=0;
    do{
        //phase 1: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
#ifdef JP_PROFILE
        int num_nodes_L=0, num_nodes_S=0;
        #pragma omp parallel reduction(+:num_nodes_L), reduction(+:num_nodes_S) 
#else
        #pragma omp parallel
#endif
        {
            int tid = omp_get_thread_num();
            vector<INT> candiLrg;
            vector<INT> candiSml;
            #pragma omp for
            for(INT vit=0; vit<QTail; vit++){
                const auto v   = Q[vit];
                const auto vwt = WeightRnd[v];
                char bDomain=0x03;  //0:neither, 1: LargeDomain, 2:SmallDomain, 3 Both/Reserve/Init;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto nb = verInd[k];
                    if(vtxColors[nb]!=-1) continue;
                    const auto nbwt = WeightRnd[nb];
                    if( (bDomain&0x1) && ( (vwt<nbwt)||(vwt==nbwt&&v>nb) ) ) bDomain&=0xFE;
                    if( (bDomain&0x2) && ( (vwt>nbwt)||(vwt==nbwt&&v>nb) ) ) bDomain&=0xFD;
                    if( bDomain==0) break;
                }
                if(bDomain==0) {
                    QQ[tid].push_back(v);
                }
                else{
                    if(bDomain!=2) candiLrg.push_back(v);
                    else           candiSml.push_back(v);
                }
            } //end of omp for

            for(auto v : candiLrg) 
                vtxColors[v]=nLoops*2;
            for(auto v : candiSml) 
                vtxColors[v]=nLoops*2+1;
#ifdef JP_PROFILE
            num_nodes_L = candiLrg.size();
            num_nodes_S = candiSml.size();
#endif
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
    
#ifdef JP_PROFILE
        { //for profile
            INT colors=0;
            #pragma omp parallel for reduction(max:colors)
            for(int i=0; i<N; i++){
                auto c = vtxColors[i];
                if(c>colors) colors=c;
            }
            colors++;
            profile_num_nodes_L.push_back(num_nodes_L);
            profile_num_nodes_S.push_back(num_nodes_S);
            profile_num_nodes_remains.push_back(QTail);
            profile_num_colors.push_back(colors);
            printf("loop %lld num_colored_L %d num_colored_S %d remains %lld\n", nLoops, profile_num_nodes_L.back(), profile_num_nodes_S.back(), QTail);
        }//end for profile
#endif 
        nLoops++;
    }while(QTail!=0);

    tim_MxC = -omp_get_wtime();
    colors=0;
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        auto c = vtxColors[i];
        if(c>colors) colors=c;
    }
    colors++;
    tim_MxC += omp_get_wtime();

    tim_Tot =tim_Wgt+ tim_MIS+tim_ReG+tim_MxC;
    printf("@JPLaS_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);

    /*
    {
        stringstream ss;
        ss<<"JPLaS_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JPLaS_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JPLas_num_colors_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }
    */
    return _TRUE;
}


//option ==1 origJP + greedyGM-3Phase
//option ==2 oirgJP + greedyGM-MultiPhase
//option ==3 origJP + serial-greedy
//option ==4 origJP + stream
int SMPGCInterface::D1_OMP_JP_hyper_orig_greedy(int nT, INT&colors, vector<INT>&vtxColors, const int option, const INT switch_iter=1) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    string tag ="hyper";
    vector<INT> conflictQ;

    double tim_Wgt =0;                    // run time
    double tim_MIS =0;                    // run time
    double tim_ReG =0;                    // run time
    double tim_Greedy=0;
    double tim_MxC =0;                    // run time
    double tim_Tot =0;                    // run time
    INT    nLoops = 0;                    // number of iteration
 
    const INT N = nodes();                // number of vertex
    const INT MaxDegreeP1 = maxDeg()+1;   // maxDegree
    
    INT const * const verPtr=CSRiaPtr();  // ia of csr
    INT const * const verInd=CSRjaPtr();  // ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
    
    vector<INT> Q; Q.resize(N,-1);
    INT QTail = N;
    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }

    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    if(option==JP_HYPER_GREEDY_GM3P) {
        tag = "GM3P";
    }
    else if(option==JP_HYPER_GREEDY_GMMP) {
        tag = "GMMP";
        conflictQ.resize(N,-1);
    }
    else if(option==JP_HYPER_GREEDY_SERL) {
        tag = "SERL";
    }
    else if(option==JP_HYPER_STREAM) {
        tag ="STEAM";
    }
    else{
        printf("Error! Unknown option %d, should be {1,2,3,4}\n",option);
        exit(1);
    }


//    // weight by uniform
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
//    std::default_random_engine generator(12345);
//    std::uniform_real_distribution<double> distribution(0.0,1.0);
//    tim_Wgt =-omp_get_wtime();
//        for(int i=0; i<N; i++)
//            WeightRnd[i] = distribution(generator);
//    tim_Wgt +=omp_get_wtime();

    // weight by shuffle
    vector<INT> WeightRnd; WeightRnd.reserve(N);
    for(int i=0; i<N; i++)
        WeightRnd.push_back(i);
    tim_Wgt =-omp_get_wtime();
        std::random_shuffle ( WeightRnd.begin(), WeightRnd.end() );
    tim_Wgt +=omp_get_wtime();

//    // weight by file
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    {
//        ifstream in('RandomWeight1000.mat');
//        int cnt=0;
//        while(!in.eof && cnt<N)
//            in>>WeightRnd[cnt++];
//        if(cnt<N) { printf("Error! only read %d random numers expect %d\n",cnt, N); exit(1); }
//    }

    colors=0;
    // phase 1 : JP Part
    do{
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> MASK; MASK.reserve(MaxDegreeP1);
            vector<INT> candi;
            #pragma omp for
            for(INT vit=0; vit<QTail; vit++){
                const auto v   = Q[vit];
                const auto vwt = WeightRnd[v];
                bool bDomain=true;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto nb = verInd[k];
                    if(vtxColors[nb]!=-1) continue;
                    const auto nbwt = WeightRnd[nb];
                    if(vwt<nbwt || (vwt==nbwt && v>nb)){
                        bDomain=false;
                        break;
                    }
                }
                if(bDomain) candi.push_back(v);
                else        QQ[tid].push_back(v);
            } //end of omp for

            for(auto v : candi) {
                MASK.assign(MaxDegreeP1,-1);
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto nb      = verInd[k];
                    const auto nbcolor = vtxColors[nb];
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
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].clear();
        }
        tim_ReG += omp_get_wtime();
        nLoops++;
        if(nLoops >= switch_iter) break;
    }while( QTail!=0);

    // Phase 2, Greedy Coloring
    if(option==JP_HYPER_GREEDY_GM3P) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_GM3P  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_GREEDY_GMMP) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_GMMP  (Q,QTail,conflictQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_GREEDY_SERL) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_serial  (Q,QTail,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_STREAM) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_stream  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else{
        printf("Error!, option =%d should be in {1,2,3,4}\n",option);
        exit(1);
    }

    // Phase 3, get max color
    tim_MxC = -omp_get_wtime();
    colors=0;
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        auto c = vtxColors[i];
        if(c>colors) colors=c;
    }
    colors++;
    tim_MxC += omp_get_wtime();

    tim_Tot =tim_Wgt+ tim_MIS+tim_ReG+tim_Greedy + tim_MxC;
    printf("@JPHyperOrig%s_nT_c_T_TWgt_TMISC_TReG_TGreedy_TMxC_SwitchnLoop\t",tag.c_str());
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_Greedy);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);
    
    
    return _TRUE;
}

//option ==1 origJP + greedyGM-3Phase
//option ==2 oirgJP + greedyGM-MultiPhase
//option ==3 origJP + serial-greedy
//option ==4 origJP + stream
int SMPGCInterface::D1_OMP_JP_hyper_LaS_greedy(int nT, INT&colors, vector<INT>&vtxColors, const int option, const INT switch_iter = 0) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    string tag="Hyper";
    vector<INT> conflictQ;
    double tim_Wgt =0;    //run time
    double tim_MIS =0;    //run time
    double tim_ReG =0;    //run time
    double tim_Greedy=0;
    double tim_MxC =0;    //run time
    double tim_Tot =0;    //run time
    INT    nLoops = 0;    //Number of iterations 
 
    const INT N = nodes(); //number of vertex
    //const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();      //ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
   
#ifdef JP_PROFILE
    vector<int> profile_num_nodes_L;
    vector<int> profile_num_nodes_S;
    vector<int> profile_num_nodes_remains;
    vector<int> profile_num_colors;
#endif

    vector<INT> Q; Q.resize(N,-1);
    INT QTail=N;
    #pragma omp parallel for
    for(INT i=0; i<N; i++){
        Q[i]  = ordered_vertex()[i];
    }

    vector<vector<INT>> QQ; QQ.resize(nT); 
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    if(option==JP_HYPER_GREEDY_GM3P) {
        tag = "GM3P";
    }
    else if(option==JP_HYPER_GREEDY_GMMP) {
        tag = "GMMP";
        conflictQ.resize(N,-1);
    }
    else if(option==JP_HYPER_GREEDY_SERL) {
        tag = "SERL";
    }
    else if(option==JP_HYPER_STREAM) {
        tag ="STEAM";
    }
    else{
        printf("Error! Unknown option %d, should be {1,2,3,4}\n",option);
        exit(1);
    }

//    // weight by uniform
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
//    std::default_random_engine generator(12345);
//    std::uniform_real_distribution<double> distribution(0.0,1.0);
//    tim_Wgt =-omp_get_wtime();
//        for(int i=0; i<N; i++)
//            WeightRnd[i] = distribution(generator);
//    tim_Wgt +=omp_get_wtime();

    // weight by shuffle
    vector<INT> WeightRnd; WeightRnd.reserve(N);
    for(int i=0; i<N; i++)
        WeightRnd.push_back(i);
    tim_Wgt =-omp_get_wtime();
        std::random_shuffle ( WeightRnd.begin(), WeightRnd.end() );
    tim_Wgt +=omp_get_wtime();

//    // weight by file
//    vector<double> WeightRnd; WeightRnd.resize(N);
//    {
//        ifstream in('RandomWeight1000.mat');
//        int cnt=0;
//        while(!in.eof && cnt<N)
//            in>>WeightRnd[cnt++];
//        if(cnt<N) { printf("Error! only read %d random numers expect %d\n",cnt, N); exit(1); }
//    }


    colors=0;
    // phase 1:  JP part
    do{
        tim_MIS -= omp_get_wtime();
#ifdef JP_PROFILE
        int num_nodes_L=0, num_nodes_S=0;
        #pragma omp parallel reduction(+:num_nodes_L), reduction(+:num_nodes_S) 
#else
        #pragma omp parallel
#endif
        {
            int tid = omp_get_thread_num();
            vector<INT> candiLrg;
            vector<INT> candiSml;
            #pragma omp for
            for(INT vit=0; vit<QTail; vit++){
                const auto v = Q[vit];
                const auto vwt = WeightRnd[v];
                int bDomain=0x3;  //0 neither; 1 LargeDomain; 2 SmallestDomain; 3 Both/Reserve/Init;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto nb = verInd[k];
                    if(vtxColors[nb]!=-1) continue;
                    const auto nbwt = WeightRnd[nb];
                    if( (bDomain&0x1) && ( (vwt<nbwt)||(vwt==nbwt&&v>nb) ) ) bDomain^=0x1;
                    if( (bDomain&0x2) && ( (vwt>nbwt)||(vwt==nbwt&&v>nb) ) ) bDomain^=0x2;
                    if(bDomain==0)  break;
                }

                if(bDomain==0) {
                    QQ[tid].push_back(v);
                }
                else{
                    if(bDomain!=2)  candiLrg.push_back(v);
                    else            candiSml.push_back(v);
                }
            } // end of omp for

            for(auto v : candiLrg) 
                vtxColors[v]=nLoops*2;
            for(auto v : candiSml) 
                vtxColors[v]=nLoops*2+1;

#ifdef JP_PROFILE
            num_nodes_L = candiLrg.size();
            num_nodes_S = candiSml.size();
#endif
        } // end of omp parallel
        tim_MIS += omp_get_wtime();
        
        tim_ReG -= omp_get_wtime();
        // phase 1.2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid]) 
                Q[QTail++]=v;
            QQ[tid].clear();
        }
        tim_ReG += omp_get_wtime();
    
#ifdef JP_PROFILE
        { //for profile
            INT colors=0;
            #pragma omp parallel for reduction(max:colors)
            for(int i=0; i<N; i++){
                auto c = vtxColors[i];
                if(c>colors) colors=c;
            }
            colors++;
            profile_num_nodes_L.push_back(num_nodes_L);
            profile_num_nodes_S.push_back(num_nodes_S);
            profile_num_nodes_remains.push_back(QTail);
            profile_num_colors.push_back(colors);
            printf("loop %lld num_colored_L %d num_colored_S %d remains %lld\n", nLoops, profile_num_nodes_L.back(), profile_num_nodes_S.back(), QTail);
        }//end for profile
#endif 
        
        if(++nLoops>=switch_iter) break;
    }while(QTail!=0);


    // Phase 2, Greedy coloring
    if(option==JP_HYPER_GREEDY_GM3P) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_GM3P  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_GREEDY_GMMP) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_GMMP  (Q,QTail,conflictQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_GREEDY_SERL) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_greedy_serial  (Q,QTail,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else if(option==JP_HYPER_STREAM) {
        tim_Greedy -= omp_get_wtime();
        hyperJP_stream  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Greedy += omp_get_wtime();
    }
    else{
        printf("Error! option=%d, should be in {1,2,3,4}\n",option);
        exit(1);
    }

    // Phase 3, get max colors
    tim_MxC = -omp_get_wtime();
    colors=0;
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        auto c = vtxColors[i];
        if(c>colors) colors=c;
    }
    colors++;
    tim_MxC += omp_get_wtime();

    tim_Tot =tim_Wgt+ tim_MIS + tim_ReG + tim_Greedy + tim_MxC;
    printf("@JPHyperLaS%s_nT_c_T_TWgt_TMISC_TReG_TGreedy_TMxC_SwitchnLoop\t",tag.c_str());
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_Greedy);
    printf("%lf\t", tim_MxC);
    printf("%lld\n", nLoops);

    /*
    {
        stringstream ss;
        ss<<"JPLaS_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JPLaS_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JPLas_num_colors_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }
    */
    return _TRUE;
}




inline
void SMPGCInterface::hyperJP_greedy_GM3P(vector<INT>&Q, const INT &QTail, vector<vector<INT>>& QQ, const int &nT, INT const * const &verPtr, INT const * const &verIdx, INT& colors, vector<INT>&vtxColor){
    const INT MaxDegreeP1 = maxDeg()+1;
    const INT N = nodes();
    #pragma omp parallel
    {
        // Phase 0: Partition
        // Phase 1: Pseudo-color
        vector<bool> Mask;
        #pragma omp for
        for(INT it=0; it<N; it++) {
            const auto v=Q[it];
            Mask.assign(MaxDegreeP1, false);
            for(INT jt=verPtr[v], jtEnd=verPtr[v+1]; jt!=jtEnd; jt++ ) {
                const auto wColor=vtxColor[verInd[jt]];
                if( wColor >= 0) 
                    Mask[wColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mask[c] == false)
                    break;
            vtxColor[v] = c;
        } //End of omp for
    }//end of omp parallel

// Phase 2: Detect conflicts
    #pragma omp parallel
    {
        const int tid=omp_get_thread_num();
        vector<INT>& Qtmp = QQ[tid];
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
    
// Phase 3: Resolve conflicts
    vector<bool> Mark;
    for(int tid=0;tid<nT; tid++){
        if(QQ[tid].empty()) continue;
        //nConflicts+=QQ[tid].size();
        for(const auto v: QQ[tid]){
            Mark.assign(MaxDegreeP1, false);
            for(auto wit=verPtr[v], witEnd=verPtr[v+1]; wit!=witEnd; wit++ ) {
                const auto wc =vtxColor[verInd[wit]];
                if( wc >= 0) 
                    Mark[wc] = true; //assert(adjColor<Mark.size())
            } 
            INT c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mark[c] == false)
                    break;
            vtxColor[v] = c;
        }
    }
} // end funtion

void SMPGCInterface::hyperJP_greedy_GMMP(vector<INT>&readyQ, const INT &origQTail, vector<INT>&conflictQ, const int& nT, INT const * const &verPtr, INT const * const &verIdx, INT& colors, vector<INT>&vtxColors){
    const INT MaxDegreeP1 = maxDeg()+1;
    const INT N = nodes();
    INT QTail = origQTail;
    do{
        const INT num_nodes_remains = QTail;
        QTail = 0;
        #pragma omp parallel
        {
            vector<INT> Mark; Mark.reserve(MaxDegreeP1);
            #pragma omp for
            for(INT i=0; i< num_nodes_remains; i++) {
                const auto v = readyQ[i];
                Mark.assign(MaxDegreeP1,-1);
                for(INT wit = verPtr[v], witEnd=verPtr[v+1]; wit !=witEnd; wit++ ) {
                    const auto w = verInd[wit];
                    const auto nbColor=vtxColors[w];
                    if(nbColor >= 0) 
                        Mark[nbColor] = w; //assert(adjColor<Mark.size())
                } 
                INT c;
                for (c=0; c!=MaxDegreeP1; c++)
                    if(Mark[c] == -1)
                        break;
                vtxColors[v] = c;
            }

        #pragma omp for
        for (INT i=0; i<num_nodes_remains; i++) {
            const auto v = readyQ[i];
            const auto vc = vtxColors[v];
            for(INT wit=verPtr[v],witEnd=verPtr[v+1]; wit!=witEnd; wit++) {
                const auto w = verInd[wit];
                if(v>w && vc == vtxColors[w]) {
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


    }while();
} // end function

void SMPGCInterface::hyperJP_greedy_serial(vector<INT>&Q, const INT &QTail, INT const * const &verPtr, INT const * const &verIdx, INT& colors, vector<INT>&vtxColors){
    //TODO
    printf("under construction\n");
    exit(1);
}

void SMPGCInterface::hyperJP_stream(vector<INT>&Q, const INT &QTail, vector<vector<INT>>&QQ, const int&nT, INT const * const &verPtr, INT const * const &verIdx, INT& colors, vector<INT>&vtxColors){
    //TODO
    printf("under construction\n");
    exit(1);
}








