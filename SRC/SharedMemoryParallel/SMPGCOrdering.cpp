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





