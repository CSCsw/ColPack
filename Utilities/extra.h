/************************************************************************************
    Copyright (C) 2005-2008 Assefaw H. Gebremedhin, Arijit Tarafdar, Duc Nguyen,
    Alex Pothen

    This file is part of ColPack.

    ColPack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ColPack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ColPack.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************************/

#ifndef EXTRA_H
#define EXTRA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
//#include <cctype> //for toupper()

#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
#include "Definitions.h"
#include "Pause.h"
*/

/// Convert all the characters in input to upper case, ' ', '\t', '\n' will be converted to '_'
string toUpper(string input);

///Read the sparse matrix from Matrix-Market-format file and convert to compress sparse row format "uip3_SparsityPattern" & "dp3_Value"
/** Read in a matrix from matrix-market format file and create a matrix stored in compressed sparse row format
The Matrix-Market-format has 3 values in each row, the row index, column index and numerical value of each nonzero.
contains the value column
The last 4 parameters of the this routine are output parameters (unsigned int *** uip3_SparsityPattern, double*** dp3_Value,int &rowCount, int &columnCount)

ConvertSymmetricMatrixMarketFormatToCompressedSparseRowFormat assumes that only the lower half of a symmetric matrix is stored.
*/
int ConvertSymmetricMatrixMarketFormatToCompressedSparseRowFormat(string s_InputFile, unsigned int *** uip3_SparsityPattern, double*** dp3_Value, int &rowCount, int &columnCount);

///Read the sparse matrix from Matrix-Market-format file and convert to compress sparse row format "uip3_SparsityPattern" & "dp3_Value"
/** Same as ConvertSymmetricMatrixMarketFormatToCompressedSparseRowFormat except that every non-zero entry of the matrix is stored.
*/
int ConvertGeneralMatrixMarketFormatToCompressedSparseRowFormat(string s_InputFile, unsigned int *** uip3_SparsityPattern, double*** dp3_Value, int &rowCount, int &columnCount);

/// Multiply the original sparse matrix (uip3_SparsityPattern,dp3_Value) (in compress sparse row format) with the seed matrix dp2_seed and store the result in "dp3_CompressedMatrix"
/** (*dp3_CompressedMatrix) = (*dp3_Value) * dp2_seed
*/
int MatrixMultiplication_VxS(unsigned int ** uip3_SparsityPattern, double** dp3_Value, int rowCount, int columnCount, double** dp2_seed, int colorCount, double*** dp3_CompressedMatrix);

/// Multiply the seed matrix dp2_seed with the original sparse matrix (uip3_SparsityPattern,dp3_Value) (in compress sparse row format) and store the result in "dp3_CompressedMatrix"
/** (*dp3_CompressedMatrix) = dp2_seed * (*dp3_Value)
*/
int MatrixMultiplication_SxV(unsigned int ** uip3_SparsityPattern, double** dp3_Value, int rowCount, int columnCount, double** dp2_seed, int colorCount, double*** dp3_CompressedMatrix);

///Compare dp3_Value with dp3_NewValue and see if all the values are equal.
/**
	If (compare_exact == 0) num1 and num2 are consider equal if 0.99 <= num1/num2 <= 1.02
	If (print_all == 1) all cases of non-equal will be print out. Normally (when print_all == 0), this rountine will stop after the first non-equal.
*/
bool CompressedRowMatricesREqual(double** dp3_Value, double** dp3_NewValue, int rowCount, bool compare_exact = 1, bool print_all = 0);

///just manipulate the value of dp2_Values a little bit. Each non-zero entry in the matrix * 2 + 1.5.
int Times2Plus1point5(double** dp2_Values, int i_RowCount, int i_ColumnCount);

///just manipulate the value of dp2_Values a little bit. Each non-zero entry in the matrix * 2.
int Times2(double** dp2_Values, int i_RowCount, int i_ColumnCount);

///Allocate memory and generate random values for dp3_Value
int GenerateValues(unsigned int ** uip2_SparsityPattern, int rowCount, double*** dp3_Value);

#ifndef EXTRA_H_TEMPLATE_FUNCTIONS
#define EXTRA_H_TEMPLATE_FUNCTIONS

template<class T>
int deleteMatrix(T** xp2_matrix, int rowCount) {
//cout<<"IN deleteM 2"<<endl<<flush;
//printf("* deleteMatrix rowCount=%d \n",rowCount);
//Pause();
	for(int i = 0; i < rowCount; i++) {
//printf("delete xp2_matrix[%d][0] = %7.2f \n", i, (float) xp2_matrix[i][0]);
		delete xp2_matrix[i];
	}
//cout<<"MID deleteM 2"<<endl<<flush;
	delete xp2_matrix;
//cout<<"OUT deleteM 2"<<endl<<flush;
	return 0;
}

