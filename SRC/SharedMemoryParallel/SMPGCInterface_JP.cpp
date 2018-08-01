/*************************************************************************
    > File Name: SMPGCInterface_JP.cpp
    > Author: xc
    > Descriptions: 
    > Created Time: Fri 25 May 2018 02:55:49 PM EDT
 ************************************************************************/
#include <sstream>
#include "SMPGCInterface.h"
#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;

#ifdef DEBUG_JP_PROFILE
    extern string profile_name;
#endif



unsigned int mhash(unsigned int a, unsigned int seed){
    a ^= seed;
    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) + (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a ^ 0xd3a2646c) + (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) + (a >> 16);
    return a;
}

//option ==1 origJP + greedyGM-3Phase
//option ==2 oirgJP + greedyGM-MultiPhase
//option ==3 origJP + serial-greedy
//option ==4 origJP + stream
int SMPGCInterface::D1_OMP_JP2S_hyber(int nT, INT&colors, vector<INT>&vtxColors, const int option, const INT switch_iter = 0) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    string tag="Hyber";
    vector<INT> conflictQ;
    double tim_Wgt =0;    //run time
    double tim_MIS =0;    //run time
    double tim_ReG =0;    //run time
    double tim_Hyb =0;
    double tim_MxC =0;    //run time
    double tim_Tot =0;    //run time
    INT    nLoops = 0;    //Number of iterations 
 
    const INT N = nodes(); //number of vertex
    //const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();      //ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
   
    const vector<INT> &orig_ordered_vertices = ordered_vertex();
    INT QTail=N;

    vector<vector<INT>> QQ; QQ.resize(nT); 
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num(); 
        QQ[tid].reserve(N/nT+1+16);
        #pragma omp for
        for(int i=0; i<N; i++){
            QQ[tid].push_back(orig_ordered_vertices[i]);
        }
    }

    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tag = "GM3P";
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tag = "GMMP";
        conflictQ.resize(N,-1);
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tag = "GREEDY";
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
        tag ="STEAM";
    }
    else{
        printf("Error! Unknown option %d, should be {1,2,3,4}\n",option);
        exit(1);
    }

#ifdef DEBUG_JP_PROFILE
    vector<int>    profile_nodes;
    vector<double> profile_times;
#endif

    colors=0;
    // phase 1:  JP part
    while(QTail!=0) {
        QTail=0;
#ifdef DEBUG_JP_PROFILE
    auto t_jp_color_curiter = -tim_MIS;
    auto t_jp_recon_curiter = -tim_ReG;
    auto n_jp_nodes_curiter = QTail;
#endif
        if(nLoops>=switch_iter) break;
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> cand_nodes_color;
            vector<INT> &Q = QQ[tid];
            const INT remains_num_nodes = Q.size();
            INT num_leftover = 0;
            const unsigned int seed   = SMPGC_RAND_SEED;
            const unsigned int shift  = 0xC2A50F;
            const unsigned int num_hash = SMPGC_NUM_HASH;
            const int capacity  = 2*num_hash;
            const int color_base = nLoops*capacity;
            for(INT vit=0; vit<remains_num_nodes; vit++){
                const auto v = Q[vit];
                int vwt[num_hash];
                for(int i=0; i<num_hash; i++) vwt[i]=mhash(v, seed+shift*i);
                int bDomain= (1<<capacity)-1;  //0 neither; 1 LargeDomain; 2 SmallestDomain; 3 Both/Reserve/Init;
                const INT kEnd=verPtr[v+1];
                for(INT k=verPtr[v]; k!=kEnd; k++){
                    const auto w = verInd[k];
                    if(vtxColors[w]!=-1) continue;
                    for(int i=0; i<num_hash; i++) {
                        const auto wwt = mhash(w, seed+shift*i);
                        if( (bDomain&(0x1<<(i<<1))) && (vwt[i] <= wwt) ) bDomain^= (0x1<<(i<<1));
                        if( (bDomain&(0x2<<(i<<1))) && (vwt[i] >= wwt) ) bDomain^= (0x2<<(i<<1));
                    }
                    if(bDomain==0)  break;
                }
                if(bDomain==0) {
                    Q[num_leftover++]=v;
                }
                else{
                    for(int i=0; i<capacity; i++){
                        if(bDomain&(1<<i)){
                            cand_nodes_color.push_back(v);
                            cand_nodes_color.push_back(color_base+i);
                            break;
                        }
                    }
                }
            } // end for
            Q.resize(num_leftover);
            #pragma omp barrier
            for(int i=0; i<cand_nodes_color.size(); i+=2) 
                vtxColors[ cand_nodes_color[i] ]= cand_nodes_color[i+1];
        } // end of omp parallel
        tim_MIS += omp_get_wtime();
        
        tim_ReG -= omp_get_wtime();
        for(int tid=0; tid<nT; tid++){
            QTail+=QQ[tid].size();
        }
        tim_ReG += omp_get_wtime();
        nLoops++;

