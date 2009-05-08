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
	//Private Function 1501
	int GraphOrdering::OrderVertices(string s_OrderingVariant)
	{
		s_OrderingVariant = toUpper(s_OrderingVariant);

		if((s_OrderingVariant.compare("NATURAL") == 0))
		{
			return(NaturalOrdering());
		}
		else
		if((s_OrderingVariant.compare("LARGEST_FIRST") == 0))
		{
			return(LargestFirstOrdering());
		}
		else
		if((s_OrderingVariant.compare("DYNAMIC_LARGEST_FIRST") == 0))
		{
			return(DynamicLargestFirstOrdering());
		}
		else
		if((s_OrderingVariant.compare("DISTANCE_TWO_LARGEST_FIRST") == 0))
		{
			return(DistanceTwoLargestFirstOrdering());
		}
		else
		if((s_OrderingVariant.compare("SMALLEST_LAST") == 0))
		{
			return(SmallestLastOrdering());
		}
		else
		if((s_OrderingVariant.compare("DISTANCE_TWO_SMALLEST_LAST") == 0))
		{
			return(DistanceTwoSmallestLastOrdering());
		}
		else
		if((s_OrderingVariant.compare("INCIDENCE_DEGREE") == 0))
		{
			return(IncidenceDegreeOrdering());
		}
		else
		if((s_OrderingVariant.compare("DISTANCE_TWO_INCIDENCE_DEGREE") == 0))
		{
			return(DistanceTwoIncidenceDegreeOrdering());
		}
		else
		{
			cerr<<endl;
			cerr<<"Unknown Ordering Method";
			cerr<<endl;
		}

		return(_TRUE);
	}

	//Private Function 1301
	int GraphOrdering::CheckVertexOrdering(string s_VertexOrderingVariant)
	{
		if(m_s_VertexOrderingVariant.compare(s_VertexOrderingVariant) == 0)
		{
			return(_TRUE);
		}

		if(m_s_VertexOrderingVariant.compare("ALL") != 0)
		{
			m_s_VertexOrderingVariant = s_VertexOrderingVariant;
		}

		return(_FALSE);
	}

	//Public Constructor 1351
	GraphOrdering::GraphOrdering()
	{
		Clear();

		GraphInputOutput::GraphInputOutput();
	}

	
	//Public Destructor 1352
	GraphOrdering::~GraphOrdering()
	{
		Clear();
	}

	
	//Virtual Function 1353
	void GraphOrdering::Clear()
	{
		m_d_OrderingTime = _UNKNOWN;

		m_s_VertexOrderingVariant.clear();
		m_vi_OrderedVertices.clear();

		GraphCore::Clear();

		return;
	}

	
	//Public Function 1354
	int GraphOrdering::NaturalOrdering()
	{
		if(CheckVertexOrdering("NATURAL") == _TRUE)
		{
			return(_TRUE);
		}
		
		int i;

		int i_VertexCount;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		m_vi_OrderedVertices.clear();

		m_vi_OrderedVertices.resize((unsigned) i_VertexCount);

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_OrderedVertices[i] = i;
		}

		return(_TRUE);
	}

	
	//Public Function 1355
	int GraphOrdering::LargestFirstOrdering()
	{
		if(CheckVertexOrdering("LARGEST FIRST") == _TRUE)
		{
			return(_TRUE);
		}

		int i, j;

		int i_VertexCount;
	    
		int i_VertexDegree, i_VertexDegreeCount;

		vector< vector<int> > v2i_GroupedVertexDegree;

		m_i_MaximumVertexDegree = _FALSE;
	    
		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		v2i_GroupedVertexDegree.resize((unsigned) i_VertexCount);

		for(i=0; i<i_VertexCount; i++)
		{
			i_VertexDegree = m_vi_Vertices[STEP_UP(i)] - m_vi_Vertices[i];

			v2i_GroupedVertexDegree[i_VertexDegree].push_back(i);

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
			}
		}

		m_vi_OrderedVertices.clear();

		for(i=m_i_MaximumVertexDegree; i>=0; i--)
		{
			i_VertexDegreeCount = (signed) v2i_GroupedVertexDegree[i].size();

			for(j=0; j<i_VertexDegreeCount; j++)
			{
				m_vi_OrderedVertices.push_back(v2i_GroupedVertexDegree[i][j]);
			}
		}
		
		return(_TRUE);
	}

	int GraphOrdering::printVertexEdgeMap(vector< vector< pair< int, int> > > &vvpii_VertexEdgeMap) {
		ostringstream oout;
		string tempS;
		cout<<"vvpii_VertexEdgeMap.size() = "<<vvpii_VertexEdgeMap.size()<<endl;

		for(int i=0; i<vvpii_VertexEdgeMap.size(); i++) {
			cout<<'['<<setw(4)<<i<<']';
			for(int j=0; j< vvpii_VertexEdgeMap[i].size(); j++) {
				oout.str("");
				oout << '(' << vvpii_VertexEdgeMap[i][j].first << ", " << vvpii_VertexEdgeMap[i][j].second << ')';
				tempS = oout.str();
				cout<<setw(10)<<tempS;
				if(j%5 == 4 && j != vvpii_VertexEdgeMap[i].size() - 1) cout<<endl<<setw(6)<<' ';
			}
			cout<<endl;
		}

		cout<<"*****************"<<endl;

		return _TRUE;
	}

	struct less_degree_than {
		bool operator()(const pair< int, int> *a, const pair< int, int> *b) const {
			//Compare induced degree of a and b
			if(a->second < b->second) return true;
			if(a->second > b->second) return false;
			//a->second == b->second, now use the vertex ID itself to decide the result
			// Higher ID will be consider to be smaller.
			return (a->first > b->first);
		}
	};
	
	//Public Function 1356
	int GraphOrdering::DynamicLargestFirstOrdering() {
		if(CheckVertexOrdering("DYNAMIC LARGEST FIRST") == _TRUE)
		{
			return(_TRUE);
		}

		int i;
	   
		int i_HighestInducedVertexDegree;

		int i_VertexCount, i_InducedVertexDegree;
	    
		int i_InducedVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_InducedVertexDegree;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		vi_InducedVertexDegree.clear();

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_VertexCount);

		vlit_VertexLocation.clear();
	    
		i_SelectedVertex = _UNKNOWN;

		i_HighestInducedVertexDegree = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			//get vertex degree for each vertex
			i_InducedVertexDegree = m_vi_Vertices[STEP_UP(i)] - m_vi_Vertices[i];
			
			//vi_InducedVertexDegree[i] = vertex degree of vertex i
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			// list vli_GroupedInducedVertexDegree[i] = all the vertices with degree i
			// for every new vertex with degree i, it will be pushed to the front of list vli_GroupedInducedVertexDegree[i]
			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			//vlit_VertexLocation[i] = location of vertex i in vli_GroupedInducedVertexDegree[i_InducedVertexDegree]
			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			//get max degree (i_HighestInducedVertexDegree)
			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}