template<class T>
int deleteMatrix(T*** xp3_matrix, int rowCount) {
//cout<<"IN deleteM 3"<<endl<<flush;
	deleteMatrix(*xp3_matrix,rowCount);
//cout<<"MID deleteM 3"<<endl<<flush;
	delete xp3_matrix;
//cout<<"OUT deleteM 3"<<endl<<flush;
	return 0;
}
template<class T>
void displayCompressedRowMatrix(T** xp2_Value, int rowCount, bool structureOnly = false) {
	unsigned int estimateColumnCount = 30;
	cout<<setw(4)<<"["<<setw(3)<<"\\"<<"]";
	if(structureOnly) {
		for(unsigned int j=0; j < estimateColumnCount; j++) cout<<setw(4)<<j;
	}
	else {
		for(unsigned int j=0; j < estimateColumnCount; j++) cout<<setw(9)<<j;
	}
	cout<<endl;

	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		cout<<setw(4)<<"["<<setw(3)<<i<<"]";
		unsigned int numOfNonZeros = (unsigned int)xp2_Value[i][0];
		if(structureOnly) {
			for(unsigned int j=0; j < numOfNonZeros; j++) cout<<setw(4)<<(int)xp2_Value[i][j];
		}
		else {
			//for(unsigned int j=0; j <= numOfNonZeros; j++) cout<<setw(8)<<xp2_Value[i][j];
			for(unsigned int j=0; j <= numOfNonZeros; j++) printf("  %7.2f",(float)xp2_Value[i][j]);
		}
		cout<<endl<<flush;
	}
	cout<<endl<<endl;
}

template<class T>
void displayMatrix(T** xp2_Value, int rowCount, int columnCount, bool structureOnly = false) {
	cout<<setw(4)<<"["<<setw(3)<<"\\"<<"]";
	if(structureOnly) {
		for(unsigned int j=0; j < (unsigned int)columnCount; j++) cout<<setw(3)<<j;
	}
	else {
		for(unsigned int j=0; j < (unsigned int)columnCount; j++) cout<<setw(9)<<j;
	}
	cout<<endl;

	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		cout<<setw(4)<<"["<<setw(3)<<i<<"]";
		if(structureOnly) {
			for(unsigned int j=0; j < (unsigned int)columnCount; j++) cout<<setw(3)<<(bool)xp2_Value[i][j];
		}
		else {
			for(unsigned int j=0; j < (unsigned int)columnCount; j++) printf("  %7.2f",(float)xp2_Value[i][j]);
			//for(unsigned int j=0; j < (unsigned int)columnCount; j++) cout<<setw(8)<<xp2_Value[i][j];
		}
		cout<<endl<<flush;
	}
	cout<<endl<<endl;
}

template<class T>
void displayVector(T* xp2_Value, int size, bool structureOnly = false) {
	if(structureOnly) {
		for(unsigned int i=0; i < (unsigned int)size; i++) {
			cout<<setw(4)<<"["<<setw(3)<<i<<"]";
			cout<<setw(3)<<(bool)xp2_Value[i];
			cout<<endl<<flush;
		}
	}
	else {
		for(unsigned int i=0; i < (unsigned int)size; i++) {
			cout<<setw(4)<<"["<<setw(3)<<i<<"]";
			printf("  %7.2f",(float)xp2_Value[i]);
			//cout<<setw(8)<<xp2_Value[i];
			cout<<endl<<flush;
		}
	}
	cout<<endl<<endl;
}

/// Used mainly to debug GraphColoringInterface::IndirectRecover() routine
template<class T>
void displayAdjacencyMatrix(vector< vector<T> > &xp2_Value, bool structureOnly = false) {
	unsigned int estimateColumnCount = 30;
	cout<<setw(4)<<"["<<setw(3)<<"\\"<<"]";
	if(structureOnly) {
		for(unsigned int j=0; j < estimateColumnCount; j++) cout<<setw(3)<<j;
	}
	else {
		for(unsigned int j=0; j < estimateColumnCount; j++) cout<<setw(9)<<j;
	}
	cout<<endl;

	unsigned int rowCount = xp2_Value.size();
	for(unsigned int i=0; i < rowCount; i++) {
		cout<<setw(4)<<"["<<setw(3)<<i<<"]";
		unsigned int numOfNonZeros = (int)xp2_Value[i].size();
		cout<<"("<<setw(5)<<numOfNonZeros<<")";
		if(structureOnly) {
			for(unsigned int j=0; j < numOfNonZeros; j++) cout<<setw(3)<<(bool)xp2_Value[i][j];
		}
		else {
			for(unsigned int j=0; j < numOfNonZeros; j++) cout<<setw(9)<<xp2_Value[i][j];
		}
		cout<<endl<<flush;
	}
	cout<<endl<<endl;
}


#endif //EXTRA_H_TEMPLATE_FUNCTIONS

#endif //EXTRA_H



