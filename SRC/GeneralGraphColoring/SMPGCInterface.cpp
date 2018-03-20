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
int SMPGCInterface::Coloring(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord, const string& method){
    if     (method.compare("DISTANCE_ONE_OMP_GM")==0) return D1_OMP_GM(nT, colors, vtxColors, loc_ord);
    else if(method.compare("DISTANCE_ONE_OMP_IP")==0) return D1_OMP_IP(nT, colors, vtxColors, loc_ord);
    else if(method.compare("DISTANCE_ONE_OMP_JP")==0) return D1_OMP_JP(nT, colors, vtxColors, loc_ord);
    else if(method.compare("DISTANCE_ONE_OMP_LB")==0) return D1_OMP_JP(nT, colors, vtxColors, loc_ord);
    else if(method.compare("DISTANCE_ONE_OMP_JP_AW_LF")==0) return D1_OMP_JP(nT, colors, vtxColors, loc_ord);
    else if(method.compare("DISTANCE_ONE_OMP_JP_AW_SL")==0) return D1_OMP_JP(nT, colors, vtxColors, loc_ord);
    else { fprintf(stdout, "Unknow method %s\n",method.c_str()); exit(1); }   
    return _TRUE;
}

// ============================================================================
// Construction
// ============================================================================
SMPGCInterface::SMPGCInterface(const string& graph_name){
    GraphCore::m_s_InputFile = graph_name;
    do_read_MM_struct(graph_name, m_vi_Vertices, m_vi_Edges, &m_i_MaximumVertexDegree, &m_i_MaximumVertexDegree, &m_d_AverageVertexDegree);
    GraphOrdering::NaturalOrdering();
}

// ============================================================================
// Construction
// ============================================================================
SMPGCInterface::SMPGCInterface(const string& graph_name, const string& glb_order){
    GraphCore::m_s_InputFile = graph_name;
    do_read_MM_struct(graph_name, m_vi_Vertices, m_vi_Edges, &m_i_MaximumVertexDegree, &m_i_MaximumVertexDegree, &m_d_AverageVertexDegree);
    GraphOrdering::OrderVertices(glb_order);
}

// ============================================================================
// DeConstruction
// ============================================================================
SMPGCInterface::~SMPGCInterface(){
}

