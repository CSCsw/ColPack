/*************************************************************************
    File Name: PD2SMPGC.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef PD2SMPGC_H
#define PD2SMPGC_H
#include <vector>
#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <map>
#include <list>
#include <set>
#include "BipartiteGraphCore.h"
#include "BipartiteGraphInputOutput.h" //#include "ColPackHeaders.h" //#include "GraphOrdering.h"
#include <random>
#include <algorithm>

using namespace std;

namespace ColPack {

//=============================================================================
// Partial Distance Two Shared Memeory Parallel (Greedy)/Graph Coloring -> PD2SMPGC
// ----------------------------------------------------------------------------
// Planed service for Source data structure and file io. Since have been 
// implemented in the BipartiteGrpah* class. So, just inherit from them
// And only thing left is marco define.
//=============================================================================

class PD2SMPGC : public BipartiteGraphInputOutput {//public BipartiteGraphInputOut{
public:
    static const int L = 1;
    static const int R = 2;
    static const int ROW = 1;
    static const int COL = 2;
public:
    const string ORDER_NATURAL = "NATURAL";
     const string ORDER_RANDOM  = "RANDOM";
     const string ORDER_LARGEST_FIRST = "LARGEST_FIRST";
     const string ORDER_SMALLEST_LAST = "SMALLEST_LAST";
public:
    const int _TRUE = 1;

public:
    PD2SMPGC(){}
    virtual ~PD2SMPGC(){};
};


}// endof namespace ColPack
#endif

