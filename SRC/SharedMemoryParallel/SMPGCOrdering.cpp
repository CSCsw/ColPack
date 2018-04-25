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
: SMPGCCore(graph_name, fmt, iotime), mt_(5489u) {
    int N = SMPGCCore::nodes();
    ordered_vertex_.assign(N,0);
    set_global_ordering(order, ordtime);
}


SMPGCOrdering::~SMPGCOrdering(){}
void SMPGCOrdering::dump(){}



// ============================================================================
// 
// ============================================================================
void SMPGCOrdering::set_global_ordering(const string& order="NATURAL", double * ordtime=nullptr){
    if(ordtime) *(time_t*)ordtime=-clock();

    if(order == "NATURAL") 
        global_natural_ordering();
    else if(order == "RANDOM") 
        global_random_ordering ();
    else{
        fprintf(stderr, "Err! SMPGCOrdering::Unknow order %s\n",order.c_str());
        exit(1);
    }
    if(ordtime){ *(time_t*)ordtime+=clock(); *ordtime =(double)(*(time_t*)ordtime)/CLOCKS_PER_SEC; }
}


// ============================================================================
// 
// ============================================================================
void SMPGCOrdering::set_local_ordering(vector<INT>& vtxs, const string& order, double * ordtime=nullptr){
    if(ordtime) *(time_t*)ordtime=-clock();

    if(order == "NATURAL") 
        local_natural_ordering(vtxs);
    else if(order == "RANDOM") 
        local_random_ordering (vtxs);
    else if(order == "LARGEST_FIRST")
        local_largest_degree_first_ordering(vtxs);
    else if(order == "SMALLEST_LAST")
        local_smallest_degree_last_ordering(vtxs);
    else{
        fprintf(stderr, "Err! SMPGCOrdering::Unknow local order %s\n",order.c_str());
        exit(1);
    }
    if(ordtime){ *(time_t*)ordtime+=clock(); *ordtime =(double)(*(time_t*)ordtime)/CLOCKS_PER_SEC; }
}





// ============================================================================
// Natural is 0 1 2 3 4 5 6 7 ...
// ============================================================================
void SMPGCOrdering::global_natural_ordering(){
    INT N = SMPGCCore::nodes();
    ordered_vertex_.resize(N);
    for(INT i=0; i<N; i++) ordered_vertex_[i]=i;
}

// ============================================================================
// Random is shuffle to natural
// ============================================================================
void SMPGCOrdering::global_random_ordering () {
    const INT N = SMPGCCore::nodes();
    ordered_vertex_.resize(N);
    for(INT i=0; i<N; i++) ordered_vertex_[i]=i;
    if(N<1) return;
    for(INT i=0; i<N-1; i++){
        uniform_int_distribution<INT> dist(i, N-1); 
        swap(ordered_vertex_[i], ordered_vertex_[dist(mt_)]);
    }
}


// ============================================================================
// local Natural is just sort ...
// ============================================================================
void SMPGCOrdering::local_natural_ordering(vector<INT>&vtxs){
    sort(vtxs.begin(), vtxs.end());
}

// ============================================================================
// Random is shuffle to natural
// ============================================================================
void SMPGCOrdering::local_random_ordering (vector<INT>&vtxs) {
    sort(vtxs.begin(), vtxs.end());
    INT N=vtxs.size();
    if(N<1) return;
    for(INT i=0; i<N-1; i++){
        uniform_int_distribution<INT> dist(i, N-1); 
        swap(vtxs[i], vtxs[dist(mt_)]);
    }
}




// ============================================================================
// Largest Degree First
// ============================================================================
void SMPGCOrdering::local_largest_degree_first_ordering(vector<INT>& vtxs){
    INT const * const verPtr  = CSRiaPtr();      //ia of csr
    //INT const * const verInd  = CSRjaPtr();      //ja of csr
    const INT MaxDegreeP1 = maxDeg()+1; //maxDegree

    vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
    
    for(auto v: vtxs){ 
        INT deg = verPtr[v+1]-verPtr[v];
        GroupedVertexDegree[deg].push_back(v);
    }
    
    vtxs.clear();
    for(INT d=MaxDegreeP1-1,it=MaxDegreeP1; it!=0; it--,d--){
        vtxs.insert(vtxs.end(), GroupedVertexDegree[d].begin(), GroupedVertexDegree[d].end());
    }
    GroupedVertexDegree.clear();
}


