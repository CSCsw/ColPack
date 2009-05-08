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
	//Private Function 3401
	int BipartiteGraphOrdering::CheckVertexOrdering(string s_VertexOrderingVariant)
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

	
	//Public Constructor 3451
	BipartiteGraphOrdering::BipartiteGraphOrdering()
	{
		Clear();
	}

	
	//Public Destructor 3452
	BipartiteGraphOrdering::~BipartiteGraphOrdering()
	{
		Clear();
	}

	
	//Virtual Function 3453
	void BipartiteGraphOrdering::Clear()
	{
		BipartiteGraphVertexCover::Clear();

		m_d_OrderingTime = _UNKNOWN;

		m_s_VertexOrderingVariant.clear();

		m_vi_OrderedVertices.clear();

		return;
	}

	
	//Virtual Function 3454
	void BipartiteGraphOrdering::Reset()
	{
		BipartiteGraphVertexCover::Reset();

		m_d_OrderingTime = _UNKNOWN;

		m_s_VertexOrderingVariant.clear();

		m_vi_OrderedVertices.clear();

		return;
	}
	
	//Public Function 3455
	int BipartiteGraphOrdering::NaturalOrdering()
	{
		if(CheckVertexOrdering("NATURAL"))
		{
			return(_TRUE);
		}

		int i;

		int i_LeftVertexCount, i_RightVertexCount;
		
		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		m_vi_OrderedVertices.clear();

		for(i=0; i<i_LeftVertexCount; i++)
		{
			m_vi_OrderedVertices.push_back(i);
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
			m_vi_OrderedVertices.push_back(i + i_LeftVertexCount);
		}
		
		return(_TRUE);
	}

	
	//Public Function 3456
	int BipartiteGraphOrdering::LargestFirstOrdering()
	{
		if(CheckVertexOrdering("LARGEST_FIRST"))
		{
			return(_TRUE);
		}

		int i, j;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_HighestDegreeVertex;
	    
		int i_VertexDegree, i_VertexDegreeCount;

		vector< vector<int> > v2i_GroupedVertexDegree;

		m_i_MaximumVertexDegree = _FALSE;

		i_HighestDegreeVertex = _UNKNOWN;
	    
		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		v2i_GroupedVertexDegree.clear();
		v2i_GroupedVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		for(i=0; i<i_LeftVertexCount; i++)
		{
			i_VertexDegree = m_vi_LeftVertices[STEP_UP(i)] - m_vi_LeftVertices[i];

			v2i_GroupedVertexDegree[i_VertexDegree].push_back(i);

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;

				i_HighestDegreeVertex = i;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
			i_VertexDegree = m_vi_RightVertices[STEP_UP(i)] - m_vi_RightVertices[i];

			v2i_GroupedVertexDegree[i_VertexDegree].push_back(i + i_LeftVertexCount);
		    
			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;

				i_HighestDegreeVertex = i + i_LeftVertexCount;
			}
		}

		m_vi_OrderedVertices.clear();

		if(i_HighestDegreeVertex < i_LeftVertexCount)
		{
			for(i=m_i_MaximumVertexDegree; i>=0; i--)
			{
				i_VertexDegreeCount = (signed) v2i_GroupedVertexDegree[i].size();
		
				for(j=0; j<i_VertexDegreeCount; j++)
				{
					m_vi_OrderedVertices.push_back(v2i_GroupedVertexDegree[i][j]);
				}
			}
		}
		else
		{
			for(i=m_i_MaximumVertexDegree; i>=0; i--)
			{
				i_VertexDegreeCount = (signed) v2i_GroupedVertexDegree[i].size();

				for(j=STEP_DOWN(i_VertexDegreeCount); j>=0; j--)
				{
					m_vi_OrderedVertices.push_back(v2i_GroupedVertexDegree[i][j]);
				}
			}
		}
		
#if DEBUG == 3456

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3456 | Bipartite Graph Bicoloring | Bipartite Largest First Ordering"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();
	    
		for(i=0; i<i_OrderedVertexCount; i++)
		{
			if(i == STEP_DOWN(i_OrderedVertexCount))
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<" ("<<i_OrderedVertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<", ";
			}
		}
	    
		cout<<endl;
		cout<<"[Highest Vertex Degree = "<<m_i_MaximumVertexDegree<<"; Highest Degree Vertex = "<<i_HighestDegreeVertex<<"; [Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;
	    