#if DEBUG == 1358

		int j;

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 1358 | Vertex Ordering | Vertex Degree"<<endl;
		cout<<endl;

		for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
		{
			cout<<"Degree "<<i<<"\t"<<" : ";

			i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();
			 
			j = _FALSE;
			
			for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
			{
				if(j==STEP_DOWN(i_InducedVertexDegreeCount))
				{
					cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_InducedVertexDegreeCount<<")";
				}
				else
				{
					cout<<STEP_UP(*lit_ListIterator)<<", ";
				}

				j++;
			}

			cout<<endl;
		}

		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		// just counting the number of vertices that we have worked with, 
		// stop when i_SelectedVertexCount == i_VertexCount, i.e. we have looked through all the vertices
		while(i_SelectedVertexCount < i_VertexCount) 
		{
			//pick the vertex with largest degree
			for(i = i_HighestInducedVertexDegree; i >= 0; i--)
			{
				i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();

				if(i_InducedVertexDegreeCount != _FALSE)
				{
					i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();

					break;
				}
			}	

			//for every D1 neighbor of the i_SelectedVertex, decrease their degree by one and then update their position in vli_GroupedInducedVertexDegree
			for(i=m_vi_Vertices[i_SelectedVertex]; i<m_vi_Vertices[STEP_UP(i_SelectedVertex)]; i++)
			{
				if(vi_InducedVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
				{
					continue;
				}

				vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);

				vi_InducedVertexDegree[m_vi_Edges[i]]--;

				vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
			    
				vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].begin();
			    
			}

			//remove the i_SelectedVertex from vli_GroupedInducedVertexDegree
			vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			//Mark the i_SelectedVertex as read (_UNKNOWN), so that we don't look at it again
			vi_InducedVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			//Select the vertex by pushing it to the end of m_vi_OrderedVertices
			m_vi_OrderedVertices.push_back(i_SelectedVertex);

			//increment i_SelectedVertexCount
			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}