// ============================================================================
// Smallest Degree Last 
// ----------------------------------------------------------------------------
// There are many varivations
//  * the smallest degree vertices are picked 
//    A.  one by one
//    B.  whole as a batch
//  * the smallest degree is 
//    1.  calculated accurately
//    2.  considering only increasing for each iteration
// ----------------------------------------------------------------------------
// In term of accurate, A>B, 1>2;
// In term of speed,    B>A, 2>1;
// The following implementation is B1.
// ----------------------------------------------------------------------------
// Smallest Degree Last Local
// ----------------------------------------------------------------------------
// local make things complicated, since inter(cross) edge does not update
//  * a. consider the cross edges as lighter weight edges
//  * b. consider the cross edge is the same as inner edge
// The following implementation is b
// ============================================================================
void SMPGCOrdering::local_smallest_degree_last_ordering(vector<INT>& vtxs){
    INT const * const verPtr = CSRiaPtr();
    INT const * const verVal = CSRjaPtr();
    const INT MaxDegreeP1 = maxDeg()+1;
    const INT N = nodes();
    const auto Nloc = vtxs.size();
    vector<INT> Vertex2Degree(N,-1);
    vector<INT> Vertex2Index(N,-1);
    vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
    INT max_deg = 0;
    INT min_deg = MaxDegreeP1-1;
    // set up environment
    for(auto v: vtxs){ 
        INT deg = verPtr[v+1]-verPtr[v];
        Vertex2Degree[v]=deg;
        Vertex2Index [v]=GroupedVertexDegree[deg].size();
        GroupedVertexDegree[deg].push_back(v);
        if(max_deg<deg) max_deg=deg;
        if(min_deg>deg) min_deg=deg;
    }

    vtxs.clear();
    while(vtxs.size()!=Nloc){
        const auto prev_vtxs_size=vtxs.size();
        
        // picked up lowest degree vertices, move to order, remove from graph
        for(; min_deg<=max_deg; min_deg++){
            if(GroupedVertexDegree[min_deg].empty())
                continue;
            vtxs.insert(vtxs.end(), GroupedVertexDegree[min_deg].begin(), GroupedVertexDegree[min_deg].end());
            for(auto v : GroupedVertexDegree[min_deg]){
                Vertex2Degree[v]=-1;
                Vertex2Index [v]=-1;
            }
            break;
        }
        GroupedVertexDegree[min_deg].clear();
        // for all their neighbors decrease degree by one, if it's a inner edge
        for(auto vit=prev_vtxs_size; vit<vtxs.size(); vit++){
            auto v= vtxs[vit];  //selected v
            for(auto wit = verPtr[v], witEnd=verPtr[v+1]; wit<witEnd; wit++) {
                auto w = verVal[wit];
                INT deg = Vertex2Degree[w];
                if(deg<=0){ // <0 means w is not local, or have deleted; =0 should not happe  
                    continue;
                }
                INT degM1 = deg-1;
                if(min_deg > degM1) min_deg=degM1;
                auto tmpv = GroupedVertexDegree[deg][Vertex2Index[w]] = GroupedVertexDegree[deg].back();
                Vertex2Index [tmpv] = Vertex2Index[w];
                GroupedVertexDegree[deg].pop_back();
                Vertex2Degree[w] = degM1;
                Vertex2Index [w] = GroupedVertexDegree[degM1].size();
                GroupedVertexDegree[degM1].push_back(w);
            }//end of for w
        }//end of for v
    
    }//end of while
    return;    
}