// ============================================================================
// Read MatrixMarket only structure into memory
// ----------------------------------------------------------------------------
// Note: store as sparsed CSR format
// ============================================================================
void SMPGCInterface::do_read_MM_struct(const string& graph_name, vector<int>&ia, vector<int>&ja, int* pMaxDeg, int* pMinDeg, double* pAvgDeg) {
    if(graph_name.empty()) error("graph name is empty");
    FILE*fp = fopen(graph_name.c_str(), "r");
    if(!fp) error("cannot open graph",graph_name);
    
    bool bSymmetric=true;
    bool bPattern  =false;
    MM_typecode matcode;
    mm_read_banner(fp, &matcode);
    if(!mm_is_matrix(matcode)) { fclose(fp); error(graph_name, " is not MarixMarket matrix "); }
    if(!mm_is_symmetric(matcode)) {bSymmetric=false; fprintf(stdout, "Warning! %s is not symmetric\n",graph_name.c_str()); }
    if(mm_is_complex(matcode)) { fclose(fp); error(graph_name, " is complex which is not support "); }
    if(mm_is_pattern(matcode)) bPattern = true;  
    int N; //number of vertex
    unordered_map<int, vector<int> > G; 
    if(mm_is_sparse(matcode)) { 
        int tmpR, tmpC, tmpNNZ;
        double tmpV;
        mm_read_mtx_crd_size(fp, &tmpR, &tmpC, &tmpNNZ);
        if(tmpR!=tmpC) { fclose(fp); error(graph_name, "number of Row does not equal number of Col"); }
        N=tmpR;
        //M=0;
        for(int i=0; i<tmpNNZ; i++){
            if(bPattern)
                fscanf(fp, "%d %d\n", &tmpR, &tmpC);
            else
                fscanf(fp, "%d %d %lg\n", &tmpR, &tmpC, &tmpV);
            if(tmpR==tmpC) continue;
            tmpR--; tmpC--; //1-based to 0-based
            
            G[tmpR].push_back(tmpC);
            //M++;
            if(bSymmetric){
                G[tmpC].push_back(tmpR);
                //M++;

                if(tmpR<tmpC) {
                    printf("%d %d\n",tmpR,tmpC);
                    error(graph_name, "Find a nonzero in upper triangular in symmetric graph");
                }
            }
        }
    } // endof if mm_is_sparse(matcode)
    else{
        fprintf(stdout,"Warning! it is weird to store graph into dense array! %s \n",graph_name.c_str());
        int tmpR, tmpC;
        double tmpV;
        mm_read_mtx_array_size(fp, &tmpR, &tmpC);
        if(tmpR!=tmpC) { fclose(fp); error(graph_name, "number of Row does not equal number of Col"); }
        N=tmpR;
        for(int c=0; c<tmpC; c++){
            for(int r=(bSymmetric?c:0); r<tmpR; c++){
                fscanf(fp, "%lg\n", &tmpV);
                if(tmpV==.0) continue; //I hate double == 0 condition
                if(c==r) continue;
                G[r].push_back(c); //M++;
                if(bSymmetric){
                    G[c].push_back(r);// M++;
                }
            }
        }
    } // endof else mm_is_sparse(matcode)
    fclose(fp); fp=nullptr;

    //for(auto it=G.begin(), it!=G.end(); it++)
    //    sort((it->second).begin(), (it->second).end());

    ia.push_back(ja.size());
    for(int i=0; i<N; i++){
        auto it=G.find(i);
        if(it!=G.end()) ja.insert(ja.end(), (it->second).begin(), (it->second).end());
        ia.push_back(ja.size());
    }
    
    // calc max degree if needed
    if(pMaxDeg||pMinDeg){
        int maxDeg=0, minDeg=ia.size()-1;
        for(auto it : G){
            int d = (it.second).size();
            maxDeg = (maxDeg<d)?d:maxDeg;
            minDeg = (minDeg>d)?d:minDeg;
        }
        if(pMaxDeg) *pMaxDeg = maxDeg;
        if(pMinDeg) *pMinDeg = minDeg;
    }
    if(pAvgDeg) *pAvgDeg=1.0*(ja.size())/(ia.size()-1);
    

}

// ============================================================================
// dump information
// ============================================================================
void SMPGCInterface::dump(){
    ;
}

// ============================================================================
// error and stop
// ============================================================================
void SMPGCInterface::error(const string&s1) { 
    fprintf(stdout, "Error in class SMPGCInterface with message \"%s\"\n",s1.c_str()); 
    exit(1); 
}

// ============================================================================
// error and stop
// ============================================================================
void SMPGCInterface::error(const string&s1, const string&s2) { 
    fprintf(stdout, "Error in class SMPGCInterface with message \"%s %s\"\n",s1.c_str(), s2.c_str()); 
    exit(1); 
}

// ============================================================================
// order local vertex according to some heuristic
// ============================================================================
int SMPGCInterface::do_local_ordering(vector<int>&ordered_vertex, const string& o){
    //TODO
    return _TRUE;
}

// ============================================================================
// check if the graph is correct colored
// ============================================================================
bool SMPGCInterface::do_verify_colors(int colors, const vector<int>& vtxColors){

    const int N = GraphCore::GetVertexCount();
    vector<int>& verPtr = GraphCore::m_vi_Vertices;
    vector<int>& verInd = GraphCore::m_vi_Edges;
    int Conflicts=0;
#pragma omp parallel for
    for (int v=0; v < N; v++ ) {
        for(int nbit=verPtr[v],nbitEnd=verPtr[v+1]; nbit!=nbitEnd; nbit++ ) {
            if ( vtxColors[v] == vtxColors[verInd[nbit]] ) {
                __sync_fetch_and_add(&Conflicts, 1); //increment the counter
            }
        }
    }
    return (Conflicts==0?true:false);
}



