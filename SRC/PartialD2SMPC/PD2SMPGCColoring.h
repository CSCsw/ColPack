/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef PD2SMPGCColoring_H
#define PD2SMPGCColoring_H
#include <vector>
#include <omp.h>
#include "PD2SMPGCOrdering.h" //#include "GraphOrdering.h"

using namespace std;

namespace ColPack {

// ============================================================================
// Shared Memeory Parallel Greedy/Graph Coloring
// ============================================================================
class PD2SMPGCColoring : public PD2SMPGCOrdering {
public: // Constructions
    PD2SMPGCColoring(const string& graph_name);
    PD2SMPGCColoring(const string& graph_name, const string& fmt, double*iotime, const int side, const string&ord, double*ordtime);
    virtual ~PD2SMPGCColoring(){}

        // Deplete constructions
        PD2SMPGCColoring(PD2SMPGCColoring&&)=delete;
        PD2SMPGCColoring(const PD2SMPGCColoring&)=delete;
        PD2SMPGCColoring& operator=(PD2SMPGCColoring&&)=delete;
        PD2SMPGCColoring& operator=(const PD2SMPGCColoring&)=delete;

public: // API
    int Coloring(const int side, int nT, const string& method);
    
    int get_num_colors() const { return m_total_num_colors; } 
    vector<int>& get_vertex_colors() const { return m_vertex_color; }
    void         get_vertex_colors(vector<int>& x) { x.assign(m_vertex_color.begin(), m_vertex_color.end()); }
    
    // Algorithms 
    int PD2_OMP_GM3P(const int side, int nT, int&color, vector<int>&vtxColors);
    int PD2_OMP_GMMP(const int side, int nT, int&color, vector<int>&vtxColors);

    int PD2_OMP_GM3P_LO(const int side, int nT, int&color, vector<int>&vtxColors, const string& local_ordering);
    int PD2_OMP_GMMP_LO(const int side, int nT, int&color, vector<int>&vtxColors, const string& local_ordering);

    int PD2_serial(const int side, int&color, vector<int>&vtxColors);
public:

public: // Utilites
    int cnt_pd2conflict(const int side, const vector<int>& vc);

protected:
    int m_total_num_colors;
    vector<int> m_vertex_color;
    string m_method;

}; // end of class SMPGCInterface


}// endof namespace ColPack
#endif