#endif

		return(_TRUE);
}

	//Public Function 3457
	int BipartiteGraphOrdering::SmallestLastOrdering()
	{
		if(CheckVertexOrdering("SMALLEST_LAST"))
		{
			return(_TRUE);
		}

		int i;
	   
		int _FOUND;

		int i_HighestInducedVertexDegree, i_HighestInducedDegreeVertex;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_VertexCountMinus1; // = i_LeftVertexCount + i_RightVertexCount - 1, used when inserting selected vertices into m_vi_OrderedVertices 

		int i_InducedVertexDegree;
	    
		int i_InducedVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_InducedVertexDegree;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		list<int>::iterator lit_ListIterator;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		i_VertexCountMinus1 = i_LeftVertexCount + i_RightVertexCount - 1;

		vi_InducedVertexDegree.clear();

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		vlit_VertexLocation.clear();
	    
		i_HighestInducedVertexDegree = _FALSE;

		i_HighestInducedDegreeVertex = _UNKNOWN;

		i_SelectedVertex = _UNKNOWN;

		for(i=0; i<i_LeftVertexCount; i++)
		{
			i_InducedVertexDegree = m_vi_LeftVertices[STEP_UP(i)] - m_vi_LeftVertices[i];
			
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;

				i_HighestInducedDegreeVertex = i;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
			i_InducedVertexDegree = m_vi_RightVertices[STEP_UP(i)] - m_vi_RightVertices[i];
			
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i + i_LeftVertexCount);

			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;

				i_HighestInducedDegreeVertex = i + i_LeftVertexCount;
			}
		}
	    

#if DEBUG == 3457

		int j;

		cout<<endl;
		cout<<"DEBUG 3457 | Bipartite Graph Coloring | Bipartite Smallest Last Ordering"<<endl;
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
		cout<<"[Highest Degree Vertex = "<<STEP_UP(i_HighestInducedDegreeVertex)<<"; Highest Induced Vertex Degree = "<<i_HighestInducedVertexDegree<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();
		m_vi_OrderedVertices.resize(i_LeftVertexCount + i_RightVertexCount, _UNKNOWN);

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_LeftVertexCount + i_RightVertexCount)
		{
			for(i=0; i<STEP_UP(i_HighestInducedVertexDegree); i++)
			{
				i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();

				if(i_InducedVertexDegreeCount == _FALSE)
				{
					continue;
				}

				if(i_HighestInducedDegreeVertex < i_LeftVertexCount)
				{
					_FOUND = _FALSE;

					for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
					{
						if(*lit_ListIterator < i_LeftVertexCount)
						{
							i_SelectedVertex = *lit_ListIterator;
			  
							_FOUND = _TRUE;

							break;
						}
					}
			
					if(!_FOUND)
					{
						i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();
					}

					break;
				}
				else
				{
					_FOUND = _FALSE;

					for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
					{
						if(*lit_ListIterator >= i_LeftVertexCount)
						{
							i_SelectedVertex = *lit_ListIterator;
			  
							_FOUND = _TRUE;

							break;
						}
					}
			
					if(!_FOUND)
					{
						i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();
					}
				}  

				break;
			}	

			if(i_SelectedVertex < i_LeftVertexCount)
			{   
				for(i=m_vi_LeftVertices[i_SelectedVertex]; i<m_vi_LeftVertices[STEP_UP(i_SelectedVertex)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].erase(vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount]);
			
					vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]);
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].push_front(m_vi_Edges[i] + i_LeftVertexCount);
			
					vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].begin();
				}
			}
			else
			{
				for(i=m_vi_RightVertices[i_SelectedVertex - i_LeftVertexCount]; i<m_vi_RightVertices[STEP_UP(i_SelectedVertex - i_LeftVertexCount)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);
			
					vi_InducedVertexDegree[m_vi_Edges[i]] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i]]);
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
			
					vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].begin();		
				}
			}

			vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_InducedVertexDegree[i_SelectedVertex] = _UNKNOWN;
	    
			//insert at the beginning (push_front)? BAD!!!! due to moving
			//m_vi_OrderedVertices.insert(m_vi_OrderedVertices.begin(), i_SelectedVertex);

			//Alternative method for the above (commented) line