// ============================================================================
// based on Gebremedhin and Manne's GM algorithm [1]
// ============================================================================
int SMPGCInterface::D1_OMP_GM(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double tim_0  =0,tim_1=0,tim_2=0,tim_3=0;    //run time
    double tim_Tot=0;               //run time
    int    nConflicts = 0;                     //Number of conflicts 
    
    const int N = GraphCore::GetVertexCount(); //number of vertex
    
    vector<int> &verPtr=m_vi_Vertices;      //ia of csr
    vector<int> &verInd=m_vi_Edges;         //ja of csr
    
    const int MaxDegreeP1 = m_i_MaximumVertexDegree+1; //maxDegree
    
    colors=0;
    vtxColors.assign(N, -1);


    vector<int> Q;
    Q.assign(N,-1);

    vector<vector<int> > QQ, QQtmp;
    QQ.resize(nT);
    QQtmp.resize(nT);

    const int qtnt = N/nT;
    const int rmnd = N%nT;

    #pragma omp parallel for
    for(int tid=0; tid<nT; tid++){
        const int Nloc = qtnt + (tid<rmnd?1:0);
        QQ[tid].resize(Nloc);
        QQtmp[tid].reserve(Nloc);
    }
    
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]=m_vi_OrderedVertices[i];
    }


