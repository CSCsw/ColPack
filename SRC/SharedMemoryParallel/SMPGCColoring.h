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
    int Coloring(int nT, const string& method);
    int Coloring(int nT, const string& method, const string &s_option, const int switch_iter);
    
    int get_num_colors(){ return m_total_num_colors; } 
    const vector<int>& get_vertex_colors() const { return m_vertex_color; }
    void get_vertex_colors(vector<int>& x) { x.assign(m_vertex_color.begin(), m_vertex_color.end()); }
    
    // Algorithms 
    int D1_serial(int &color, vector<int>&vtxColors);

    int D1_OMP_GM3P(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_GMMP(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_JP(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_JP2S(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_LB(int nT, int&color, vector<int>&vtxColors);

    int D1_OMP_GM3P_LO(int nT, int&color, vector<int>&vtxColors, const int local_ordering);
    int D1_OMP_GMMP_LO(int nT, int&color, vector<int>&vtxColors, const int local_ordering);
    int D1_OMP_JP_LO  (int nT, int&color, vector<int>&vtxColors,   const int local_ordering);
    
    int D1_OMP_JP_hyber(int nT, int&color, vector<int>&vtxColors, const int option, const int swtich_iter);
    int D1_OMP_JP2S_hyber(int nT, int&color, vector<int>&vtxColors, const int option, const int switch_iter);

    // Algorithm for distance two coloring
    int D2_serial(int &color, vector<int>&vtxColors);

    int D2_OMP_GM3P(int nT, int&color, vector<int>&vtxColors);
    int D2_OMP_GMMP(int nT, int&color, vector<int>&vtxColors);
    int D2_OMP_GM3P_LO(int nT, int&color, vector<int>&vtxColors, const int local_ordering);
    int D2_OMP_GMMP_LO(int nT, int&color, vector<int>&vtxColors, const int local_ordering);
    
    // Some experimental funtions
    int D1_OMP_GMMP_LO_once(int nT, int&color, vector<int>&vtxColors, const int local_ordering);

    int D1_OMP_JP2S_noRepartition(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_JP2Shash(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_JP2Shashsingle(int nT, int&color, vector<int>&vtxColors);
    int D1_OMP_JP2S_hyber_slow(int nT, int&color, vector<int>&vtxColors, const int option, const int switch_iter);
    int D2_OMP_GM3P_(int nT, int&color, vector<int>&vtxColors);
    int D2_OMP_GMMP_(int nT, int&color, vector<int>&vtxColors);
    int D2_OMP_GM3P_LO_(int nT, int&color, vector<int>&vtxColors, const string& local_ordering);
    int D2_OMP_GMMP_LO_(int nT, int&color, vector<int>&vtxColors, const string& local_ordering);


private:
    inline void hyberJP_implement_GM3P(vector<int>&Q, const int &QTail, vector<vector<int>>&QQ, const int&nT, int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    inline void hyberJP_implement_GMMP(vector<int>&Q, const int &QTail, vector<int>&conflictQ,                int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    void hyberJP_implement_greedy_serial(vector<int>&Q, const int &QTail,                              int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 
    inline void hyberJP_implement_stream(vector<int>&Q, const int &QTail, vector<vector<int>>&QQ, const int&nT, int const * const &verPtr, int const * const &verInd, int& colors, vector<int>&vtxColors); 


public: // Utilites
    int cnt_d1conflict(const vector<int>& vc);
    int cnt_d2conflict(const vector<int>& vc);

protected:
    int         m_total_num_colors;
    vector<int> m_vertex_color;
    string      m_method;

}; // end of class SMPGCColoring


}// endof namespace ColPack
#endif