//printf("m_vi_OrderedVertices[i_VertexCountMinus1 (%d) - i_SelectedVertexCount (%d)] (%d) = i_SelectedVertex (%d) \n", i_VertexCountMinus1, i_SelectedVertexCount, i_VertexCountMinus1 - i_SelectedVertexCount, i_SelectedVertex);
			m_vi_OrderedVertices[i_VertexCountMinus1 - i_SelectedVertexCount] = i_SelectedVertex;

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}

#if DEBUG == 3457

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3457 | Bipartite Graph Coloring | Bipartite Smallest Last Ordering"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			if(i == STEP_DOWN(i_OrderedVertexCount))
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<" ("<<i_OrderedVertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<", ";
			}
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}
	
	//Public Function 3458	
	int BipartiteGraphOrdering::IncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("INCIDENCE_DEGREE"))
		{
			return(_TRUE);
		}

		int i;

		int _FOUND;
	   
		int i_HighestIncidenceDegreeVertex, i_HighestIncidenceVertexDegree;

		int i_LeftVertexCount, i_RightVertexCount;
	    
		int i_VertexDegree;

		int i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_IncidenceVertexDegree;

		vector< list<int> > vli_GroupedIncidenceVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		list<int>::iterator lit_ListIterator;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		vi_IncidenceVertexDegree.clear();

		vli_GroupedIncidenceVertexDegree.clear();
		vli_GroupedIncidenceVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		vlit_VertexLocation.clear();
	    
		i_HighestIncidenceDegreeVertex = i_HighestIncidenceVertexDegree = _UNKNOWN;

		i_IncidenceVertexDegree = _FALSE;

		i_SelectedVertex = _UNKNOWN;

		for(i=0; i<i_LeftVertexCount; i++)
		{		
			vi_IncidenceVertexDegree.push_back(i_IncidenceVertexDegree);

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i);

			vlit_VertexLocation.push_back(vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin());

			i_VertexDegree = m_vi_LeftVertices[STEP_UP(i)] - m_vi_LeftVertices[i];

			if(i_HighestIncidenceVertexDegree < i_VertexDegree)
			{
				i_HighestIncidenceVertexDegree = i_VertexDegree;

				i_HighestIncidenceDegreeVertex = i;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{		
			vi_IncidenceVertexDegree.push_back(i_IncidenceVertexDegree);

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i + i_LeftVertexCount);

			vlit_VertexLocation.push_back(vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin());

			i_VertexDegree = m_vi_RightVertices[STEP_UP(i)] - m_vi_RightVertices[i];

			if(i_HighestIncidenceVertexDegree < i_VertexDegree)
			{
				i_HighestIncidenceVertexDegree = i_VertexDegree;

				i_HighestIncidenceDegreeVertex = i + i_LeftVertexCount;
			}
		}

#if DEBUG == 3458

		int j;

		cout<<endl;
		cout<<"DEBUG 3458 | Bipartite Graph Coloring | Bipartite Incidence Degree Ordering"<<endl;
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
		cout<<"[Highest Degree Vertex = "<<STEP_UP(i_HighestIncidenceDegreeVertex)<<"; Highest Vertex Degree = "<<i_HighestIncidenceVertexDegree<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_LeftVertexCount + i_RightVertexCount)
		{
			if(i_SelectedVertexCount == _FALSE)
			{
				i_SelectedVertex = i_HighestIncidenceDegreeVertex;
			}
			else
			{
				for(i=i_HighestIncidenceVertexDegree; i>=0; i--)
				{
					i_IncidenceVertexDegreeCount = (signed) vli_GroupedIncidenceVertexDegree[i].size();

					if(i_IncidenceVertexDegreeCount == _FALSE)
					{
						continue;
					}

					if(i_HighestIncidenceDegreeVertex < i_LeftVertexCount)
					{
						_FOUND = _FALSE;

						for(lit_ListIterator = vli_GroupedIncidenceVertexDegree[i].begin(); lit_ListIterator != vli_GroupedIncidenceVertexDegree[i].end(); lit_ListIterator++)
						{
							if(*lit_ListIterator < i_LeftVertexCount)
							{
								i_SelectedVertex = *lit_ListIterator;
					    
								_FOUND = _TRUE;
					    
								break;
							}
						}
				
						if(!_FOUND)
						{
							i_SelectedVertex = vli_GroupedIncidenceVertexDegree[i].front();
						}

						break;
					}
					else
					{
						_FOUND = _FALSE;

						for(lit_ListIterator = vli_GroupedIncidenceVertexDegree[i].begin(); lit_ListIterator != vli_GroupedIncidenceVertexDegree[i].end(); lit_ListIterator++)
						{
							if(*lit_ListIterator >= i_LeftVertexCount)
							{
								i_SelectedVertex = *lit_ListIterator;
					    
								_FOUND = _TRUE;
					    
								break;
							}
						}
				    
						if(!_FOUND)
						{
							i_SelectedVertex = vli_GroupedIncidenceVertexDegree[i].front();
						}
					}  
				
					break;
				}		    
			}

			if(i_SelectedVertex < i_LeftVertexCount)
			{
				for(i=m_vi_LeftVertices[i_SelectedVertex]; i<m_vi_LeftVertices[STEP_UP(i_SelectedVertex)]; i++)
				{
					if(vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].erase(vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].push_front(m_vi_Edges[i] + i_LeftVertexCount);
					
					vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].begin();

				}
			}
			else
			{
				for(i=m_vi_RightVertices[i_SelectedVertex - i_LeftVertexCount]; i<m_vi_RightVertices[STEP_UP(i_SelectedVertex - i_LeftVertexCount)]; i++)
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

			}
		    
			vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_IncidenceVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.push_back(i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}