#if DEBUG == 1358

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 1358 | Vertex Ordering | Smallest Last"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_VertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}
		/*
	int GraphOrdering::DynamicLargestFirstOrdering()
	{
		if(CheckVertexOrdering("LARGEST FIRST") == _TRUE)
		{
			return(_TRUE);
		}

		m_vi_OrderedVertices.clear();

		int i_VertexCount = m_vi_Vertices.size() - 1;
		int i_D1Neighbor = _UNKNOWN;

		cout<<"i_VertexCount = "<<i_VertexCount<<endl;

		pair< int, int> p_NeighborAndIndex;
		p_NeighborAndIndex.first = _UNKNOWN; // The neighbor vertex that the current vertex connected to
		p_NeighborAndIndex.second = _UNKNOWN; // Index (Place) of the pair where that neighbor point back to the current vertex

		// vvpii_VertexEdgeMap[1][2] = {4,5} means (1,4) is the edge and vvpii_VertexEdgeMap[4][5] = {1,2};
		// Reset the size of vvpii_VertexEdgeMap to be the number of vertices
		vector< vector< pair< int, int> > > vvpii_VertexEdgeMap(i_VertexCount);

		//For each edge in the graph, populate vvpii_VertexEdgeMap
		for(int i=0; i <  i_VertexCount; i++) {
			for(int j = m_vi_Vertices[i]; j < m_vi_Vertices[i+1]; j++) {
				if(m_vi_Edges[j] > i) {
					i_D1Neighbor = m_vi_Edges[j];

					p_NeighborAndIndex.first = i_D1Neighbor;
					p_NeighborAndIndex.second = vvpii_VertexEdgeMap[i_D1Neighbor].size();
					vvpii_VertexEdgeMap[i].push_back(p_NeighborAndIndex);

					p_NeighborAndIndex.first = i;
					p_NeighborAndIndex.second = vvpii_VertexEdgeMap[i].size() - 1;
					vvpii_VertexEdgeMap[i_D1Neighbor].push_back(p_NeighborAndIndex);
				}
			}
		}

		printVertexEdgeMap(vvpii_VertexEdgeMap);
		Pause();

		pair< int, int> p_VertexAndInducedDegree;
		vector< pair< int, int>> vpii_ListOfVertexAndInducedDegree(i_VertexCount);
		priority_queue< pair< int, int>*, 
			vector< pair< int, int>* >, 
			less_degree_than > hpii_VertexHeap;

		for(int i = 0; i < i_VertexCount; i++) {
			p_VertexAndInducedDegree.first = i;
			p_VertexAndInducedDegree.second = vvpii_VertexEdgeMap[i].size();
			vpii_ListOfVertexAndInducedDegree[i] = p_VertexAndInducedDegree;
			hpii_VertexHeap.push(&vpii_ListOfVertexAndInducedDegree[i]);
		}

		cout<<"The order is: ";
		while(!hpii_VertexHeap.empty()) {
			p_VertexAndInducedDegree = *hpii_VertexHeap.top();
			hpii_VertexHeap.pop();
			cout << '(' << setw(4) << p_VertexAndInducedDegree.first 
				<< ", " << setw(4) << p_VertexAndInducedDegree.second << ")\t";
		}
		cout<<endl;
		Pause();

		//Now do the ordering
		//remember not to pop_back any vertices, just reset them to (-1, -1)
		for(int i = 0; i < i_VertexCount; i++) {
			p_VertexAndInducedDegree = *hpii_VertexHeap.top();
			//...
			m_vi_OrderedVertices.push_back(p_VertexAndInducedDegree.first);
			hpii_VertexHeap.pop();
		}
		//NEED TO CREATE A HEAP STRUCTURE JUST FOR THIS PROBLEM

		return(_TRUE);
	}
	//*/
	
	//Public Function 1357
	int GraphOrdering::DistanceTwoLargestFirstOrdering()
	{
		if(CheckVertexOrdering("DISTANCE TWO LARGEST FIRST") == _TRUE)
		{
			return(_TRUE);
		}
			
		int i, j, k;

		int i_VertexCount;

		int i_HighestDistanceTwoVertexDegree;
	    
		int i_DistanceTwoVertexDegree, i_DistanceTwoVertexDegreeCount;

		vector<int> vi_IncludedVertices;

		vector< vector<int> > v2i_GroupedDistanceTwoVertexDegree;

		i_HighestDistanceTwoVertexDegree = _FALSE;
	    
		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		v2i_GroupedDistanceTwoVertexDegree.resize((unsigned) i_VertexCount);
	    
		for(i=0; i<i_VertexCount; i++)
		{
			vi_IncludedVertices.clear();
			vi_IncludedVertices.resize((unsigned) i_VertexCount, _FALSE);

			vi_IncludedVertices[i] = _TRUE;

			i_DistanceTwoVertexDegree = _FALSE;
			
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(vi_IncludedVertices[m_vi_Edges[j]] == _FALSE)
				{
				i_DistanceTwoVertexDegree++;

				vi_IncludedVertices[m_vi_Edges[j]] = _TRUE;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(vi_IncludedVertices[m_vi_Edges[k]] == _FALSE)
					{
						i_DistanceTwoVertexDegree++;

						vi_IncludedVertices[m_vi_Edges[k]] = _TRUE;
					}
				}
			}

			v2i_GroupedDistanceTwoVertexDegree[i_DistanceTwoVertexDegree].push_back(i);

			if(i_HighestDistanceTwoVertexDegree < i_DistanceTwoVertexDegree)
			{
				i_HighestDistanceTwoVertexDegree = i_DistanceTwoVertexDegree;
			}
		}

		m_vi_OrderedVertices.clear();

		for(i=i_HighestDistanceTwoVertexDegree; i>=0; i--)
		{
			i_DistanceTwoVertexDegreeCount = (signed) v2i_GroupedDistanceTwoVertexDegree[i].size();

			for(j=0; j<i_DistanceTwoVertexDegreeCount; j++)
			{
				m_vi_OrderedVertices.push_back(v2i_GroupedDistanceTwoVertexDegree[i][j]);
			}
		}
		
		return(_TRUE);
	}

	
	//Public Function 1358
	int GraphOrdering::SmallestLastOrdering()
	{
		if(CheckVertexOrdering("SMALLEST LAST") == _TRUE)
		{
			return(_TRUE);
		}

		int i;
	   
		int i_HighestInducedVertexDegree;

		int i_VertexCount, i_InducedVertexDegree;

		int i_VertexCountMinus1; // = i_VertexCount - 1, used when inserting selected vertices into m_vi_OrderedVertices 
	    
		int i_InducedVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_InducedVertexDegree;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		i_VertexCountMinus1 = i_VertexCount - 1;

		vi_InducedVertexDegree.clear();

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_VertexCount);

		vlit_VertexLocation.clear();
	    
		i_SelectedVertex = _UNKNOWN;

		i_HighestInducedVertexDegree = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			//get vertex degree for each vertex
			i_InducedVertexDegree = m_vi_Vertices[STEP_UP(i)] - m_vi_Vertices[i];
			
			//vi_InducedVertexDegree[i] = vertex degree of vertex i
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			// list vli_GroupedInducedVertexDegree[i] = all the vertices with degree i
			// for every new vertex with degree i, it will be pushed to the front of list vli_GroupedInducedVertexDegree[i]
			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			//vlit_VertexLocation[i] = location of vertex i in vli_GroupedInducedVertexDegree[i_InducedVertexDegree]
			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			//get max degree (i_HighestInducedVertexDegree)
			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}

