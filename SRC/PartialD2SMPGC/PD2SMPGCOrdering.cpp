/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "PD2SMPGCOrdering.h"
#include <ctime>  //clock
using namespace std;
using namespace ColPack;


// ============================================================================
// Construction
// ============================================================================
PD2SMPGCOrdering::PD2SMPGCOrdering(const string& graph_name, const string& fmt, double*iotime, const int side, const string& order, double* ordtime) 
: m_global_ordered_method(order), m_global_ordered_side(side), m_mt(5489u) {
    if( fmt!=FORMAT_MM) {
        printf("Error!PD2SMPGCOrdering only support MatrixMarket format (mm). \"%s\" with fmt \"%s\" is under construction... \n", graph_name.c_str(), fmt.c_str());
        exit(1);
    }
    if(iotime) *(time_t*) iotime=-clock();
    ReadMMBipartiteGraphCpp11(graph_name);
    if(iotime) { *(time_t*)iotime+=clock(); *iotime= (*(time_t*)iotime)*1.0/CLOCKS_PER_SEC; }
    global_ordering(side, order, ordtime);
}

PD2SMPGCOrdering::~PD2SMPGCOrdering(){}


// ============================================================================
// 
// ============================================================================
void PD2SMPGCOrdering::global_ordering(const int side, const string& order, double * ordtime){
    if(ordtime) *(time_t*)ordtime=-clock();
    if(order == ORDER_STR_NATURAL)
        global_natural_ordering(side);
    else if(order == ORDER_STR_RANDOM) 
        global_random_ordering(side);
    else{
        fprintf(stderr, "Err! PD2SMPGCOrdering::Unknow order %s\n",order.c_str());
        exit(1);
    }
    if(ordtime){ *(time_t*)ordtime+=clock(); *ordtime =(double)(*(time_t*)ordtime)/CLOCKS_PER_SEC; }
}

// ============================================================================
// Natural is 0 1 2 3 4 5 6 7 ...
// ============================================================================
void PD2SMPGCOrdering::global_natural_ordering(const int side){
    m_global_ordered_side   = side;
    m_global_ordered_method = ORDER_NATURAL; 
    const int N      = (side==L)?GetLeftVertexCount():GetRightVertexCount();
    m_global_ordered_vertex.resize(N);
    for(int i=0; i<N; i++) m_global_ordered_vertex[i]=i;
}

// ============================================================================
// Random is shuffle to natural
// ============================================================================
void PD2SMPGCOrdering::global_random_ordering(const int side) {
    m_global_ordered_side  = side;
    m_global_ordered_method= ORDER_RANDOM;
    const int N     = (side==L)?GetLeftVertexCount():GetRightVertexCount();
    m_global_ordered_vertex.resize(N);
    for(int i=0; i<N; i++) m_global_ordered_vertex[i]=i;
    if(N<=1) return;
    for(int i=0; i<N-1; i++){
        uniform_int_distribution<int> dist(i, N-1);
        swap(m_global_ordered_vertex[i], m_global_ordered_vertex[dist(m_mt)]);
    }
}

// ============================================================================
// local Natural is just sort ...
// ============================================================================
void PD2SMPGCOrdering::local_natural_ordering(vector<int>&vtxs){
    sort(vtxs.begin(), vtxs.end());
}

// ============================================================================
// Random is shuffle to natural
// ============================================================================
void PD2SMPGCOrdering::local_random_ordering(vector<int>&vtxs) {
    sort(vtxs.begin(), vtxs.end());
    const int N=vtxs.size();
    if(N<=1) return;
    for(int i=0; i<N-1; i++){
        uniform_int_distribution<int> dist(i, N-1); 
        swap(vtxs[i], vtxs[dist(m_mt)]);
    }
}

// ============================================================================
// Largest Degree First
// ============================================================================
void PD2SMPGCOrdering::local_largest_degree_first_ordering(const int side, vector<int>& vtxs){
    const int MaxDegreeP1     = (side==L)?(GetMaximumLeftVertexDegree()+1):(GetMaximumRightVertexDegree()+1); //maxDegree
    const vector<int>& verPtr = (side==L)?(GetLeftVertices()):(GetRightVertices());

    vector<vector<int>> GroupedVertexDegree(MaxDegreeP1);
    for(const auto v : vtxs) {
        int deg = verPtr[v+1]-verPtr[v];
        GroupedVertexDegree[deg].push_back(v);
    }
    vtxs.clear();
    for(int d=MaxDegreeP1-1, it=MaxDegreeP1; it!=0; it--, d--){
        vtxs.insert(vtxs.end(), GroupedVertexDegree[d].begin(), GroupedVertexDegree[d].end());
    }
    GroupedVertexDegree.clear();
}


// ============================================================================
// Largest Degree First
// ============================================================================
void PD2SMPGCOrdering::local_largest_degree_first_ordering(const int side, vector<int>& vtxs, const int beg, const int end){
    const int MaxDegreeP1 = (side==L)?(GetMaximumLeftVertexDegree()+1):(GetMaximumRightVertexDegree()+1); //maxDegree
    const vector<int>& verPtr = (side==L)?(GetLeftVertices()):(GetRightVertices());

    vector<vector<int>> GroupedVertexDegree(MaxDegreeP1);
    
    for(auto i=beg; i<end; i++){
        auto v  = vtxs[i];
        int deg = verPtr[v+1]-verPtr[v];
        GroupedVertexDegree[deg].push_back(v);
    }
    int pos=beg;
    for(int d=MaxDegreeP1-1, it=MaxDegreeP1; it!=0; it--, d--){
        for(auto v : GroupedVertexDegree[d]){
            vtxs[pos++]=v;
        }
    }
    GroupedVertexDegree.clear();
}