#if DEBUG == 3458

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3458 | Bipartite Graph Coloring | Bipartite Incidence Degree Ordering"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			if(i == STEP_DOWN(i_OrderedVertexCount))
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<" ("<<i_OrderedVertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<", ";
			}
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}
	
	//Public Function 3459
	int BipartiteGraphOrdering::SelectiveLargestFirstOrdering()
	{
		if(CheckVertexOrdering("SELECTVE_LARGEST_FIRST"))
		{
			return(_TRUE);
		}

		int i, j;

		int i_LeftVertexCount, i_RightVertexCount;
	    
		int i_VertexDegree, i_VertexDegreeCount;

		vector< vector<int> > v2i_GroupedVertexDegree;

		m_i_MaximumVertexDegree = _FALSE;
	       
		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		v2i_GroupedVertexDegree.clear();
		v2i_GroupedVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		for(i=0; i<i_LeftVertexCount; i++)
		{
			if(m_vi_IncludedLeftVertices[i] == _FALSE)
			{
				continue;
			}
		
			i_VertexDegree = _FALSE;

			for(j=m_vi_LeftVertices[i]; j<m_vi_LeftVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedRightVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_VertexDegree++;
			}

			v2i_GroupedVertexDegree[i_VertexDegree].push_back(i);

			if(m_i_MaximumVertexDegree < i_VertexDegree)
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
			if(m_vi_IncludedRightVertices[i] == _FALSE)
			{
				continue;
			}
		
			i_VertexDegree = _FALSE;

			for(j=m_vi_RightVertices[i]; j<m_vi_RightVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedLeftVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_VertexDegree++;
			}

			v2i_GroupedVertexDegree[i_VertexDegree].push_back(i + i_LeftVertexCount);
	    
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
		
#if DEBUG == 3459

		int i_VertexCount;

		cout<<endl;
		cout<<"DEBUG 3459 | Bipartite Graph Bicoloring | Largest First Ordering"<<endl;
		cout<<endl;

		i_VertexCount = (signed) m_vi_OrderedVertices.size();
	    
		for(i=0; i<i_VertexCount; i++)
		{
			if(i == STEP_DOWN(i_VertexCount))
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<" ("<<i_VertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<", ";
			}
		}
	    
		cout<<endl;
		cout<<"[Highest Vertex Degree = "<<m_i_MaximumVertexDegree<<"]"<<endl;
		cout<<endl;
	    
#endif

		return(_TRUE);
	}
	
	//Public Function 3460
	int BipartiteGraphOrdering::SelectiveSmallestLastOrdering()
	{
		if(CheckVertexOrdering("SELECTIVE_SMALLEST_LAST"))
		{
			return(_TRUE);
		}

		int i, j;
	   
		int i_HighestInducedVertexDegree;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_InducedVertexDegree;
	    
		int i_InducedVertexDegreeCount;

		int i_IncludedVertexCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_InducedVertexDegree;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		vi_InducedVertexDegree.clear();
		vi_InducedVertexDegree.resize((signed) i_LeftVertexCount + i_RightVertexCount, _UNKNOWN);

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		vlit_VertexLocation.clear();
		vlit_VertexLocation.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		i_IncludedVertexCount = _FALSE;

		i_HighestInducedVertexDegree = _FALSE;

		i_SelectedVertex = _UNKNOWN;

		for(i=0; i<i_LeftVertexCount; i++)
		{
      		if(m_vi_IncludedLeftVertices[i] == _FALSE)
			{
				continue;
			}

			i_IncludedVertexCount++;
		
			i_InducedVertexDegree = _FALSE;

			for(j=m_vi_LeftVertices[i]; j<m_vi_LeftVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedRightVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_InducedVertexDegree++;
			}
		
			vi_InducedVertexDegree[i] = i_InducedVertexDegree;

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			vlit_VertexLocation[vli_GroupedInducedVertexDegree[i_InducedVertexDegree].front()] = vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin();

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
      		if(m_vi_IncludedRightVertices[i] == _FALSE)
			{
				continue;
			}

			i_IncludedVertexCount++;
			
			i_InducedVertexDegree = _FALSE;

			for(j=m_vi_RightVertices[i]; j<m_vi_RightVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedLeftVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_InducedVertexDegree++;
			}
		
			vi_InducedVertexDegree[i + i_LeftVertexCount] = i_InducedVertexDegree;

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i + i_LeftVertexCount);

			vlit_VertexLocation[vli_GroupedInducedVertexDegree[i_InducedVertexDegree].front()] = vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin();

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}
	    

