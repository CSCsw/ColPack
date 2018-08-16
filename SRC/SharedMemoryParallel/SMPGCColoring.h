/*************************************************************************
    File Name: SMPGCColoring.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCColoring_H
#define SMPGCColoring_H
#include <vector>
#include <omp.h>
#include "ColPackHeaders.h" //#include "GraphOrdering.h"
#include "SMPGCOrdering.h"

using namespace std;

namespace ColPack {

//=============================================================================
// Shared Memeory Parallel (Greedy)/Graph Coloring -> SMPGC
// ----------------------------------------------------------------------------
// 
// SMPGC includes three main algorithms
// * GM's Algorithm: Gebremedhin and Manne[1].
// * IP's Algorithm: Catalyurek Feo Gebremedhin and Halappanavar[2]
// * JP's Algorithm: Jones and Plassmann[3]
// * Luby's Alg
// * JP-LF
// * JP_SL
// ----------------------------------------------------------------------------
// [1] Scalable Parallel Graph Coloring Algorithms
// [2] Grah coloring algorithms for multi-core and massively multithreaded architectures
// [3] A Parallel Graph Coloring Heuristic
//=============================================================================
    

// ============================================================================
// Shared Memeory Parallel Greedy/Graph Coloring
// ============================================================================
class SMPGCColoring : public SMPGCOrdering {
public: // Constructions
    SMPGCColoring(const string& graph_name);
    SMPGCColoring(const string& graph_name, const string& fmt, double*iotime, const string&ord, double*ordtime);
    virtual ~SMPGCColoring(){}

        // Deplete constructions
        SMPGCColoring(SMPGCColoring&&)=delete;
        SMPGCColoring(const SMPGCColoring&)=delete;
        SMPGCColoring& operator=(SMPGCColoring&&)=delete;
        SMPGCColoring& operator=(const SMPGCColoring&)=delete;

public: // API
    int Coloring(int nT, const string& method, const int switch_iter);
    
    int get_num_colors(){ return m_total_num_colors; } 
    const vector<int>& get_vertex_colors() const { return m_vertex_color; }
    void get_vertex_colors(vector<int>& x) { x.assign(m_vertex_color.begin(), m_vertex_color.end()); }
    
    // Algorithms 
    int D1_serial(int &color, vector<int>&vtxColors, const int local_order);

    int D1_OMP_GM3P(int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D1_OMP_GMMP(int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D1_OMP_LB  (int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D1_OMP_JP  (int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D1_OMP_MTJP(int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D1_OMP_HBJP  (int nT, int&color, vector<int>&vtxColors, const int local_order, const int option, const int swtich_iter);
    int D1_OMP_HBMTJP(int nT, int&color, vector<int>&vtxColors, const int local_order, const int option, const int switch_iter);

    // Algorithm for distance two coloring
    int D2_serial(int &color, vector<int>&vtxColors, const int local_order=ORDER_NONE);

    int D2_OMP_GM3P   (int nT, int&color, vector<int>&vtxColors, const int local_order);
    int D2_OMP_GMMP   (int nT, int&color, vector<int>&vtxColors, const int local_order);
    
    // inner Algorithm for Hybird 
    inline void hybrid_GM3P(const int nT, vector<int>&vtxColors, vector<vector<int>>&Q, const int local_order); 
    inline void hybrid_GMMP(const int nT, vector<int>&vtxColors, vector<vector<int>>&Q, const int local_order); 
    inline void hybrid_Serial(vector<int>&vtxColors, vector<vector<int>>&Q, const int local_order); 
    
    // Some experimental funtions
    //int D1_OMP_GMMP_LO_once(int nT, int&color, vector<int>&vtxColors, const int local_ordering);
    //int D1_OMP_JP2S_noRepartition(int nT, int&color, vector<int>&vtxColors);
    //int D1_OMP_JP2Shash(int nT, int&color, vector<int>&vtxColors);
    //int D1_OMP_JP2Shashsingle(int nT, int&color, vector<int>&vtxColors);
    //int D1_OMP_JP2S_hybrid_slow(int nT, int&color, vector<int>&vtxColors, const int option, const int switch_iter);
    //int D2_OMP_GM3P_(int nT, int&color, vector<int>&vtxColors);
    //int D2_OMP_GMMP_(int nT, int&color, vector<int>&vtxColors);
    //int D2_OMP_GM3P_LO_(int nT, int&color, vector<int>&vtxColors, const string& local_ordering);
    //int D2_OMP_GMMP_LO_(int nT, int&color, vector<int>&vtxColors, const string& local_ordering);
//private:
    //inline void hybridJP_implement_GM3P(vector<int>&Q, const int &QTail, vector<vector<int>>&QQ, const int&nT, int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    //inline void hybridJP_implement_GMMP(vector<int>&Q, const int &QTail, vector<int>&conflictQ,                int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    //inline void hybridJP_implement_greedy_serial(vector<int>&Q, const int &QTail,                              int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    //inline void hybridJP_implement_stream(vector<int>&Q, const int &QTail, vector<vector<int>>&QQ, const int&nT, int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 


public: // Utilites
    int cnt_d1conflict(const vector<int>& vc, bool bVerbose);
    int cnt_d2conflict(const vector<int>& vc, bool bVerbose);

private:

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




protected:
    int         m_total_num_colors;
    vector<int> m_vertex_color;
    string      m_method;

}; // end of class SMPGCColoring


}// endof namespace ColPack
#endif

