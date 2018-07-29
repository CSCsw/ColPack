/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCInterface_H
#define SMPGCInterface_H
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
class SMPGCInterface : public SMPGCOrdering {
public: // Constructions
    SMPGCInterface(const string& graph_name);
    SMPGCInterface(const string& graph_name, const string& fmt, double*iotime, const string&ord, double*ordtime);
    virtual ~SMPGCInterface(){}

        // Deplete constructions
        SMPGCInterface(SMPGCInterface&&)=delete;
        SMPGCInterface(const SMPGCInterface&)=delete;
        SMPGCInterface& operator=(SMPGCInterface&&)=delete;
        SMPGCInterface& operator=(const SMPGCInterface&)=delete;

public: // API
    virtual void dump();
    int Coloring(int nT, const string& method);
    int Coloring(int nT, const string& method, const string &optionStr, const INT switch_iter);
    
    INT get_num_colors(){ return num_colors_; } 
    vector<INT>& get_vertex_colors() { return vertex_color_; }
    void get_vertex_colors(vector<INT> x) { x.assign(vertex_color_.begin(), vertex_color_.end()); }
    

    
    // Algorithms 
    int D1_OMP_GM3P(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_GMMP(int nT, INT&color, vector<INT>&vtxColors);

    int D1_OMP_LB(int nT, INT&color, vector<INT>&vtxColors);
   
    int D1_OMP_JP(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP2S(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP2S_noRepartition(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP2Shash(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP2Shashsingle(int nT, INT&color, vector<INT>&vtxColors);

    int D1_OMP_GM3P_LO(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);
    int D1_OMP_GMMP_LO_perloop(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);
    int D1_OMP_GMMP_LO_once(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);

    int D1_OMP_JP_LF(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP_SL(int nT, INT&color, vector<INT>&vtxColors);
    
    int D1_OMP_JP_hyber(int nT, INT&color, vector<INT>&vtxColors, const int option, const INT swtich_iter);
    int D1_OMP_JP2S_hyber(int nT, INT&color, vector<INT>&vtxColors, const int option, const INT switch_iter);
    int D1_OMP_JP2S_hyber_slow(int nT, INT&color, vector<INT>&vtxColors, const int option, const INT switch_iter);

    int D1Greedy(int nT, INT&color, vector<INT>&vtxColors);


    int D2_OMP_GM3P(int nT, INT&color, vector<INT>&vtxColors);
    int D2_OMP_GMMP(int nT, INT&color, vector<INT>&vtxColors);
    int D2_OMP_GM3P_LO(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);
    int D2_OMP_GMMP_LO(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);
    
    int D2_OMP_GM3P_(int nT, INT&color, vector<INT>&vtxColors);
    int D2_OMP_GMMP_(int nT, INT&color, vector<INT>&vtxColors);
    int D2_OMP_GM3P_LO_(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);
    int D2_OMP_GMMP_LO_(int nT, INT&color, vector<INT>&vtxColors, const string& local_ordering);

public:


private:
    inline void hyberJP_implement_GM3P(vector<INT>&Q, const INT &QTail, vector<vector<INT>>&QQ, const int&nT, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors); 
    inline void hyberJP_implement_GMMP(vector<INT>&Q, const INT &QTail, vector<INT>&conflictQ,                INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors); 
    void hyberJP_implement_greedy_serial(vector<INT>&Q, const INT &QTail,                              INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors); 
    inline void hyberJP_implement_stream(vector<INT>&Q, const INT &QTail, vector<vector<INT>>&QQ, const int&nT, INT const * const &verPtr, INT const * const &verInd, INT& colors, vector<INT>&vtxColors); 


public: // Utilites
    INT cnt_d1conflict(const vector<INT>& vc);
    INT cnt_d2conflict(const vector<INT>& vc);
protected:
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCInterface with msg \"%s\"\n",s.c_str()); exit(1);}

protected:
    INT num_colors_;
    vector<INT> vertex_color_;
    string method_;

}; // end of class SMPGCInterface


}// endof namespace ColPack
#endif