tim_1 =- omp_get_wtime();
#pragma omp parallel
    {
        int tid=omp_get_thread_num();
        vector<bool> Mask;

        // Phase 0: Partition
        const int Nloc =  qtnt + (tid<rmnd?1:0);
        const int low  =  tid*qtnt + (tid<rmnd?tid:rmnd);
        const int high =  low+Nloc;
        // Phase 1: Pseudo-color
        for(int qit=low; qit<high; qit++){
            int v=Q[qit];
            Mask.assign(MaxDegreeP1, false);
            for(int nbit=verPtr[v], nbitEnd=verPtr[v+1], nbColor; 
                    nbit!=nbitEnd; nbit++ ) {
                if( (nbColor=vtxColors[verInd[nbit]]) >= 0) 
                    Mask[nbColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mask[c] == false)
                    break;
            vtxColors[v] = c;
        } //End of outer for loop: for each vertex
    }//end of openmp parallel
    tim_1  += omp_get_wtime();    

/*
tim_1 =- omp_get_wtime();
#pragma omp parallel
    {
        int tid=omp_get_thread_num();
        vector<bool> Mask;
        
        // Phase 1: Pseudo-color

#pragma omp for
        for(int qit=0; qit<N; qit++){
            int v = Q[qit];
            Mask.assign(MaxDegreeP1, false);
            for(int nbit=verPtr[v], nbitEnd=verPtr[v+1], nbColor; 
                    nbit!=nbitEnd; nbit++ ) {
                if( (nbColor=vtxColors[verInd[nbit]]) >= 0) 
                    Mask[nbColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mask[c] == false)
                    break;
            vtxColors[v] = c;
        } //End of outer for loop: for each vertex
    }//end of openmp parallel
    tim_1  += omp_get_wtime();
*/


// Phase 2: Detect conflicts
    tim_2 =- omp_get_wtime();
    #pragma omp parallel for
    for (int tid=0; tid<nT; tid++){
        vector<int>& Q = QQ[tid];
        vector<int>& Qtmp = QQtmp[tid];
        for(auto v : Q){
            for(int k=verPtr[v],kEnd=verPtr[v+1],nb; k!=kEnd; k++) {
                nb = verInd[k];
                if(v>nb && vtxColors[v] == vtxColors[nb]) {
                    Qtmp.push_back(v);
                    vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                    if(Qtmp.size()==1) cout<<Qtmp.size()<<endl;
                    break;
                } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
            } //End of inner for loop: w in adj(v)
        }//End of outer for loop: for each vertex
    }
    tim_2  += omp_get_wtime();
    
// Phase 3: Resolve conflicts
    tim_3 =- omp_get_wtime();
    for(int tid=0;tid<nT; tid++){
        if(QQtmp[tid].empty()) continue;
        vector<bool> Mark;
        for(auto v: QQtmp[tid]){
            Mark.resize(MaxDegreeP1, false);
            for(int k=verPtr[v], kEnd=verPtr[v+1], nbColor; 
                    k!=kEnd; k++ ) {
                if( (nbColor=vtxColors[verInd[k]]) >= 0) 
                    Mark[nbColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mark[c] == false)
                    break;
            vtxColors[v] = c;
        }
    }
    tim_3 += omp_get_wtime();

    for(auto q:QQtmp)
        nConflicts+=q.size();

    tim_Tot = tim_0+tim_1+tim_2+tim_3;
    // get number of colors
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1


#ifndef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Total Time                 : %lf sec\n", tim_Tot);
    printf("Phase 0 Time               : %lf sec\n", tim_0);
    printf("Phase 1 Time               : %lf sec\n", tim_1);
    printf("Phase 2 Time               : %lf sec\n", tim_2);
    printf("Phase 3 Time               : %lf sec\n", tim_3);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("%d\t",  colors);    
    printf("%lf\t", tim_Tot);
    printf("%lf\t", tim_0);
    printf("%lf\t", tim_1);
    printf("%lf\t", tim_2);
    printf("%lf\t", tim_3);
    printf("%d\t", nConflicts);
    printf("\n");
    return _TRUE;   
}

// ============================================================================
// based on Catalyurek et al 's IP algorithm [2]
// ============================================================================
int SMPGCInterface::D1_OMP_IP(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double time1=0, time2=0, totalTime=0;   //run time
    long   nConflicts = 0;                  //Number of conflicts 
    int    nLoops = 0;                      //Number of rounds 
    
    const int N   = GraphCore::GetVertexCount(); //number of vertex
    
    int *verPtr  = &m_vi_Vertices[0];      //ia of csr
    int *verInd  = &m_vi_Edges[0];         //ja of csr
    
    const int MaxDegreeP1 = m_i_MaximumVertexDegree+1; //maxDegree
    
    colors=0;
    vtxColors.assign(N, -1);

    vector<int> Q, Qtmp;
    int QTail=0, QtmpTail=0;
    Q.resize(N);
    Qtmp.resize(N);
    
    #pragma omp parallel for
    for (int i=0; i<N; i++) {
        Q[i] = m_vi_OrderedVertices[i];
        Qtmp[i]= -1; //Empty queue
    }
    QTail = N;	//Queue all vertices

    
    do {
        // phase psedue color
        time1 -= omp_get_wtime();
        #pragma omp parallel for
        for (int Qi=0; Qi<QTail; Qi++) {
            int v = Q[Qi]; 
            vector<bool> Mark(MaxDegreeP1, false);

            for(int k = verPtr[v], kEnd=verPtr[v+1], nbColor; k !=kEnd; k++ ) {
                if( (nbColor=vtxColors[verInd[k]]) >= 0) 
                    Mark[nbColor] = true; //assert(adjColor<Mark.size())
            } 
            int c;
            for (c=0; c!=MaxDegreeP1; c++)
                if(Mark[c] == false)
                    break;
            vtxColors[v] = c;
        } //End of outer for loop: for each vertex
        time1  += omp_get_wtime();


        //phase Detect Conflicts:
        time2 -= omp_get_wtime();
        #pragma omp parallel for
        for (int Qi=0; Qi<QTail; Qi++) {
            int v = Q[Qi]; 
            //Browse the adjacency set of vertex v
            for(int k=verPtr[v],kEnd=verPtr[v+1],nb; k!=kEnd; k++) {
                nb = verInd[k];
                if(vtxColors[v] == vtxColors[nb] && v>nb) {
                    int whereInQ = __sync_fetch_and_add(&QtmpTail, 1);
                    Qtmp[whereInQ] = v;//Add to the queue
                    vtxColors[v] = -1;  //Will prevent v from being in conflict in another pairing
                    break;
                } //End of if( vtxColor[v] == vtxColor[verInd[k]] )
            } //End of inner for loop: w in adj(v)
        }//End of outer for loop: for each vertex
        time2  += omp_get_wtime();
        
        nConflicts += QtmpTail;
        nLoops++;
#ifdef PRINT_DETAILED_STATS_
        printf("Loops (1-based)     : %d\n", nLoops);
        printf("Time Pseudo Coloring: %lg sec.\n", time1);
        printf("Time Detection      : %lg sec.\n", time2);
        printf("Num  Conflicts      : %ld \n", QtmpTail);
#endif
        Q.swap(Qtmp);
        QTail=QtmpTail;
        QtmpTail=0;
    } while (QTail > 0);

    totalTime = time1+time2;

    // get number of colors
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColors[i]);
    }
    colors++; //number of colors = largest color(0-based) + 1