#ifdef DEBUG_JP_PROFILE
        n_jp_nodes_curiter -=QTail;
        t_jp_color_curiter +=tim_MIS;
        t_jp_recon_curiter +=tim_ReG;
        
        profile_nodes.push_back(n_jp_nodes_curiter);
        profile_times.push_back(t_jp_color_curiter+t_jp_recon_curiter);
#endif


    } // end while(QTail!=0);


    double tim_cx =- omp_get_wtime(); 
    vector<INT> Q;
    for(int i=0; i<nT; i++)
        Q.insert(Q.end(), QQ[i].begin(), QQ[i].end());
    QTail=Q.size();
    tim_cx += omp_get_wtime();

    // Phase 2, Greedy coloring
    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GM3P  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GMMP  (Q,QTail,conflictQ,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_greedy_serial  (Q,QTail, verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_stream  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else{
        tim_Hyb = 0;
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

    tim_Tot =tim_Wgt+ tim_MIS + tim_ReG + tim_Hyb + tim_MxC;
    printf("@JP2SHyber%s_@%lld_nT_c_T_TWgt_TMISC_TReG_Tcx_THyb_TMxC\t",tag.c_str(), switch_iter);
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_cx);
    printf("%lf\t", tim_Hyb);
    printf("%lf\t", tim_MxC);
    printf("\n"); 

#ifdef DEBUG_JP_PROFILE
    {
        profile_name; 
        ofstream of(profile_name.c_str());
        stringstream ss;
        ss<<profile_name;
        ss<<"\nnodes_per_iter\ttime_per_iter\tt/n_per_iter\n";
        for(int i=0; i<profile_nodes.size(); i++)
            ss<<profile_nodes[i]<<"\t"<<profile_times[i]<<"\t"<<profile_times[i]/profile_nodes[i]<<"\n";
        of<<ss.str();
        of.close();
    }
#endif

    
    

    return _TRUE;


}


int SMPGCInterface::D1_OMP_JP2Shash(int nT, INT& colors, vector<INT>&vtxColors) {
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
    vector<int> profile_num_nodes_remains;
    vector<int> profile_num_colors;
#endif

    const vector<INT> &orig_ordered_vertexs = ordered_vertex();
    INT QTail=N;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        QQ[tid].reserve(N/nT+1+16);
        #pragma omp for
        for(int i=0;i<N; i++){
            QQ[tid].push_back(orig_ordered_vertexs[i]);
        }
    }
    


    colors=0;
    while(QTail!=0){
        QTail=0;
        //phase 1: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> cand_vetex;
            vector<INT> cand_color;
            vector<INT>& Q = QQ[tid];
            const INT orig_Q_len= Q.size();
            INT num_leftover = 0;
            const unsigned int seed     = SMPGC_RAND_SEED;
            const unsigned int shift    = 0xC2A50F;
            const unsigned int num_hash = SMPGC_NUM_HASH;
            const int capacity  = 2*num_hash;
            const int color_base = nLoops*capacity;
            for(INT vit=0; vit<orig_Q_len; vit++){
                const auto v = Q[vit];
                int vwt[num_hash];
                for(int i=0;i<num_hash; i++) vwt[i]=mhash(v,seed+shift*i);//WeightRnd[v];
                int bDomain=(1<<(capacity))-1;  //0:neither, 1: LargeDomain, 2:SmallDomain, 3 Both/Reserve/Init;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto w = verInd[k];
                    if(vtxColors[w]!=-1) continue;
                    for(int i=0; i<num_hash; i++) {
                        const auto wwt = mhash(w, seed+shift*i);
                        if( (bDomain&(1<<(i<<1))) && vwt[i] <= wwt) bDomain ^=(1<<(i<<1));
                        if( (bDomain&(2<<(i<<1))) && vwt[i] >= wwt) bDomain ^=(2<<(i<<1));
                    }
                    if( bDomain==0 ){
                        break;
                    }
                }

                if(bDomain==0) Q[num_leftover++]=v;
                else{
                    for(int i=0; i<capacity; i++){
                        if(bDomain&(1<<i)){
                            cand_vetex.push_back(v);
                            cand_color.push_back(color_base+i);
                            break;
                        }
                    }
                }
            }
            Q.resize(num_leftover);
            #pragma omp barrier
            for(int i=0; i<cand_vetex.size(); i++)
                vtxColors[cand_vetex[i]]=cand_color[i];
        }   //end of omp parallel
        tim_MIS += omp_get_wtime();
        
        tim_ReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        for(int tid=0; tid<nT; tid++){
            QTail += QQ[tid].size();
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
            profile_num_nodes_remains.push_back(QTail);
            profile_num_colors.push_back(colors);
            printf("loop %d remains %d\n", nLoops, QTail);
        }//end for profile
