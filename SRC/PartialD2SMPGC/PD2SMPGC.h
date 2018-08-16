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
    static const int  ORDER_NONE    = 0;
    static const int  ORDER_NATURAL = 1;
    static const int  ORDER_RANDOM  = 2;
    static const int  ORDER_LARGEST_FIRST = 3;
    static const int  ORDER_SMALLEST_LAST = 4;

public:
    static const string FORMAT_MM; //="MM";

    static const string ORDER_STR_NATURAL;// = "NATURAL";
    static const string ORDER_STR_RANDOM ;// = "RANDOM";
    static const string ORDER_STR_LARGEST_FIRST;// = "LARGEST_FIRST";
    static const string ORDER_STR_SMALLEST_LAST;// = "SMALLEST_LAST";
public:
    static const int _TRUE = 1;

public:
    PD2SMPGC(){}
    virtual ~PD2SMPGC(){};
};


}// endof namespace ColPack
#endif

