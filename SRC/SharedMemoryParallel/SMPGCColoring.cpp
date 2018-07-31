/*************************************************************************
    File Name: SMPGCColoring.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "SMPGCColoring.h"

#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;


// ============================================================================
// Interface
// ============================================================================
int SMPGCColoring::Coloring(int nT, const string& method, const int switch_iter=0){
    if     (method.substr(0,17).compare("DISTANCE_ONE_OMP_")==0) {
        // distance one coloring algorithms
        const string mthd = method.substr(17);
        const auto iter_under_line = mthd.find('_');
        if(iter_under_line==string::npos){
            if     (mthd.compare("GM3P")==0) return D1_OMP_GM3P(nT, m_total_num_colors, m_vertex_color);
            else if(mthd.compare("GMMP")==0) return D1_OMP_GMMP(nT, m_total_num_colors, m_vertex_color);
            else if(mthd.compare("JP")  ==0) return D1_OMP_JP  (nT, m_total_num_colors, m_vertex_color);
            else if(mthd.compare("MTJP")==0) return D1_OMP_MTJP(nT, m_total_num_colors, m_vertex_color, false, SMPGC::NUM_HASH);
            else if(mthd.compare("SERIAL")==0) return D1_serial(m_total_num_colors, m_vertex_color);
            else { printf("Error! method \"%s\" with \"%s\" is not supported.\n", method.c_str(), mthd.c_str()); exit(1); }
        }
        else{
            // local ordered algs or hybird algs,
            const string md = mthd.substr(0, iter_under_line+1);
            if(md.compare("GM3PLO_")==0) {
                const string s_ord = mthd.substr(7);
                if     (s_ord.compare("LF")==0) return D1_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_LARGEST_FIRST);
                else if(s_ord.compare("SL")==0) return D1_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_SMALLEST_LAST);
                else if(s_ord.compare("NT")==0) return D1_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_NATURAL);
                else if(s_ord.compare("RD")==0) return D1_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_RANDOM);
                else { printf("Error! method \"%s\" with \"%s\" is not supported.\n",method.c_str(), s_ord.c_str()); exit(1); }
            }
            else if(md.compare("GMMPLO_")==0) {
                const string s_ord = mthd.substr(7);
                if     (s_ord.compare("LF")==0) return D1_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_LARGEST_FIRST);
                else if(s_ord.compare("SL")==0) return D1_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_SMALLEST_LAST);
                else if(s_ord.compare("NT")==0) return D1_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_NATURAL);
                else if(s_ord.compare("RD")==0) return D1_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_RANDOM);
                else { printf("Error! method \"%s\" with \"%s\" is not supported.\n",method.c_str(), s_ord.c_str()); exit(1); }
            }
            else if(md.compare("JPLO_")==0) {
                const string s_ord = mthd.substr(5);
                if     (s_ord.compare("LF")==0) return D1_OMP_JP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_LARGEST_FIRST);
                else if(s_ord.compare("SL")==0) return D1_OMP_JP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_SMALLEST_LAST);
                else if(s_ord.compare("NT")==0) return D1_OMP_JP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_NATURAL);
                else if(s_ord.compare("RD")==0) return D1_OMP_JP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_RANDOM);
                else { printf("Error! method \"%s\" with \"%s\" is not supported.\n",method.c_str(), s_ord.c_str()); exit(1); }
            }
            else if(md.compare("HYBIRD_")==0) {
                const string m = mthod.substr(7);
                const auto   under_line_pos = m.find('_');
                const string alg1 = rest.substr(0, under_line_pos);
                const string alg2 = rest.substr(under_line_pos);
                if(alg1.compare("JP")==0){
                    if     (alg2.compare("_GM3P")==0) return D1_OMP_HYBIRD_JP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_GM3P, switch_iter);
                    else if(alg2.compare("_GMMP")==0) return D1_OMP_HYBIRD_JP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_GMMP, switch_iter);
                    else if(alg2.compare("_SERIAL")==0) return D1_OMP_HYBIRD_JP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_SERIAL, switch_iter);
                    else if(alg2.compare("_STREAM")==0) return D1_OMP_HYBIRD_JP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_STREAM, switch_iter);
                    else { printf("Error! method \"%s\" is not supported.\n", method.c_str()); exit(1);}
                }
                else if(alg1.compare("MTJP")==0){
                    if     (alg2.compare("_GM3P")==0) return D1_OMP_HYBIRD_MTJP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_GM3P, switch_iter);
                    else if(alg2.compare("_GMMP")==0) return D1_OMP_HYBIRD_MTJP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_GMMP, switch_iter);
                    else if(alg2.compare("_SERIAL")==0) return D1_OMP_HYBIRD_MTJP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_SERIAL, switch_iter);
                    else if(alg2.compare("_STREAM")==0) return D1_OMP_HYBIRD_MTJP(nT, m_num_colors, m_vertex_color, SMPGC::HYBIRD_STREAM, switch_iter);
                    else { printf("Error! method \"%s\" is not supported.\n", method.c_str()); exit(1); }
                }
                else { printf("Error! method \"%s\" is not supported.\n", method.c_str()); exit(1); }
            }
            else{
                printf("Error! method \"%s\" is not support.\n",method.c_str()); 
                exit(1);
            }
        }
    }
    else if(method.substr(0,17).compare("DISTANCE_TWO_OMP_")==0) {
        // distance two coloring algorithms
        const string mthd = method.substr(17);
        const auto iter_under_line = mthd.find('_');
        if(iter_under_line==string::npos){
            if     (mthd.compare("GM3P")==0) return D2_OMP_GM3P(nT, m_total_num_colors, m_vertex_color);
            else if(mthd.compare("GMMP")==0) return D2_OMP_GMMP(nT, m_total_num_colors, m_vertex_color);
            else if(mthd.compare("SERIAL")==0) return D2_serial(m_total_num_colors, m_vertex_color);
            else { printf("Error! method \"%s\" is not supported.\n", method.c_str()); exit(1); }
        }
        else{
            if     (mthd.substr(0,7).compare("GM3PLO_")==0) {
                const string s_ord = mthd.substr(7);
                if     (s_ord.compare("LF")==0) return D2_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_LARGEST_FIRST);
                else if(s_ord.compare("SL")==0) return D2_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_SMALLEST_LAST);
                else if(s_ord.compare("NT")==0) return D2_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_NATURAL);
                else if(s_ord.compare("RD")==0) return D2_OMP_GM3P_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_RANDOM);
                else { printf("Error! method \"%s\" with \"%s\" is not supported.\n",method.c_str(), s_ord.c_str()); exit(1); }
            }
            else if(mthd.substr(0,7).compare("GMMPLO_")==0) {
                const string s_ord = mthd.substr(7);
                if     (s_ord.compare("LF")==0) return D2_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_LARGEST_FIRST);
                else if(s_ord.compare("SL")==0) return D2_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_SMALLEST_LAST);
                else if(s_ord.compare("NT")==0) return D2_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_NATURAL);
                else if(s_ord.compare("RD")==0) return D2_OMP_GMMP_LO(nT, m_total_num_colors, m_vertex_color, SMPGC::ORDER_RANDOM);
                else { printf("Error! method \"%s\" with \"%s\" is not supported.\n",method.c_str(), s_ord.c_str()); exit(1); }
            }
            else{
                printf("Error! method \"%s\" with \"%s\" is not support.\n", method.c_str(), mthd.c_str());
                exit(1);
            }
        }
    }
    else{
        printf("Error! method \"%s\" is not supported.\n", method.c_str());
        exit(1);
    }
} //end function

// ============================================================================
// Construction
// ============================================================================
SMPGCColoring::SMPGCInterface(const string& graph_name)
: SMPGCOrdering(graph_name, FORMAT_MM, nullptr, "NATURAL",nullptr) {
    m_vertex_color.reserve(num_nodes());
    m_total_num_colors=0;
}

// ============================================================================
// Construction
// ============================================================================
SMPGCColoring::SMPGCInterface(const string& graph_name, const string& fmt, double* iotime, const string& glb_order="NATURAL", double *ordtime=nullptr) 
: SMPGCOrdering(graph_name, fmt, iotime, glb_order, ordtime){
    m_vertex_color.reserve(num_nodes());
    m_total_num_colors=0;
}


// ============================================================================
// check if the graph is correct colored
// ============================================================================
int SMPGCColoring::cnt_d1conflict(const vector<INT>& vtxColorConst, bool bVerbose=false){
    vector<int>         vtxColor(vtxColorConst);
    const int N         = num_nodes();
    const vector<int>& vtxPtr = get_CSR_ia();
    const vector<int>& vtxVal = get_CSR_ja();
    
    int n_uncolored=0;
    int n_conflicts=0;
    #pragma omp parallel reduction(+:n_conflicts), reduction(+:n_uncolored)
    {
        #pragma omp for
        for(int v=0; v<N; v++) {
            const int vc=vtxColor[v];
            if(vc<0) {
                n_uncolored++;
                continue;
            }
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++) {
                const int w = vtxVal[iw];
                if(v>=w) continue; // only check one side
                if(vc == vtxColor[w] ) {
                    vtxColor[w]=-1; // prevent further conflicts, however, since no synchronize used. May count more conflicts than actual.
                    n_conflicts++;
                    break;  
                }
            }
        }
    }
    if(bVerbose && n_uncolored) printf("There are %d vertex uncolored\nThere are %d vertex has conflicts with other nodes.\n",n_uncolored, n_conflicts);
    return n_uncolored+n_conflicts;
}

// ============================================================================
// check the graph validation
// ----------------------------------------------------------------------------
// uncolored vertex will not conflict with any other vertex
// ============================================================================
int SMPGCColoring::cnt_d2conflict(const vector<int>&vtxColorConst, bool bVerbose) {
    //do it serial
    if(0)
    {
        vector<int> vtxColor(vtxColorConst);
        const int N = num_nodes();
        const vector<int>& vtxPtr = get_CSR_ia();
        const vector<int>& vtxVal = get+CSR_ja();
        
        vector<int>                            uncolored_nodes;
        unordered_map<int, unordered_set<int>> conflicts_nodes;

        for(int v=0; v<N; v++){
            const auto vc = vtxColor[v];
            if(vc<0) { uncolored_nodes.push_back(v); continue; }
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++){  // check d1 neighbors
                const auto w = vtxVal[iw];
                if( vc==vtxColor[w] ) 
                    conflicts_nodes[ min(v,w) ].insert(max(v,w));
            }
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++) { 
                const auto w = vtxVal[iw];
                for(int iu=vtxPtr[w]; iu!=vtxPtr[w+1]; iu++){  // check d2 neighbors
                    const auto u = vtxVal[iu];
                    if(v==u) continue;
                    if( vc == vtxColor[u])  
                        conflicts_nodes[min(v,u)].insert(max(v,u));
                }
            }
        }

        if(bVerbose) {
            printf("There is %d vertex uncolored\nThere is %d vertex conflicts with other nodes.\n", 
                    (int)uncolored_nodes.size(), (int)conflicts_nodes.size());
        }
        if(!uncolored_nodes.empty()){
            printf("uncolored_nodes[%d]: ", (int)uncolored_nodes.size());
            for(int i=0; i<min(uncolored_nodes.size(), 10); i++) 
                printf("\t%d", uncolored_nodes[i]);
            printf("\n");
        }
        if(!conflicts_nodes.empty()){
            printf("conflicts_nodes[%d]:\n", (int)conflicts_nodes.size());
            int cnt_rows=0;
            for(const auto &x : conflicts_nodes){
                if(cnt_rows++>10) { 
                    printf("...");
                    break;
                }
                printf("[%d(%d)]:", x.first, vtxColor[x.first]);
                int cnt_cols=0;
                for(const auto &y : x.second) {
                    if(cnt_cols++>10) {
                        printf("...");
                        break;
                    }
                    printf("\t%d(%d)",y, vtxColor[y]);
                }
                printf("\n");
            }
            printf("\n");
        }
        return uncolored_nodes.size()+conflicts_nodes.size();
    }
    // do it in parallel
    vector<int>  vtxColor( vtxColorConst );
    const int N = num_nodes();
    const vector<int>& vtxPtr = get_CSR_ia();
    const vector<int>& vtxVal = get_CSR_ja();
    int   n_conflicts = 0;
    int   n_uncolored = 0;
    
    #pragma omp parallel reduction(+:n_conflicts), reduction(+: n_uncolored)
    {
        #pragma omp for
        for(int v=0; v<N; v++){
            const auto vc = vtxColor[v];
            if(vc<0) { n_uncolored++; continue; }
            bool b_visbad = false;
            for(int iw=vtxPtr[v]; iw!=vtxPtr[v+1]; iw++){  // check d1 neighbors
                const auto w = vtxVal[iw];
                if( v>=w ) continue;   // only check one side
                if( vc==vtxColor[w] ) { 
                    n_conflicts ++;
                    vtxColor[v]=-1;
                    b_visbad = true;
                    break;
                }
            }
            for(int iw=vtxPtr[v]; b_visbad==false && iw!=vtxPtr[v+1]; iw++) { 
                const auto w = vtxVal[iw];
                for(int iu=vtxPtr[w]; iu!=vtxPtr[w+1]; iu++){  // check d2 neighbors
                    const auto u = vtxVal[iu];
                    if(v >= u) continue;
                    if( vc == vtxColor[u]) {
                        n_conflicts ++;
                        vtxColor[v] =-1;
                        b_visbad=true;
                    }
                }
            }
        }
    }
    if(bVerbose) {
        printf("There is %d uncolored vertices.\nThere is %d vertices conflict with other nodes.\n", (int)n_uncolored, (int)n_conflicts);
    }
    return n_uncolored + n_conflicts;
}