#endif 
        nLoops++;
    } //while(QTail!=0);

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
    printf("@JP2Shash_nT_c_T_TWgt_TMISC_TReG_TMxC_nL\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);

#ifdef JP_PROFILE
    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoRepart_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_colored_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }

    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoPepart_num_colored_nodes_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(int i=0; i< profile_num_nodes_remains.size(); i++) {
            if(i==0)
                of1<<N-profile_num_nodes_remains[i]<<endl;
            else
                of1<<profile_num_nodes_remains[i+1]-profile_num_nodes_remains[i]<<endl;
        }
    }
#endif

    return _TRUE;
}




int SMPGCInterface::D1_OMP_JP2S_noRepartition(int nT, INT& colors, vector<INT>&vtxColors) {
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
    vector<int> profile_num_nodes_remains;
    vector<int> profile_num_colors;
#endif

    const vector<INT> &orig_ordered_vertexs = ordered_vertex();
    INT QTail=N;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        QQ[tid].reserve(N/nT+1+16);
        #pragma omp for
        for(int i=0;i<N; i++){
            QQ[tid].push_back(orig_ordered_vertexs[i]);
        }
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
#ifdef SMPGC_RAND_SEED
    srand(SMPGC_RAND_SEED);
#else
    srand(12345);
#endif
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
    while(QTail!=0){
        QTail=0;
        //phase 1: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> candiLrg;
            vector<INT> candiSml;
            vector<INT>& Q = QQ[tid];
            const INT orig_Q_len= Q.size();
            INT num_leftover = 0;
            for(INT vit=0; vit<orig_Q_len; vit++){
                const auto v   = Q[vit];
                const auto vwt = WeightRnd[v];
                char bDomain=0x03;  //0:neither, 1: LargeDomain, 2:SmallDomain, 3 Both/Reserve/Init;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto w = verInd[k];
                    if(vtxColors[w]!=-1) continue;
                    const auto wwt = WeightRnd[w];
                    if( (bDomain&0x1) && ( (vwt<wwt)||(vwt==wwt&&v>w) ) ) bDomain&=0xFE;
                    if( (bDomain&0x2) && ( (vwt>wwt)||(vwt==wwt&&v>w) ) ) bDomain&=0xFD;
                    if( bDomain==0) break;
                }
                if(bDomain==0) {
                    Q[num_leftover++]=v;
                }
                else{
                    if(bDomain!=2) candiLrg.push_back(v);
                    else           candiSml.push_back(v);
                }
            }
            Q.resize(num_leftover);
            #pragma omp barrier
            for(auto v : candiLrg) 
                vtxColors[v]=(nLoops<<1);
            for(auto v : candiSml) 
                vtxColors[v]=(nLoops<<1) +1;

        }   //end of omp parallel
        tim_MIS += omp_get_wtime();
        
        tim_ReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        for(int tid=0; tid<nT; tid++){
            QTail += QQ[tid].size();
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
            profile_num_nodes_remains.push_back(QTail);
            profile_num_colors.push_back(colors);
            printf("loop %d remains %d\n", nLoops, QTail);
        }//end for profile
#endif 
        nLoops++;
    } //while(QTail!=0);

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
    printf("@JP2SnR_nT_c_T_TWgt_TMISC_TReG_TMxC_nL\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);

#ifdef JP_PROFILE
    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoRepart_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_colored_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }

    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoPepart_num_colored_nodes_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(int i=0; i< profile_num_nodes_remains.size(); i++) {
            if(i==0)
                of1<<N-profile_num_nodes_remains[i]<<endl;
            else
                of1<<profile_num_nodes_remains[i+1]-profile_num_nodes_remains[i]<<endl;
        }
    }
#endif

    return _TRUE;
}






// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP2S(int nT, INT&colors, vector<INT>&vtxColors) {
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

#ifdef SMPGC_RAND_SEED
    srand(SMPGC_RAND_SEED);
#else
    srand(12345);