#if DEBUG == 3460

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 3460 | Vertex Ordering | Vertex Degree"<<endl;
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

		while(i_SelectedVertexCount < i_IncludedVertexCount)
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

			if(i_SelectedVertex < i_LeftVertexCount)
			{   
				for(i=m_vi_LeftVertices[i_SelectedVertex]; i<m_vi_LeftVertices[STEP_UP(i_SelectedVertex)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].erase(vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].push_front(m_vi_Edges[i] + i_LeftVertexCount);
					
					vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].begin();		
				}
			}
			else
			{
				for(i=m_vi_RightVertices[i_SelectedVertex - i_LeftVertexCount]; i<m_vi_RightVertices[STEP_UP(i_SelectedVertex - i_LeftVertexCount)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
					{
						continue;
					}
				
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);
					
					vi_InducedVertexDegree[m_vi_Edges[i]] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i]]);
					
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
					
					vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].begin();		
				}
			}

			vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_InducedVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.insert(m_vi_OrderedVertices.begin(), i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}


#if DEBUG == 3460

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3460 | Vertex Ordering | Smallest Last"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	
	//Public Function 3461
	int BipartiteGraphOrdering::SelectiveIncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("SELECTIVE_INCIDENCE_DEGREE"))
		{
			return(_TRUE);
		}

		int i, j;
	   
		int i_HighestDegreeVertex, m_i_MaximumVertexDegree;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;

		int i_IncludedVertexCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_IncidenceVertexDegree;

		vector< list<int> > vli_GroupedIncidenceVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		vi_IncidenceVertexDegree.clear();
		vi_IncidenceVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount, _UNKNOWN);

		vli_GroupedIncidenceVertexDegree.clear();
		vli_GroupedIncidenceVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		vlit_VertexLocation.clear();
		vlit_VertexLocation.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		i_SelectedVertex = _UNKNOWN;

		i_IncludedVertexCount = _FALSE;

		i_HighestDegreeVertex = m_i_MaximumVertexDegree = _UNKNOWN;

		for(i=0; i<i_LeftVertexCount; i++)
		{		
			if(m_vi_IncludedLeftVertices[i] == _FALSE)
			{
				continue;
			}

			i_IncludedVertexCount++;
			
			i_IncidenceVertexDegree = _FALSE;

			vi_IncidenceVertexDegree[i] = i_IncidenceVertexDegree;

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i);

			vlit_VertexLocation[vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].front()] = vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin();

			for(j=m_vi_LeftVertices[i]; j<m_vi_LeftVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedRightVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_IncidenceVertexDegree++;
			}

			if(m_i_MaximumVertexDegree < i_IncidenceVertexDegree)
			{
				m_i_MaximumVertexDegree = i_IncidenceVertexDegree;

				i_HighestDegreeVertex = i;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{		
      		if(m_vi_IncludedRightVertices[i] == _FALSE)
			{
				continue;
			}

			i_IncludedVertexCount++;
			
			i_IncidenceVertexDegree = _FALSE;

			vi_IncidenceVertexDegree[i + i_LeftVertexCount] = i_IncidenceVertexDegree;

			vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].push_front(i + i_LeftVertexCount);

			vlit_VertexLocation[vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].front()] = vli_GroupedIncidenceVertexDegree[i_IncidenceVertexDegree].begin();

			for(j=m_vi_RightVertices[i]; j<m_vi_RightVertices[STEP_UP(i)]; j++)
			{
				if(m_vi_IncludedLeftVertices[m_vi_Edges[j]] == _FALSE)
				{
					continue;
				}

				i_IncidenceVertexDegree++;
			}

			if(m_i_MaximumVertexDegree < i_IncidenceVertexDegree)
			{
				m_i_MaximumVertexDegree = i_IncidenceVertexDegree;

				i_HighestDegreeVertex = i + i_LeftVertexCount;
			}
		}

