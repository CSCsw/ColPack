/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "PD2SMPGCColoring.h"
#include <chrono> //c++11 system time
#include <random> //c++11 random
using namespace std;
using namespace ColPack;


// ============================================================================
// Interface
// ============================================================================
int PD2SMPGCColoring::Coloring(const int side, int nT, const string& method){
    m_method = method;
    if     (method.compare("PD2_OMP_GM3P")==0) return PD2_OMP_GM3P(side, nT, m_total_num_colors, m_vertex_color);
    else if(method.compare("PD2_OMP_GMMP")==0) return PD2_OMP_GMMP(side, nT, m_total_num_colors, m_vertex_color);
    else if(method.compare("PD2_OMP_GM3P_LOLF")==0) return PD2_OMP_GM3P(side, nT, m_total_num_colors, m_vertex_color, ORDER_LARGEST_FIRST);
    else if(method.compare("PD2_OMP_GMMP_LOLF")==0) return PD2_OMP_GMMP(side, nT, m_total_num_colors, m_vertex_color, ORDER_LARGEST_FIRST);
    else if(method.compare("PD2_OMP_GM3P_VBBIT_LOLF")==0) return PD2_OMP_GM3P_VBBIT_EF(side, nT, m_total_num_colors, m_vertex_color, ORDER_LARGEST_FIRST);
    else if(method.compare("PD2_OMP_GMMP_VBBIT_LOLF")==0) return PD2_OMP_GMMP_VBBIT_EF(side, nT, m_total_num_colors, m_vertex_color, ORDER_LARGEST_FIRST);
    else if(method.compare("PD2_OMP_GM3P_VBBIT")==0) return PD2_OMP_GM3P_VBBIT_EF(side, nT, m_total_num_colors, m_vertex_color, ORDER_NONE);
    else if(method.compare("PD2_OMP_GMMP_VBBIT")==0) return PD2_OMP_GMMP_VBBIT_EF(side, nT, m_total_num_colors, m_vertex_color, ORDER_NONE);
    else if(method.compare("PD2_OMP_SERIAL")==0) return PD2_serial(side, m_total_num_colors, m_vertex_color);
    else if(method.compare("PD2_SERIAL")==0) return PD2_serial(side, m_total_num_colors, m_vertex_color);
    
    
    else { fprintf(stdout, "Unknow method %s\n",method.c_str()); exit(1); }   
    return _TRUE;
}


// ============================================================================
// Construction
// ============================================================================
PD2SMPGCColoring::PD2SMPGCColoring(const string& graph_name)
: PD2SMPGCOrdering(graph_name, FORMAT_MM, nullptr, L, ORDER_STR_NATURAL, nullptr), m_total_num_colors(0) {
}

// ============================================================================
// Construction
// ============================================================================
PD2SMPGCColoring::PD2SMPGCColoring(const string& graph_name, const string& fmt, double* iotime, const int side, const string& glb_order, double *ordtime) 
: PD2SMPGCOrdering(graph_name, fmt, iotime, side, glb_order, ordtime), m_total_num_colors(0) {
}

// ============================================================================
// check if the graph is correct colored
// ----------------------------------------------------------------------------
// return number_of_uncolor_vertex + number_of_incorrect_colored_vertex
// ============================================================================
int PD2SMPGCColoring::cnt_pd2conflict(const int side, const vector<int>& vtxColor){
    const int          N             = (side==PD2SMPGC::L)?(GetLeftVertexCount()):(GetRightVertexCount()); 
    const vector<int>& srcPtr        = (side==PD2SMPGC::L)?(GetLeftVertices()   ):(GetRightVertices()   );
    const vector<int>& dstPtr        = (side==PD2SMPGC::L)?(GetRightVertices()  ):(GetLeftVertices()    );
    const vector<int>& vtxVal        = GetEdges();
    const vector<int>& ordered_queue = global_ordered_vertex(side);
    
    if(N!=(signed)vtxColor.size() || N!=(signed)ordered_queue.size()){
        printf("Error! number of vertex in %s side %d is not according to vtxColor size %d or ordered_queue size %d.(graph %s)\n",
                (side==PD2SMPGC::L?"L":"R"), N, (signed)vtxColor.size(), (signed)ordered_queue.size(), m_s_InputFile.c_str());
        exit(1);
    }

    int conflicts=0;
    for (int i=0; i<N; i++){
        const int v = ordered_queue[i];
        const int vc= vtxColor[v];
        if(vc<0) { conflicts++; continue; }
        for(int iw=srcPtr[v]; iw!=srcPtr[v+1]; iw++){
            const int w = vtxVal[iw];
            for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++){
                const int u = vtxVal[iu];
                if(v==u) continue;
                const int uc = vtxColor[u];
                if(v<w && vc==uc)
                    conflicts++;
            }
        }
    }
    return conflicts;
}