#if DEBUG == 1358

		int j;

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 1358 | Vertex Ordering | Vertex Degree"<<endl;
		cout<<endl;

		for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
		{
			cout<<"Degree "<<i<<"\t"<<" : ";

			i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();
			 
			j = _FALSE;
			
			for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
			{
				if(j==STEP_DOWN(i_InducedVertexDegreeCount))
				{
					cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_InducedVertexDegreeCount<<")";
				}
				else
				{
					cout<<STEP_UP(*lit_ListIterator)<<", ";
				}

				j++;
			}

			cout<<endl;
		}

		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();
		m_vi_OrderedVertices.resize(i_VertexCount, _UNKNOWN);

		i_SelectedVertexCount = _FALSE;

		// just counting the number of vertices that we have worked with, 
		// stop when i_SelectedVertexCount == i_VertexCount, i.e. we have looked through all the vertices
		while(i_SelectedVertexCount < i_VertexCount) 
		{
			//pick the vertex with smallest degree
			for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
			{
				i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();

				if(i_InducedVertexDegreeCount != _FALSE)
				{
					i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();

					break;
				}
			}	

			//for every D1 neighbor of the i_SelectedVertex, decrease their degree by one and then update their position in vli_GroupedInducedVertexDegree
			for(i=m_vi_Vertices[i_SelectedVertex]; i<m_vi_Vertices[STEP_UP(i_SelectedVertex)]; i++)
			{
				if(vi_InducedVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
				{
					continue;
				}

				vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);

				vi_InducedVertexDegree[m_vi_Edges[i]]--;

				vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
			    
				vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].begin();
			    
			}

			//remove the i_SelectedVertex from vli_GroupedInducedVertexDegree
			vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			//Mark the i_SelectedVertex as read, so that we don't look at it again
			vi_InducedVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			//insert at the beginning (push_front)? BAD!!!! due to moving
			//m_vi_OrderedVertices.insert(m_vi_OrderedVertices.begin(), i_SelectedVertex);

			//Alternative method for the above (commented) line
//printf("m_vi_OrderedVertices[i_VertexCountMinus1 (%d) - i_SelectedVertexCount (%d)] (%d) = i_SelectedVertex (%d) \n", i_VertexCountMinus1, i_SelectedVertexCount, i_VertexCountMinus1 - i_SelectedVertexCount, i_SelectedVertex);
			m_vi_OrderedVertices[i_VertexCountMinus1 - i_SelectedVertexCount] = i_SelectedVertex;

			//increment i_SelectedVertexCount
			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}