#ifndef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Number of rounds           : %d \n", nLoops);      
    printf("Total Time                 : %lf sec\n", totalTime);
    printf("Time1                      : %lf sec\n", time1);
    printf("Time2                      : %lf sec\n", time2);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  


    printf("%d\t",  colors);    
    printf("%lf\t", totalTime);
    printf("%lf\t", time1);
    printf("%lf\t", time2);
    printf("%ld\t", nConflicts);  
    printf("%d\n",  nLoops);      
    return _TRUE;
}


// ============================================================================
// based on LB's algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_LB(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double timeMIS=0;    //run time
    double timeRnd=0;    //run time
    double timeReG=0;    //run time
    double timeTot=0;               //run time
    
    const int N = GraphCore::GetVertexCount(); //number of vertex
    
    vector<int> &verPtr=m_vi_Vertices;      //ia of csr
    vector<int> &verInd=m_vi_Edges;         //ja of csr
    
    colors=0;
    vtxColors.resize(N, -1);

    const int qtnt = N/nT;
    //const int rmnd = N%nT;
    
    vector<int> Q;
    Q.resize(N,-1);
    int QTail=0;
    
    vector<vector<int>>  QQ;
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(qtnt+1);
    }

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]=m_vi_OrderedVertices[i];
    }
    QTail = N;

    // generate rando m numbers
timeRnd =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
timeRnd +=omp_get_wtime();
    
    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
timeMIS -= omp_get_wtime();
#pragma omp parallel
        {
            int tid = omp_get_thread_num();
#pragma omp for
        for(int vit=0; vit<QTail; vit++){
            int v = Q[vit];
            double vwt = WeightRnd[v];
            bool bDomain=true;
            for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                int nb = verInd[k];
                double nbwt = WeightRnd[nb];
                if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) 
                vtxColors[v]=colors;
            else
                QQ[tid].push_back(v);
        } //end of omp for
        } //end of omp parallel
        colors++;
timeMIS += omp_get_wtime();
timeReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].resize(0);
        }
        
timeReG += omp_get_wtime();
    }while(QTail!=0);

timeTot = timeMIS+timeReG;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Total Time(without RndTime): %lf sec\n", totalTime);
    printf("Rnd Time               : %lf sec\n", timeRnd);
    printf("Mis Time               : %lf sec\n", timeMIS);
    printf("Reg Time               : %lf sec\n", timeReG);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("%d\t",  colors);    
    printf("%lf\t", timeTot);
    printf("%lf\t", timeRnd);
    printf("%lf\t", timeMIS);
    printf("%lf\t", timeReG);
    return _TRUE;
}

// ============================================================================
// based on Jone Plassmann's JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double timeMIS=0;    //run time
    double timeRnd=0;    //run time
    double timeReG=0;    //run time
    double timeTot=0;               //run time
    int    nLoops = 0;                         //Number of rounds 
    
    const int N = GraphCore::GetVertexCount(); //number of vertex
    
    vector<int> &verPtr=m_vi_Vertices;      //ia of csr
    vector<int> &verInd=m_vi_Edges;         //ja of csr
    
    const int MaxDegreeP1 = m_i_MaximumVertexDegree+1; //maxDegree
    
    colors=0;
    vtxColors.resize(N, -1);

    const int qtnt = N/nT;
    //const int rmnd = N%nT;
    
    vector<int> Q;
    Q.resize(N,-1);
    int QTail=0;
    
    vector<vector<int>>  QQ;
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(qtnt+1);
    }

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]=m_vi_OrderedVertices[i];
    }
    QTail = N;

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    vector<double> WeightDeg;
    WeightDeg.resize(N,-1);