// ============================================================================
// get the low bound of number of coloring
// ----------------------------------------------------------------------------
// the uncolored side's max degree is the low bound of number of coloring
// ============================================================================
int PD2SMPGCColoring::get_lowbound_coloring(const int side){
    return (side==PD2SMPGC::L)?GetMaximumRightVertexDegree():GetMaximumLeftVertexDegree();
}

// ============================================================================
// return sample stand deviation
// ----------------------------------------------------------------------------
//
// ============================================================================
double PD2SMPGCColoring::get_std_degree(const int side){
    if(side==PD2SMPGC::L){
        const int N = GetLeftVertexCount();
        const double mean = m_d_AverageLeftVertexDegree;
        const vector<int>& srcPtr = GetLeftVertices();
        double sum=.0;
        for(int v=0; v<N; v++){
            const double deg = (double)-(srcPtr[v]-srcPtr[v+1]);
            sum+=(deg-mean)*(deg-mean);
        }
        return sqrt(sum/(N-1));
    }
    else if(side==PD2SMPGC::R) {
        const int N = GetRightVertexCount();
        const double mean = m_d_AverageRightVertexDegree;
        const vector<int>& srcPtr = GetRightVertices();
        double sum=.0;
        for(int v=0; v<N; v++){
            const double deg = (double)-(srcPtr[v]-srcPtr[v+1]);
            sum+=(deg-mean)*(deg-mean);
        }
        return sqrt(sum/(N-1));
    }else{
        const int NL = GetLeftVertexCount();
        const int NR = GetRightVertexCount();
        const int mean = m_d_AverageVertexDegree;
        const vector<int>& srcLPtr = GetLeftVertices();
        const vector<int>& srcRPtr = GetRightVertices();
        double sum=.0;
        for(int v=0; v<NL; v++){
            const double deg = (double)-(srcLPtr[v]-srcLPtr[v+1]);
            sum += (deg-mean)*(deg-mean);
        }
        for(int v=0; v<NR; v++){
            const double deg = (double)-(srcRPtr[v]-srcRPtr[v+1]);
            sum += (deg-mean)*(deg-mean);
        }
        return sqrt(sum/(NL+NR-1));
    }
}


int PD2SMPGCColoring::PD2_serial(const int side, int&colors, vector<int>&vtxColor) {
    double tim_color = 0;
    const int          N             = (side==PD2SMPGC::L)?(GetLeftVertexCount()):(GetRightVertexCount()); 
    const vector<int>& srcPtr        = (side==PD2SMPGC::L)?(GetLeftVertices()   ):(GetRightVertices()   );
    const vector<int>& dstPtr        = (side==PD2SMPGC::L)?(GetRightVertices()  ):(GetLeftVertices()    );
    const vector<int>& vtxVal        = GetEdges();
    const vector<int>& ordered_queue = global_ordered_vertex(side);
    const int          srcMaxDegree  = (side==PD2SMPGC::L)?GetMaximumLeftVertexDegree():GetMaximumRightVertexDegree();
    const int          dstMaxDegree  = (side==PD2SMPGC::L)?GetMaximumRightVertexDegree():GetMaximumLeftVertexDegree();
    const int          BufSize       = min(N, dstMaxDegree * srcMaxDegree +1);
    
    colors=0;                       
    vtxColor.assign(N, -1);

    tim_color =- omp_get_wtime();
    vector<int> Mask; Mask.assign(BufSize,-1);
    for(const auto v : ordered_queue){    //v-w-u
        for(int iw=srcPtr[v]; iw!=srcPtr[v+1]; iw++){
            const auto w = vtxVal[iw];
            for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++){
                const auto u = vtxVal[iu];
                if(v==u) continue;
                if(vtxColor[u]==-1) continue;
                Mask[vtxColor[u]] = v;
            }
        }
        int c = 0;
        for(; c<BufSize; c++)
            if(Mask[c]!=v) 
                break;
        vtxColor[v]=c;
        if(colors<c) colors=c;
    }
    tim_color += omp_get_wtime();
    
    colors++; //number of different colors is max color+1, base 0

    printf("@PD2Serial_side_c_T_Tcolor_Tdetect_Trecolor_TmaxC_nCnf\t");
    printf("%s\t", (side==PD2SMPGC::L)?"L":"R");
    printf("%d\t",  colors);    
    printf("%lf\t", tim_color);
#ifdef PD2_VARIFY
    printf("%s", cnt_pd2conflict(side, vtxColor)?"Failed":"Varified");