#if DEBUG == 1358

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 1358 | Vertex Ordering | Smallest Last"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_VertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 1359
	int GraphOrdering::DistanceTwoSmallestLastOrdering()
	{
		if(CheckVertexOrdering("DISTANCE TWO SMALLEST LAST") == _TRUE)
		{
			return(_TRUE);
		}

		int i, j, k;
	   
		int i_HighestInducedVertexDegree;

		int i_VertexCount, i_InducedVertexDegree;
	    
		int i_InducedVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_IncludedVertices;

		vector<int> vi_InducedVertexDegrees;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocations;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		vi_InducedVertexDegrees.clear();

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_VertexCount);

		vlit_VertexLocations.clear();
	    
		i_SelectedVertex = _UNKNOWN;

		i_HighestInducedVertexDegree = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			vi_IncludedVertices.clear();
			vi_IncludedVertices.resize((unsigned) i_VertexCount, _FALSE);

			vi_IncludedVertices[i] = _TRUE;

			i_InducedVertexDegree = _FALSE;
			
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(vi_IncludedVertices[m_vi_Edges[j]] == _FALSE)
				{
					i_InducedVertexDegree++;

					vi_IncludedVertices[m_vi_Edges[j]] = _TRUE;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(vi_IncludedVertices[m_vi_Edges[k]] == _FALSE)
					{
						i_InducedVertexDegree++;

						vi_IncludedVertices[m_vi_Edges[k]] = _TRUE;
					}
				}
			}

			vi_InducedVertexDegrees.push_back(i_InducedVertexDegree);

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			vlit_VertexLocations.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}


#if DEBUG == 1359

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 1359 | Vertex Ordering | Vertex Degree"<<endl;
		cout<<endl;

		for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
		{
			cout<<"Degree "<<i<<"\t"<<" : ";

			i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();
			 
			j = _FALSE;
			
			for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
			{
				if(j==STEP_DOWN(i_InducedVertexDegreeCount))
				{
					cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_InducedVertexDegreeCount<<")";
				}
				else
				{
					cout<<STEP_UP(*lit_ListIterator)<<", ";
				}

				j++;
			}

			cout<<endl;
		}

		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_VertexCount)
		{
			for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
			{
				i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();

				if(i_InducedVertexDegreeCount != _FALSE)
				{
					i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();

					break;
				}
			}	

			vi_IncludedVertices.clear();
			vi_IncludedVertices.resize((unsigned) i_VertexCount, _FALSE);
			    
			vi_IncludedVertices[i_SelectedVertex] = _TRUE;

			for(i=m_vi_Vertices[i_SelectedVertex]; i<m_vi_Vertices[STEP_UP(i_SelectedVertex)]; i++)
			{
				if(vi_InducedVertexDegrees[m_vi_Edges[i]] == _UNKNOWN)
				{
					continue;
				}
			    
				if(vi_IncludedVertices[m_vi_Edges[i]] == _FALSE)
				{
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[i]]].erase(vlit_VertexLocations[m_vi_Edges[i]]);
					
					vi_InducedVertexDegrees[m_vi_Edges[i]] = STEP_DOWN(vi_InducedVertexDegrees[m_vi_Edges[i]]);
					
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
					
					vlit_VertexLocations[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[i]]].begin();
					
					vi_IncludedVertices[m_vi_Edges[i]] = _TRUE;
				}

				for(j=m_vi_Vertices[m_vi_Edges[i]]; j<m_vi_Vertices[STEP_UP(m_vi_Edges[i])]; j++)
				{
					if(vi_InducedVertexDegrees[m_vi_Edges[j]] == _UNKNOWN)
					{
						continue;
					}

					if(vi_IncludedVertices[m_vi_Edges[j]] == _FALSE)
					{
						vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[j]]].erase(vlit_VertexLocations[m_vi_Edges[j]]);
					    
						vi_InducedVertexDegrees[m_vi_Edges[j]] = STEP_DOWN(vi_InducedVertexDegrees[m_vi_Edges[j]]);
					    
						vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[j]]].push_front(m_vi_Edges[j]);
					    
						vlit_VertexLocations[m_vi_Edges[j]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[m_vi_Edges[j]]].begin();

						vi_IncludedVertices[m_vi_Edges[j]] = _TRUE;
					}
				}
			}

			vli_GroupedInducedVertexDegree[vi_InducedVertexDegrees[i_SelectedVertex]].erase(vlit_VertexLocations[i_SelectedVertex]);

			vi_InducedVertexDegrees[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.insert(m_vi_OrderedVertices.begin(), i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}

#if DEBUG == 1359

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 1359 | Vertex Ordering | Smallest Last"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_VertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 1360
	int GraphOrdering::IncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("INCIDENCE DEGREE") == _TRUE)
		{
			return(_TRUE);
		}

		int i;
	   
		int i_HighestDegreeVertex, m_i_MaximumVertexDegree;

		int i_VertexCount, i_VertexDegree;

		int i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_IncidenceVertexDegree;

		vector< list<int> > vli_GroupedIncidenceVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		vi_IncidenceVertexDegree.clear();

		vli_GroupedIncidenceVertexDegree.clear();
		vli_GroupedIncidenceVertexDegree.resize((unsigned) i_VertexCount);

		vlit_VertexLocation.clear();
	    
		i_HighestDegreeVertex = m_i_MaximumVertexDegree = _UNKNOWN;

		i_SelectedVertex = _UNKNOWN;

		i_IncidenceVertexDegree = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{		
			vi_IncidenceVertexDegree.push_back(i_IncidenceVertexDegree);

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i);

			vlit_VertexLocation.push_back(vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin());

			i_VertexDegree = m_vi_Vertices[STEP_UP(i)] - m_vi_Vertices[i];

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;

				i_HighestDegreeVertex = i;
			}
		}