timeRnd =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
timeRnd +=omp_get_wtime();
    
    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
timeMIS -= omp_get_wtime();
#pragma omp parallel
{
    int tid = omp_get_thread_num();
#pragma omp for
        for(int vit=0; vit<QTail; vit++){
            int v = Q[vit];
            double vwt = WeightRnd[v];
            bool bDomain=true;
            for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                int nb = verInd[k];
                double nbwt = WeightRnd[nb];
                if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) {
                vector<int> MASK(MaxDegreeP1,-1);
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
            else
                QQ[tid].push_back(v);
        } //end of omp for
}   //end of omp parallel
timeMIS += omp_get_wtime();
timeReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].resize(0);
        }
        
timeReG += omp_get_wtime();
    nLoops++;
    }while(QTail!=0);

timeTot = timeMIS+timeReG;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Number of loops            : %ld \n",nLoops);  
    printf("Total Time                 : %lf sec\n", timeTot);
    printf("Rnd Time                   : %lf sec\n", timeRnd);
    printf("MISCOLOR Time              : %lf sec\n", timeMIS);
    printf("Reg Time                   : %lf sec\n", timeReG);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("%d\t",  colors);    
    printf("%lf\t", timeTot);
    printf("%lf\t", timeRnd);
    printf("%lf\t", timeMIS);
    printf("%lf\t", timeReG);
    printf("%d\t", nLoops);
    
    return _TRUE;
}

// ============================================================================
// based on Allwright's LF JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_AW_LF(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double timeMIS=0;    //run time
    double timeRnd=0;    //run time
    double timeReG=0;    //run time
    double timeTot=0;               //run time
    int    nLoops = 0;                         //Number of rounds 
    
    const int N = GraphCore::GetVertexCount(); //number of vertex
    
    vector<int> &verPtr=m_vi_Vertices;      //ia of csr
    vector<int> &verInd=m_vi_Edges;         //ja of csr
    
    const int MaxDegreeP1 = m_i_MaximumVertexDegree+1; //maxDegree
    
    colors=0;
    vtxColors.resize(N, -1);

    const int qtnt = N/nT;
    //const int rmnd = N%nT;
    
    vector<int> Q;
    Q.resize(N,-1);
    int QTail=0;
    
    vector<vector<int>>  QQ;
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(qtnt+1);
    }

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]=m_vi_OrderedVertices[i];
    }
    QTail = N;

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    vector<double> WeightDeg;
    WeightDeg.resize(N,-1);

timeRnd =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
    for(auto v: Q)
        WeightDeg[v] = verPtr[v+1]-verPtr[v];
timeRnd +=omp_get_wtime();
    
    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
timeMIS -= omp_get_wtime();
#pragma omp parallel
{
    int tid = omp_get_thread_num();
#pragma omp for
        for(int vit=0; vit<QTail; vit++){
            int v = Q[vit];
            double vwt = WeightDeg[v];
            bool bDomain=true;
            for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                int nb = verInd[k];
                double nbwt = WeightDeg[nb];
                if(vwt<nbwt || (vwt==nbwt && WeightRnd[v]>WeightRnd[nb])){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) {
                vector<int> MASK(MaxDegreeP1,-1);
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
            else
                QQ[tid].push_back(v);
        } //end of omp for
}
timeMIS += omp_get_wtime();
timeReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].resize(0);
        }
        
timeReG += omp_get_wtime();
    nLoops++;
    }while(QTail!=0);

timeTot = timeMIS+ timeReG;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Number of loops            : %ld \n",nLoops);  
    printf("Total Time                 : %lf sec\n", timeTot);
    printf("Rnd Time                   : %lf sec\n", timeRnd);
    printf("MISCOLOR Time              : %lf sec\n", timeMIS);
    printf("Reg Time                   : %lf sec\n", timeReG);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("%d\t",  colors);    
    printf("%lf\t", timeTot);
    printf("%lf\t", timeRnd);
    printf("%lf\t", timeMIS);
    printf("%lf\t", timeReG);
    
    return _TRUE;
}

