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

#include "ColPackHeaders.h"

using namespace std;

namespace ColPack
{

	int HessianRecovery::DirectRecover_ADOLCFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, double*** dp3_HessianValue) {
		if(g==NULL) {
			cerr<<"g==NULL"<<endl;
			return _FALSE;
		}

		int rowCount = g->GetVertexCount();
		int colorCount = g->GetVertexColorCount();
		//cout<<"colorCount="<<colorCount<<endl;
		vector<int> vi_VertexColors;
		g->GetVertexColors(vi_VertexColors);
		
		//Do (column-)color statistic for each row, i.e., see how many elements in that row has color 0, color 1 ...
		int** colorStatistic = new int*[rowCount];	//color statistic for each row. For example, colorStatistic[0] is color statistic for row 0
													//If row 0 has 5 columns with color 3 => colorStatistic[0][3] = 5;
		//Allocate memory for colorStatistic[rowCount][colorCount] and initilize the matrix
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			colorStatistic[i] = new int[colorCount];
			for(unsigned int j=0; j < (unsigned int)colorCount; j++) colorStatistic[i][j] = 0;
		}

		//populate colorStatistic
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			for(unsigned int j=1; j <= (unsigned int)numOfNonZeros; j++) {
				//non-zero in the Hessian: [i][uip2_HessianSparsityPattern[i][j]]
				//color of that column: vi_VertexColors[uip2_HessianSparsityPattern[i][j]]
				colorStatistic[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]]++;
			}
		}

		//allocate memory for *dp3_HessianValue. The dp3_HessianValue and uip2_HessianSparsityPattern matrices should have the same size
		*dp3_HessianValue = new double*[rowCount];
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			unsigned int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			(*dp3_HessianValue)[i] = new double[numOfNonZeros+1];
			(*dp3_HessianValue)[i][0] = numOfNonZeros; //initialize value of the 1st entry
			for(unsigned int j=1; j <= numOfNonZeros; j++) (*dp3_HessianValue)[i][j] = 0.; //initialize value of other entries
		}

		//Now, go to the main part, recover the values of non-zero entries in the Hessian
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			unsigned int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			for(unsigned int j=1; j <= numOfNonZeros; j++) {
				if(i == uip2_HessianSparsityPattern[i][j]) { // the non-zero is in the diagonal of the matrix
					(*dp3_HessianValue)[i][j] = dp2_CompressedMatrix[i][vi_VertexColors[i]];
				}
				else {// i != uip2_HessianSparsityPattern[i][j] // the non-zero is NOT in the diagonal of the matrix
					if(colorStatistic[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]]==1) {
						(*dp3_HessianValue)[i][j] = dp2_CompressedMatrix[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]];
					}
					else {
						(*dp3_HessianValue)[i][j] = dp2_CompressedMatrix[uip2_HessianSparsityPattern[i][j]][vi_VertexColors[i]];
					}
				}
			}
		}

		delete colorStatistic;
		return (_TRUE);
	}


	int HessianRecovery::DirectRecover_CoordinateFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_HessianValue) {
		if(g==NULL) {
			cerr<<"g==NULL"<<endl;
			return _FALSE;
		}

		int rowCount = g->GetVertexCount();
		int colorCount = g->GetVertexColorCount();
		//cout<<"colorCount="<<colorCount<<endl;
		vector<int> vi_VertexColors;
		g->GetVertexColors(vi_VertexColors);
		
		//Do (column-)color statistic for each row, i.e., see how many elements in that row has color 0, color 1 ...
		int** colorStatistic = new int*[rowCount];	//color statistic for each row. For example, colorStatistic[0] is color statistic for row 0
													//If row 0 has 5 columns with color 3 => colorStatistic[0][3] = 5;
		//Allocate memory for colorStatistic[rowCount][colorCount] and initilize the matrix
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			colorStatistic[i] = new int[colorCount];
			for(unsigned int j=0; j < (unsigned int)colorCount; j++) colorStatistic[i][j] = 0;
		}

		//populate colorStatistic
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			for(unsigned int j=1; j <= (unsigned int)numOfNonZeros; j++) {
				//non-zero in the Hessian: [i][uip2_HessianSparsityPattern[i][j]]
				//color of that column: vi_VertexColors[uip2_HessianSparsityPattern[i][j]]
				colorStatistic[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]]++;
			}
		}

		vector<unsigned int> RowIndex;
		vector<unsigned int> ColumnIndex;
		vector<double> HessianValue;

		//Now, go to the main part, recover the values of non-zero entries in the Hessian
		for(unsigned int i=0; i < (unsigned int)rowCount; i++) {
			unsigned int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			for(unsigned int j=1; j <= numOfNonZeros; j++) {
				if(uip2_HessianSparsityPattern[i][j]<i) continue;

				if(i == uip2_HessianSparsityPattern[i][j]) { // the non-zero is in the diagonal of the matrix
					HessianValue.push_back(dp2_CompressedMatrix[i][vi_VertexColors[i]]);
				}
				else {// i != uip2_HessianSparsityPattern[i][j] // the non-zero is NOT in the diagonal of the matrix
					if(colorStatistic[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]]==1) {
						HessianValue.push_back(dp2_CompressedMatrix[i][vi_VertexColors[uip2_HessianSparsityPattern[i][j]]]);
					}
					else {
						HessianValue.push_back(dp2_CompressedMatrix[uip2_HessianSparsityPattern[i][j]][vi_VertexColors[i]]);
					}
				}
				RowIndex.push_back(i);
				ColumnIndex.push_back(uip2_HessianSparsityPattern[i][j]);
			}
		}

		unsigned int numOfNonZeros = RowIndex.size();
		(*ip2_RowIndex) = new unsigned int[numOfNonZeros];
		(*ip2_ColumnIndex) = new unsigned int[numOfNonZeros];
		(*dp2_HessianValue) = new double[numOfNonZeros]; //allocate memory for *dp2_HessianValue.

		for(int i=0; i < numOfNonZeros; i++) {
			(*ip2_RowIndex)[i] = RowIndex[i];
			(*ip2_ColumnIndex)[i] = ColumnIndex[i];
			(*dp2_HessianValue)[i] = HessianValue[i];
		}

		delete colorStatistic;
		return (numOfNonZeros);
	}