#if DEBUG == 1360

		int j;

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 1360 | Vertex Ordering | Vertex Degree"<<endl;
		cout<<endl;

		for(i=m_i_MaximumVertexDegree; i>=0; i--)
		{
			cout<<"Degree "<<i<<"\t"<<" : ";

			i_IncidenceVertexDegreeCount = (signed) vli_GroupedIncidenceVertexDegree[i].size();
			 
			j = _FALSE;
			
			for(lit_ListIterator = vli_GroupedIncidenceVertexDegree[i].begin(); lit_ListIterator != vli_GroupedIncidenceVertexDegree[i].end(); lit_ListIterator++)
			{
				if(j==STEP_DOWN(i_IncidenceVertexDegreeCount))
				{
					cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_IncidenceVertexDegreeCount<<")";
				}
				else
				{
					cout<<STEP_UP(*lit_ListIterator)<<", ";
				}

				j++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Highest Degree Vertex = "<<STEP_UP(i_HighestDegreeVertex)<<"; Highest Vertex Degree = "<<m_i_MaximumVertexDegree<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_VertexCount)
		{
			if(i_SelectedVertexCount == _FALSE)
			{
				i_SelectedVertex = i_HighestDegreeVertex;
			}
			else
			{
				for(i=m_i_MaximumVertexDegree; i>=0; i--)
				{
					i_IncidenceVertexDegreeCount = (signed) vli_GroupedIncidenceVertexDegree[i].size();

					if(i_IncidenceVertexDegreeCount != _FALSE)
					{
						i_SelectedVertex = vli_GroupedIncidenceVertexDegree[i].front();

						break;
					}
				}		    
			}

			for(i=m_vi_Vertices[i_SelectedVertex]; i<m_vi_Vertices[STEP_UP(i_SelectedVertex)]; i++)
			{
				if(vi_IncidenceVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
				{
					continue;
				}

				vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);

				vi_IncidenceVertexDegree[m_vi_Edges[i]] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[i]]);

				vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
			    
				vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].begin();
			    
			}

			vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_IncidenceVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.push_back(i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}

#if DEBUG == 1360

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 1360 | Vertex Ordering | Incidence Degree"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_VertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 1361
	int GraphOrdering::DistanceTwoIncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("DISTANCE TWO INCIDENCE DEGREE") == _TRUE)
		{
			return(_TRUE);
		}

		int i, j, k;
	   
		int i_HighestDistanceTwoDegreeVertex, i_HighestDistanceTwoVertexDegree;

		int i_VertexCount;

		int i_DistanceTwoVertexDegree;

		int i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_IncludedVertices;

		vector<int> vi_IncidenceVertexDegree;

		vector< list<int> > vli_GroupedIncidenceVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		vi_IncidenceVertexDegree.clear();

		vli_GroupedIncidenceVertexDegree.clear();
		vli_GroupedIncidenceVertexDegree.resize((unsigned) i_VertexCount);

		vlit_VertexLocation.clear();

		i_SelectedVertex = _UNKNOWN;

		i_HighestDistanceTwoDegreeVertex = i_HighestDistanceTwoVertexDegree = _UNKNOWN;

		i_IncidenceVertexDegree = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{		
			vi_IncidenceVertexDegree.push_back(i_IncidenceVertexDegree);

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i);

			vlit_VertexLocation.push_back(vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin());
			
			vi_IncludedVertices.clear();
			vi_IncludedVertices.resize((unsigned) i_VertexCount, _FALSE);

			vi_IncludedVertices[i] = _TRUE;

			i_DistanceTwoVertexDegree = _FALSE;
			
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(vi_IncludedVertices[m_vi_Edges[j]] == _FALSE)
				{
					i_DistanceTwoVertexDegree++;

					vi_IncludedVertices[m_vi_Edges[j]] = _TRUE;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(vi_IncludedVertices[m_vi_Edges[k]] == _FALSE)
					{
						i_DistanceTwoVertexDegree++;

						vi_IncludedVertices[m_vi_Edges[k]] = _TRUE;
					}
				}
			}

			if(i_HighestDistanceTwoVertexDegree < i_DistanceTwoVertexDegree)
			{
				i_HighestDistanceTwoVertexDegree = i_DistanceTwoVertexDegree;

				i_HighestDistanceTwoDegreeVertex = i;
			}
		}