// ============================================================================
// based on Allwright's SL JP algorithm [3]
// ============================================================================
int SMPGCInterface::D1_OMP_JP_AW_SL(int nT, int&colors, vector<int>&vtxColors, const string&loc_ord) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);
    
    double timeMIS=0;    //run time
    double timeRnd=0;    //run time
    double timeReG=0;    //run time
    double timeTot=0;               //run time
    int    nLoops = 0;                         //Number of rounds 
    
    const int N = GraphCore::GetVertexCount(); //number of vertex
    
    vector<int> &verPtr=m_vi_Vertices;      //ia of csr
    vector<int> &verInd=m_vi_Edges;         //ja of csr
    
    const int MaxDegreeP1 = m_i_MaximumVertexDegree+1; //maxDegree
    
    colors=0;
    vtxColors.resize(N, -1);

    const int qtnt = N/nT;
    //const int rmnd = N%nT;
    
    vector<int> Q;
    Q.resize(N,-1);
    int QTail=0;
    
    vector<vector<int>>  QQ;
    #pragma omp parallel for
    for(int i=0;i<nT; i++){
        QQ[i].reserve(qtnt+1);
    }

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        Q[i]=m_vi_OrderedVertices[i];
    }
    QTail = N;

    // weight 
    vector<double> WeightRnd;
    WeightRnd.resize(N,-1);
    vector<double> WeightDeg;
    WeightDeg.resize(N,-1);

timeRnd =-omp_get_wtime();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0; i<N; i++)
        WeightRnd[i] = distribution(generator);
    for(auto v: Q)
        WeightDeg[v] = verPtr[v+1]-verPtr[v];
timeRnd +=omp_get_wtime();
    
    colors=0;
    do{
        //phase 0: find maximal indenpenent set, and color it
timeMIS -= omp_get_wtime();
#pragma omp parallel 
    { 
        int tid=omp_get_thread_num();
#pragma omp for 
        for(int vit=0; vit<QTail; vit++){
            int v = Q[vit];
            double vwt = WeightRnd[v];
            bool bDomain=true;
            for(int k=verPtr[v],kEnd=verPtr[v+1]; k!=kEnd; k++){
                int nb = verInd[k];
                double nbwt = WeightRnd[nb];
                if(vwt<nbwt || (vwt==nbwt && v>nb)){
                    bDomain=false;
                    break;
                }
            }
            if(bDomain) {
                vector<int> MASK(MaxDegreeP1,-1);
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
            else
                QQ[tid].push_back(v);
        } //end of omp for
    }//end of omp paralle
timeMIS += omp_get_wtime();
timeReG -= omp_get_wtime();
        //phase 2: rebuild the graph
        QTail=0;
        for(int tid=0; tid<nT; tid++){
            for(auto v : QQ[tid])
                Q[QTail++]=v;
            QQ[tid].resize(0);
        }
        
timeReG += omp_get_wtime();
    nLoops++;
    }while(QTail!=0);

timeTot = timeMIS+timeReG;
#ifdef PRINT_DETAILED_STATS_
    printf("***********************************************\n");
    printf("Total number of threads    : %d \n", nT);    
    printf("Total number of colors used: %d \n", colors);    
    printf("Number of conflicts overall: %ld \n",nConflicts);  
    printf("Total Time                 : %lf sec\n", totalTime);
    printf("Rnd Time                   : %lf sec\n", timeRnd);
    printf("MISCOLOR Time              : %lf sec\n", timeMIS);
    printf("Reg Time                   : %lf sec\n", timeReG);
    if( do_verify_colors(colors, vtxColors)) 
        printf("Verified, correct.\n");
    else 
        printf("Verified, fail.\n");
    printf("***********************************************\n");

#endif  

    printf("%d\t",  colors);    
    printf("%lf\t", timeTot);
    printf("%lf\t", timeRnd);
    printf("%lf\t", timeMIS);
    printf("%lf\t", timeReG);
    
    return _TRUE;
}