//error: vi_IncludedVertices (should be vd_IncludedVertices)
//bug: vli_GroupedInducedVertexDegrees should be push_back at the end instead of push_front
	int HessianRecovery::IndirectRecover_ADOLCFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, double*** dp3_HessianValue) {
//#define DEBUG 5103
		if(g==NULL) {
			cerr<<"g==NULL"<<endl;
			return _FALSE;
		}

		int i=0,j=0;
		int i_VertexCount = _UNKNOWN;
		int i_EdgeID, i_SetID;
		vector<int> vi_Sets;
		map< int, vector<int> > mivi_VertexSets;
		vector<int> vi_Vertices;
		g->GetVertices(vi_Vertices);
		vector<int> vi_Edges;
		g->GetEdges(vi_Edges);
		vector<int> vi_VertexColors;
		g->GetVertexColors(vi_VertexColors);
		map< int, map< int, int> > mimi2_VertexEdgeMap;
		g->GetVertexEdgeMap(mimi2_VertexEdgeMap);
		DisjointSets ds_DisjointSets;
		g->GetDisjointSets(ds_DisjointSets);

		//populate vi_Sets & mivi_VertexSets
		vi_Sets.clear();
		mivi_VertexSets.clear();

		i_VertexCount = g->GetVertexCount();
	       
	//cout<<"**populate vi_Sets & mivi_VertexSets  i_VertexCount ="<<i_VertexCount<<endl;
	//ds_DisjointSets.Print();
		for(i=0; i<i_VertexCount; i++) // for each vertex A (indexed by i)
		{
	//printf("\t i=%d vi_Vertices[i]=%d vi_Vertices[STEP_UP(i)]=%d\n",i,vi_Vertices[i],vi_Vertices[STEP_UP(i)]);
			for(j=vi_Vertices[i]; j<vi_Vertices[STEP_UP(i)]; j++) // for each of the vertex B that connect to A
			{
	//printf("\t\t i=%d vi_Edges[j]=%d\n",i,vi_Edges[j]);
				if(i < vi_Edges[j]) // if the index of A (i) is less than the index of B (vi_Edges[j])
										//basic each edge is represented by (vertex with smaller ID, vertex with larger ID). This way, we don't insert a specific edge twice
				{
					i_EdgeID = mimi2_VertexEdgeMap[i][vi_Edges[j]];

					i_SetID = ds_DisjointSets.FindAndCompress(i_EdgeID);
					
	//printf("\t\t\t i_EdgeID=%d i_SetID=%d\n",i_EdgeID,i_SetID);
					if(i_SetID == i_EdgeID) // that edge is the root of the set => create new set
					{
						vi_Sets.push_back(i_SetID);
					}

					mivi_VertexSets[i_SetID].push_back(i);
					mivi_VertexSets[i_SetID].push_back(vi_Edges[j]);
				}
			}
		}
		//*****DONE

	//cout<<"*1"<<endl;

		int i_MaximumVertexDegree;

		int i_HighestInducedVertexDegree;

		int i_LeafVertex, i_ParentVertex, i_PresentVertex;

		int i_VertexDegree;

		int i_SetCount, i_SetSize;
		//i_SetCount = vi_Sets.size();
		//i_SetSize: size (number of edges?) in a bicolored tree

		double d_Value;

		vector<int> vi_EvaluatedDiagonals;

		vector<int> vi_InducedVertexDegrees;

		vector<double> vd_IncludedVertices;

		vector< vector<int> > v2i_VertexAdjacency;

		vector< vector<double> > v2d_NonzeroAdjacency;

		vector< list<int> > vli_GroupedInducedVertexDegrees;

		vector< list<int>::iterator > vlit_VertexLocations;

		i_MaximumVertexDegree = g->GetMaximumVertexDegree();
	//cout<<"----i_MaximumVertexDegree="<<i_MaximumVertexDegree<<endl;

	    
	#if DEBUG == 5103

		cout<<endl;
		cout<<"DEBUG 5103 | Hessian Evaluation | Bicolored Sets"<<endl;
		cout<<endl;
	    
		i_SetCount = (signed) vi_Sets.size();

		for(i=0; i<i_SetCount; i++)
		{
			cout<<STEP_UP(vi_Sets[i])<<"\t"<<" : ";

			i_SetSize = (signed) mivi_VertexSets[vi_Sets[i]].size();

			for(j=0; j<i_SetSize; j++)
			{
				if(j == STEP_DOWN(i_SetSize))
				{
				cout<<STEP_UP(mivi_VertexSets[vi_Sets[i]][j])<<" ("<<i_SetSize<<")"<<endl;
				}
				else
				{
				cout<<STEP_UP(mivi_VertexSets[vi_Sets[i]][j])<<", ";
				}
			}
		}

		cout<<endl;
		cout<<"[Set Count = "<<i_SetCount<<"]"<<endl;
		cout<<endl;

	#endif

	//cout<<"*4 Start Step 5"<<endl;
		//Step 5: from here on
		i_VertexCount = g->GetVertexCount();

		v2i_VertexAdjacency.clear();
		v2i_VertexAdjacency.resize((unsigned) i_VertexCount);

		v2d_NonzeroAdjacency.clear();
		v2d_NonzeroAdjacency.resize((unsigned) i_VertexCount);

		vi_EvaluatedDiagonals.clear();
		vi_EvaluatedDiagonals.resize((unsigned) i_VertexCount, _FALSE);

		vi_InducedVertexDegrees.clear();
		vi_InducedVertexDegrees.resize((unsigned) i_VertexCount, _FALSE);
	    
		vd_IncludedVertices.clear();
		vd_IncludedVertices.resize((unsigned) i_VertexCount, _UNKNOWN);

		i_ParentVertex = _UNKNOWN;

		i_SetCount = (signed) vi_Sets.size();

	//cout<<"*41 i_SetCount="<<i_SetCount<<endl;
		for(i=0; i<i_SetCount; i++)
		{
			vli_GroupedInducedVertexDegrees.clear();
			vli_GroupedInducedVertexDegrees.resize((unsigned) STEP_UP(i_MaximumVertexDegree));

			vlit_VertexLocations.clear();
			vlit_VertexLocations.resize((unsigned) i_VertexCount);
		
			i_HighestInducedVertexDegree = _UNKNOWN;

			i_SetSize = (signed) mivi_VertexSets[vi_Sets[i]].size();

	//cout<<"*411 i_SetSize="<<i_SetSize<<endl;
			for(j=0; j<i_SetSize; j++)
			{
	//cout<<"*4111 j="<<j<<endl;
				i_PresentVertex = mivi_VertexSets[vi_Sets[i]][j];

				vd_IncludedVertices[i_PresentVertex] = _FALSE;

				if(vi_InducedVertexDegrees[i_PresentVertex] != _FALSE)
				{
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].erase(vlit_VertexLocations[i_PresentVertex]);
				}

	//cout<<"*4112 vi_InducedVertexDegrees.size()="<<vi_InducedVertexDegrees.size()<<endl;
				vi_InducedVertexDegrees[i_PresentVertex]++;

				if(i_HighestInducedVertexDegree < vi_InducedVertexDegrees[i_PresentVertex])
				{
					i_HighestInducedVertexDegree = vi_InducedVertexDegrees[i_PresentVertex];
				}

	//cout<<"*4113 vli_GroupedInducedVertexDegrees.size()="<<vli_GroupedInducedVertexDegrees.size()<<endl;
				vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].push_front(i_PresentVertex);

				vlit_VertexLocations[i_PresentVertex] = vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].begin();
			}

	#if DEBUG == 5103

			int k;

			list<int>::iterator lit_ListIterator;

			cout<<endl;
			cout<<"DEBUG 5103 | Hessian Evaluation | Induced Vertex Degrees | Set "<<STEP_UP(i)<<endl;
			cout<<endl;

			for(j=0; j<STEP_UP(i_HighestInducedVertexDegree); j++)
			{
				i_SetSize = (signed) vli_GroupedInducedVertexDegrees[j].size();
		
				if(i_SetSize == _FALSE)
				{
					continue;
				}

				k = _FALSE;
		    
				cout<<"Degree "<<j<<"\t"<<" : ";

				for(lit_ListIterator=vli_GroupedInducedVertexDegrees[j].begin(); lit_ListIterator!=vli_GroupedInducedVertexDegrees[j].end(); lit_ListIterator++)
				{
					if(k == STEP_DOWN(i_SetSize))
					{
						cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_SetSize<<")"<<endl;
					}
					else
					{
						cout<<STEP_UP(*lit_ListIterator)<<", ";
					}

					k++;
				}
			}

	#endif   

	#if DEBUG == 5103

			cout<<endl;
			cout<<"DEBUG 5103 | Hessian Evaluation | Retrieved Elements"<<"| Set "<<STEP_UP(i)<<endl;
			cout<<endl;

	#endif	
			//get the diagonal values
			for (int index = 0; index < i_VertexCount; index++) {
				if(vi_EvaluatedDiagonals[index] == _FALSE)
				{
					d_Value = dp2_CompressedMatrix[index][vi_VertexColors[index]];

	#if DEBUG == 5103

					cout<<"Element["<<STEP_UP(index)<<"]["<<STEP_UP(index)<<"] = "<<d_Value<<endl;

	#endif
					v2i_VertexAdjacency[index].push_back(index);
					v2d_NonzeroAdjacency[index].push_back(d_Value);

					vi_EvaluatedDiagonals[index] = _TRUE;

				}
			}

			for ( ; ; )
			{
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);
				if(vli_GroupedInducedVertexDegrees[_TRUE].empty()) // If there is no leaf left on the color tree
				{
					i_LeafVertex = vli_GroupedInducedVertexDegrees[_FALSE].front();

					vi_InducedVertexDegrees[i_LeafVertex] = _FALSE;

					vd_IncludedVertices[i_LeafVertex] = _UNKNOWN;

					break;
				}

				i_LeafVertex = vli_GroupedInducedVertexDegrees[_TRUE].front();
		    
				vli_GroupedInducedVertexDegrees[_TRUE].pop_front();
		    
		//printf("*i_LeafVertex=%d \t vi_EvaluatedDiagonals[i_LeafVertex]=%d\n",STEP_UP(i_LeafVertex),vi_EvaluatedDiagonals[i_LeafVertex]);

				//Find i_ParentVertex
				for(j=vi_Vertices[i_LeafVertex]; j<vi_Vertices[STEP_UP(i_LeafVertex)]; j++)
				{
					if(vd_IncludedVertices[vi_Edges[j]] != _UNKNOWN)
					{
						i_ParentVertex = vi_Edges[j];

						break;
					}
				}
		//printf("*i_LeafVertex=%d \t i_ParentVertex=%d\n",STEP_UP(i_LeafVertex),STEP_UP(i_ParentVertex));

				d_Value = dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]] - vd_IncludedVertices[i_LeafVertex];
		//printf("*At the end: d_Value(%f) = dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]](%f) - vd_IncludedVertices[i_LeafVertex](%f);\n",d_Value,dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]],vd_IncludedVertices[i_LeafVertex]);
		//printf("*At the end: d_Value = dp2_CompressedMatrix[%d][vi_VertexColors[%d](%d)] - vd_IncludedVertices[%d];\n",i_LeafVertex,i_ParentVertex,vi_VertexColors[i_ParentVertex],i_LeafVertex);

				vd_IncludedVertices[i_ParentVertex] += d_Value;
		//printf("*At the end: vd_IncludedVertices[i_ParentVertex](%f) += d_Value(%f); \n",vd_IncludedVertices[i_ParentVertex],d_Value);
		
				vi_InducedVertexDegrees[i_LeafVertex] = _FALSE;
				vd_IncludedVertices[i_LeafVertex] = _UNKNOWN;
		//cout<<"*?1"<<endl<<flush;
		//printf("vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex(%d)](%d)].erase(vlit_VertexLocations[i_ParentVertex](%d)); %x\n",STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex],*vlit_VertexLocations[i_ParentVertex],vlit_VertexLocations[i_ParentVertex]);
	//printf("*Remove i_ParentVertex=%d from degree %d\n", STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex]);
				if(vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].size()>1) {
	//printf("*\tIn if, erase\n");
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].erase(vlit_VertexLocations[i_ParentVertex]);
				}
				else {
	//printf("*\tIn else, pop_back\n");
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].pop_back();
				}
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);

				vi_InducedVertexDegrees[i_ParentVertex]--;

	//printf("*Push i_ParentVertex=%d into degree %d\n", STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex]);
				vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].push_back(i_ParentVertex);
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);

				//Update position of the iterator pointing to i_ParentVertex in "InducedVertexDegrees" structure
				vlit_VertexLocations[i_ParentVertex] = vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].end(); 
				--vlit_VertexLocations[i_ParentVertex]; 
		  
				v2i_VertexAdjacency[i_LeafVertex].push_back(i_ParentVertex);
				v2d_NonzeroAdjacency[i_LeafVertex].push_back(d_Value);

				v2i_VertexAdjacency[i_ParentVertex].push_back(i_LeafVertex);
				v2d_NonzeroAdjacency[i_ParentVertex].push_back(d_Value);
		    

	#if DEBUG == 5103

				cout<<"Element["<<STEP_UP(i_LeafVertex)<<"]["<<STEP_UP(i_ParentVertex)<<"] = "<<d_Value<<endl;
	#endif
		    
			}
		}


		//allocate memory for *dp3_HessianValue. The dp3_HessianValue and uip2_HessianSparsityPattern matrices should have the same size
		*dp3_HessianValue = new double*[i_VertexCount];
		for(unsigned int i=0; i < (unsigned int)i_VertexCount; i++) {
			unsigned int numOfNonZeros = uip2_HessianSparsityPattern[i][0];
			(*dp3_HessianValue)[i] = new double[numOfNonZeros+1];
			(*dp3_HessianValue)[i][0] = numOfNonZeros; //initialize value of the 1st entry
			for(unsigned int j=1; j <= numOfNonZeros; j++) (*dp3_HessianValue)[i][j] = 0.; //initialize value of other entries
		}

	//cout<<"*Display displayAdjacencyMatrix(v2i_VertexAdjacency);"<<endl;
		//displayAdjacencyMatrix(v2i_VertexAdjacency);
	//cout<<"*Display displayAdjacencyMatrix(v2d_NonzeroAdjacency);"<<endl;
		//displayAdjacencyMatrix(v2d_NonzeroAdjacency);

	//cout<<"*5"<<endl; 
		// MODIFY THIS PART!!! all I need is the matrix v2i_VertexAdjacency & v2d_NonzeroAdjacency
		// i.e., populate m_vi_HessianVertices, m_vi_HessianEdges, m_vd_HessianValues

		//instead, populate dp3_HessianValue base on uip2_HessianSparsityPattern 
		//i_VertexCount: number of rows (or columns)
		//v2i_VertexAdjacency[i].size() is the number of of non-zero in row i

		//populate dp3_HessianValue row by row, column by column
		for(i=0; i<i_VertexCount; i++) {
			int NumOfNonzeros = uip2_HessianSparsityPattern[i][0];
			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();
	//printf("\ti=%d, \t NumOfNonzeros=%d,\t i_VertexDegree=%d \n",i,NumOfNonzeros,i_VertexDegree);
			for(j=1; j<=NumOfNonzeros; j++) {
				int targetColumnID = uip2_HessianSparsityPattern[i][j];
	//printf("\t j=%d,\t targetColumnID=%d \n",j,targetColumnID);
				for (int k=0; k<i_VertexDegree; k++) {// search through the v2i_VertexAdjacency matrix to find the correct column
	//printf("\t k=%d \n",k);
					if(targetColumnID == v2i_VertexAdjacency[i][k]) { //found it
	//printf("\t Found it!\t v2d_NonzeroAdjacency[i][k]=%f \n",v2d_NonzeroAdjacency[i][k]);
						(*dp3_HessianValue)[i][j] = v2d_NonzeroAdjacency[i][k];
						break;
					}
				}
			}
		}
		/*
		//convert matrix to graph. Used v2i_VertexAdjacency & v2d_NonzeroAdjacency
		m_vi_HessianVertices.clear();
		m_vi_HessianEdges.clear();
		m_vd_HessianValues.clear();

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_HessianVertices.push_back((signed) m_vi_HessianEdges.size());

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_HessianEdges.push_back(v2i_VertexAdjacency[i][j]);

				m_vd_HessianValues.push_back(v2d_NonzeroAdjacency[i][j]);
			}
		}

		m_vi_HessianVertices.push_back((signed) m_vi_HessianEdges.size());
		*/

	#undef DEBUG

		return (_TRUE);
	}


