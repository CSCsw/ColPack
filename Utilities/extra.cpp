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

#include "extra.h"

string toUpper(string input) {
	string output = input;

	for(int i = input.size() - 1; i>=0; i--) {
		if(input[i]==' ' || input[i]=='\t' || input[i]=='\n') {
			output[i] = '_';
		}
		else {
			output[i] = toupper(input[i]);
		}
	}

	return output;
}

//just manipulate the value of dp2_Values a little bit
int Times2Plus1point5(double** dp2_Values, int i_RowCount, int i_ColumnCount) {
	for(int i=0; i < i_RowCount; i++) {
		for(int j=0; j < i_ColumnCount; j++) {
			if(dp2_Values[i][j] != 0.) dp2_Values[i][j] = dp2_Values[i][j]*2 + 1.5; //for each non-zero entry in the matrix, do the manipulation.
		}

	}
	return 0;
}
int Times2(double** dp2_Values, int i_RowCount, int i_ColumnCount) {
	for(int i=0; i < i_RowCount; i++) {
		for(int j=0; j < i_ColumnCount; j++) {
			if(dp2_Values[i][j] != 0.) dp2_Values[i][j] = dp2_Values[i][j]*2; 
		}

	}
	return 0;
}

int GenerateValues(unsigned int ** uip2_SparsityPattern, int rowCount, double*** dp3_Value) {
	srand(time(NULL));

	(*dp3_Value) = new double*[rowCount];
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = uip2_SparsityPattern[i][0];
		(*dp3_Value)[i] = new double[numOfNonZeros + 1];
		(*dp3_Value)[i][0] = (double)numOfNonZeros;
		for(unsigned int j=1; j <= numOfNonZeros; j++) {
			(*dp3_Value)[i][j] = (rand()%2001 - 1000)/1000.0;
			//printf("(*dp3_Value)[%d][%d] = (%d % 2001 - 1000)/1000.0 = %7.2f \n",i,j,rand(),(*dp3_Value)[i][j]);
		}
	}

	return 0;
}

int ConvertSymmetricMatrixMarketFormatToCompressedSparseRowFormat(string s_InputFile, unsigned int *** uip3_SparsityPattern, double*** dp3_Value, int &rowCount, int &columnCount) {
	ifstream InputStream (s_InputFile.c_str());
	if(!InputStream) {
		cout<<"File "<<s_InputFile<<" Not Found"<<endl;
		return 1;
	}
	else	cout<<"Found File "<<s_InputFile<<endl;

	string s_InputLine;
	int i_LineCount=0; //the number of lines have been read. Comment lines and the current line do not count.
	istringstream iin;
	int row = 0, column = 0;
	double value=0.;
	vector< vector<int> > SparsityPattern;
	vector< vector<double> > Value;

	do
	{
		getline(InputStream, s_InputLine);
		
		if(s_InputLine[0]=='%') 
		{
			continue;
		}

		if(!InputStream)
		{
			break;
		}

		if(i_LineCount == 0) { //first line, contain info about the size of the matrix (row, column) and the # of non-zeros
			iin.clear();
			iin.str(s_InputLine);
			iin >> rowCount >> columnCount;
			SparsityPattern.resize(rowCount);
			Value.resize(rowCount);
			(*uip3_SparsityPattern) = new unsigned int*[rowCount];
			(*dp3_Value) = new double*[rowCount];
		}
		else {
			iin.clear();
			iin.str(s_InputLine);
			iin >> row >> column >> value;
			row--;
			column--;
			SparsityPattern[row].push_back(column);
			Value[row].push_back(value);
			if(column!= row) { //non-diagonal
				SparsityPattern[column].push_back(row);
				Value[column].push_back(value);
			}
		}

		i_LineCount++;
		//Pause();
	}while (InputStream);

//cout<<"Out of While()"<<endl;
//Pause();
	//populate *uip3_SparsityPattern and *dp3_Value
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = SparsityPattern[i].size();
		//Allocate memory for each row
		(*uip3_SparsityPattern)[i] = new unsigned int[numOfNonZeros+1];
		(*dp3_Value)[i] = new double[numOfNonZeros+1];

		(*uip3_SparsityPattern)[i][0] = numOfNonZeros;
		(*dp3_Value)[i][0] = (double)numOfNonZeros;

		for(unsigned int j=0; j < numOfNonZeros; j++) {
			(*uip3_SparsityPattern)[i][j+1] = SparsityPattern[i][j];
			(*dp3_Value)[i][j+1] = Value[i][j];
		}
	}