#if DEBUG == 1361

		int j;

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 1361 | Vertex Ordering | Vertex Degree"<<endl;
		cout<<endl;

		for(i=m_i_MaximumVertexDegree; i>=0; i--)
		{
			cout<<"Degree "<<i<<"\t"<<" : ";

			i_IncidenceVertexDegreeCount = (signed) vli_GroupedIncidenceVertexDegree[i].size();
			 
			j = _FALSE;
			
			for(lit_ListIterator = vli_GroupedIncidenceVertexDegree[i].begin(); lit_ListIterator != vli_GroupedIncidenceVertexDegree[i].end(); lit_ListIterator++)
			{
				if(j==STEP_DOWN(i_IncidenceVertexDegreeCount))
				{
					cout<<STEP_UP(*lit_ListIterator)<<" ("<<i_IncidenceVertexDegreeCount<<")";
				}
				else
				{
					cout<<STEP_UP(*lit_ListIterator)<<", ";
				}

				j++;
			}

			cout<<endl;
		}

		cout<<endl;
		cout<<"[Highest Distance Two Degree Vertex = "<<STEP_UP(i_HighestDistanceTwoDegreeVertex)<<"; Highest Distance Two Vertex Degree = "<<i_HighestDistanceTwoVertexDegree<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_VertexCount)
		{
			if(i_SelectedVertexCount == _FALSE)
			{
				i_SelectedVertex = i_HighestDistanceTwoDegreeVertex;
			}
			else
			{
				for(i=i_HighestDistanceTwoVertexDegree; i>=0; i--)
				{
					i_IncidenceVertexDegreeCount = (signed) vli_GroupedIncidenceVertexDegree[i].size();

					if(i_IncidenceVertexDegreeCount != _FALSE)
					{
						i_SelectedVertex = vli_GroupedIncidenceVertexDegree[i].front();

						break;
					}
				}		    
			}

			vi_IncludedVertices.clear();
			vi_IncludedVertices.resize((unsigned) i_VertexCount, _FALSE);
			    
			vi_IncludedVertices[i_SelectedVertex] = _TRUE;

			for(i=m_vi_Vertices[i_SelectedVertex]; i<m_vi_Vertices[STEP_UP(i_SelectedVertex)]; i++)
			{
				if(vi_IncidenceVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
				{
					continue;
				}

				if(vi_IncludedVertices[m_vi_Edges[i]] == _FALSE)
				{
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);

					vi_IncidenceVertexDegree[m_vi_Edges[i]] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[i]]);

					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
				    
					vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].begin();
				    
					vi_IncludedVertices[m_vi_Edges[i]] = _TRUE;
				}

				for(j=m_vi_Vertices[m_vi_Edges[i]]; j<m_vi_Vertices[STEP_UP(m_vi_Edges[i])]; j++)
				{
					if(vi_IncidenceVertexDegree[m_vi_Edges[j]] == _UNKNOWN)
					{
						continue;
					}
				
					if(vi_IncludedVertices[m_vi_Edges[j]] == _FALSE)
					{    
						vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[j]]].erase(vlit_VertexLocation[m_vi_Edges[j]]);
					    
						vi_IncidenceVertexDegree[m_vi_Edges[j]] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[j]]);
					    
						vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[j]]].push_front(m_vi_Edges[j]);
					    
						vlit_VertexLocation[m_vi_Edges[j]] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[j]]].begin();

						vi_IncludedVertices[m_vi_Edges[j]] = _TRUE;
					    
					}
				}
			}

			vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_IncidenceVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.push_back(i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}