//error: vi_IncludedVertices (should be vd_IncludedVertices)
//bug: vli_GroupedInducedVertexDegrees should be push_back at the end instead of push_front
	int HessianRecovery::IndirectRecover_CoordinateFormat(GraphColoringInterface* g, double** dp2_CompressedMatrix, unsigned int ** uip2_HessianSparsityPattern, unsigned int** ip2_RowIndex, unsigned int** ip2_ColumnIndex, double** dp2_HessianValue) {
//#define DEBUG 5103
		if(g==NULL) {
			cerr<<"g==NULL"<<endl;
			return _FALSE;
		}

		int i=0,j=0;
		int i_VertexCount = _UNKNOWN;
		int i_EdgeID, i_SetID;
		vector<int> vi_Sets;
		map< int, vector<int> > mivi_VertexSets;
		vector<int> vi_Vertices;
		g->GetVertices(vi_Vertices);
		vector<int> vi_Edges;
		g->GetEdges(vi_Edges);
		vector<int> vi_VertexColors;
		g->GetVertexColors(vi_VertexColors);
		map< int, map< int, int> > mimi2_VertexEdgeMap;
		g->GetVertexEdgeMap(mimi2_VertexEdgeMap);
		DisjointSets ds_DisjointSets;
		g->GetDisjointSets(ds_DisjointSets);

		//populate vi_Sets & mivi_VertexSets
		vi_Sets.clear();
		mivi_VertexSets.clear();

		i_VertexCount = g->GetVertexCount();
	       
	//cout<<"**populate vi_Sets & mivi_VertexSets  i_VertexCount ="<<i_VertexCount<<endl;
	//ds_DisjointSets.Print();
		for(i=0; i<i_VertexCount; i++) // for each vertex A (indexed by i)
		{
	//printf("\t i=%d vi_Vertices[i]=%d vi_Vertices[STEP_UP(i)]=%d\n",i,vi_Vertices[i],vi_Vertices[STEP_UP(i)]);
			for(j=vi_Vertices[i]; j<vi_Vertices[STEP_UP(i)]; j++) // for each of the vertex B that connect to A
			{
	//printf("\t\t i=%d vi_Edges[j]=%d\n",i,vi_Edges[j]);
				if(i < vi_Edges[j]) // if the index of A (i) is less than the index of B (vi_Edges[j])
										//basic each edge is represented by (vertex with smaller ID, vertex with larger ID). This way, we don't insert a specific edge twice
				{
					i_EdgeID = mimi2_VertexEdgeMap[i][vi_Edges[j]];

					i_SetID = ds_DisjointSets.FindAndCompress(i_EdgeID);
					
	//printf("\t\t\t i_EdgeID=%d i_SetID=%d\n",i_EdgeID,i_SetID);
					if(i_SetID == i_EdgeID) // that edge is the root of the set => create new set
					{
						vi_Sets.push_back(i_SetID);
					}

					mivi_VertexSets[i_SetID].push_back(i);
					mivi_VertexSets[i_SetID].push_back(vi_Edges[j]);
				}
			}
		}
		//*****DONE

	//cout<<"*1"<<endl;

		int i_MaximumVertexDegree;

		int i_HighestInducedVertexDegree;

		int i_LeafVertex, i_ParentVertex, i_PresentVertex;

		int i_VertexDegree;

		int i_SetCount, i_SetSize;
		//i_SetCount = vi_Sets.size();
		//i_SetSize: size (number of edges?) in a bicolored tree

		double d_Value;

		vector<int> vi_EvaluatedDiagonals;

		vector<int> vi_InducedVertexDegrees;

		vector<double> vd_IncludedVertices;

		vector< vector<int> > v2i_VertexAdjacency;

		vector< vector<double> > v2d_NonzeroAdjacency;

		vector< list<int> > vli_GroupedInducedVertexDegrees;

		vector< list<int>::iterator > vlit_VertexLocations;

		i_MaximumVertexDegree = g->GetMaximumVertexDegree();
	//cout<<"----i_MaximumVertexDegree="<<i_MaximumVertexDegree<<endl;

	    
	#if DEBUG == 5103

		cout<<endl;
		cout<<"DEBUG 5103 | Hessian Evaluation | Bicolored Sets"<<endl;
		cout<<endl;
	    
		i_SetCount = (signed) vi_Sets.size();

		for(i=0; i<i_SetCount; i++)
		{
			cout<<STEP_UP(vi_Sets[i])<<"\t"<<" : ";

			i_SetSize = (signed) mivi_VertexSets[vi_Sets[i]].size();

			for(j=0; j<i_SetSize; j++)
			{
				if(j == STEP_DOWN(i_SetSize))
				{
				cout<<STEP_UP(mivi_VertexSets[vi_Sets[i]][j])<<" ("<<i_SetSize<<")"<<endl;
				}
				else
				{
				cout<<STEP_UP(mivi_VertexSets[vi_Sets[i]][j])<<", ";
				}
			}
		}

		cout<<endl;
		cout<<"[Set Count = "<<i_SetCount<<"]"<<endl;
		cout<<endl;

	#endif

	//cout<<"*4 Start Step 5"<<endl;
		//Step 5: from here on
		i_VertexCount = g->GetVertexCount();

		v2i_VertexAdjacency.clear();
		v2i_VertexAdjacency.resize((unsigned) i_VertexCount);

		v2d_NonzeroAdjacency.clear();
		v2d_NonzeroAdjacency.resize((unsigned) i_VertexCount);

		vi_EvaluatedDiagonals.clear();
		vi_EvaluatedDiagonals.resize((unsigned) i_VertexCount, _FALSE);

		vi_InducedVertexDegrees.clear();
		vi_InducedVertexDegrees.resize((unsigned) i_VertexCount, _FALSE);
	    
		vd_IncludedVertices.clear();
		vd_IncludedVertices.resize((unsigned) i_VertexCount, _UNKNOWN);

		i_ParentVertex = _UNKNOWN;

		i_SetCount = (signed) vi_Sets.size();

	//cout<<"*41 i_SetCount="<<i_SetCount<<endl;
		for(i=0; i<i_SetCount; i++)
		{
			vli_GroupedInducedVertexDegrees.clear();
			vli_GroupedInducedVertexDegrees.resize((unsigned) STEP_UP(i_MaximumVertexDegree));

			vlit_VertexLocations.clear();
			vlit_VertexLocations.resize((unsigned) i_VertexCount);
		
			i_HighestInducedVertexDegree = _UNKNOWN;

			i_SetSize = (signed) mivi_VertexSets[vi_Sets[i]].size();

	//cout<<"*411 i_SetSize="<<i_SetSize<<endl;
			for(j=0; j<i_SetSize; j++)
			{
	//cout<<"*4111 j="<<j<<endl;
				i_PresentVertex = mivi_VertexSets[vi_Sets[i]][j];

				vd_IncludedVertices[i_PresentVertex] = _FALSE;

				if(vi_InducedVertexDegrees[i_PresentVertex] != _FALSE)
				{
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].erase(vlit_VertexLocations[i_PresentVertex]);
				}

	//cout<<"*4112 vi_InducedVertexDegrees.size()="<<vi_InducedVertexDegrees.size()<<endl;
				vi_InducedVertexDegrees[i_PresentVertex]++;

				if(i_HighestInducedVertexDegree < vi_InducedVertexDegrees[i_PresentVertex])
				{
					i_HighestInducedVertexDegree = vi_InducedVertexDegrees[i_PresentVertex];
				}

	//cout<<"*4113 vli_GroupedInducedVertexDegrees.size()="<<vli_GroupedInducedVertexDegrees.size()<<endl;
				vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].push_front(i_PresentVertex);

				vlit_VertexLocations[i_PresentVertex] = vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_PresentVertex]].begin();
			}

	#if DEBUG == 5103

			int k;

			list<int>::iterator lit_ListIterator;

			cout<<endl;
			cout<<"DEBUG 5103 | Hessian Evaluation | Induced Vertex Degrees | Set "<<STEP_UP(i)<<endl;
			cout<<endl;

			for(j=0; j<STEP_UP(i_HighestInducedVertexDegree); j++)
			{
				i_SetSize = (signed) vli_GroupedInducedVertexDegrees[j].size();
		
				if(i_SetSize == _FALSE)
				{
					continue;
				}

				k = _FALSE;
		    
				cout<<"Degree "<<j<<"\t"<<" : ";

				for(lit_ListIterator=vli_GroupedInducedVertexDegrees[j].begin(); lit_ListIterator!=vli_GroupedInducedVertexDegrees[j].end(); lit_ListIterator++)
				{
					if(k == STEP_DOWN(i_SetSize))
					{
						cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_SetSize<<")"<<endl;
					}
					else
					{
						cout<<STEP_UP(*lit_ListIterator)<<", ";
					}

					k++;
				}
			}

	#endif   

	#if DEBUG == 5103

			cout<<endl;
			cout<<"DEBUG 5103 | Hessian Evaluation | Retrieved Elements"<<"| Set "<<STEP_UP(i)<<endl;
			cout<<endl;

	#endif	
			//get the diagonal values
			for (int index = 0; index < i_VertexCount; index++) {
				if(vi_EvaluatedDiagonals[index] == _FALSE)
				{
					d_Value = dp2_CompressedMatrix[index][vi_VertexColors[index]];

	#if DEBUG == 5103

					cout<<"Element["<<STEP_UP(index)<<"]["<<STEP_UP(index)<<"] = "<<d_Value<<endl;

	#endif
					v2i_VertexAdjacency[index].push_back(index);
					v2d_NonzeroAdjacency[index].push_back(d_Value);

					vi_EvaluatedDiagonals[index] = _TRUE;

				}
			}

			for ( ; ; )
			{
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);
				if(vli_GroupedInducedVertexDegrees[_TRUE].empty()) // If there is no leaf left on the color tree
				{
					i_LeafVertex = vli_GroupedInducedVertexDegrees[_FALSE].front();

					vi_InducedVertexDegrees[i_LeafVertex] = _FALSE;

					vd_IncludedVertices[i_LeafVertex] = _UNKNOWN;

					break;
				}

				i_LeafVertex = vli_GroupedInducedVertexDegrees[_TRUE].front();
		    
				vli_GroupedInducedVertexDegrees[_TRUE].pop_front();
		    
		//printf("*i_LeafVertex=%d \t vi_EvaluatedDiagonals[i_LeafVertex]=%d\n",STEP_UP(i_LeafVertex),vi_EvaluatedDiagonals[i_LeafVertex]);

				//Find i_ParentVertex
				for(j=vi_Vertices[i_LeafVertex]; j<vi_Vertices[STEP_UP(i_LeafVertex)]; j++)
				{
					if(vd_IncludedVertices[vi_Edges[j]] != _UNKNOWN)
					{
						i_ParentVertex = vi_Edges[j];

						break;
					}
				}
		//printf("*i_LeafVertex=%d \t i_ParentVertex=%d\n",STEP_UP(i_LeafVertex),STEP_UP(i_ParentVertex));

				d_Value = dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]] - vd_IncludedVertices[i_LeafVertex];
		//printf("*At the end: d_Value(%f) = dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]](%f) - vd_IncludedVertices[i_LeafVertex](%f);\n",d_Value,dp2_CompressedMatrix[i_LeafVertex][vi_VertexColors[i_ParentVertex]],vd_IncludedVertices[i_LeafVertex]);
		//printf("*At the end: d_Value = dp2_CompressedMatrix[%d][vi_VertexColors[%d](%d)] - vd_IncludedVertices[%d];\n",i_LeafVertex,i_ParentVertex,vi_VertexColors[i_ParentVertex],i_LeafVertex);

				vd_IncludedVertices[i_ParentVertex] += d_Value;
		//printf("*At the end: vd_IncludedVertices[i_ParentVertex](%f) += d_Value(%f); \n",vd_IncludedVertices[i_ParentVertex],d_Value);
		
				vi_InducedVertexDegrees[i_LeafVertex] = _FALSE;
				vd_IncludedVertices[i_LeafVertex] = _UNKNOWN;
		//cout<<"*?1"<<endl<<flush;
		//printf("vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex(%d)](%d)].erase(vlit_VertexLocations[i_ParentVertex](%d)); %x\n",STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex],*vlit_VertexLocations[i_ParentVertex],vlit_VertexLocations[i_ParentVertex]);
	//printf("*Remove i_ParentVertex=%d from degree %d\n", STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex]);
				if(vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].size()>1) {
	//printf("*\tIn if, erase\n");
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].erase(vlit_VertexLocations[i_ParentVertex]);
				}
				else {
	//printf("*\tIn else, pop_back\n");
					vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].pop_back();
				}
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);

				vi_InducedVertexDegrees[i_ParentVertex]--;

	//printf("*Push i_ParentVertex=%d into degree %d\n", STEP_UP(i_ParentVertex),vi_InducedVertexDegrees[i_ParentVertex]);
				vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].push_back(i_ParentVertex);
	//PrintInducedVertexDegrees(i, i_HighestInducedVertexDegree, vli_GroupedInducedVertexDegrees);

				//Update position of the iterator pointing to i_ParentVertex in "InducedVertexDegrees" structure
				vlit_VertexLocations[i_ParentVertex] = vli_GroupedInducedVertexDegrees[vi_InducedVertexDegrees[i_ParentVertex]].end(); 
				--vlit_VertexLocations[i_ParentVertex]; 
		  
				v2i_VertexAdjacency[i_LeafVertex].push_back(i_ParentVertex);
				v2d_NonzeroAdjacency[i_LeafVertex].push_back(d_Value);

				v2i_VertexAdjacency[i_ParentVertex].push_back(i_LeafVertex);
				v2d_NonzeroAdjacency[i_ParentVertex].push_back(d_Value);
		    

	#if DEBUG == 5103

				cout<<"Element["<<STEP_UP(i_LeafVertex)<<"]["<<STEP_UP(i_ParentVertex)<<"] = "<<d_Value<<endl;
	#endif
		    
			}
		}

		vector<unsigned int> RowIndex;
		vector<unsigned int> ColumnIndex;
		vector<double> HessianValue;

	//cout<<"*Display displayAdjacencyMatrix(v2i_VertexAdjacency);"<<endl;
		//displayAdjacencyMatrix(v2i_VertexAdjacency);
	//cout<<"*Display displayAdjacencyMatrix(v2d_NonzeroAdjacency);"<<endl;
		//displayAdjacencyMatrix(v2d_NonzeroAdjacency);

	//cout<<"*5"<<endl; 
		// MODIFY THIS PART!!! all I need is the matrix v2i_VertexAdjacency & v2d_NonzeroAdjacency
		// i.e., populate m_vi_HessianVertices, m_vi_HessianEdges, m_vd_HessianValues

		//instead, populate dp3_HessianValue base on uip2_HessianSparsityPattern 
		//i_VertexCount: number of rows (or columns)
		//v2i_VertexAdjacency[i].size() is the number of of non-zero in row i

		//populate dp3_HessianValue row by row, column by column
		for(i=0; i<i_VertexCount; i++) {
			int NumOfNonzeros = uip2_HessianSparsityPattern[i][0];
			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();
	//printf("\ti=%d, \t NumOfNonzeros=%d,\t i_VertexDegree=%d \n",i,NumOfNonzeros,i_VertexDegree);
			for(j=1; j<=NumOfNonzeros; j++) {
				int targetColumnID = uip2_HessianSparsityPattern[i][j];
				if(targetColumnID<i) continue;
	//printf("\t j=%d,\t targetColumnID=%d \n",j,targetColumnID);
				for (int k=0; k<i_VertexDegree; k++) {// search through the v2i_VertexAdjacency matrix to find the correct column
	//printf("\t k=%d \n",k);
					if(targetColumnID == v2i_VertexAdjacency[i][k]) { //found it
	//printf("\t Found it!\t v2d_NonzeroAdjacency[i][k]=%f \n",v2d_NonzeroAdjacency[i][k]);
						HessianValue.push_back(v2d_NonzeroAdjacency[i][k]);
						break;
					}
				}
				RowIndex.push_back(i);
				ColumnIndex.push_back(uip2_HessianSparsityPattern[i][j]);
			}
		}

		unsigned int numOfNonZeros = RowIndex.size();
		(*ip2_RowIndex) = new unsigned int[numOfNonZeros];
		(*ip2_ColumnIndex) = new unsigned int[numOfNonZeros];
		(*dp2_HessianValue) = new double[numOfNonZeros]; //allocate memory for *dp2_HessianValue.

		for(int i=0; i < numOfNonZeros; i++) {
			(*ip2_RowIndex)[i] = RowIndex[i];
			(*ip2_ColumnIndex)[i] = ColumnIndex[i];
			(*dp2_HessianValue)[i] = HessianValue[i];
		}

		return (numOfNonZeros);
	}
}