//cout<<"Finish populate"<<endl;
//Pause();

//cout<<"Finish display"<<endl;
//Pause();

	return 0;
}

int ConvertGeneralMatrixMarketFormatToCompressedSparseRowFormat(string s_InputFile, unsigned int *** uip3_SparsityPattern, double*** dp3_Value, int &rowCount, int &columnCount) {
	ifstream InputStream (s_InputFile.c_str());
	if(!InputStream) {
		cout<<"File "<<s_InputFile<<" Not Found"<<endl;
		return 1;
	}
	else	cout<<"Found File "<<s_InputFile<<endl;

	string s_InputLine;
	int i_LineCount=0; //the number of lines have been read. Comment lines and the current line do not count.
	istringstream iin;
	int row = 0, column = 0;
	double value=0.;
	vector< vector<int> > SparsityPattern;
	vector< vector<double> > Value;

	do
	{
		getline(InputStream, s_InputLine);
		
		if(s_InputLine[0]=='%') 
		{
			continue;
		}

		if(!InputStream)
		{
			break;
		}

		if(i_LineCount == 0) { //first line, contain info about the size of the matrix (row, column) and the # of non-zeros
			iin.clear();
			iin.str(s_InputLine);
			iin >> rowCount >> columnCount;
			SparsityPattern.resize(rowCount);
			Value.resize(rowCount);
			(*uip3_SparsityPattern) = new unsigned int*[rowCount];
			(*dp3_Value) = new double*[rowCount];
		}
		else {
			iin.clear();
			iin.str(s_InputLine);
			iin >> row >> column >> value;
			row--;
			column--;
			SparsityPattern[row].push_back(column);
			Value[row].push_back(value);
		}

		i_LineCount++;
		//Pause();
	}while (InputStream);

//cout<<"Out of While()"<<endl;
//Pause();
	//populate *uip3_SparsityPattern and *dp3_Value
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = SparsityPattern[i].size();
		//Allocate memory for each row
		(*uip3_SparsityPattern)[i] = new unsigned int[numOfNonZeros+1];
		(*dp3_Value)[i] = new double[numOfNonZeros+1];

		(*uip3_SparsityPattern)[i][0] = numOfNonZeros;
		(*dp3_Value)[i][0] = (double)numOfNonZeros;

		for(unsigned int j=0; j < numOfNonZeros; j++) {
			(*uip3_SparsityPattern)[i][j+1] = SparsityPattern[i][j];
			(*dp3_Value)[i][j+1] = Value[i][j];
		}
	}

//cout<<"Finish populate"<<endl;
//Pause();

//cout<<"Finish display"<<endl;
//Pause();

	return 0;
}

int MatrixMultiplication_VxS(unsigned int ** uip3_SparsityPattern, double** dp3_Value, int rowCount, int columnCount, double** dp2_seed, int colorCount, double*** dp3_CompressedMatrix) {

	//Allocate memory for (*dp3_CompressedMatrix)[rowCount][colorCount]
	//cout<<"Allocate memory for (*dp3_CompressedMatrix)[rowCount][colorCount]"<<endl;
	(*dp3_CompressedMatrix) = new double*[rowCount];
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		(*dp3_CompressedMatrix)[i] = new double[colorCount];
		for(unsigned int j=0; j < (unsigned int)colorCount; j++) {
			(*dp3_CompressedMatrix)[i][j] = 0.;
		}
	}

	//do the multiplication
	//cout<<"Do the multiplication"<<endl;
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = uip3_SparsityPattern[i][0];
		for(unsigned int j=1; j <= numOfNonZeros; j++) {
		  for(unsigned int k=0; k < (unsigned int)colorCount; k++) {
				//printf("i=%d\tj=%d\tuip3_SparsityPattern[i][j]=%d\tk=%d\n", i, j, uip3_SparsityPattern[i][j], k);
				(*dp3_CompressedMatrix)[i][k] += dp3_Value[i][j]*dp2_seed[uip3_SparsityPattern[i][j]][k];
			}
		}
	}

	return 0;
}