// ============================================================================
// Smallest Degree Last 
// ----------------------------------------------------------------------------
// There are many varivations
//  * the smallest degree vertices are picked 
//    A.  one by one
//    B.  whole as a batch
//  * the smallest degree is 
//    1.  calculated accurately
//    2.  considering only increasing for each iteration
// ----------------------------------------------------------------------------
// In term of accurate, A>B, 1>2;
// In term of speed,    B>A, 2>1;
// The following implementation is B1.
// ----------------------------------------------------------------------------
// Smallest Degree Last Local
// ----------------------------------------------------------------------------
// local make things complicated, since inter(cross) edge does not update
//  * a. consider the cross edges as lighter weight edges
//  * b. consider the cross edge is the same as inner edge
// The following implementation is a
// ============================================================================
void SMPGCOrdering::local_smallest_degree_last_ordering_B1a(vector<INT>& vtxs){
    INT const * const verPtr = CSRiaPtr();
    INT const * const verVal = CSRjaPtr();
    const INT MaxDegreeP1 = maxDeg()+1;
    const INT N = nodes();
    const auto Nloc = vtxs.size();
    vector<bool> VertexIsLocal(N, false);
    for(auto v : vtxs) VertexIsLocal[v]=true;

    vector<INT> Vertex2Degree(N,-1);
    vector<INT> Vertex2Index(N,-1);
    vector<vector<INT>> GroupedVertexDegree(MaxDegreeP1);
    INT max_deg = 0;
    INT min_deg = MaxDegreeP1-1;
    // set up environment
    for(auto v: vtxs){ 
        INT deg = verPtr[v+1]-verPtr[v];

        int inter_deg=0;
        for(auto wit = verPtr[v], witEnd=verPtr[v+1]; wit<witEnd; wit++) {
            auto w = verVal[wit];
            if(VertexIsLocal[w])
                inter_deg++;
        }
        
        deg-=inter_deg;

        Vertex2Degree[v]=deg;
        Vertex2Index [v]=GroupedVertexDegree[deg].size();
        GroupedVertexDegree[deg].push_back(v);
        if(max_deg<deg) max_deg=deg;
        if(min_deg>deg) min_deg=deg;
    }

    vtxs.clear();
    while(vtxs.size()!=Nloc){
        const auto prev_vtxs_size=vtxs.size();
        
        // picked up lowest degree vertices, move to order, remove from graph
        for(; min_deg<=max_deg; min_deg++){
            if(GroupedVertexDegree[min_deg].empty())
                continue;
            vtxs.insert(vtxs.end(), GroupedVertexDegree[min_deg].begin(), GroupedVertexDegree[min_deg].end());
            for(auto v : GroupedVertexDegree[min_deg]){
                Vertex2Degree[v]=-1;
                Vertex2Index [v]=-1;
            }
            break;
        }
        GroupedVertexDegree[min_deg].clear();
        // for all their neighbors decrease degree by one, if it's a inner edge
        for(auto vit=prev_vtxs_size; vit<vtxs.size(); vit++){
            auto v= vtxs[vit];  //selected v
            for(auto wit = verPtr[v], witEnd=verPtr[v+1]; wit<witEnd; wit++) {
                auto w = verVal[wit];
                INT deg = Vertex2Degree[w];
                if(deg<=0){ // <0 means w is not local, or have deleted; =0 should not happe  
                    continue;
                }
                INT degM1 = deg-1;
                if(min_deg > degM1) min_deg=degM1;
                auto tmpv = GroupedVertexDegree[deg][Vertex2Index[w]] = GroupedVertexDegree[deg].back();
                Vertex2Index [tmpv] = Vertex2Index[w];
                GroupedVertexDegree[deg].pop_back();
                Vertex2Degree[w] = degM1;
                Vertex2Index [w] = GroupedVertexDegree[degM1].size();
                GroupedVertexDegree[degM1].push_back(w);
            }//end of for w
        }//end of for v
    
    }//end of while
    return;    
}


// ==
//
// ==
//void SMPGCOrdering::DynamicLargestDegreeFirstOrdering(vector<INT>& vtxs, INT N){

//}