#if DEBUG == 3461

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree | Vertex Degrees"<<endl;
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
		cout<<"[Highest Degree Vertex = "<<STEP_UP(i_HighestDegreeVertex)<<"; Highest Vertex Degree = "<<m_i_MaximumVertexDegree<<"; Candidate Vertex Count = "<<i_IncludedVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_IncludedVertexCount)
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

			if(i_SelectedVertex < i_LeftVertexCount)
			{

#if DEBUG == 3461

				cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree | Selected Left Vertex | "<<STEP_UP(i_SelectedVertex)<<" [Selection "<<STEP_UP(i_SelectedVertexCount)<<"]"<<endl;

#endif

				for(i=m_vi_LeftVertices[i_SelectedVertex]; i<m_vi_LeftVertices[STEP_UP(i_SelectedVertex)]; i++)
				{
					if(vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] == _UNKNOWN)
					{
						continue;
					}
				
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].erase(vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]);
					
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].push_front(m_vi_Edges[i] + i_LeftVertexCount);
					
					vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].begin();

#if DEBUG == 3461

					cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree | Repositioned Right Vertex | "<<STEP_UP(m_vi_Edges[i] + i_LeftVertexCount)<<endl;

#endif

				}
			}
			else
			{

#if DEBUG == 3461

				cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree | Selected Right Vertex | "<<STEP_UP(i_SelectedVertex)<<" [Selection "<<STEP_UP(i_SelectedVertexCount)<<"]"<<endl;

#endif
		  
				for(i=m_vi_RightVertices[i_SelectedVertex - i_LeftVertexCount]; i<m_vi_RightVertices[STEP_UP(i_SelectedVertex - i_LeftVertexCount)]; i++)
				{
					if(vi_IncidenceVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
					{
						continue;
					}
				
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);
					
					vi_IncidenceVertexDegree[m_vi_Edges[i]] = STEP_UP(vi_IncidenceVertexDegree[m_vi_Edges[i]]);
					
					vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
					
					vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[m_vi_Edges[i]]].begin();
				
#if DEBUG == 3461

					cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree | Repositioned Left Vertex | "<<STEP_UP(m_vi_Edges[i])<<endl;

#endif

				}
			}

			vli_GroupedIncidenceVertexDegree[vi_IncidenceVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_IncidenceVertexDegree[i_SelectedVertex] = _UNKNOWN;
		    
			m_vi_OrderedVertices.push_back(i_SelectedVertex);

			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);

		}