#endif
    printf("\n");
    return _TRUE;   
}



int PD2SMPGCColoring::PD2_OMP_GM3P(const int side, int nT, int&colors, vector<int>&vtxColor, const int local_order) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double tim_local_order =.0;
    double tim_color       =.0;                     // run time
    double tim_detect      =.0;                     // run time
    double tim_recolor     =.0;                     // run time
    double tim_maxc        =.0;
    double tim_total       =.0;                     // run time
    int    n_conflicts     = 0;                     // Number of conflicts 

    const int          N             = (side==PD2SMPGC::L)?(GetLeftVertexCount()):(GetRightVertexCount()); 
    const vector<int>& srcPtr        = (side==PD2SMPGC::L)?(GetLeftVertices()   ):(GetRightVertices()   );
    const vector<int>& dstPtr        = (side==PD2SMPGC::L)?(GetRightVertices()  ):(GetLeftVertices()    );
    const vector<int>& vtxVal        = GetEdges();
    const vector<int>& ordered_queue = global_ordered_vertex(side);
    const int          srcMaxDegree  = (side==PD2SMPGC::L)?GetMaximumLeftVertexDegree():GetMaximumRightVertexDegree();
    const int          dstMaxDegree  = (side==PD2SMPGC::L)?GetMaximumRightVertexDegree():GetMaximumLeftVertexDegree();
    const int          BufSize       = min(N, dstMaxDegree * srcMaxDegree +1);
    colors=0;                       
    vtxColor.assign(N, -1);

    // pre-partition
    vector<vector<int>> QQ(nT);
    for(int tid=0; tid<nT; tid++) QQ[tid].reserve(N/nT+1+16); // +1 for even/odd, +16 for bus wide of 64bit machine
    {
        vector<int> lens(nT,N/nT), disp(nT+1,0);
        for(int tid=0; tid<N%nT; tid++) lens[tid]++;
        for(int tid=0; tid<nT; tid++) disp[tid+1]=disp[tid] + lens[tid];
        for(int tid=0; tid<nT; tid++)
            QQ[tid].insert(QQ[tid].end(), ordered_queue.begin()+disp[tid], ordered_queue.begin()+disp[tid+1]);
    }


    tim_local_order =-omp_get_wtime();
    switch(local_order){
        case ORDER_NONE: break;
        case ORDER_NATURAL:
            #pragma omp parallel
            {
                const int tid = omp_get_thread_num();
                local_natural_ordering(QQ[tid]);
            }
            break;
        case ORDER_RANDOM:
            #pragma omp parallel
            {
                const int tid = omp_get_thread_num();
                local_random_ordering(QQ[tid]);
            }
            break;
        case ORDER_LARGEST_FIRST:
            #pragma omp parallel
            {
                const int tid = omp_get_thread_num();
                local_largest_degree_first_ordering(side, QQ[tid]);
            }
            break;
        default:
            printf("Error! PD2_OMP_GM3P tring to use order no. %d. But it is not supported.\n", local_order);
            exit(1);
    }
    tim_local_order +=omp_get_wtime();

    tim_color =- omp_get_wtime();
    // phase pseudo coloring
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        const vector<int>& Q = QQ[tid];
        vector<int> Mask; Mask.assign(BufSize,-1);
        for(const auto v : Q){    //v-w-u
            for(int iw=srcPtr[v]; iw!=srcPtr[v+1]; iw++){
                const auto w = vtxVal[iw];
                for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++){
                    const auto u = vtxVal[iu];
                    if(v==u) continue;
                    if(vtxColor[u]==-1) continue;
                    Mask[vtxColor[u]] = v;
                }
            }
            int c = 0;
            for(; c<BufSize; c++)
                if(Mask[c]!=v) 
                    break;
            vtxColor[v]=c;
        } // end for
    }// end omp parallel
    tim_color += omp_get_wtime();
    // phase conflicts detection
    tim_detect =- omp_get_wtime();
    n_conflicts=0;
    #pragma omp parallel reduction(+:n_conflicts)
    {
        const int tid = omp_get_thread_num();
        vector<int>& Q = QQ[tid];
        for(int iv=0; iv<(signed)Q.size(); iv++){
            bool  b_false_colored = false;
            const int v = Q[iv];
            const int vc = vtxColor[v];
            for(int iw=srcPtr[v]; b_false_colored==false && iw!=srcPtr[v+1]; iw++) {
                const int w = vtxVal[iw];
                for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++) {
                    const auto u = vtxVal[iu];
                    if(v == u) continue;
                    if(vc== vtxColor[u]){
                        b_false_colored=true;
                        Q[n_conflicts++]=v;
                        vtxColor[v]=-1;
                        break;
                    }
                }
            }
        }
        Q.resize(n_conflicts);
    }// end omp parallel
    tim_detect  += omp_get_wtime();

    tim_recolor =- omp_get_wtime();
    // serial coloring remains part
    {
        vector<int> Mask; Mask.assign(BufSize,-1);
        for(int tid=0; tid<nT; tid++){
            const vector<int>& Q = QQ[tid];
            for(const auto v : Q) {
                for(int iw=srcPtr[v]; iw!=srcPtr[v+1]; iw++) {
                    const auto w = vtxVal[iw];
                    for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++) {
                        const auto u = vtxVal[iu];
                        if(v==u) continue;
                        if(vtxColor[u]==-1) continue;
                        Mask[vtxColor[u]] = v; 
                    }
                }
                int c=0; 
                for(; c<BufSize; c++) 
                    if(Mask[c]!=v) 
                        break;
                vtxColor[v]=c;
            }
        }
    }
    tim_recolor += omp_get_wtime();

    // get number of colors
    tim_maxc = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_maxc += omp_get_wtime();

    tim_total = tim_color+tim_detect+tim_recolor+tim_maxc+tim_local_order;

    string lotag = "";
    switch(local_order){
        case ORDER_NONE:    lotag="None"; break;
        case ORDER_NATURAL: lotag="NT"; break;
        case ORDER_RANDOM:  lotag="RD"; break;
        case ORDER_LARGEST_FIRST: lotag="LF"; break;
        default:
            printf("Error! PD2_OMP_GM3P tring to use local order %d. which is not supported.\n",local_order); 
            exit(1);
    }
    printf("@PD2GM3PLO(%s)_side_nT_c_T_Tcolor_Tdetect_Trecolor_TmaxC_Tlo_nCnf\t",lotag.c_str());
    printf("%s ", (side==PD2SMPGC::L)?"L":"R");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_recolor);
    printf("%lf\t", tim_maxc);
    printf("%lf\t", tim_local_order);
    printf("%d\t",  n_conflicts);