#endif

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
    printf("@JP2S_nT_c_T_TWgt_TMISC_TReG_TMxC_nLoop\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);

#ifdef JP_PROFILE
    {
        stringstream ss;
        ss<<"JP2S_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JP2S_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    {
        stringstream ss; //ss.str("");
        ss<<"JP2S_num_colors_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }
#endif

    return _TRUE;
}




//option ==1 origJP + greedyGM-3Phase
//option ==2 oirgJP + greedyGM-MultiPhase
//option ==3 origJP + serial-greedy
//option ==4 origJP + stream
int SMPGCInterface::D1_OMP_JP_hyber(int nT, INT&colors, vector<INT>&vtxColors, const int option, const INT switch_iter=1) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    string tag ="hyber";
    vector<INT> conflictQ;

    double tim_Wgt =0;                    // run time
    double tim_MIS =0;                    // run time
    double tim_ReG =0;                    // run time
    double tim_Hyb =0;
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
    for(INT i=0; i<N; i++){
        Q[i] = ((ordered_vertex())[i]);
    }
    colors = 0;

    vector<vector<INT>>  QQ; QQ.resize(nT);    
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tag = "GM3P";
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tag = "GMMP";
        conflictQ.resize(N,-1);
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tag = "Greedy";
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
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
#ifdef SMPGC_RAND_SEED 
    srand(123456);
#else
    srand(SMPGC_RAND_SEED);
#endif

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

#ifdef  DEBUG_JP_PROFILE
    vector<int>    profile_nodes;
    vector<double> profile_times;
#endif


    colors=0;
    // phase 1 : JP Part
    while(QTail!=0){
        if(nLoops >= switch_iter) break;


#ifdef DEBUG_JP_PROFILE
        auto t_jp_color_curiter = -tim_MIS;
        auto t_jp_recon_curiter = -tim_ReG;
        auto n_jp_nodes_curiter = QTail;
#endif

        tim_MIS -= omp_get_wtime();

        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> MASK; MASK.resize(MaxDegreeP1,-1);
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
                    if(vwt<nbwt){ //if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    //if(v<nb){
                        bDomain=false;
                        break;
                    }
                }
                if(bDomain) candi.push_back(v);
                else        QQ[tid].push_back(v);
            } //end of omp for

            for(auto v : candi) {
                //MASK.assign(MaxDegreeP1,-1);
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto nb      = verInd[k];
                    const auto nbcolor = vtxColors[nb];
                    if( nbcolor>=0 ){
                        MASK[nbcolor]=v;
                    }
                }
                int c;
                for(c=0; c<MaxDegreeP1; c++)
                    if(MASK[c]!=v) {
                        vtxColors[v]=c;
                        break;
                    }

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


#ifdef DEBUG_JP_PROFILE
        n_jp_nodes_curiter -=QTail;
        t_jp_color_curiter +=tim_MIS;
        t_jp_recon_curiter +=tim_ReG;
        
        profile_nodes.push_back(n_jp_nodes_curiter);
        profile_times.push_back(t_jp_color_curiter+t_jp_recon_curiter);
#endif


        nLoops++;
    } //end while( QTail!=0);

    // Phase 2, Coloring rest of graphs
    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GM3P  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GMMP  (Q,QTail,conflictQ,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tim_Hyb =- omp_get_wtime();
        //auto start = std::chrono::high_resolution_clock::now();
        hyberJP_implement_greedy_serial  (Q,QTail,verPtr,verInd,colors,vtxColors);
        //auto elapsed = std::chrono::high_resolution_clock::now() - start;
        //long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        //printf("\nmicroseconds %lld\n", microseconds);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_stream  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else{
        tim_Hyb=0;
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

    tim_Tot =tim_Wgt+ tim_MIS+tim_ReG+tim_Hyb + tim_MxC;
    printf("@JPHyber%s_@%lld_nT_c_T_TWgt_TMISC_TReG_THyb_TMxC\t",tag.c_str(), switch_iter);
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_Hyb);
    printf("%lf\t", tim_MxC);
    printf("\n"); 

#ifdef DEBUG_JP_PROFILE
    {
        profile_name; 
        ofstream of(profile_name.c_str());
        stringstream ss;
        ss<<profile_name;
        ss<<"\nnodes_per_iter\ttime_per_iter\tt/n_per_iter\n";
        for(int i=0; i<profile_nodes.size(); i++)
            ss<<profile_nodes[i]<<"\t"<<profile_times[i]<<"\t"<<profile_times[i]/profile_nodes[i]<<"\n";
        of<<ss.str();
        of.close();
    }
