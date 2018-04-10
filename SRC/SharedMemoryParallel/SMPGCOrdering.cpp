/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "SMPGCOrdering.h"
#include <time.h>  //clock
using namespace std;
using namespace ColPack;


// ============================================================================
// Construction
// ============================================================================
SMPGCOrdering::SMPGCOrdering(const string& graph_name, const string& fmt, double*iotime,  const string& order="NATURAL", double* ordtime=nullptr) 
: SMPGCCore(graph_name, fmt, iotime) {
    int N = SMPGCCore::nodes();
    ordered_vertex_.assign(N,0);
    if(ordtime) *(time_t*)ordtime=-clock();

    if(order == "NATURAL") 
        NaturalOrdering(ordered_vertex_, N);
    else if(order == "RANDOM") 
        RandomOrdering( ordered_vertex_, N);
    else{
        stringstream ss;
        ss<<"Unknown order\""<<order<<"\"";
        error(ss.str());
    }

    if(ordtime){ *(time_t*)ordtime+=clock(); *ordtime =(double)(*(time_t*)ordtime)/CLOCKS_PER_SEC; }
}


SMPGCOrdering::~SMPGCOrdering(){}
void SMPGCOrdering::dump(){}

// ============================================================================
// Natural is 0 1 2 3 4 5 6 7 ...
// ============================================================================
void SMPGCOrdering::NaturalOrdering(vector<INT>&ordering,INT N=-1){
    N=(N==-1)?ordering.size():N;
    for(INT i=0; i<N; i++) ordering[i]=i;
}

// ============================================================================
// Random is shuffle to natural
// ============================================================================
void SMPGCOrdering::RandomOrdering (vector<INT>&ordering, INT N=-1) {
    N=(N==-1)?ordering.size():N;
    srand(time(NULL));
    for(INT i=0; i<N; i++) ordering[i]=i;
    int rnd_num;
    for(INT i=0; i<N; i++){
        rnd_num = (int)(((double)rand() / RAND_MAX) * (N-1 - i)) + i;
        swap(ordering[i],ordering[rnd_num]);
    }
}

// ============================================================================
// Largest Degree First
// ============================================================================
void SMPGCOrdering::LargestDegreeFirstOrdering(vector<INT>& vtxs, INT N){
    vtxs.clear();
    vtxs.reserve(N);
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    INT const * const verInd  = CSRjaPtr();      //ja of csr
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree

    vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
    for(INT v=0; v<N; v++){
        INT deg = verPtr[v+1]-verPtr[v];
        GroupedVertexDegree[deg].push_back(v);
    }

    for(INT d=MaxDegreeP1-1,it=MaxDegreeP1; it!=0; it--,d--){
        vtxs.insert(vtxs.end(), GroupedVertexDegree[d].begin(), GroupedVertexDegree[d].end());
    }
    GroupedVertexDegree.clear();
}


// ============================================================================
// Smallest Degree Last
// ============================================================================
void SMPGCOrdering::SmallestDegreeLastOrdering(vector<INT>& vtxs, INT N){

}

// ==
//
// ==
void SMPGCOrdering::DynamicLargestDegreeFirstOrdering(vector<INT>& vtxs, INT N){

}