#ifdef PD2_VARIFY
    printf("%s", cnt_pd2conflict(side, vtxColor)?"Failed":"Varified");
#endif
    printf("\n");
    return _TRUE;   
}




// ============================================================================
// ============================================================================
int PD2SMPGCColoring::PD2_OMP_GMMP(const int side, int nT, int &colors, vector<int>&vtxColor, const int local_order) {
    if(nT<=0) { printf("Warning, number of threads changed from %d to 1\n",nT); nT=1; }
    omp_set_num_threads(nT);

    double tim_local_order=.0;
    double tim_color      =.0;                     // run time
    double tim_detect     =.0;                     // run time
    double tim_maxc       =.0;
    double tim_total      =.0;                          // run time
    int    n_conflicts    = 0;                     // Number of conflicts 
    int    n_loops        = 0;
    const int          N             = (side==PD2SMPGC::L)?(GetLeftVertexCount()):(GetRightVertexCount()); 
    const vector<int>& srcPtr        = (side==PD2SMPGC::L)?(GetLeftVertices()   ):(GetRightVertices()   );
    const vector<int>& dstPtr        = (side==PD2SMPGC::L)?(GetRightVertices()  ):(GetLeftVertices()    );
    const vector<int>& vtxVal        = GetEdges();
    const vector<int>& ordered_queue = global_ordered_vertex(side);
    const int          srcMaxDegree  = (side==PD2SMPGC::L)?GetMaximumLeftVertexDegree():GetMaximumRightVertexDegree();
    const int          dstMaxDegree  = (side==PD2SMPGC::L)?GetMaximumRightVertexDegree():GetMaximumLeftVertexDegree();
    const int          BufSize       = min(N, dstMaxDegree * srcMaxDegree +1);
    colors=0;                       
    vtxColor.assign(N, -1);

    // pre-partition
    vector<vector<int>> QQ(nT);
    for(int tid=0; tid<nT; tid++) QQ[tid].reserve(N/nT+1+16); // +1 for even/odd, +16 for bus wide of 64bit machine
    {
        vector<int> lens(nT,N/nT), disp(nT+1,0);
        for(int tid=0; tid<N%nT; tid++) lens[tid]++;
        for(int tid=0; tid<nT; tid++) disp[tid+1]=disp[tid] + lens[tid];
        for(int tid=0; tid<nT; tid++)
            QQ[tid].insert(QQ[tid].end(), ordered_queue.begin()+disp[tid], ordered_queue.begin()+disp[tid+1]);
    }



    int num_uncolored_vertex = N;
    while(num_uncolored_vertex!=0){

        tim_local_order -=omp_get_wtime();
        switch(local_order){
            case ORDER_NONE: break;
            case ORDER_NATURAL:
                #pragma omp parallel
                {
                    const int tid = omp_get_thread_num();
                    local_natural_ordering(QQ[tid]);
                }
                break;
            case ORDER_RANDOM:
                #pragma omp parallel
                {
                    const int tid = omp_get_thread_num();
                    local_random_ordering(QQ[tid]);
                }
                break;
            case ORDER_LARGEST_FIRST:
                #pragma omp parallel
                {
                    const int tid = omp_get_thread_num();
                    local_largest_degree_first_ordering(side, QQ[tid]);
                }
                break;
            default:
                printf("Error! PD2_OMP_GMMP tring to use order no. %d. But it is not supported.\n", local_order);
                exit(1);
        }
        tim_local_order +=omp_get_wtime();

        tim_color -= omp_get_wtime();
        // phase pseudo coloring
        #pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            const vector<int>& Q = QQ[tid];
            vector<int> Mask; Mask.assign(BufSize,-1);
            for(const auto v : Q){    //v-w-u
                for(int iw=srcPtr[v]; iw!=srcPtr[v+1]; iw++){
                    const auto w = vtxVal[iw];
                    for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++){
                        const auto u = vtxVal[iu];
                        if(v==u) continue;
                        if(vtxColor[u]==-1) continue;
                        Mask[vtxColor[u]] = v;
                    }
                }
                int c = 0;
                for(; c<BufSize; c++)
                    if(Mask[c]!=v) 
                        break;
                vtxColor[v]=c;
            } // end for
        }// end omp parallel
        tim_color += omp_get_wtime();
    
        // phase conflicts detection
        tim_detect -= omp_get_wtime();
        num_uncolored_vertex=0;
        #pragma omp parallel reduction(+:num_uncolored_vertex)
        {
            const int tid = omp_get_thread_num();
            vector<int>& Q = QQ[tid];
            for(int iv=0; iv<(signed)Q.size(); iv++){
                bool  b_visfalse_colored = false;
                const int v = Q[iv];
                const int vc = vtxColor[v];
                for(int iw=srcPtr[v]; b_visfalse_colored==false && iw!=srcPtr[v+1]; iw++) {
                    const int w = vtxVal[iw];
                    for(int iu=dstPtr[w]; iu!=dstPtr[w+1]; iu++) {
                        const auto u = vtxVal[iu];
                        if(v <= u) continue;
                        if(vc== vtxColor[u]){
                            b_visfalse_colored=true;
                            Q[num_uncolored_vertex++]=v;
                            vtxColor[v]=-1;
                            break;
                        }
                    }
                }
            }
            Q.resize(num_uncolored_vertex);
        } //end omp parallel
        tim_detect  += omp_get_wtime();
        
        n_conflicts += num_uncolored_vertex;
        n_loops      += 1;
    } //end while

    // get number of colors
    tim_maxc = -omp_get_wtime();
    #pragma omp parallel for reduction(max:colors)
    for(int i=0; i<N; i++){
        colors = max(colors, vtxColor[i]);
    }
    colors++; //number of colors, 
    tim_maxc += omp_get_wtime();

    tim_total = tim_color+tim_detect + tim_maxc+tim_local_order;

    string lotag = "";
    switch(local_order){
        case ORDER_NONE:    lotag="None"; break;
        case ORDER_NATURAL: lotag="NT"; break;
        case ORDER_RANDOM:  lotag="RD"; break;
        case ORDER_LARGEST_FIRST: lotag="LF"; break;
        default:
            printf("Error! PD2_OMP_GMMP tring to use local order %d. which is not supported.\n", local_order); 
            exit(1);
    }


    printf("@PD2GMMPLO(%s)_side_nT_c_T_Tcolor_Tdetect_TmaxC_Tlo_nCnf_nLoop\t", lotag.c_str());
    printf("%s ", (side==PD2SMPGC::L)?"L":"R");
    printf("%d\t",  nT);    
    printf("%d\t",  colors);    
    printf("%lf\t", tim_total);
    printf("%lf\t", tim_color);
    printf("%lf\t", tim_detect);
    printf("%lf\t", tim_maxc);
    printf("%lf\t", tim_local_order);
    printf("%d\t",  n_conflicts);
    printf("%d\t", n_loops);
#ifdef PD2_VARIFY
    printf("%s", cnt_pd2conflict(side, vtxColor)?"Failed":"Varified");
#endif
    printf("\n");
    return _TRUE;   
}