#if DEBUG == 3461

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3461 | Vertex Ordering | Incidence Degree"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_OrderedVertices[i])<<endl;
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}


	//Public Function 3462
	int BipartiteGraphOrdering::DynamicLargestFirstOrdering()
	{
		if(CheckVertexOrdering("DYNAMIC_LARGEST_FIRST"))
		{
			return(_TRUE);
		}

		int i;
	   
		int _FOUND;

		int i_HighestInducedVertexDegree, i_HighestInducedDegreeVertex;

		int i_LeftVertexCount, i_RightVertexCount;

		int i_InducedVertexDegree;
	    
		int i_InducedVertexDegreeCount;

		int i_SelectedVertex, i_SelectedVertexCount;

		vector<int> vi_InducedVertexDegree;

		vector< list<int> > vli_GroupedInducedVertexDegree;

		vector< list<int>::iterator > vlit_VertexLocation;

		list<int>::iterator lit_ListIterator;

		i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());

		vi_InducedVertexDegree.clear();

		vli_GroupedInducedVertexDegree.clear();
		vli_GroupedInducedVertexDegree.resize((unsigned) i_LeftVertexCount + i_RightVertexCount);

		vlit_VertexLocation.clear();
	    
		i_SelectedVertex = _UNKNOWN;

		i_HighestInducedVertexDegree = _FALSE;

		i_HighestInducedDegreeVertex = _UNKNOWN;

		for(i=0; i<i_LeftVertexCount; i++)
		{
			i_InducedVertexDegree = m_vi_LeftVertices[STEP_UP(i)] - m_vi_LeftVertices[i];
			
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i);

			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;

				i_HighestInducedDegreeVertex = i;
			}
		}

		for(i=0; i<i_RightVertexCount; i++)
		{
			i_InducedVertexDegree = m_vi_RightVertices[STEP_UP(i)] - m_vi_RightVertices[i];
			
			vi_InducedVertexDegree.push_back(i_InducedVertexDegree);

			vli_GroupedInducedVertexDegree[i_InducedVertexDegree].push_front(i + i_LeftVertexCount);

			vlit_VertexLocation.push_back(vli_GroupedInducedVertexDegree[i_InducedVertexDegree].begin());

			if(i_HighestInducedVertexDegree < i_InducedVertexDegree)
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;

				i_HighestInducedDegreeVertex = i + i_LeftVertexCount;
			}
		}
	    

#if DEBUG == 3462

		int j;

		cout<<endl;
		cout<<"DEBUG 3462 | Bipartite Graph Coloring | Bipartite Smallest Last Ordering"<<endl;
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
		cout<<"[Highest Degree Vertex = "<<STEP_UP(i_HighestInducedDegreeVertex)<<"; Highest Induced Vertex Degree = "<<i_HighestInducedVertexDegree<<"]"<<endl;
		cout<<endl;

#endif

		m_vi_OrderedVertices.clear();

		i_SelectedVertexCount = _FALSE;

		while(i_SelectedVertexCount < i_LeftVertexCount + i_RightVertexCount)
		{
			for(i=i_HighestInducedVertexDegree; i>=0; i--)
			{
				i_InducedVertexDegreeCount = (signed) vli_GroupedInducedVertexDegree[i].size();

				if(i_InducedVertexDegreeCount == _FALSE)
				{
					continue;
				}

				if(i_HighestInducedDegreeVertex < i_LeftVertexCount)
				{
					_FOUND = _FALSE;
		  
					for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
					{
						if(*lit_ListIterator < i_LeftVertexCount)
						{
							i_SelectedVertex = *lit_ListIterator;
		      
							_FOUND = _TRUE;
		      
							break;
						}
					}
		  
					if(!_FOUND)
					{
						i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();
					}
		  
					break;
				}
				else
				{
					_FOUND = _FALSE;
		  
					for(lit_ListIterator = vli_GroupedInducedVertexDegree[i].begin(); lit_ListIterator != vli_GroupedInducedVertexDegree[i].end(); lit_ListIterator++)
					{
						if(*lit_ListIterator >= i_LeftVertexCount)
						{
							i_SelectedVertex = *lit_ListIterator;
		      
							_FOUND = _TRUE;
		      
							break;
						}
					}
		  
					if(!_FOUND)
					{
						i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front();
					}
				}  
		
				break;
			}
	      
			if(i_SelectedVertex < i_LeftVertexCount)
			{   
				for(i=m_vi_LeftVertices[i_SelectedVertex]; i<m_vi_LeftVertices[STEP_UP(i_SelectedVertex)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].erase(vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount]);
			
					vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]);
				
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].push_front(m_vi_Edges[i] + i_LeftVertexCount);
				
					vlit_VertexLocation[m_vi_Edges[i] + i_LeftVertexCount] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i] + i_LeftVertexCount]].begin();
				}
			}
			else
			{
				for(i=m_vi_RightVertices[i_SelectedVertex - i_LeftVertexCount]; i<m_vi_RightVertices[STEP_UP(i_SelectedVertex - i_LeftVertexCount)]; i++)
				{
					if(vi_InducedVertexDegree[m_vi_Edges[i]] == _UNKNOWN)
					{
						continue;
					}
			
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].erase(vlit_VertexLocation[m_vi_Edges[i]]);
						
					vi_InducedVertexDegree[m_vi_Edges[i]] = STEP_DOWN(vi_InducedVertexDegree[m_vi_Edges[i]]);
						
					vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].push_front(m_vi_Edges[i]);
						
					vlit_VertexLocation[m_vi_Edges[i]] = vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[m_vi_Edges[i]]].begin();		
				}
			}

			vli_GroupedInducedVertexDegree[vi_InducedVertexDegree[i_SelectedVertex]].erase(vlit_VertexLocation[i_SelectedVertex]);

			vi_InducedVertexDegree[i_SelectedVertex] = _UNKNOWN;
	    
			m_vi_OrderedVertices.push_back(i_SelectedVertex);
	      
			i_SelectedVertexCount = STEP_UP(i_SelectedVertexCount);
		}
	    