#if DEBUG == 1361

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 1361 | Vertex Ordering | Incidence Degree"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_VertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 1362
	string GraphOrdering::GetVertexOrderingVariant()
	{
		return(m_s_VertexOrderingVariant);
	}
	
	//Public Function 1363
	void GraphOrdering::GetOrderedVertices(vector<int> &output)
	{
		output = (m_vi_OrderedVertices);
	}

	
	//Public Function 1364
	double GraphOrdering::GetVertexOrderingTime()
	{
		return(m_d_OrderingTime);
	}

	int GraphOrdering::GetBackDegree(int index) {
		if(m_vi_OrderedVertices.size() == 0) return -1; //Vertices has not been ordered. Call OrderVertices() first

		int i_DegreeCount = 0;

		//For each distance 1 neighbor, see if it has been ordered before this vertex
		for(int i = m_vi_Vertices[m_vi_OrderedVertices[index]]; i < m_vi_Vertices[m_vi_OrderedVertices[index] + 1]; i++) {
			for(int j = 0; j < index; j++) {
				if (m_vi_OrderedVertices[j] == m_vi_Edges[i]) {
					i_DegreeCount++;
					break;
				}
			}
		}

		return i_DegreeCount;
	}

	int GraphOrdering::GetMaxBackDegree() {
		if(m_vi_OrderedVertices.size() == 0) return -1; //Vertices has not been ordered. Call OrderVertices() first

		int i_MaxBackDegree = -1;
		int i_CurrentVertexBackDegre = -1;

		//For each vertex in m_vi_OrderedVertices, find its Back Degree
		for(unsigned int i = 0; i < m_vi_OrderedVertices.size(); i++) {
			i_CurrentVertexBackDegre = GetBackDegree(i);
//if(i%1000==0) printf("[%d] i_CurrentVertexBackDegre = %d \n", i, i_CurrentVertexBackDegre);
			if(i_MaxBackDegree < i_CurrentVertexBackDegre) {
				i_MaxBackDegree = i_CurrentVertexBackDegre;
//cout<<"**i_MaxBackDegree = "<<i_MaxBackDegree<<endl;
			}
		}

		return i_MaxBackDegree;
	}
	

	int GraphOrdering::GetBackDegree2(int index) {
		if(m_vi_OrderedVertices.size() == 0) return -1; //Vertices has not been ordered. Call OrderVertices() first

		int i_DegreeCount = 0;
		int i_VertexID = m_vi_OrderedVertices[index];
		vector<int>::iterator vit_Begin = m_vi_Edges.begin();
		vector<int>::iterator vit_End = m_vi_Edges.begin();
		vit_Begin += m_vi_Vertices[ i_VertexID ];
		vit_End += m_vi_Vertices[ i_VertexID + 1 ]; 

		//For each of the vertices v2 ordered before this vertex v1, see if v2 is Distance-1 neighbour of v1
		//If yes, increase the count
		for(int i = 0; i < index; i++) {
//if(i_VertexID == 54863) {
//	printf("[%d] search d1 neighbor of %d [%d, %d) for %d \n", i, m_vi_OrderedVertices[i], m_vi_Vertices[ m_vi_OrderedVertices[i] ], m_vi_Vertices[ m_vi_OrderedVertices[i] + 1], i_VertexID );
//	if(m_vi_OrderedVertices[i] == 54869) PrintVertexD1Neighbor(m_vi_OrderedVertices[i]);
//}
			if( binary_search(vit_Begin , vit_End, m_vi_OrderedVertices[i]) ) i_DegreeCount++;
		}

		return i_DegreeCount;
	}

	int GraphOrdering::GetMaxBackDegree2() {
		if(m_vi_OrderedVertices.size() == 0) return -1; //Vertices has not been ordered. Call OrderVertices() first

		int i_MaxBackDegree = -1;
		int i_CurrentVertexBackDegre = -1;
		vector<int>::iterator vit_Begin = m_vi_Edges.begin();
		vector<int>::iterator vit_End = m_vi_Edges.end();

//cout<<"*Start sorting m_vi_Vertices.size()="<<m_vi_Vertices.size()<<" m_vi_Edges.size()="<<m_vi_Edges.size()<<endl;
//Pause();
		//For each vertex in m_vi_Vertices, sort its edges for quicker find
		for(unsigned int i = 0; i < GetVertexCount(); i++) {
//printf("[%d] \n",i);
			vit_Begin = vit_End = m_vi_Edges.begin();
			vit_Begin += m_vi_Vertices[i];
			vit_End += m_vi_Vertices[i+1]; 
			sort(vit_Begin, vit_End);
		}
//cout<<"*Finish sorting"<<endl;
//Pause();

		//For each vertex in m_vi_OrderedVertices, find its Back Degree
		for(unsigned int i = 0; i < m_vi_OrderedVertices.size(); i++) {
//printf("[%d] \n",i);
			i_CurrentVertexBackDegre = GetBackDegree2(i);
//			if(i_CurrentVertexBackDegre != GetBackDegree(i)) {
//				cout<<"ERRRRRR"<<endl;
//				Pause();
//			}
//else printf("\t Match \n");
//if(i%1000==0) printf("[%d] i_CurrentVertexBackDegre = %d \n", i, i_CurrentVertexBackDegre);
			if(i_MaxBackDegree < i_CurrentVertexBackDegre) {
				i_MaxBackDegree = i_CurrentVertexBackDegre;
//cout<<"**i_MaxBackDegree = "<<i_MaxBackDegree<<endl;
			}
		}
//cout<<"*Finish getting back degree"<<endl;

		return i_MaxBackDegree;
	}
	
}