#endif


    return _TRUE;
}

//option ==1 origJP + greedyGM-3Phase
//option ==2 oirgJP + greedyGM-MultiPhase
//option ==3 origJP + serial-greedy
//option ==4 origJP + stream
int SMPGCInterface::D1_OMP_JP2S_hyber_slow(int nT, INT&colors, vector<INT>&vtxColors, const int option, const INT switch_iter = 0) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    string tag="Hyber";
    vector<INT> conflictQ;
    double tim_Wgt =0;    //run time
    double tim_MIS =0;    //run time
    double tim_ReG =0;    //run time
    double tim_Hyb =0;
    double tim_MxC =0;    //run time
    double tim_Tot =0;    //run time
    INT    nLoops = 0;    //Number of iterations 
 
    const INT N = nodes(); //number of vertex
    //const INT MaxDegreeP1 = maxDeg()+1; //maxDegree
    
    INT const * const verPtr=CSRiaPtr();      //ia of csr
    INT const * const verInd=CSRjaPtr();      //ja of csr
    
    colors=0;
    vtxColors.clear(); vtxColors.resize(N, -1);
   
    vector<INT> Q; Q.resize(N,-1);
    INT QTail=N;
    for(INT i=0; i<N; i++){
        Q[i]  = ((ordered_vertex())[i]);
    }

    vector<vector<INT>> QQ; QQ.resize(nT); 
    for(int i=0;i<nT; i++){
        QQ[i].reserve(N/nT+1+16);
    }

    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tag = "GM3P";
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tag = "GMMP";
        conflictQ.resize(N,-1);
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tag = "GREEDY";
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
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
#ifdef SMPGC_RAND_SEED
    srand(SMPGC_RAND_SEED);
#else
    srand(123456);
#endif

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

#ifdef DEBUG_JP_PROFILE
    vector<int>    profile_nodes;
    vector<double> profile_times;
