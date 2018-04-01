/*************************************************************************
    File Name: SMPGCInterface.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:46:58 AM EST
*********************************************************************/

#include "SMPGCCore.h"
#include <time.h>   //clock
using namespace std;
using namespace ColPack;



// ============================================================================
// Construction
// ============================================================================
SMPGCCore::SMPGCCore(const string& graph_name, const string& format, double* iotime) {
    if(format!="binary")
        do_read_MM_struct(graph_name, ia_, ja_, &maxDeg_, &minDeg_, &avgDeg_, iotime);
    else 
        do_read_Binary_struct(graph_name, ia_, ja_, &maxDeg_, &minDeg_, &avgDeg_, iotime);
}


SMPGCCore::~SMPGCCore(){
}



// ============================================================================
// Read MatrixMarket only structure into memory
// ----------------------------------------------------------------------------
// Note: store as sparsed CSR format
// ============================================================================
void SMPGCCore::do_read_MM_struct(const string& graph_name, vector<INT>&ia, vector<INT>&ja, INT* pMaxDeg, INT* pMinDeg, double* pAvgDeg, double* iotime) {
    if(graph_name.empty()) error("graph name is empty");
    FILE*fp = fopen(graph_name.c_str(), "r");
    if(!fp) {stringstream ss; ss<<"cannot open graph "<<graph_name<<"\n"; error(ss.str()); }
    
    if(iotime) { *iotime=0; *(clock_t *)iotime = -clock(); }
    bool bSymmetric=true;
    bool bPattern  =false;
    MM_typecode matcode;
    mm_read_banner(fp, &matcode);
    if(!mm_is_matrix(matcode)) { fclose(fp); stringstream ss; ss<<graph_name<<" is not MatrixMarket matrix"; error(ss.str()); }
    if(!mm_is_symmetric(matcode)) {bSymmetric=false; fprintf(stdout, "Warning! %s is not symmetric\n",graph_name.c_str()); }
    if(mm_is_complex(matcode)) { fclose(fp); stringstream ss; ss<<graph_name<<" is complex which is not support!"; error(ss.str()); }
    if(mm_is_pattern(matcode)) bPattern = true;  
    int N; //number of vertex
    unordered_map<int, vector<int> > G; 
    if(mm_is_sparse(matcode)) { 
        int tmpR, tmpC, tmpNNZ;
        double tmpV;
        mm_read_mtx_crd_size(fp, &tmpR, &tmpC, &tmpNNZ);
        if(tmpR!=tmpC) { fclose(fp); stringstream ss; ss<<"Graph \""<<graph_name<<"\" number of Row does not equal number of Col!"; error(ss.str()); }
        N=tmpR;
        //M=0;
        for(int i=0; i<tmpNNZ; i++){
            if(bPattern)
                fscanf(fp, "%d %d\n", &tmpR, &tmpC);
            else
                fscanf(fp, "%d %d %lg\n", &tmpR, &tmpC, &tmpV);
            if(tmpR==tmpC) continue;
            tmpR--; tmpC--; //1-based to 0-based
            
            G[tmpR].push_back(tmpC);
            //M++;
            if(bSymmetric){
                G[tmpC].push_back(tmpR);
                //M++;

                if(tmpR<tmpC) {
                    fclose(fp); stringstream ss; ss<<"Graph \""<<graph_name<<"\" Find a nonzero in upper triangular in symmetric graph";
                    ss<<"row,col = "<<tmpR<<" , "<<tmpC;
                    error(ss.str());
                }
            }
        }
    } // endof if mm_is_sparse(matcode)
    else{
        fprintf(stdout,"Warning! it is weird to store graph into dense array! %s \n",graph_name.c_str());
        int tmpR, tmpC;
        double tmpV;
        mm_read_mtx_array_size(fp, &tmpR, &tmpC);
        if(tmpR!=tmpC) { fclose(fp); stringstream ss; ss<<"Graph \""<<graph_name<<"\" number of Row does not equal number of Col!"; error(ss.str()); }
        N=tmpR;
        for(int c=0; c<tmpC; c++){
            for(int r=(bSymmetric?c:0); r<tmpR; c++){
                fscanf(fp, "%lg\n", &tmpV);
                if(tmpV==.0) continue; //I hate double == 0 condition
                if(c==r) continue;
                G[r].push_back(c); //M++;
                if(bSymmetric){
                    G[c].push_back(r);// M++;
                }
            }
        }
    } // endof else mm_is_sparse(matcode)
    fclose(fp); fp=nullptr;

    if(iotime) { *(clock_t*)iotime += clock(); *iotime = double(*((clock_t*)iotime))/CLOCKS_PER_SEC; }
    //for(auto it=G.begin(), it!=G.end(); it++)
    //    sort((it->second).begin(), (it->second).end());

    ia.push_back(ja.size());
    for(int i=0; i<N; i++){
        auto it=G.find(i);
        if(it!=G.end()) ja.insert(ja.end(), (it->second).begin(), (it->second).end());
        ia.push_back(ja.size());
    }
    
    // calc max degree if needed
    if(pMaxDeg||pMinDeg){
        int maxDeg=0, minDeg=ia.size()-1;
        for(auto it : G){
            int d = (it.second).size();
            maxDeg = (maxDeg<d)?d:maxDeg;
            minDeg = (minDeg>d)?d:minDeg;
        }
        if(pMaxDeg) *pMaxDeg = maxDeg;
        if(pMinDeg) *pMinDeg = minDeg;
    }
    if(pAvgDeg) *pAvgDeg=1.0*(ja.size())/(ia.size()-1);

}


// ============================================================================
//
// ============================================================================

void SMPGCCore::do_read_Binary_struct(const string&graph_name, vector<INT>&ia_, vector<INT>&ja_, INT* maxDeg_,INT* minDeg_,double* avgDeg_,double* iotime){
    return;
}
void SMPGCCore::do_write_Binary_struct(const string&graph_name, vector<INT>&ia_, vector<INT>&ja_,double* iotime){
    return;
}


// ============================================================================
// dump information
// ============================================================================
void SMPGCCore::dump(){
    ;
}