int MatrixMultiplication_SxV(unsigned int ** uip3_SparsityPattern, double** dp3_Value, int rowCount, int columnCount, double** dp2_seed, int colorCount, double*** dp3_CompressedMatrix) {

	//Allocate memory for (*dp3_CompressedMatrix)[colorCount][columnCount]
	//cout<<"Allocate memory for (*dp3_CompressedMatrix)[colorCount][columnCount]"<<endl;
	(*dp3_CompressedMatrix) = new double*[colorCount];
	for(unsigned int i=0; i < (unsigned int)colorCount; i++) {
		(*dp3_CompressedMatrix)[i] = new double[columnCount];
		for(unsigned int j=0; j < (unsigned int)columnCount; j++) {
			(*dp3_CompressedMatrix)[i][j] = 0.;
		}
	}

	//do the multiplication
	//cout<<"Do the multiplication"<<endl;
	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = uip3_SparsityPattern[i][0];
		for(unsigned int j=1; j <= numOfNonZeros; j++) {
		  for(unsigned int k=0; k < (unsigned int)colorCount; k++) {
				//printf("i=%d\tj=%d\tuip3_SparsityPattern[i][j]=%d\tk=%d\n", i, j, uip3_SparsityPattern[i][j], k);
				(*dp3_CompressedMatrix)[k][uip3_SparsityPattern[i][j]] += dp2_seed[k][i]*dp3_Value[i][j];
			}
		}
	}

	return 0;
}

bool CompressedRowMatricesREqual(double** dp3_Value, double** dp3_NewValue, int rowCount, bool compare_exact, bool print_all) {
	double ratio = 1.;
	int none_equal_count = 0;

	for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
		unsigned int numOfNonZeros = (unsigned int)dp3_Value[i][0];
		if (numOfNonZeros != (unsigned int)dp3_NewValue[i][0]) {
			printf("Number of non-zeros in row %d are not equal. dp3_Value[i][0] = %d; dp3_NewValue[i][0] = %d; \n",i,(unsigned int)dp3_Value[i][0],(unsigned int)dp3_NewValue[i][0]);
			if (print_all) {
				none_equal_count++;
				continue;
			}
			else return false;
		}
		for(unsigned int j=0; j <= numOfNonZeros; j++) {
			if (compare_exact) {
				if (dp3_Value[i][j] != dp3_NewValue[i][j]) {
					printf("At row %d, column %d, dp3_Value[i][j](%f) != dp3_NewValue[i][j](%f) \n",i,j,dp3_Value[i][j],dp3_NewValue[i][j]);
					if (print_all) {
						none_equal_count++;
					}
					else {
						printf("You may want to set the flag \"compare_exact\" to 0 to compare the values approximately\n");
						return false;
					}
				}
			}
			else {
				if(dp3_NewValue[i][j] == 0.) {
					if(dp3_Value[i][j] != 0.) {
						printf("At row %d, column %d, dp3_Value[i][j](%f) != dp3_NewValue[i][j](0) \n",i,j,dp3_Value[i][j]);
						if (print_all) {
							none_equal_count++;
						}
						else return false;
					}
				}
				else {
					ratio = dp3_Value[i][j] / dp3_NewValue[i][j];
					if( ratio < .99 || ratio > 1.02) {
						printf("At row %d, column %d, dp3_Value[i][j](%f) != dp3_NewValue[i][j](%f) ; dp3_Value[i][j] / dp3_NewValue[i][j]=%f\n",i,j,dp3_Value[i][j],dp3_NewValue[i][j], ratio);
						if (print_all) {
							none_equal_count++;
						}
						else return false;
					}
				}
			}
		}
	}

	if(none_equal_count!=0) {
		printf("Total: %d lines. (The total # of non-equals can be greater)\n",none_equal_count);
		if (compare_exact) printf("You may want to set the flag \"compare_exact\" to 0 to compare the values approximately\n");
		return false;
	}
	else return true;
}