#endif


    colors=0;
    // phase 1:  JP part
    while(QTail!=0) {

#ifdef DEBUG_JP_PROFILE
    auto t_jp_color_curiter = -tim_MIS;
    auto t_jp_recon_curiter = -tim_ReG;
    auto n_jp_nodes_curiter = QTail;
#endif
        if(nLoops>=switch_iter) break;
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> candiLrg;
            vector<INT> candiSml;
            const int remains_num_nodes = QTail;
            #pragma omp for
            for(INT vit=0; vit<remains_num_nodes; vit++){
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
        nLoops++;

#ifdef DEBUG_JP_PROFILE
        n_jp_nodes_curiter -=QTail;
        t_jp_color_curiter +=tim_MIS;
        t_jp_recon_curiter +=tim_ReG;
        
        profile_nodes.push_back(n_jp_nodes_curiter);
        profile_times.push_back(t_jp_color_curiter+t_jp_recon_curiter);
#endif


    } // end while(QTail!=0);


    
    // Phase 2, Greedy coloring
    if(option==JP_HYBER_IMPLEMENT_GM3P) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GM3P  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GMMP) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_GMMP  (Q,QTail,conflictQ,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_GREEDY) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_greedy_serial  (Q,QTail, verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else if(option==JP_HYBER_IMPLEMENT_STREAM) {
        tim_Hyb =- omp_get_wtime();
        hyberJP_implement_stream  (Q,QTail,QQ,nT,verPtr,verInd,colors,vtxColors);
        tim_Hyb += omp_get_wtime();
    }
    else{
        tim_Hyb = 0;
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

    tim_Tot =tim_Wgt+ tim_MIS + tim_ReG + tim_Hyb + tim_MxC;
    printf("@JP2SHyber%s_slow_@%lld_nT_c_T_TWgt_TMISC_TReG_THyb_TMxC\t",tag.c_str(), switch_iter);
    printf("%d\t",  nT);    
    printf("%lld\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_Hyb);
    printf("%lf\t", tim_MxC);
    printf("\n"); 

#ifdef DEBUG_JP_PROFILE
    {
        profile_name; 
        ofstream of(profile_name.c_str());
        stringstream ss;
        ss<<profile_name;
        ss<<"\nnodes_per_iter\ttime_per_iter\tt/n_per_iter\n";
        for(int i=0; i<profile_nodes.size(); i++)
            ss<<profile_nodes[i]<<"\t"<<profile_times[i]<<"\t"<<profile_times[i]/profile_nodes[i]<<"\n";
        of<<ss.str();
        of.close();
    }
#endif

    
    

    return _TRUE;


}


//  ===========================================================================
//  inline hyberJP_implement_GM3P(Q, Qlen, QofQ, nT, verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_GMMP(Q, Qlen, Qbuf,     verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_greedy_serial(Q, Qlen,  verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_stream(....)
//  ===========================================================================
inline
void SMPGCInterface::hyberJP_implement_GM3P(vector<INT>&Q, const INT &QTail, vector<vector<INT>>& QQ, const int &nT, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColor){
    const INT MaxDegreeP1 = maxDeg()+1;
    //const INT N = nodes();
    #pragma omp parallel
    {
        // Phase 0: Partition
        // Phase 1: Pseudo-color
        vector<bool> Mask;
        #pragma omp for
        for(INT it=0; it<QTail; it++) {
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
    
        // Phase 2: Detect conflicts
        const int tid=omp_get_thread_num();
        vector<INT>& Qtmp = QQ[tid];
        #pragma omp for
        for(INT it=0; it<QTail; it++){
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
} // end function

//  ===========================================================================
//  inline hyberJP_implement_GM3P(Q, Qlen, QofQ,    verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_GMMP(Q, Qlen, Qbuf,    verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_greedy_serial(Q, Qlen, verPtr, verInd, colors, vtxColors)
//  inline hyberJP_implement_stream(....)
//  ===========================================================================
inline void SMPGCInterface::hyberJP_implement_GMMP(vector<INT>&readyQ, const INT &QTail, vector<INT>&conflictQ, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors){
    const INT MaxDegreeP1 = maxDeg()+1;
    INT Q_len = QTail;
    while(Q_len!=0){
        const INT num_nodes_remains = Q_len;
        Q_len = 0;
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
        } 
        
        //nConflicts += Q_len;
        //nLoops++;
        conflictQ.resize(Q_len);
        readyQ.resize(Q_len);
        readyQ.swap(conflictQ);
    }
} // end function

/*
inline void SMPGCInterface::hyberJP_implement_greedy_serial(vector<INT>&Q, const INT &QTail, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors){
    //printf("wocao(%d)\t",QTail); 
     
    const INT MaxDegreeP1 = maxDeg()+1;
    //const INT N = nodes();
    vector<INT> Mark; Mark.assign(MaxDegreeP1,-1);
    for(INT i=0; i<QTail; i++){
        const auto v = Q[i];
        for(INT wit = verPtr[v]; wit!=verPtr[v+1]; wit++) {
            const auto w = verInd[wit];
            const auto wc = vtxColors[w];
            if(wc>=0)
                Mark[wc] = v;
        }
        INT c;
        for(c=0; c!=MaxDegreeP1; c++)
            if(Mark[c]!=v){
                vtxColors[v]=c;
                break;
            }
    }
}
*/
 void SMPGCInterface::hyberJP_implement_greedy_serial(vector<INT>&Q, const INT &QTail, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors){
    double tim_order=0;
    double tim_color=0;
    const INT MaxDegreeP1 = maxDeg()+1;
    const INT N = QTail;
    //printf("Begin ");
   // printf("(input N%d)\t",N);
    //printf("(MaxDegreeP1 %d)",MaxDegreeP1);
    //printf("(Q.size() %d, expect %d)",Q.size(), QTail);
    
    //Q.clear();
    //for(int i=0; i<N; i++)
    //    Q.push_back(i);

    //vtxColors.clear();
    //for(int i=0; i<N; i++)
    //    vtxColors.push_back(-1);

    //colors=0;
    
    
    //vector<INT> Qtmp; Qtmp.reserve(QTail);
    tim_order = -omp_get_wtime();
    { //ordering
        vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
        for(auto i=0; i<N; i++){
            auto v=Q[i];
            INT deg = verPtr[v+1]-verPtr[v];
            GroupedVertexDegree[deg].push_back(v);
        }
        Q.clear();
        
        for(int i=MaxDegreeP1-1; i>=0; i--){
            int i_VertexDegreeCount = (signed)GroupedVertexDegree[i].size();
            for(int j=0; j<i_VertexDegreeCount; j++) {
                Q.push_back(GroupedVertexDegree[i][j]);
            }
        }
    
        //INT pos=0;
        //for(INT d=MaxDegreeP1-1, it=MaxDegreeP1; it!=0; it--, d--){
        //    for(auto v : GroupedVertexDegree[d]){
        //        Q[pos++]=v;
        //    }
        //}

        GroupedVertexDegree.clear();   

    }
    
    tim_order+=omp_get_wtime();

    vector<INT> Mask(MaxDegreeP1,-1);
    tim_color=-omp_get_wtime();
    for(INT vit=0; vit<N; vit++){
        const auto v   = Q[vit];
        for(INT wit=verPtr[v]; wit<verPtr[v+1]; wit++){
            const auto w = verInd[wit];
            const auto wc = vtxColors[w];
            if(wc<0) continue;
            Mask[wc]=v;
        }
        INT c;
        for(c=0; c<MaxDegreeP1; c++)
            if(Mask[c]!=v){
                vtxColors[v]=c;
                //if(colors<c) colors=c;
                break;
            }

    }
   
    tim_color +=omp_get_wtime();
    colors=0;
    for(int i=0; i<N; i++){
        if(colors<vtxColors[i]){
            colors = vtxColors[i];
        }
     }
    //printf("(c%d)\t",colors+1);
    //printf("(%g=%g+%g)\t",tim_order+tim_color, tim_order, tim_color);
    //printf("END\t");
}







inline void SMPGCInterface::hyberJP_implement_stream(vector<INT>&Q, const INT &QTail, vector<vector<INT>>&QQ, const int&nT, INT const * const &verPtr, INT const * const &verIdx, INT& colors, vector<INT>&vtxColors){
    //TODO
    printf("under construction\n");
    exit(1);
}



int SMPGCInterface::D1_OMP_JP2Shashsingle(int nT, INT& colors, vector<INT>&vtxColors) {
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
    vector<int> profile_num_nodes_remains;
    vector<int> profile_num_colors;
#endif

    const vector<INT> &orig_ordered_vertexs = ordered_vertex();
    INT QTail=N;
    
    vector<vector<INT>>  QQ; QQ.resize(nT);    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        QQ[tid].reserve(N/nT+1+16);
        #pragma omp for
        for(int i=0;i<N; i++){
            QQ[tid].push_back(orig_ordered_vertexs[i]);
        }
    }
    

    unsigned int seed = 123456;
    colors=0;
    while(QTail!=0){
        QTail=0;
        //phase 1: find maximal indenpenent set, and color it
        tim_MIS -= omp_get_wtime();
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            vector<INT> candiLrg;
            vector<INT> candiSml;
            vector<INT>&Q = QQ[tid];
            const INT orig_Q_len = Q.size();
            INT num_leftover = 0;
            for(INT vit=0; vit<orig_Q_len; vit++){
                const auto v = Q[vit];
                const auto vwt = mhash(v, seed);
                char bDomain = 0x03;
                for(INT k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                    const auto w = verInd[k];
                    if(vtxColors[w]!=-1) continue;
                    const auto wwt = mhash(w, seed);
                    if( (bDomain&0x1) && ( (vwt<wwt)||(vwt==wwt&&v>w) ) ) bDomain&=0xFE;
                    if( (bDomain&0x2) && ( (vwt>wwt)||(vwt==wwt&&v>w) ) ) bDomain&=0xFD;
                    if( bDomain==0) break;
                }
                if(bDomain==0) Q[num_leftover++]=v;
                else{
                    if(bDomain!=2) candiLrg.push_back(v);
                    else           candiSml.push_back(v);
                }
            }
            Q.resize(num_leftover);
            #pragma omp barrier
            for(auto v : candiLrg)
                vtxColors[v]=2*nLoops;
            for(auto v : candiSml)
                vtxColors[v]=2*nLoops+1;
        }   //end of omp parallel
        tim_MIS += omp_get_wtime();
        
        tim_ReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        for(int tid=0; tid<nT; tid++){
            QTail += QQ[tid].size();
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
            profile_num_nodes_remains.push_back(QTail);
            profile_num_colors.push_back(colors);
            printf("loop %d remains %d\n", nLoops, QTail);
        }//end for profile
#endif 
        nLoops++;
    } //while(QTail!=0);

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
    printf("@JP2Shash_nT_c_T_TWgt_TMISC_TReG_TMxC_nL\t");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_Wgt);
    printf("%lf\t", tim_MIS);
    printf("%lf\t", tim_ReG);
    printf("%lf\t", tim_MxC);
    printf("%d\n", nLoops);

#ifdef JP_PROFILE
    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoRepart_num_nodes_L_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_L)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_nodes_S_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_nodes_S)
            of1<<x<<endl;
        of1.close();
    }

    if(0)
    {
        stringstream ss; //ss.str("");
        ss<<"JP2SnoRepart_num_colored_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(auto x : profile_num_colors)
            of1<<x<<endl;//fprintf(of2, "%d\n",x);
        of1.close();
    }

    if(0)
    {
        stringstream ss;
        ss<<"JP2SnoPepart_num_colored_nodes_nT"<<nT<<".log";
        ofstream of1(ss.str().c_str());
        for(int i=0; i< profile_num_nodes_remains.size(); i++) {
            if(i==0)
                of1<<N-profile_num_nodes_remains[i]<<endl;
            else
                of1<<profile_num_nodes_remains[i+1]-profile_num_nodes_remains[i]<<endl;
        }
    }
