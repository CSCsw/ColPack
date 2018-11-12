/******************************************************************************
    This file is part of ColPack, which is under its License protection.
    You should have received a copy of the License. If not, see 
    <https://github.com/CSCsw/ColPack>
*******************************************************************************/
#ifndef PD2SMPGCORDERING_H
#define PD2SMPGCORDERING_H
#include <vector>
#include <iostream>
#include "PD2SMPGC.h"
#include <random>
#include <algorithm>

using namespace std;

namespace ColPack {

// ============================================================================
// Partial Distance Two Shared Memory Parallel Greedy/Graph Coloring Ordering wrap
// ============================================================================
class PD2SMPGCOrdering : public PD2SMPGC {
public: // construction
    PD2SMPGCOrdering(const string& file_name, const string& fmt, double*iotime, const int side, const string& order, double *ordtime);
    virtual ~PD2SMPGCOrdering();

public: // deplete construction
    PD2SMPGCOrdering(PD2SMPGCOrdering&&)=delete;
    PD2SMPGCOrdering(const PD2SMPGCOrdering&)=delete;
    PD2SMPGCOrdering& operator=(PD2SMPGCOrdering&&)=delete;
    PD2SMPGCOrdering& operator=(const PD2SMPGCOrdering&)=delete;

public: // user api 
    void global_ordering(const int side, const string& order, double*tim);

protected: // developer api
    void global_natural_ordering(const int side);
    void global_random_ordering(const int side);
    void global_largest_degree_first_ordering(const int side);

    void local_natural_ordering(vector<int>&vtxs);
    void local_random_ordering(vector<int>&vtxs);
    void local_largest_degree_first_ordering(const int side, vector<int>& vtxs); 
    void local_largest_degree_first_ordering(const int side, vector<int>& vtxs, const int beg, const int end);  
    void set_rseed(const int x){ m_mt.seed(x); }
   
    int            global_ordered_side()                 const { return m_global_ordered_side; }
    const string&  global_ordered_method()               const { return m_global_ordered_method; }
    const vector<int>&   global_ordered_vertex()               const { return m_global_ordered_vertex; }
    const vector<int>&   global_ordered_vertex(const int side) const { if(side==m_global_ordered_side) return m_global_ordered_vertex; else { printf("Error! Trying to get wrong side ordered vertex\n"); exit(1);  return m_global_ordered_vertex; } }

protected: // members
    vector<int> m_global_ordered_vertex;
    string      m_global_ordered_method;
    int         m_global_ordered_side;
    mt19937 m_mt;
};




}// endof namespace ColPack
#endif