#if DEBUG == 3462

		int i_OrderedVertexCount;

		cout<<endl;
		cout<<"DEBUG 3462 | Bipartite Graph Coloring | Bipartite Dynamic Largest First Ordering"<<endl;
		cout<<endl;

		i_OrderedVertexCount = (signed) m_vi_OrderedVertices.size();

		for(i=0; i<i_OrderedVertexCount; i++)
		{
			if(i == STEP_DOWN(i_OrderedVertexCount))
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<" ("<<i_OrderedVertexCount<<")"<<endl;
			}
			else
			{
				cout<<STEP_UP(m_vi_OrderedVertices[i])<<", ";
			}
		}

		cout<<endl;
		cout<<"[Ordered Vertex Count = "<<i_OrderedVertexCount<<"/"<<i_LeftVertexCount + i_RightVertexCount<<"]"<<endl;
		cout<<endl;

#endif

		return(_TRUE);
	}

	//Public Function 3463
	string BipartiteGraphOrdering::GetVertexOrderingVariant()
	{

		if(m_s_VertexOrderingVariant.compare("NATURAL") == 0)
		{
			return("Natural");
		}
		else
		if(m_s_VertexOrderingVariant.compare("LARGEST_FIRST") == 0)
		{
			return("Largest First");
		}
		else
		if(m_s_VertexOrderingVariant.compare("SMALLEST_LAST") == 0)
		{
			return("Smallest Last");
		}
		else
		if(m_s_VertexOrderingVariant.compare("INCIDENCE_DEGREE") == 0)
		{
			return("Incidence Degree");
		}
		else
		if(m_s_VertexOrderingVariant.compare("SELECTVE_LARGEST_FIRST") == 0)
		{
			return("Selective Largest First");
		}
		else
		if(m_s_VertexOrderingVariant.compare("SELECTVE_SMALLEST_FIRST") == 0)
		{
			return("Selective Smallest Last");
		}
		else
		if(m_s_VertexOrderingVariant.compare("SELECTIVE_INCIDENCE_DEGREE") == 0)
		{
			return("Selective Incidence Degree");
		}
		else
		if(m_s_VertexOrderingVariant.compare("DYNAMIC_LARGEST_FIRST") == 0)
		{
			return("Dynamic Largest First");
		}
		else
		{
			return("Unknown");
		}
	}

	//Public Function 3464
	void BipartiteGraphOrdering::GetOrderedVertices(vector<int> &output)
	{
		output = (m_vi_OrderedVertices);
	}

	int BipartiteGraphOrdering::OrderVertices(string s_OrderingVariant) {
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
		if((s_OrderingVariant.compare("SMALLEST_LAST") == 0))
		{
			return(SmallestLastOrdering());
		}
		else
		if((s_OrderingVariant.compare("INCIDENCE_DEGREE") == 0))
		{
			return(IncidenceDegreeOrdering());
		}
		else
		{
			cerr<<endl;
			cerr<<"Unknown Ordering Method";
			cerr<<endl;
		}

		return(_TRUE);
	}

}