#endif

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





/*
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
*/





/*
int SMPGCInterface::D1Greedy(int nT, INT&colors, vector<INT>&vtxColors) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    vector<INT> conflictQ;

    double tim_Wgt =0;                    // run time
    double tim_MIS =0;                    // run time
    double tim_ReG =0;                    // run time
    double tim_Hyb =0;
    double tim_MxC =0;                    // run time
    double tim_Tot =0;                    // run time
    INT    nLoops = 0;                    // number of iteration
 
    const INT N = nodes();                // number of vertex
    const INT MaxDegreeP1 = maxDeg()+1;   // maxDegree
   
    INT const * const verPtr=CSRiaPtr();  // ia of csr
    INT const * const verInd=CSRjaPtr();  // ja of csr
    
    INT QTail = N;
    vector<INT> Q;

 
    Q.clear();
    for(int i=0; i<N; i++)
        Q.push_back(i);
    vtxColors.assign(N,-1);
    colors=0;
   
    hyberJP_implement_greedy_serial  (Q,N,verPtr,verInd,colors,vtxColors);
    
    
    double tim_order =0;
    double tim_color =0;                      // run time
    //const INT N = nodes();                   // number of vertex
    //const INT MaxDegreeP1 = maxDeg()+1;    // maxDegree
    
    //INT const * const verPtr=CSRiaPtr();     // ia of csr
    //INT const * const verInd=CSRjaPtr();     // ja of csr
    
    Q.clear();
    for(int i=0; i<N; i++)
        Q.push_back(i);
    vtxColors.assign(N,-1);
    colors=0;

    hyberJP_implement_greedy_serial(Q, N, verPtr, verInd, colors, vtxColors); //vector<INT>&Q, const INT &QTail, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors){


    Q.clear();
    for(int i=0; i<N; i++)
        Q.push_back(i);
    vtxColors.assign(N,-1);
    colors=0;



    tim_order = -omp_get_wtime();
    { //ordering
        vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
        for(auto i=0; i<N; i++){
            const auto v = Q[i];
            INT deg = verPtr[v+1]-verPtr[v];
            GroupedVertexDegree[deg].push_back(v);
        }
        
        Q.clear();
        for(int i=MaxDegreeP1-1; i>=0; i--){
            int i_VertexDegreeCount = (signed)GroupedVertexDegree[i].size();
            for(int j=0; j<i_VertexDegreeCount; j++) {
                Q.push_back(GroupedVertexDegree[i][j]);
            }
        }
    
        //INT pos=0;
        //for(INT d=MaxDegreeP1-1, it=MaxDegreeP1; it!=0; it--, d--){
        //    for(auto v : GroupedVertexDegree[d]){
        //        Q[pos++]=v;
        //    }
        //}

        GroupedVertexDegree.clear();   

    }
    
    tim_order+=omp_get_wtime();

    vector<INT> Mask(MaxDegreeP1,-1);
    tim_color=-omp_get_wtime();
    for(INT vit=0; vit<N; vit++){
        const auto v   = Q[vit];
        for(INT wit=verPtr[v]; wit<verPtr[v+1]; wit++){
            const auto w = verInd[wit];
            const auto wc = vtxColors[w];
            if(wc<0) continue;
            Mask[wc]=v;
        }
        INT c;
        for(c=0; c<MaxDegreeP1; c++)
            if(Mask[c]!=v){
                vtxColors[v]=c;
                if(colors<c) colors=c;
                break;
            }

    }
   
    tim_color +=omp_get_wtime();
    //tim_MxC = -omp_get_wtime();
    //colors=0;
    //#pragma omp parallel for reduction(max:colors)
    //for(int i=0; i<N; i++){
    //    auto c = vtxColors[i];
    //    if(c>colors) colors=c;
   // }
    colors++;
    //tim_MxC += omp_get_wtime();

    tim_Tot = tim_order+ tim_color;
    printf("@Greedy_nT_c_T_Torder_Tcolor\t");
    printf("%d\t",  nT);
    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_order);
    printf("%lf\t", tim_color);
    printf("\n");

    return _TRUE;
}

*/





