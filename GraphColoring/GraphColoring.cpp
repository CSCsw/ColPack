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
	//Private Function 1401
	int GraphColoring::FindCycle(int i_Vertex, int i_AdjacentVertex, int i_DistanceOneVertex, int i_SetID, vector<int> & vi_CandidateColors, vector<int> & vi_FirstVisitedOne, vector<int> & vi_FirstVisitedTwo)
	{
		int i_VertexOne, i_VertexTwo;

		if(i_SetID != _UNKNOWN)
		{
			i_VertexOne = vi_FirstVisitedOne[i_SetID];
			i_VertexTwo = vi_FirstVisitedTwo[i_SetID];
			
			if(i_VertexOne != i_Vertex)
			{
				vi_FirstVisitedOne[i_SetID] = i_Vertex;
				vi_FirstVisitedTwo[i_SetID] = i_AdjacentVertex;
			}
			else
			if((i_VertexOne == i_Vertex) && (i_VertexTwo != i_AdjacentVertex))    
			{	
				vi_CandidateColors[m_vi_VertexColors[i_DistanceOneVertex]] = i_Vertex;   

#if DEBUG == 1401

				cout<<"DEBUG 1401 | Acyclic Coloring | Found Cycle | Vertex "<<STEP_UP(i_Vertex)<<endl;
#endif
	    
			}	
		}

		return(_TRUE);
	}

	
	//Private Function 1402
	int GraphColoring::UpdateSet(int i_Vertex, int i_AdjacentVertex, int i_DistanceOneVertex, map< int, map<int, int> > & mimi2_VertexEdgeMap, vector<int> & vi_FirstSeenOne, vector<int> & vi_FirstSeenTwo, vector<int> & vi_FirstSeenThree)
	{
		int i_ColorID;

		int i_VertexOne, i_VertexTwo, i_VertexThree;

		i_ColorID = m_vi_VertexColors[i_AdjacentVertex];
	    
		i_VertexOne = vi_FirstSeenOne[i_ColorID];
		i_VertexTwo = vi_FirstSeenTwo[i_ColorID];
		i_VertexThree = vi_FirstSeenThree[i_ColorID];

		if(i_VertexOne != i_Vertex)
		{
			vi_FirstSeenOne[i_ColorID] = i_Vertex;
			vi_FirstSeenTwo[i_ColorID] = i_AdjacentVertex;
			vi_FirstSeenThree[i_ColorID] = i_DistanceOneVertex;
		}
		else
		{
			if(i_VertexTwo < i_VertexThree) 
			{
				return(mimi2_VertexEdgeMap[i_VertexTwo][i_VertexThree]);
			}
			else
			{
				return(mimi2_VertexEdgeMap[i_VertexThree][i_VertexTwo]);
			}
		}
	    
		return(_UNKNOWN);
	}

	
	//Private Function 1403
	int GraphColoring::SearchDepthFirst(int i_RootVertex, int i_ParentVertex, int i_Vertex, vector<int> & vi_TouchedVertices)
	{
		int i;

		int i_VertexCount;

		int i_ViolationCount;

		i_ViolationCount = _FALSE;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		for(i=m_vi_Vertices[i_Vertex]; i<m_vi_Vertices[STEP_UP(i_Vertex)]; i++)
		{
			if(m_vi_Edges[i] == i_ParentVertex)
			{
				continue;
			}

			if(m_vi_Edges[i] == i_RootVertex)
			{
				i_ViolationCount++;

				if(i_ViolationCount == _TRUE)
				{
					cout<<endl;
					cout<<"Acyclic Coloring | Violation Check | "<<m_s_InputFile<<endl;
					cout<<endl;
				}

				cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(i_RootVertex)<<" ["<<STEP_UP(m_vi_VertexColors[i_RootVertex])<<"] ... "<<STEP_UP(i_ParentVertex)<<" ["<<STEP_UP(m_vi_VertexColors[i_ParentVertex])<<"] - "<<STEP_UP(i_Vertex)<<" ["<<STEP_UP(m_vi_VertexColors[i_Vertex])<<"] - "<<STEP_UP(m_vi_Edges[i])<<" ["<<STEP_UP(m_vi_VertexColors[m_vi_Edges[i]])<<"]"<<endl;
			}

			if(m_vi_VertexColors[m_vi_Edges[i]] == m_vi_VertexColors[i_Vertex])
			{
				i_ViolationCount++;

				if(i_ViolationCount == _TRUE)
				{
					cout<<endl;
					cout<<"Acyclic Coloring | Violation Check | "<<m_s_InputFile<<endl;
					cout<<endl;
				}

				cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(i_Vertex)<<" ["<<STEP_UP(m_vi_VertexColors[i_Vertex])<<"] - "<<STEP_UP(m_vi_Edges[i])<<" ["<<STEP_UP(m_vi_VertexColors[m_vi_Edges[i]])<<"]"<<endl;
		    
			}
		
			if(vi_TouchedVertices[m_vi_Edges[i]] == _TRUE)
			{
				continue;
			}

			if(m_vi_VertexColors[m_vi_Edges[i]] != m_vi_VertexColors[i_ParentVertex])
			{
				continue;;
			}

			vi_TouchedVertices[m_vi_Edges[i]] = _TRUE;

			i_ViolationCount = SearchDepthFirst(i_RootVertex, i_Vertex, m_vi_Edges[i], vi_TouchedVertices);

		}    

		return(i_ViolationCount);

	}

	
	//Private Function 1404
	int GraphColoring::CheckVertexColoring(string s_GraphColoringVariant)
	{
		if(m_s_VertexColoringVariant.compare(s_GraphColoringVariant) == 0)
		{
			return(_TRUE);
		}

		if(m_s_VertexColoringVariant.compare("ALL") != 0)
		{
			m_s_VertexColoringVariant = s_GraphColoringVariant;
		}

		if(m_s_VertexOrderingVariant.empty())
		{
			NaturalOrdering();
		}

		return(_FALSE);
	}

	
	//Private Function 1405
	int GraphColoring::CalculateVertexColorClasses()
	{
		if(m_s_VertexColoringVariant.empty())
		{
			return(_FALSE);
		}

		int i_TotalVertexColors = STEP_UP(m_i_VertexColorCount);

		m_vi_VertexColorFrequency.clear();
		m_vi_VertexColorFrequency.resize((unsigned) i_TotalVertexColors, _FALSE);

		int i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		for(int i = 0; i < i_VertexCount; i++)
		{
			m_vi_VertexColorFrequency[m_vi_VertexColors[i]]++;
		}
		
		for(int i = 0; i < i_TotalVertexColors; i++)
		{
			if(m_i_LargestColorClassSize < m_vi_VertexColorFrequency[i])
			{
				m_i_LargestColorClass = i;

				m_i_LargestColorClassSize = m_vi_VertexColorFrequency[i];

			}
			
			if(m_i_SmallestColorClassSize == _UNKNOWN)
			{
				m_i_SmallestColorClass = i;

				m_i_SmallestColorClassSize = m_vi_VertexColorFrequency[i];
			}
			else
			if(m_i_SmallestColorClassSize > m_vi_VertexColorFrequency[i])
			{
				m_i_SmallestColorClass = i;

				m_i_SmallestColorClassSize = m_vi_VertexColorFrequency[i];
			}
		}

		m_d_AverageColorClassSize = i_TotalVertexColors / i_VertexCount;

		return(_TRUE);
	}


	//Public Constructor 1451
	GraphColoring::GraphColoring()
	{
		Clear();

		GraphOrdering::GraphOrdering();
	}

	
	//Public Destructor 1452
	GraphColoring::~GraphColoring()
	{
		Clear();
	}
	
	//Virtual Function 1453
	void GraphColoring::Clear()
	{
		GraphOrdering::Clear();

		m_i_VertexColorCount = _UNKNOWN;
	
		m_i_LargestColorClass = _UNKNOWN;
		m_i_SmallestColorClass = _UNKNOWN;

		m_i_LargestColorClassSize = _UNKNOWN;
		m_i_SmallestColorClassSize = _UNKNOWN;

		m_d_AverageColorClassSize = _UNKNOWN;

		m_i_ColoringUnits = _UNKNOWN;

		m_d_ColoringTime = _UNKNOWN;
		m_d_CheckingTime = _UNKNOWN;

		m_s_VertexColoringVariant.clear();

		m_vi_VertexColors.clear();
		
		m_vi_VertexColorFrequency.clear();
		

		return;
	}

	
	//Public Function 1454
	int GraphColoring::DistanceOneColoring()
	{
		if(CheckVertexColoring("DISTANCE ONE"))
		{
			return(_TRUE);
		}

		int i, j;

		int i_PresentVertex;

		int i_VertexCount;

		vector<int> vi_CandidateColors;
	    
		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1454 | Distance One Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
		    
			}
		
			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;

					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		}

		return(_TRUE);

	}

	
	//Public Function 1455
	int GraphColoring::DistanceTwoColoring()
	{
		if(CheckVertexColoring("DISTANCE TWO"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_PresentVertex;

		int i_VertexCount;

		vector<int> vi_CandidateColors;
	    
		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1455 | Distance Two Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
/*
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}
				vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
//*/
				if(m_vi_VertexColors[m_vi_Edges[j]] != _UNKNOWN) vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					//is this "if" statement really necessary? because the i_PresentVertex is not colored anyway
					// say it another way, the second if statement will take care of the i_PresentVertex
/*
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
//*/

					if(m_vi_VertexColors[m_vi_Edges[k]] != _UNKNOWN)
					{
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
					}	
				}
			}
		
			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;

					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		}

		return(_TRUE);
	}

	
	//Public Function 1456
	int GraphColoring::NaiveStarColoring()
	{    
		if(CheckVertexColoring("NAIVE STAR"))
		{
			return(_TRUE);
		}

		int i, j, k, l;

		int i_PresentVertex;

		int i_VertexCount;

		vector<int> vi_CandidateColors;
	    
		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

		cout<<"DEBUG 1456 | Naive Star Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] != _UNKNOWN)
				{		
					vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
					{
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
					}
					else
					{
						for(l=m_vi_Vertices[m_vi_Edges[k]]; l<m_vi_Vertices[STEP_UP(m_vi_Edges[k])]; l++)
						{
							if(m_vi_Edges[l] == m_vi_Edges[j])
							{
								continue;
							}

							if(m_vi_VertexColors[m_vi_Edges[l]] == _UNKNOWN)
							{
								continue;
							}

							if(m_vi_VertexColors[m_vi_Edges[l]] == m_vi_VertexColors[m_vi_Edges[j]])
							{
								vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;

								break;
							}
						}
					}   
				}
			}
			
			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;
				
					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		}

		return(_TRUE);

	}

	
	//Public Function 1457
	int GraphColoring::RestrictedStarColoring()
	{    
		if(CheckVertexColoring("RESTRICTED STAR"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_PresentVertex;
	    
		int i_VertexCount;

		vector<int> vi_CandidateColors;
	    
		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 
	    
		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		for(i=0; i<i_VertexCount; i++)
		{

			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1457 | Restricted Star Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] != _UNKNOWN)
				{		
					vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
					{
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
					}
					else
					if(m_vi_VertexColors[m_vi_Edges[k]] < m_vi_VertexColors[m_vi_Edges[j]])
					{
						 vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
					}   
				}
			}
			
			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;

					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		}

		return(_TRUE);

	}

	
	//Public Function 1458
	int GraphColoring::StarColoring()
	{
		if(CheckVertexColoring("STAR"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int _FOUND;

		int i_ColorID, i_StarID;

		int i_PresentVertex;

		int i_VertexCount, i_EdgeCount;

		int i_VertexOne, i_VertexTwo;

		vector<int> vi_MemberEdges;

		vector<int> vi_CandidateColors;
	 
		vector<int> vi_EdgeStarMap;
		vector<int> vi_StarHubMap;

		vector<int> vi_FirstTreated;
	 
		vector<int> vi_FirstSeenOne, vi_FirstSeenTwo;

		map< int, map<int, int> > mimi2_VertexEdgeMap;

		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		i_EdgeCount = (signed) m_vi_Edges.size();

		vi_EdgeStarMap.clear();
		vi_EdgeStarMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		vi_StarHubMap.clear();
		vi_StarHubMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_FirstSeenOne.clear();
		vi_FirstSeenOne.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenTwo.clear();
		vi_FirstSeenTwo.resize((unsigned) i_VertexCount, _UNKNOWN);

	//    vi_FirstTreated.clear();
	//    vi_FirstTreated.resize((unsigned) i_EdgeCount, _UNKNOWN);

		vi_FirstTreated.clear();
		vi_FirstTreated.resize((unsigned) i_VertexCount, _UNKNOWN);

		k = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					mimi2_VertexEdgeMap[i][m_vi_Edges[j]] = k;

					vi_EdgeStarMap[k] = k;

					k++;
				}
			}
		}

#if VERBOSE == _TRUE

		cout<<endl;

#endif

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1458 | Star Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				i_ColorID = m_vi_VertexColors[m_vi_Edges[j]];

				if(i_ColorID == _UNKNOWN)
				{
				  continue;
				}
				
				vi_CandidateColors[i_ColorID] = i_PresentVertex;
			    
				i_VertexOne = vi_FirstSeenOne[i_ColorID];
				i_VertexTwo = vi_FirstSeenTwo[i_ColorID];
		    
				if(i_VertexOne == i_PresentVertex)
				{
					if(vi_FirstTreated[i_VertexTwo] != i_PresentVertex)
					{
					    
						for(k=m_vi_Vertices[i_VertexTwo]; k<m_vi_Vertices[STEP_UP(i_VertexTwo)]; k++)
						{
							if(m_vi_Edges[k] == i_PresentVertex)
							{
								continue;
							}
						
							if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
							{
								continue;
							}  
						
							vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;

						}
					    
						vi_FirstTreated[i_VertexTwo] = i_PresentVertex;
					}
					
					for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
					{
						if(m_vi_Edges[k] == i_PresentVertex)
						{
							continue;
						}
					    
						if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
						{
							continue;
						}  
					    
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;

					}
				
					vi_FirstTreated[m_vi_Edges[j]] = i_PresentVertex;
				}
				else
				{		
	      			vi_FirstSeenOne[i_ColorID] = i_PresentVertex;
					vi_FirstSeenTwo[i_ColorID] = m_vi_Edges[j];

					for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
					{
						if(m_vi_Edges[k] == i_PresentVertex)
						{
							continue;
						}
					
						if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
						{
							continue;
						}

						if(m_vi_Edges[j] < m_vi_Edges[k])
						{
							if(vi_StarHubMap[vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]]] == m_vi_Edges[k])
							{
								vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
							}
						}
						else
						{
							if(vi_StarHubMap[vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]]] == m_vi_Edges[k])
							{
								vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
							}
						}
					}
				}
			}

			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;
			    
					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{	    
				_FOUND = _FALSE;
		    
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}
		    
				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
			
					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}  
			
					if(m_vi_VertexColors[m_vi_Edges[k]] == m_vi_VertexColors[i_PresentVertex])
					{
						_FOUND = _TRUE;
				    
						if(m_vi_Edges[j] < m_vi_Edges[k])
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]];

							vi_StarHubMap[i_StarID] = m_vi_Edges[j];
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
						else
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]];

							vi_StarHubMap[i_StarID] = m_vi_Edges[j];
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
				    
						break;
					}
				}
		    
				if (!_FOUND)
				{
					i_VertexOne = vi_FirstSeenOne[m_vi_VertexColors[m_vi_Edges[j]]];
					i_VertexTwo = vi_FirstSeenTwo[m_vi_VertexColors[m_vi_Edges[j]]];
				
					if((i_VertexOne == i_PresentVertex) && (i_VertexTwo != m_vi_Edges[j]))
					{
						if(i_PresentVertex < i_VertexTwo)
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][i_VertexTwo]];

							vi_StarHubMap[i_StarID] = i_PresentVertex;
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
						else
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[i_VertexTwo][i_PresentVertex]];

							vi_StarHubMap[i_StarID] = i_PresentVertex;
						
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
					}
				}
			}
		}

#if VERBOSE == _TRUE

		cout<<endl;

#endif

#if STATISTICS == _TRUE

		vector<int> vi_Hubs;

		vi_Hubs.resize((unsigned) i_EdgeCount/2, _FALSE);

		m_i_ColoringUnits = _FALSE;

		for(i=0; i<i_EdgeCount/2; i++)
		{
			if(vi_StarHubMap[i] == _UNKNOWN)
			{
				m_i_ColoringUnits++;

				continue;
			}

			if(vi_Hubs[vi_StarHubMap[i]] == _FALSE)
			{
				vi_Hubs[vi_StarHubMap[i]] = _TRUE;

				m_i_ColoringUnits++;
			}
		}

#endif

		return(_TRUE);

	}

	
	//Public Function 1459
	int GraphColoring::StarColoring(vector<int> & vi_StarHubMap, vector<int> & vi_EdgeStarMap, map< int, map<int, int> > & mimi2_VertexEdgeMap)
	{
		int i, j, k;

		int _FOUND;

		int i_ColorID, i_StarID;

		int i_PresentVertex;

		int i_VertexCount, i_EdgeCount;

		int i_VertexOne, i_VertexTwo;

		vector<int> vi_MemberEdges;

		vector<int> vi_CandidateColors;
	 
		vector<int> vi_FirstTreated;
	 
		vector<int> vi_FirstSeenOne, vi_FirstSeenTwo;

		m_i_VertexColorCount = _UNKNOWN;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		i_EdgeCount = (signed) m_vi_Edges.size();

		vi_EdgeStarMap.clear();
		vi_EdgeStarMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		vi_StarHubMap.clear();
		vi_StarHubMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_FirstSeenOne.clear();
		vi_FirstSeenOne.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenTwo.clear();
		vi_FirstSeenTwo.resize((unsigned) i_VertexCount, _UNKNOWN);

	//    vi_FirstTreated.clear();
	//    vi_FirstTreated.resize((unsigned) i_EdgeCount, _UNKNOWN);

		vi_FirstTreated.clear();
		vi_FirstTreated.resize((unsigned) i_VertexCount, _UNKNOWN);

		k = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					mimi2_VertexEdgeMap[i][m_vi_Edges[j]] = k;

					vi_EdgeStarMap[k] = k;

					k++;
				}
			}
		}

#if VERBOSE == _TRUE

		cout<<endl;

#endif

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 305 | Star Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				i_ColorID = m_vi_VertexColors[m_vi_Edges[j]];

				if(i_ColorID == _UNKNOWN)
				{
				  continue;
				}
				
				vi_CandidateColors[i_ColorID] = i_PresentVertex;
			    
				i_VertexOne = vi_FirstSeenOne[i_ColorID];
				i_VertexTwo = vi_FirstSeenTwo[i_ColorID];
		    
				if(i_VertexOne == i_PresentVertex)
				{
					if(vi_FirstTreated[i_VertexTwo] != i_PresentVertex)
					{
					    
						for(k=m_vi_Vertices[i_VertexTwo]; k<m_vi_Vertices[STEP_UP(i_VertexTwo)]; k++)
						{
							if(m_vi_Edges[k] == i_PresentVertex)
							{
								continue;
							}
						
							if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
							{
								continue;
							}  
						
							vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;

						}
					    
						vi_FirstTreated[i_VertexTwo] = i_PresentVertex;
					}
					
					for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
					{
						if(m_vi_Edges[k] == i_PresentVertex)
						{
							continue;
						}
					    
						if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
						{
							continue;
						}  
					    
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;

					}
				
					vi_FirstTreated[m_vi_Edges[j]] = i_PresentVertex;
				}
				else
				{		
	      			vi_FirstSeenOne[i_ColorID] = i_PresentVertex;
					vi_FirstSeenTwo[i_ColorID] = m_vi_Edges[j];

					for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
					{
						if(m_vi_Edges[k] == i_PresentVertex)
						{
							continue;
						}
					
						if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
						{
							continue;
						}

						if(m_vi_Edges[j] < m_vi_Edges[k])
						{
							if(vi_StarHubMap[vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]]] == m_vi_Edges[k])
							{
								vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
							}
						}
						else
						{
							if(vi_StarHubMap[vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]]] == m_vi_Edges[k])
							{
								vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
							}
						}
					}
				}
			}

			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;
			    
					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}
		
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{	    
				_FOUND = _FALSE;
		    
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}
		    
				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
			
					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}  
			
					if(m_vi_VertexColors[m_vi_Edges[k]] == m_vi_VertexColors[i_PresentVertex])
					{
						_FOUND = _TRUE;
				    
						if(m_vi_Edges[j] < m_vi_Edges[k])
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]];

							vi_StarHubMap[i_StarID] = m_vi_Edges[j];
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
						else
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]];

							vi_StarHubMap[i_StarID] = m_vi_Edges[j];
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
				    
						break;
					}
				}
		    
				if (!_FOUND)
				{
					i_VertexOne = vi_FirstSeenOne[m_vi_VertexColors[m_vi_Edges[j]]];
					i_VertexTwo = vi_FirstSeenTwo[m_vi_VertexColors[m_vi_Edges[j]]];
				
					if((i_VertexOne == i_PresentVertex) && (i_VertexTwo != m_vi_Edges[j]))
					{
						if(i_PresentVertex < i_VertexTwo)
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][i_VertexTwo]];

							vi_StarHubMap[i_StarID] = i_PresentVertex;
					
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
						else
						{
							i_StarID = vi_EdgeStarMap[mimi2_VertexEdgeMap[i_VertexTwo][i_PresentVertex]];

							vi_StarHubMap[i_StarID] = i_PresentVertex;
						
							if(i_PresentVertex < m_vi_Edges[j])
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]]] = i_StarID;
							}
							else
							{
								vi_EdgeStarMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex]] = i_StarID;
							}
						}
					}
				}
			}
		}

#if VERBOSE == _TRUE

		cout<<endl;

#endif

#if STATISTICS == _TRUE

		vector<int> vi_Hubs;

		vi_Hubs.resize((unsigned) i_EdgeCount/2, _FALSE);

		m_i_ColoringUnits = _FALSE;

		for(i=0; i<i_EdgeCount/2; i++)
		{
			if(vi_StarHubMap[i] == _UNKNOWN)
			{
				m_i_ColoringUnits++;

				continue;
			}

			if(vi_Hubs[vi_StarHubMap[i]] == _FALSE)
			{
				vi_Hubs[vi_StarHubMap[i]] = _TRUE;

				m_i_ColoringUnits++;
			}
		}

#endif

		return(_TRUE);

	}



	
	//Public Function 1460
	int GraphColoring::CheckStarColoring()
	{
		int i, j, k, l;

		int i_VertexCount, i_EdgeCount;

		int i_FirstColor, i_SecondColor, i_ThirdColor, i_FourthColor;

		int i_ViolationCount;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		i_EdgeCount = (signed) m_vi_Edges.size();

		i_ViolationCount = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			i_FirstColor = m_vi_VertexColors[i];

			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				i_SecondColor = m_vi_VertexColors[m_vi_Edges[j]];

				if(i_SecondColor == i_FirstColor)
				{
					i_ViolationCount++;

					if(i_ViolationCount == _TRUE)
					{
						cout<<endl;
						cout<<"Star Coloring | Violation Check | "<<m_s_InputFile<<endl;
						cout<<endl;
					}

					cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(i)<<" ["<<STEP_UP(i_FirstColor)<<"] - "<<STEP_UP(m_vi_Edges[j])<<" ["<<STEP_UP(i_SecondColor)<<"]"<<endl;

					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i)
					{
						continue;
					}

					i_ThirdColor = m_vi_VertexColors[m_vi_Edges[k]];

					if(i_ThirdColor == i_SecondColor)
					{
						i_ViolationCount++;

						if(i_ViolationCount == _TRUE)
						{
							cout<<endl;
							cout<<"Star Coloring | Violation Check | "<<m_s_InputFile<<endl;
							cout<<endl;
						}

						cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(m_vi_Edges[j])<<" ["<<STEP_UP(i_SecondColor)<<"] - "<<STEP_UP(m_vi_Edges[k])<<" ["<<STEP_UP(i_ThirdColor)<<"]"<<endl;

						continue;
					}
					
					if(i_ThirdColor != i_FirstColor)
					{
						continue;
					}
				
					if(i_ThirdColor == i_FirstColor)
					{
						for(l=m_vi_Vertices[m_vi_Edges[k]]; l<m_vi_Vertices[STEP_UP(m_vi_Edges[k])]; l++)
						{
							if((m_vi_Edges[l] == m_vi_Edges[j]))
							{
								continue;
							}

							i_FourthColor = m_vi_VertexColors[m_vi_Edges[l]];

							if(i_FourthColor == i_ThirdColor)
							{
								i_ViolationCount++;
							
								if(i_ViolationCount == _TRUE)
								{
									cout<<endl;
									cout<<"Star Coloring | Violation Check | "<<m_s_InputFile<<endl;
									cout<<endl;
								}
				    
								cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(m_vi_Edges[k])<<" ["<<STEP_UP(i_ThirdColor)<<"] - "<<STEP_UP(m_vi_Edges[l])<<" ["<<STEP_UP(i_FourthColor)<<"]"<<endl;
							
							}
						
							if(i_FourthColor == i_SecondColor)
							{
								i_ViolationCount++;

								if(i_ViolationCount == _TRUE)
								{
									cout<<endl;
									cout<<"Star Coloring | Violation Check | "<<m_s_InputFile<<endl;
									cout<<endl;
								}
							    
								cout<<"Violation "<<i_ViolationCount<<"\t : "<<STEP_UP(i)<<" ["<<STEP_UP(i_FirstColor)<<"] - "<<STEP_UP(m_vi_Edges[j])<<" ["<<STEP_UP(i_SecondColor)<<"] - "<<STEP_UP(m_vi_Edges[k])<<" ["<<STEP_UP(i_ThirdColor)<<"] - "<<STEP_UP(m_vi_Edges[l])<<" ["<<STEP_UP(i_FourthColor)<<"]"<<endl;  

								continue;
							}
						}
					}
				}
			}
		}

		if(i_ViolationCount)
		{
			cout<<endl;
			cout<<"[Total Violations = "<<i_ViolationCount<<"]"<<endl;
			cout<<endl;
		}

		return(i_ViolationCount);
	}


	
	//Public Function 1461
	int GraphColoring::AcyclicColoring()
	{
		if(CheckVertexColoring("ACYCLIC"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_VertexCount, i_EdgeCount;

		int i_AdjacentEdgeID, i_EdgeID, i_SetID;

		int i_PresentVertex;

		vector<int> vi_CandidateColors;

		vector<int> vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree;
		vector<int> vi_FirstVisitedOne, vi_FirstVisitedTwo;
	    
#if DISJOINT_SETS == _FALSE
	    
		int l;

		int i_MemberCount;

		int i_SmallerSetID, i_BiggerSetID;
	    
		vector<int> vi_DisjointSets;
		vector<int> vi_MemberEdges;
		vector<int> vi_EdgeSetMap;
	 
		vector< vector<int> > v2i_SetEdgeMap;

#endif

#if DISJOINT_SETS == _TRUE

		int i_SetOneID, i_SetTwoID;

#endif

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		k = _FALSE;

		m_mimi2_VertexEdgeMap.clear();

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					m_mimi2_VertexEdgeMap[i][m_vi_Edges[j]] = k;

					k++;
				}
			}
		}

#if DEBUG == 1461

		i_EdgeCount = (signed) v2i_EdgeVertexMap.size();

		cout<<endl;
		cout<<"DEBUG 1461 | Acyclic Coloring | Edge Vertex Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			cout<<"Edge "<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(v2i_EdgeVertexMap[i][0])<<" - "<<STEP_UP(v2i_EdgeVertexMap[i][1])<<endl;
		}
	    
		cout<<endl;
		cout<<"DEBUG 1461 | Acyclic Coloring | Vertex Edge Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			cout<<"Vertex "<<STEP_UP(v2i_EdgeVertexMap[i][0])<<" - Vertex "<<STEP_UP(v2i_EdgeVertexMap[i][1])<<"\t"<<" : "<<STEP_UP(m_mimi2_VertexEdgeMap[v2i_EdgeVertexMap[i][0]][v2i_EdgeVertexMap[i][1]])<<endl;

		}

		cout<<endl;

#endif

		i_EdgeCount = (signed) m_vi_Edges.size();

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_FirstSeenOne.clear();
		vi_FirstSeenOne.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenTwo.clear();
		vi_FirstSeenTwo.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenThree.clear();
		vi_FirstSeenThree.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstVisitedOne.clear();
		vi_FirstVisitedOne.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		vi_FirstVisitedTwo.clear();
		vi_FirstVisitedTwo.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

#if DISJOINT_SETS == _FALSE
	    
		vi_MemberEdges.clear();

		vi_EdgeSetMap.clear();
		vi_EdgeSetMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		v2i_SetEdgeMap.clear();
		v2i_SetEdgeMap.resize((unsigned) i_EdgeCount/2);

		vi_DisjointSets.clear();

#endif

#if DISJOINT_SETS == _TRUE

		m_ds_DisjointSets.SetSize(i_EdgeCount/2);

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif

		m_i_VertexColorCount = _UNKNOWN;

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1461 | Acyclic Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}
				
					if(vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] == i_PresentVertex)
					{
						continue;
					}
				
#if DISJOINT_SETS == _TRUE
				
					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = m_ds_DisjointSets.FindAndCompress(m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]);
					}
					else
					{
						i_SetID = m_ds_DisjointSets.FindAndCompress(m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]);
					}
#endif
			
#if DISJOINT_SETS == _FALSE

					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = vi_EdgeSetMap[m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]];
					}
					else
					{
						i_SetID = vi_EdgeSetMap[m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]];
					}
#endif	       

					FindCycle(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], i_SetID, vi_CandidateColors, vi_FirstVisitedOne, vi_FirstVisitedTwo);
				}
			}

			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;

					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				if(i_PresentVertex < m_vi_Edges[j])
				{
					i_EdgeID = m_mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]];
				}
				else
				{
					i_EdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex];
				}

#if DISJOINT_SETS == _FALSE

				vi_DisjointSets.push_back(_TRUE);

				vi_EdgeSetMap[i_EdgeID] = STEP_DOWN((signed) vi_DisjointSets.size());

				v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].push_back(i_EdgeID);
#endif

				i_AdjacentEdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], i_PresentVertex, m_mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
			    
				if(i_AdjacentEdgeID != _UNKNOWN)
				{

#if DISJOINT_SETS == _TRUE

					i_SetOneID = m_ds_DisjointSets.FindAndCompress(i_EdgeID);
					i_SetTwoID = m_ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1461
		
					cout<<endl;
					cout<<"DEBUG 1461 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
				    
#endif    
			
					m_ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif

#if DISJOINT_SETS == _FALSE

#if DEBUG == 1461
		
					cout<<endl;
					cout<<"DEBUG 1461 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
			    
#endif    

					if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
					{
						i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
						i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
					else
					{
						i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
						i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
			
					vi_MemberEdges.clear();
					vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
					
					vi_DisjointSets[i_BiggerSetID] = _FALSE;

					i_MemberCount = (signed) vi_MemberEdges.size();

					for(k=0; k<i_MemberCount; k++)
					{
						vi_EdgeSetMap[vi_MemberEdges[k]] = i_SmallerSetID;
					    
						v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[k]);
					}	
#endif
				}
			}

		
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
			
					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == m_vi_VertexColors[i_PresentVertex])
					{
						if(m_vi_Edges[j] <  m_vi_Edges[k])
						{
							i_AdjacentEdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]];
						}
						else
						{
							i_AdjacentEdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]];
						}

						i_EdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], m_mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
					
						if(i_EdgeID != _UNKNOWN)
						{  		

#if DISJOINT_SETS == _TRUE	

							i_SetOneID = m_ds_DisjointSets.FindAndCompress(i_EdgeID);
							i_SetTwoID = m_ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1461
							cout<<endl;
							cout<<"DEBUG 1461 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
					
							m_ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif			

#if DISJOINT_SETS == _FALSE
				     
#if DEBUG == 1461
							cout<<endl;
							cout<<"DEBUG 1461 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
		  
							if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
							{
								i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
								i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
							else
							{
								i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
								i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
					
							vi_MemberEdges.clear();
							vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
							
							vi_DisjointSets[i_BiggerSetID] = _FALSE;
							
							i_MemberCount = (signed) vi_MemberEdges.size();
							
							for(l=0; l<i_MemberCount; l++)
							{
								vi_EdgeSetMap[vi_MemberEdges[l]] = i_SmallerSetID;
							    
								v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[l]);
							}
				       
#endif
						}
					}
				}
			}

#if DEBUG == 1461

			cout<<endl;
			cout<<"DEBUG 1461 | Acyclic Coloring | Vertex Colors | Upto Vertex "<<STEP_UP(i)<<endl;
			cout<<endl;

			for(j=0; j<i_VertexCount; j++)
			{
				cout<<"Vertex "<<STEP_UP(j)<<" = "<<STEP_UP(m_vi_VertexColors[j])<<endl;
			}
#endif

		}	
	    

#if DEBUG == 1461

#if DISJOINT_SETS == _FALSE

		i_EdgeCount = (signed) v2i_EdgeVertexMap.size();

		cout<<endl;
		cout<<"DEBUG 1461 | Acyclic Coloring | Edge Set Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(vi_EdgeSetMap[i])<<endl;
		}

		cout<<endl;
		cout<<"DEBUG 1461 | Acyclic Coloring | Set Edge Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			i_MemberCount = (signed) v2i_SetEdgeMap[i].size();

			if(i_MemberCount == _FALSE)
			{
				continue;
			}

			cout<<"Set "<<STEP_UP(i)<<"\t"<<" : ";

			for(j=0; j<i_MemberCount; j++)
			{
				if(j == STEP_DOWN(i_MemberCount))
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<" ("<<i_MemberCount<<")"<<endl;
				}
				else
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<", ";
				}
			}
		}

		cout<<endl;
	    
#endif

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif

#if STATISTICS == _TRUE

#if DISJOINT_SETS == _TRUE

		m_i_ColoringUnits = m_ds_DisjointSets.Count();


#elif DISJOINT_SETS == _FALSE

		int i_SetSize;

		m_i_ColoringUnits = _FALSE;

		i_SetSize = (unsigned) v2i_SetEdgeMap.size();

		for(i=0; i<i_SetSize; i++)
		{
			if(v2i_SetEdgeMap[i].empty())
			{
				continue;
			}
		
			m_i_ColoringUnits++;
		}

#endif

#endif

		return(_TRUE);

	}


	int GraphColoring::AcyclicColoring_ForIndirectRecovery() {
//#define DEBUG 1462
		if(CheckVertexColoring("ACYCLIC"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_VertexCount, i_EdgeCount;

		int i_AdjacentEdgeID, i_EdgeID, i_SetID;

		int i_PresentVertex;

		vector<int> vi_CandidateColors;

		vector<int> vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree;
		vector<int> vi_FirstVisitedOne, vi_FirstVisitedTwo;

		//m_mimi2_VertexEdgeMap is populated and used in this function;
	    
#if DISJOINT_SETS == _FALSE
	    
		int l;

		int i_MemberCount;

		int i_SmallerSetID, i_BiggerSetID;
	    
		vector<int> vi_DisjointSets;
		vector<int> vi_MemberEdges;
		vector<int> vi_EdgeSetMap;
	 
		vector< vector<int> > v2i_SetEdgeMap;

#endif

#if DISJOINT_SETS == _TRUE

		int i_SetOneID, i_SetTwoID;
	       
		//DisjointSets ds_DisjointSets;

#endif

		if(m_s_VertexColoringVariant.compare("ALL") != 0)
		{
			m_s_VertexColoringVariant = "ACYCLIC";
		}

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		k=_FALSE;

		//populate m_mimi2_VertexEdgeMap
		//Basically assign a number (k = 1, 2, 3 ...) for each edge of the graph
		m_mimi2_VertexEdgeMap.clear();

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					m_mimi2_VertexEdgeMap[i][m_vi_Edges[j]] = k;

					k++;
				}
			}
		}

#if DEBUG == 1462
	    
		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Edge Vertex Map"<<endl;
		cout<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
				cout<<"Edge "<<STEP_UP(m_mimi2_VertexEdgeMap[i][m_vi_Edges[j]])<<"\t"<<" : "<<STEP_UP(i)<<" - "<<STEP_UP(m_vi_Edges[j])<<endl;
				}
			}
		}
			
		cout<<endl;

#endif

		i_EdgeCount = (signed) m_vi_Edges.size();

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_FirstSeenOne.clear();
		vi_FirstSeenOne.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenTwo.clear();
		vi_FirstSeenTwo.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenThree.clear();
		vi_FirstSeenThree.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstVisitedOne.clear();
		vi_FirstVisitedOne.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		vi_FirstVisitedTwo.clear();
		vi_FirstVisitedTwo.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

//cout<<"*1"<<endl;
#if DISJOINT_SETS == _FALSE
	    
		vi_MemberEdges.clear();

		vi_EdgeSetMap.clear();
		vi_EdgeSetMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		v2i_SetEdgeMap.clear();
		v2i_SetEdgeMap.resize((unsigned) i_EdgeCount/2);

		vi_DisjointSets.clear();

#endif

#if DISJOINT_SETS == _TRUE

		m_ds_DisjointSets.SetSize(i_EdgeCount/2);

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif
	    
		m_i_VertexColorCount = _UNKNOWN;

//cout<<"*11 i_VertexCount="<<i_VertexCount<<endl;
		for(i=0; i<i_VertexCount; i++)
		{
//cout<<"*12 m_vi_OrderedVertices="<<m_vi_OrderedVertices.size()<<endl;
			i_PresentVertex = m_vi_OrderedVertices[i];
//cout<<"*13 i_PresentVertex="<<i_PresentVertex<<endl;

#if VERBOSE == _TRUE
//#if DEBUG == 1462

			cout<<"DEBUG 1462 | Acyclic Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}
				
					if(vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] == i_PresentVertex)
					{
						continue;
					}
			
#if DISJOINT_SETS == _TRUE
			
					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = m_ds_DisjointSets.FindAndCompress(m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]);
					}
					else
					{
						i_SetID = m_ds_DisjointSets.FindAndCompress(m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]);
					}
#endif
		
#if DISJOINT_SETS == _FALSE

					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = vi_EdgeSetMap[m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]];
					}
					else
					{
						i_SetID = vi_EdgeSetMap[m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]];
					}
#endif	       

					FindCycle(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], i_SetID, vi_CandidateColors, vi_FirstVisitedOne, vi_FirstVisitedTwo);
				}
			}

			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;
					
					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				if(i_PresentVertex < m_vi_Edges[j])
				{
					i_EdgeID = m_mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]];
				}
				else
				{
					i_EdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex];
				}

#if DISJOINT_SETS == _FALSE

				vi_DisjointSets.push_back(_TRUE);

				vi_EdgeSetMap[i_EdgeID] = STEP_DOWN((signed) vi_DisjointSets.size());

				v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].push_back(i_EdgeID);
#endif

//cout<<"*2"<<endl;
				i_AdjacentEdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], i_PresentVertex, m_mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
		    
				if(i_AdjacentEdgeID != _UNKNOWN)
				{

#if DISJOINT_SETS == _TRUE

					i_SetOneID = m_ds_DisjointSets.FindAndCompress(i_EdgeID);
					i_SetTwoID = m_ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1462
		
					cout<<endl;
					cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
				    
#endif    
			
					m_ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif

#if DISJOINT_SETS == _FALSE

#if DEBUG == 1462
		
					cout<<endl;
					cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
			    
#endif    

					if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
					{
						i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
						i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
					else
					{
						i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
						i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
					
					vi_MemberEdges.clear();
					vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
					
					vi_DisjointSets[i_BiggerSetID] = _FALSE;

					i_MemberCount = (signed) vi_MemberEdges.size();

					for(k=0; k<i_MemberCount; k++)
					{
						vi_EdgeSetMap[vi_MemberEdges[k]] = i_SmallerSetID;
					    
						v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[k]);
					}	
#endif
				}
			}

		
//cout<<"*3"<<endl;
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
				
					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == m_vi_VertexColors[i_PresentVertex])
					{
						if(m_vi_Edges[j] <  m_vi_Edges[k])
						{
							i_AdjacentEdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]];
						}
						else
						{
							i_AdjacentEdgeID = m_mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]];
						}

						i_EdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], m_mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
				
						if(i_EdgeID != _UNKNOWN)
						{  		

#if DISJOINT_SETS == _TRUE	

							i_SetOneID = m_ds_DisjointSets.FindAndCompress(i_EdgeID);
							i_SetTwoID = m_ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1462
							cout<<endl;
							cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
				
							m_ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif			

#if DISJOINT_SETS == _FALSE
			     
#if DEBUG == 1462
				
							cout<<endl;
							cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
	  
							if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
							{
								i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
								i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
							else
							{
								i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
								i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
						
							vi_MemberEdges.clear();
							vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
							
							vi_DisjointSets[i_BiggerSetID] = _FALSE;
							
							i_MemberCount = (signed) vi_MemberEdges.size();
						
							for(l=0; l<i_MemberCount; l++)
							{
								vi_EdgeSetMap[vi_MemberEdges[l]] = i_SmallerSetID;
							    
								v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[l]);
							}
			       
#endif
						}
					}
				}
			}

#if DEBUG == 1462

			cout<<endl;
			cout<<"DEBUG 1462 | Acyclic Coloring | Vertex Colors | Upto Vertex "<<STEP_UP(i)<<endl;
			cout<<endl;

			for(j=0; j<i_VertexCount; j++)
			{
				cout<<"Vertex "<<STEP_UP(j)<<" = "<<STEP_UP(m_vi_VertexColors[j])<<endl;
			}
#endif

		}
	    

#if DEBUG == 1462

#if DISJOINT_SETS == _FALSE

		i_EdgeCount = (signed) v2i_EdgeVertexMap.size();

		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Edge Set Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(vi_EdgeSetMap[i])<<endl;
		}

		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Set Edge Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			i_MemberCount = (signed) v2i_SetEdgeMap[i].size();

			if(i_MemberCount == _FALSE)
			{
				continue;
			}

			cout<<"Set "<<STEP_UP(i)<<"\t"<<" : ";

			for(j=0; j<i_MemberCount; j++)
			{
				if(j == STEP_DOWN(i_MemberCount))
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<" ("<<i_MemberCount<<")"<<endl;
				}
				else
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<", ";
				}
			}
		}

		cout<<endl;
	    
#endif

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif


#if DISJOINT_SETS == _FALSE

		vi_Sets.clear();
		mivi_VertexSets.clear();
	    
		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					i_EdgeID = m_mimi2_VertexEdgeMap[i][m_vi_Edges[j]];

					i_SetID = vi_EdgeSetMap[i_EdgeID];

					if(mivi_VertexSets[i_SetID].empty())
					{
						vi_Sets.push_back(i_SetID);
					}

					mivi_VertexSets[i_SetID].push_back(i);
					mivi_VertexSets[i_SetID].push_back(m_vi_Edges[j]);
				}
			}
		}

#endif

		return(_TRUE);
	}

	
	//Public Function 1462
	int GraphColoring::AcyclicColoring(vector<int> & vi_Sets, map< int, vector<int> > & mivi_VertexSets)
	{
//#define DEBUG 1462
		if(CheckVertexColoring("ACYCLIC"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_VertexCount, i_EdgeCount;

		int i_AdjacentEdgeID, i_EdgeID, i_SetID;

		int i_PresentVertex;

		vector<int> vi_CandidateColors;

		vector<int> vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree;
		vector<int> vi_FirstVisitedOne, vi_FirstVisitedTwo;

		map< int, map<int, int> > mimi2_VertexEdgeMap;
	    
#if DISJOINT_SETS == _FALSE
	    
		int l;

		int i_MemberCount;

		int i_SmallerSetID, i_BiggerSetID;
	    
		vector<int> vi_DisjointSets;
		vector<int> vi_MemberEdges;
		vector<int> vi_EdgeSetMap;
	 
		vector< vector<int> > v2i_SetEdgeMap;

#endif

#if DISJOINT_SETS == _TRUE

		int i_SetOneID, i_SetTwoID;
	       
		DisjointSets ds_DisjointSets;

#endif

		if(m_s_VertexColoringVariant.compare("ALL") != 0)
		{
			m_s_VertexColoringVariant = "ACYCLIC";
		}

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		k=_FALSE;

		//populate mimi2_VertexEdgeMap
		//Basically assign a number (k = 1, 2, 3 ...) for each edge of the graph
		mimi2_VertexEdgeMap.clear();

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					mimi2_VertexEdgeMap[i][m_vi_Edges[j]] = k;

					k++;
				}
			}
		}

#if DEBUG == 1462
	    
		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Edge Vertex Map"<<endl;
		cout<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
				cout<<"Edge "<<STEP_UP(mimi2_VertexEdgeMap[i][m_vi_Edges[j]])<<"\t"<<" : "<<STEP_UP(i)<<" - "<<STEP_UP(m_vi_Edges[j])<<endl;
				}
			}
		}
			
		cout<<endl;

#endif

		i_EdgeCount = (signed) m_vi_Edges.size();

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_FirstSeenOne.clear();
		vi_FirstSeenOne.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenTwo.clear();
		vi_FirstSeenTwo.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstSeenThree.clear();
		vi_FirstSeenThree.resize((unsigned) i_VertexCount, _UNKNOWN);

		vi_FirstVisitedOne.clear();
		vi_FirstVisitedOne.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		vi_FirstVisitedTwo.clear();
		vi_FirstVisitedTwo.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

//cout<<"*1"<<endl;
#if DISJOINT_SETS == _FALSE
	    
		vi_MemberEdges.clear();

		vi_EdgeSetMap.clear();
		vi_EdgeSetMap.resize((unsigned) i_EdgeCount/2, _UNKNOWN);

		v2i_SetEdgeMap.clear();
		v2i_SetEdgeMap.resize((unsigned) i_EdgeCount/2);

		vi_DisjointSets.clear();

#endif

#if DISJOINT_SETS == _TRUE

		ds_DisjointSets.SetSize(i_EdgeCount/2);

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif
	    
		m_i_VertexColorCount = _UNKNOWN;

//cout<<"*11 i_VertexCount="<<i_VertexCount<<endl;
		for(i=0; i<i_VertexCount; i++)
		{
//cout<<"*12 m_vi_OrderedVertices="<<m_vi_OrderedVertices.size()<<endl;
			i_PresentVertex = m_vi_OrderedVertices[i];
//cout<<"*13 i_PresentVertex="<<i_PresentVertex<<endl;

#if VERBOSE == _TRUE
//#if DEBUG == 1462

			cout<<"DEBUG 1462 | Acyclic Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}
				
					if(vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] == i_PresentVertex)
					{
						continue;
					}
			
#if DISJOINT_SETS == _TRUE
			
					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = ds_DisjointSets.FindAndCompress(mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]);
					}
					else
					{
						i_SetID = ds_DisjointSets.FindAndCompress(mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]);
					}
#endif
		
#if DISJOINT_SETS == _FALSE

					if(m_vi_Edges[j] < m_vi_Edges[k])
					{
						i_SetID = vi_EdgeSetMap[mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]]];
					}
					else
					{
						i_SetID = vi_EdgeSetMap[mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]]];
					}
#endif	       

					FindCycle(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], i_SetID, vi_CandidateColors, vi_FirstVisitedOne, vi_FirstVisitedTwo);
				}
			}

			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;
					
					if(m_i_VertexColorCount < j)
					{
						m_i_VertexColorCount = j;
					}

					break;
				}
			}

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				if(i_PresentVertex < m_vi_Edges[j])
				{
					i_EdgeID = mimi2_VertexEdgeMap[i_PresentVertex][m_vi_Edges[j]];
				}
				else
				{
					i_EdgeID = mimi2_VertexEdgeMap[m_vi_Edges[j]][i_PresentVertex];
				}

#if DISJOINT_SETS == _FALSE

				vi_DisjointSets.push_back(_TRUE);

				vi_EdgeSetMap[i_EdgeID] = STEP_DOWN((signed) vi_DisjointSets.size());

				v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].push_back(i_EdgeID);
#endif

//cout<<"*2"<<endl;
				i_AdjacentEdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], i_PresentVertex, mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
		    
				if(i_AdjacentEdgeID != _UNKNOWN)
				{

#if DISJOINT_SETS == _TRUE

					i_SetOneID = ds_DisjointSets.FindAndCompress(i_EdgeID);
					i_SetTwoID = ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1462
		
					cout<<endl;
					cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
				    
#endif    
			
					ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif

#if DISJOINT_SETS == _FALSE

#if DEBUG == 1462
		
					cout<<endl;
					cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
					cout<<endl;
			    
#endif    

					if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
					{
						i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
						i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
					else
					{
						i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
						i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
					}
					
					vi_MemberEdges.clear();
					vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
					
					vi_DisjointSets[i_BiggerSetID] = _FALSE;

					i_MemberCount = (signed) vi_MemberEdges.size();

					for(k=0; k<i_MemberCount; k++)
					{
						vi_EdgeSetMap[vi_MemberEdges[k]] = i_SmallerSetID;
					    
						v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[k]);
					}	
#endif
				}
			}

		
//cout<<"*3"<<endl;
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] == _UNKNOWN)
				{
					continue;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}
				
					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == m_vi_VertexColors[i_PresentVertex])
					{
						if(m_vi_Edges[j] <  m_vi_Edges[k])
						{
							i_AdjacentEdgeID = mimi2_VertexEdgeMap[m_vi_Edges[j]][m_vi_Edges[k]];
						}
						else
						{
							i_AdjacentEdgeID = mimi2_VertexEdgeMap[m_vi_Edges[k]][m_vi_Edges[j]];
						}

						i_EdgeID = UpdateSet(i_PresentVertex, m_vi_Edges[j], m_vi_Edges[k], mimi2_VertexEdgeMap, vi_FirstSeenOne, vi_FirstSeenTwo, vi_FirstSeenThree);
				
						if(i_EdgeID != _UNKNOWN)
						{  		

#if DISJOINT_SETS == _TRUE	

							i_SetOneID = ds_DisjointSets.FindAndCompress(i_EdgeID);
							i_SetTwoID = ds_DisjointSets.FindAndCompress(i_AdjacentEdgeID);

#if DEBUG == 1462
							cout<<endl;
							cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(i_SetOneID)<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(i_SetTwoID)<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
				
							ds_DisjointSets.UnionBySize(i_SetOneID, i_SetTwoID);

#endif			

#if DISJOINT_SETS == _FALSE
			     
#if DEBUG == 1462
				
							cout<<endl;
							cout<<"DEBUG 1462 | Acyclic Coloring | Unify Tree | Tree "<<STEP_UP(vi_EdgeSetMap[i_EdgeID])<<" (Edge "<<STEP_UP(i_EdgeID)<<") and Tree "<<STEP_UP(vi_EdgeSetMap[i_AdjacentEdgeID])<<" (Edge "<<STEP_UP(i_AdjacentEdgeID)<<")"<<endl;
							cout<<endl;
				    
#endif  
	  
							if(v2i_SetEdgeMap[vi_EdgeSetMap[i_EdgeID]].size() < v2i_SetEdgeMap[vi_EdgeSetMap[i_AdjacentEdgeID]].size())
							{
								i_SmallerSetID = vi_EdgeSetMap[i_EdgeID];
								i_BiggerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
							else
							{
								i_BiggerSetID = vi_EdgeSetMap[i_EdgeID];
								i_SmallerSetID = vi_EdgeSetMap[i_AdjacentEdgeID];
							}
						
							vi_MemberEdges.clear();
							vi_MemberEdges.swap(v2i_SetEdgeMap[i_BiggerSetID]);
							
							vi_DisjointSets[i_BiggerSetID] = _FALSE;
							
							i_MemberCount = (signed) vi_MemberEdges.size();
						
							for(l=0; l<i_MemberCount; l++)
							{
								vi_EdgeSetMap[vi_MemberEdges[l]] = i_SmallerSetID;
							    
								v2i_SetEdgeMap[i_SmallerSetID].push_back(vi_MemberEdges[l]);
							}
			       
#endif
						}
					}
				}
			}

#if DEBUG == 1462

			cout<<endl;
			cout<<"DEBUG 1462 | Acyclic Coloring | Vertex Colors | Upto Vertex "<<STEP_UP(i)<<endl;
			cout<<endl;

			for(j=0; j<i_VertexCount; j++)
			{
				cout<<"Vertex "<<STEP_UP(j)<<" = "<<STEP_UP(m_vi_VertexColors[j])<<endl;
			}
#endif

		}
	    

#if DEBUG == 1462

#if DISJOINT_SETS == _FALSE

		i_EdgeCount = (signed) v2i_EdgeVertexMap.size();

		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Edge Set Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			cout<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(vi_EdgeSetMap[i])<<endl;
		}

		cout<<endl;
		cout<<"DEBUG 1462 | Acyclic Coloring | Set Edge Map"<<endl;
		cout<<endl;

		for(i=0; i<i_EdgeCount; i++)
		{
			i_MemberCount = (signed) v2i_SetEdgeMap[i].size();

			if(i_MemberCount == _FALSE)
			{
				continue;
			}

			cout<<"Set "<<STEP_UP(i)<<"\t"<<" : ";

			for(j=0; j<i_MemberCount; j++)
			{
				if(j == STEP_DOWN(i_MemberCount))
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<" ("<<i_MemberCount<<")"<<endl;
				}
				else
				{
					cout<<STEP_UP(v2i_SetEdgeMap[i][j])<<", ";
				}
			}
		}

		cout<<endl;
	    
#endif

#endif

#if VERBOSE == _TRUE

		cout<<endl;

#endif

#if DISJOINT_SETS == _TRUE
//cout<<"*Here is the difference"<<endl;
//ds_DisjointSets.Print();
		vi_Sets.clear();
		mivi_VertexSets.clear();

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		for(i=0; i<i_VertexCount; i++) // for each vertex A (indexed by i)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++) // for each of the vertex B that connect to A
			{
				if(i < m_vi_Edges[j]) // if the index of A (i) is less than the index of B (m_vi_Edges[j])
										//basic each edge is represented by (vertex with smaller ID, vertex with larger ID). This way, we don't insert a specific edge twice
				{
					i_EdgeID = mimi2_VertexEdgeMap[i][m_vi_Edges[j]];

					i_SetID = ds_DisjointSets.FindAndCompress(i_EdgeID);
					
					if(i_SetID == i_EdgeID) // that edge is the root of the set => create new set
					{
						vi_Sets.push_back(i_SetID);
					}

					mivi_VertexSets[i_SetID].push_back(i);
					mivi_VertexSets[i_SetID].push_back(m_vi_Edges[j]);
				}
			}
		}
//cout<<"*Am I here?"<<endl;

#endif

#if DISJOINT_SETS == _FALSE

		vi_Sets.clear();
		mivi_VertexSets.clear();
	    
		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
	       
		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				if(i < m_vi_Edges[j])
				{
					i_EdgeID = mimi2_VertexEdgeMap[i][m_vi_Edges[j]];

					i_SetID = vi_EdgeSetMap[i_EdgeID];

					if(mivi_VertexSets[i_SetID].empty())
					{
						vi_Sets.push_back(i_SetID);
					}

					mivi_VertexSets[i_SetID].push_back(i);
					mivi_VertexSets[i_SetID].push_back(m_vi_Edges[j]);
				}
			}
		}

#endif

		return(_TRUE);
	}

	
	//Public Function 1463
	int GraphColoring::CheckAcyclicColoring()
	{
		int i;

		int i_VertexCount;

		int i_ViolationCount;

		vector<int> vi_TouchedVertices;

		i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());  

		i_ViolationCount = _FALSE;

		for(i=0; i<i_VertexCount; i++)
		{
			vi_TouchedVertices.clear();
			vi_TouchedVertices.resize((unsigned) i_VertexCount, _FALSE);

			vi_TouchedVertices[i] = _TRUE;

			i_ViolationCount = SearchDepthFirst(i, i, i, vi_TouchedVertices);
		}

		if(i_ViolationCount)
		{
			cout<<endl;
			cout<<"[Total Violations = "<<i_ViolationCount<<"]"<<endl;
			cout<<endl;
		}

		return(i_ViolationCount);
	}

	
	//Public Function 1464
	int GraphColoring::TriangularColoring()
	{
		if(CheckVertexColoring("TRIANGULAR"))
		{
			return(_TRUE);
		}

		int i, j, k, l;

		int _FOUND;

		int i_VertexCount, i_VertexDegree;

		int i_HighestVertexDegree;

		int i_PresentVertex;

		vector<int> vi_VertexHierarchy;

		vector< vector<int> > v2i_VertexAdjacency;

		i_VertexCount = (signed) m_vi_OrderedVertices.size();

		vi_VertexHierarchy.clear();
		vi_VertexHierarchy.resize((unsigned) i_VertexCount);

		v2i_VertexAdjacency.clear();
		v2i_VertexAdjacency.resize((unsigned) i_VertexCount);
		
		for(i=0; i<i_VertexCount; i++)
		{
			vi_VertexHierarchy[m_vi_OrderedVertices[i]] = i;
		}

		//m_vi_VertexColors.clear();
		//m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN);  

		m_i_VertexColorCount = _UNKNOWN;

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1464 | Triangular Coloring | Processing Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				v2i_VertexAdjacency[i_PresentVertex].push_back(m_vi_Edges[j]);

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if((vi_VertexHierarchy[m_vi_Edges[j]] > vi_VertexHierarchy[i_PresentVertex]) && (vi_VertexHierarchy[m_vi_Edges[j]] > vi_VertexHierarchy[m_vi_Edges[k]]))
					{
						_FOUND = _FALSE;

						for(l=m_vi_Vertices[m_vi_Edges[k]]; l<m_vi_Vertices[STEP_UP(m_vi_Edges[k])]; l++)
						{
							if(m_vi_Edges[l] == i_PresentVertex)
							{
								_FOUND = TRUE;
							    
								break;
							}
						}
					    
						if(_FOUND == _FALSE)
						{
							v2i_VertexAdjacency[i_PresentVertex].push_back(m_vi_Edges[k]);
						}
					}
				} 
			}
		}

		m_vi_Vertices.clear();
		m_vi_Edges.clear();

		i_HighestVertexDegree = _UNKNOWN;

		for(i=0; i<i_VertexCount; i++)
		{
			m_vi_Vertices.push_back((signed) m_vi_Edges.size());

			i_VertexDegree = (signed) v2i_VertexAdjacency[i].size();

			if(i_HighestVertexDegree < i_VertexDegree)
			{
				i_HighestVertexDegree = i_VertexDegree;
			}

			for(j=0; j<i_VertexDegree; j++)
			{
				m_vi_Edges.push_back(v2i_VertexAdjacency[i][j]);
			}

			v2i_VertexAdjacency[i].clear();
		}

		m_vi_Vertices.push_back((signed) m_vi_Edges.size());

#if DEBUG == 1464

		int i_EdgeCount;

		cout<<endl;
		cout<<"DEBUG 1464 | Graph Coloring | Induced Matrix"<<endl;
		cout<<endl;

		i_VertexCount = (signed) m_vi_Vertices.size();
		i_EdgeCount = (signed) m_vi_Edges.size();

		for(i=0; i<i_VertexCount; i++)
		{
			for(j=m_vi_Vertices[i]; j<m_vi_Vertices[STEP_UP(i)]; j++)
			{
				cout<<"Element["<<STEP_UP(i)<<"]["<<STEP_UP(m_vi_Edges[j])<<"] = 1"<<endl;
			}
		}

		cout<<endl;
		cout<<"[Induced Vertices = "<<STEP_DOWN(i_VertexCount)<<"; Induced Edges = "<<i_EdgeCount<<"]"<<endl; 
		cout<<endl;

#endif

		SmallestLastOrdering();

		return(DistanceOneColoring());
	}


	
	//Public Function 1465
	int GraphColoring::ModifiedTriangularColoring()
	{
		if(CheckVertexColoring("MODIFIED TRIANGULAR"))
		{
			return(_TRUE);
		}

		int i, j, k;

		int i_VertexCount;

		int i_HighestColor;

		int i_PresentVertex;
    
		vector<int> vi_CandidateColors;

		vector<int> vi_VertexHierarchy;

		i_VertexCount = (signed) m_vi_OrderedVertices.size();

		vi_VertexHierarchy.clear();
		vi_VertexHierarchy.resize((unsigned) i_VertexCount);

		for(i=0; i<i_VertexCount; i++)
		{
			vi_VertexHierarchy[m_vi_OrderedVertices[i]] = i;
		}

		m_vi_VertexColors.clear();
		m_vi_VertexColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		vi_CandidateColors.clear();
		vi_CandidateColors.resize((unsigned) i_VertexCount, _UNKNOWN); 

		i_HighestColor = _UNKNOWN;

		for(i=0; i<i_VertexCount; i++)
		{
			i_PresentVertex = m_vi_OrderedVertices[i];

#if VERBOSE == _TRUE

			cout<<"DEBUG 1465 | Triangular Coloring | Coloring Vertex "<<STEP_UP(i_PresentVertex)<<"/"<<i_VertexCount<<endl;

#endif

			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				if(m_vi_VertexColors[m_vi_Edges[j]] != _UNKNOWN)
				{
					vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[j]]] = i_PresentVertex;
				}

				for(k=m_vi_Vertices[m_vi_Edges[j]]; k<m_vi_Vertices[STEP_UP(m_vi_Edges[j])]; k++)
				{
					if(m_vi_Edges[k] == i_PresentVertex)
					{
						continue;
					}

					if(m_vi_VertexColors[m_vi_Edges[k]] == _UNKNOWN)
					{
						continue;
					}

					if((vi_VertexHierarchy[m_vi_Edges[j]] > vi_VertexHierarchy[i_PresentVertex]) && (vi_VertexHierarchy[m_vi_Edges[j]] > vi_VertexHierarchy[m_vi_Edges[k]]))
					{
						vi_CandidateColors[m_vi_VertexColors[m_vi_Edges[k]]] = i_PresentVertex;
					}
				} 
			}
	
			for(j=0; j<i_VertexCount; j++)
			{
				if(vi_CandidateColors[j] != i_PresentVertex)
				{
					m_vi_VertexColors[i_PresentVertex] = j;

					if(i_HighestColor < j)
					{
						i_HighestColor = j;
					}

					break;
				}
			}
		}
    
		return(_TRUE);
}
	
	//Public Function 1466
	int GraphColoring::CheckTriangularColoring()
	{
		return(CheckAcyclicColoring());
	}

	
	//Public Function 1467
	string GraphColoring::GetVertexColoringVariant()
	{
		return(m_s_VertexColoringVariant);
	}

	
	//Public Function 1468
	int GraphColoring::GetVertexColorCount()
	{
		return(STEP_UP(m_i_VertexColorCount));
	}

	
	//Public Function 1469
	void GraphColoring::GetVertexColors(vector<int> &output)
	{
		output = (m_vi_VertexColors);
	}

	
	//Public Function 1470
	int GraphColoring::GetHubCount()
	{
		if(CheckVertexColoring("STAR"))
		{
			return(m_i_ColoringUnits);
		}
		else
		{
			return(_UNKNOWN);
		}
	}

	
	//Public Function 1471
	int GraphColoring::GetSetCount()
	{
		if(CheckVertexColoring("ACYCLIC"))
		{
			return(m_i_ColoringUnits);
		}
		else
		{
			return(_UNKNOWN);
		}
	}
	
	//Public Function 1472
	double GraphColoring::GetVertexColoringTime()
	{
		return(m_d_ColoringTime);
	}

	//Public Function 1473
	double GraphColoring::GetVertexColoringCheckingTime()
	{
		return(m_d_CheckingTime);
	}

	//Public Function 1474
	int GraphColoring::PrintVertexColors()
	{
		int i;

		int i_VertexCount;

		string _SLASH("/");

		StringTokenizer SlashTokenizer(m_s_InputFile, _SLASH);

		m_s_InputFile = SlashTokenizer.GetLastToken();

		i_VertexCount = (signed) m_vi_VertexColors.size();

		cout<<endl;
		cout<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | Vertex Colors | "<<m_s_InputFile<<endl;
		cout<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			cout<<"Vertex "<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_VertexColors[i])<<endl;
		}

#if STATISTICS == _TRUE

		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Stars = "<<m_i_ColoringUnits<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Sets = "<<m_i_ColoringUnits<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}

#endif

#if STATISTICS == _FALSE

		
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Sequencing Time = "<<m_d_SequencingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;

		}

#endif

		return(_TRUE);
	}

	
	//Public Function 1475
	int GraphColoring::FileVertexColors()
	{
		int i;

		int i_VertexCount;
	
		string s_InputFile, s_OutputFile;
	    
		string s_ColoringExtension, s_OrderingExtension;

		string _SLASH("/");

		ofstream OutputStream;

		//Determine Ordering Suffix

		if(m_s_VertexOrderingVariant.compare("NATURAL") == 0)
		{
			s_OrderingExtension = ".N.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("LARGEST_FIRST") == 0)
		{
			s_OrderingExtension = ".LF.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE_TWO_LARGEST_FIRST") == 0)
		{
			s_OrderingExtension = ".D2LF.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("SMALLEST_LAST") == 0)
		{
			s_OrderingExtension = ".SL.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE_TWO_SMALLEST_LAST") == 0)
		{
			s_OrderingExtension = ".D2SL.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("INCIDENCE_DEGREE") == 0)
		{
			s_OrderingExtension = ".ID.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE_TWO_INCIDENCE_DEGREE") == 0)
		{
			s_OrderingExtension = ".D2ID.";
		}
		else
		{
			s_OrderingExtension = ".NONE.";
		}
	  
		//Determine Coloring Suffix

		if(m_s_VertexColoringVariant.compare("DISTANCE_ONE") == 0)
		{
			s_ColoringExtension = ".D1.";
		}
		else
		if(m_s_VertexColoringVariant.compare("DISTANCE_TWO") == 0)
		{
			s_ColoringExtension = ".D2.";
		}
		else
		if(m_s_VertexColoringVariant.compare("NAIVE_STAR") == 0)
		{
			s_ColoringExtension = ".NS.";
		}
		else
		if(m_s_VertexColoringVariant.compare("RESTRICTED_STAR") == 0)
		{
			s_ColoringExtension = ".RS.";
		}
		else
		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			s_ColoringExtension = ".S.";
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			s_ColoringExtension = ".A.";
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			s_ColoringExtension = ".T.";
		}	
		else
		{
			s_ColoringExtension = ".NONE.";
		}

		StringTokenizer SlashTokenizer(m_s_InputFile, _SLASH);

		s_InputFile = SlashTokenizer.GetLastToken();

		s_OutputFile = s_InputFile;
		s_OutputFile += s_OrderingExtension;
		s_OutputFile += s_ColoringExtension;
		s_OutputFile += ".out";

		OutputStream.open(s_OutputFile.c_str());

		i_VertexCount = (signed) m_vi_VertexColors.size();

		OutputStream<<endl;
		OutputStream<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | Vertex Colors | "<<m_s_InputFile<<endl;
		OutputStream<<endl;

		for(i=0; i<i_VertexCount; i++)
		{
			OutputStream<<"Vertex "<<STEP_UP(i)<<"\t"<<" : "<<STEP_UP(m_vi_VertexColors[i])<<endl;
		}

#if STATISTICS == _TRUE

		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Stars = "<<m_i_ColoringUnits<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Sets = "<<m_i_ColoringUnits<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}

#endif

#if STATISTICS == _FALSE

		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Sequencing Time = "<<m_d_SequencingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
	    
#endif

		OutputStream.close();

		return(_TRUE);
	}


	
	//Public Function 1476
	int GraphColoring::PrintVertexColoringMetrics()
	{
		cout<<endl;
		cout<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | "<<m_s_InputFile<<endl;
		cout<<endl;

#if STATISTICS == _TRUE

		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Stars = "<<m_i_ColoringUnits<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()/2<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Sets = "<<m_i_ColoringUnits<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()/2<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()/2<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}

#endif

#if STATISTICS == _FALSE

		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()/2<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Sequencing Time = "<<m_d_SequencingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			cout<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()/2<<"]"<<endl;
			cout<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			cout<<endl;

		}
	    
#endif

		return(_TRUE);

	}
	
	//Public Function 1477
	int GraphColoring::FileVertexColoringMetrics()
	{
		string s_InputFile, s_OutputFile;
	    
		string s_ColoringExtension, s_OrderingExtension;

		string _SLASH("/");

		ofstream OutputStream;

		//Determine Ordering Suffix

		if(m_s_VertexOrderingVariant.compare("ALL") == 0)
		{
			s_OrderingExtension = ".ALL.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("NATURAL") == 0)
		{
			s_OrderingExtension = ".N.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("LARGEST FIRST") == 0)
		{
			s_OrderingExtension = ".LF.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE TWO LARGEST FIRST") == 0)
		{
			s_OrderingExtension = ".D2LF.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("SMALLEST LAST") == 0)
		{
			s_OrderingExtension = ".SL.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE TWO SMALLEST LAST") == 0)
		{
			s_OrderingExtension = ".D2SL.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("INCIDENCE DEGREE") == 0)
		{
			s_OrderingExtension = ".ID.";
		}
		else
		if(m_s_VertexOrderingVariant.compare("DISTANCE TWO INCIDENCE DEGREE") == 0)
		{
			s_OrderingExtension = ".D2ID.";
		}
		else
		{
			s_OrderingExtension = ".NONE.";
		}
	  
		//Determine Coloring Suffix

		if(m_s_VertexColoringVariant.compare("ALL") == 0)
		{
			s_ColoringExtension = ".ALL.";
		}
		else
		if(m_s_VertexColoringVariant.compare("DISTANCE ONE") == 0)
		{
			s_ColoringExtension = ".D1.";
		}
		else
		if(m_s_VertexColoringVariant.compare("DISTANCE TWO") == 0)
		{
			s_ColoringExtension = ".D2.";
		}
		else
		if(m_s_VertexColoringVariant.compare("NAIVE STAR") == 0)
		{
			s_ColoringExtension = ".NS.";
		}
		else
		if(m_s_VertexColoringVariant.compare("RESTRICTED STAR") == 0)
		{
			s_ColoringExtension = ".RS.";
		}
		else
		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			s_ColoringExtension = ".S.";
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			s_ColoringExtension = ".A.";
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			s_ColoringExtension = ".T.";
		}	
		else
		{
			s_ColoringExtension = ".NONE.";
		}

		StringTokenizer SlashTokenizer(m_s_InputFile, _SLASH);

		s_InputFile = SlashTokenizer.GetLastToken();

		s_OutputFile = s_InputFile;
		s_OutputFile += s_OrderingExtension;
		s_OutputFile += s_ColoringExtension;
		s_OutputFile += ".out";

		OutputStream.open(s_OutputFile.c_str(), ios::app);

		OutputStream<<endl;
		OutputStream<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | "<<m_s_InputFile<<endl;
		OutputStream<<endl;

#if STATISTICS == _TRUE

		if(m_s_VertexColoringVariant.compare("STAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Stars = "<<m_i_ColoringUnits<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("ACYCLIC") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"; Total Sets = "<<m_i_ColoringUnits<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}

#endif

#if STATISTICS == _FALSE

		if(m_s_VertexColoringVariant.compare("TRIANGULAR") == 0)
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Sequencing Time = "<<m_d_SequencingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
		else
		{
			OutputStream<<endl;
			OutputStream<<"[Total Colors = "<<STEP_UP(m_i_VertexColorCount)<<"]"<<endl;
			OutputStream<<"[Vertex Count = "<<STEP_DOWN(m_vi_Vertices.size())<<"; Edge Count = "<<m_vi_Edges.size()<<"]"<<endl;
			OutputStream<<"[Ordering Time = "<<m_d_OrderingTime<<"; Coloring Time = "<<m_d_ColoringTime<<"]"<<endl;
			OutputStream<<endl;
		}
	    
#endif

		OutputStream.close();

		return(_TRUE);

	}

	
	//Public Function 1478
	void GraphColoring::PrintVertexColorClasses()
	{
		if(CalculateVertexColorClasses() != _TRUE)
		{
			cout<<endl;
			cout<<"Vertex Color Classes | "<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | "<<m_s_InputFile<<" | Vertex Colors Not Set"<<endl;
			cout<<endl;

			return;
		}

		cout<<endl;
		cout<<"Vertex Color Classes | "<<m_s_VertexColoringVariant<<" Coloring | "<<m_s_VertexOrderingVariant<<" Ordering | "<<m_s_InputFile<<endl;
		cout<<endl;
		
		int i_TotalVertexColors = STEP_UP(m_i_VertexColorCount);

		for(int i = 0; i < i_TotalVertexColors; i++)
		{
			if(m_vi_VertexColorFrequency[i] <= 0)
			{
				continue;
			}

			cout<<"Color "<<STEP_UP(i)<<" : "<<m_vi_VertexColorFrequency[i]<<endl;
		}

		cout<<endl;
		cout<<"[Largest Color Class : "<<STEP_UP(m_i_LargestColorClass)<<"; Largest Color Class Size : "<<m_i_LargestColorClassSize<<"]"<<endl;
		cout<<"[Smallest Color Class : "<<STEP_UP(m_i_SmallestColorClass)<<"; Smallest Color Class Size : "<<m_i_SmallestColorClassSize<<"]"<<endl;
		cout<<"[Average Color Class Size : "<<m_d_AverageColorClassSize<<"]"<<endl;
		cout<<endl;

		return;
	}

	double** GraphColoring::GetSeedMatrix(int* i_SeedRowCount, int* i_SeedColumnCount) {
		int i_size = m_vi_VertexColors.size();
		int i_num_of_colors = m_i_VertexColorCount + 1;
		(*i_SeedRowCount) = i_size;
		(*i_SeedColumnCount) = i_num_of_colors;
		if(i_num_of_colors == 0 || i_size == 0) return NULL;
		double** Seed = new double*[i_size];

		// allocate and initialize Seed matrix
		for (int i=0; i<i_size; i++) {
			Seed[i] = new double[i_num_of_colors];
			for(int j=0; j<i_num_of_colors; j++) Seed[i][j]=0.;
		}

		// populate Seed matrix
		for (int i=0; i < i_size; i++) {
			Seed[i][m_vi_VertexColors[i]] = 1.;
		}

		return Seed;
	}

	int GraphColoring::CheckQuickDistanceTwoColoring(int Verbose)
	{
		if (m_i_MaximumVertexDegree <= STEP_UP(m_i_VertexColorCount)) return 0;

		// If the code reaches this place, DistanceTwoColoring() must have run INcorrectly.
		// Find the 2 vertices within distance-2 have the same color
		if (Verbose < 1) return 1; 

		//First, if the vertex with Maximum Degree, i.e. the max number of vertices that a vertex connects to
		int i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());
		int i_VertexWithMaxDegree = -1;
		int i_MaximumVertexDegree = -1;
		int i_VertexDegree = -1;

		for (int i = 0; i < i_VertexCount; i++)
		{
			i_VertexDegree = m_vi_Vertices[i + 1] - m_vi_Vertices[i];

			if(i_MaximumVertexDegree < i_VertexDegree)
			{
				i_MaximumVertexDegree = i_VertexDegree;
				i_VertexWithMaxDegree = i;
			}
		}

		cout<<"VertexWithMaxDegree = "<< i_VertexWithMaxDegree <<"; MaximumVertexDegree = "<< i_MaximumVertexDegree <<endl;
		if (Verbose < 2) return 1; 

		for (int i = m_vi_Vertices[i_VertexWithMaxDegree]; i < m_vi_Vertices[i_VertexWithMaxDegree + 1] - 1; i++) {
			//printf("\t*i = %d \n",i);
			for (int j = i + 1; j < m_vi_Vertices[i_VertexWithMaxDegree + 1]; j++) {
				if (m_vi_VertexColors[m_vi_Edges[i]] == m_vi_VertexColors[m_vi_Edges[j]])
					printf("\t m_vi_VertexColors[m_vi_Edges[i(%d)](%d)](%d) == m_vi_VertexColors[m_vi_Edges[j(%d)](%d)](%d)\n", i, m_vi_Edges[i], m_vi_VertexColors[m_vi_Edges[i]], j, m_vi_Edges[j], m_vi_VertexColors[m_vi_Edges[j]]);
			}
		}

		return 1;
	}

	int GraphColoring::CheckDistanceTwoColoring(int Verbose) {
		//int i = 0;
		int j = 0, k = 0;
		int i_PresentVertex, i_DistanceOneVertex, i_DistanceTwoVertex;
		int i_VertexCount = STEP_DOWN((signed) m_vi_Vertices.size());

		for(i_PresentVertex=0; i_PresentVertex<i_VertexCount; i_PresentVertex++)
		{

			//For each Distance-One Vertex, do ...
			for(j=m_vi_Vertices[i_PresentVertex]; j<m_vi_Vertices[STEP_UP(i_PresentVertex)]; j++)
			{
				i_DistanceOneVertex = m_vi_Edges[j];
				if(m_vi_VertexColors[i_PresentVertex] == m_vi_VertexColors[i_DistanceOneVertex]) {
					//Violate the requirement for DistanceTwoColoring(). Print error
					if (Verbose < 1) return 1;
					printf("D1 VIOLATION. m_vi_VertexColors[i_PresentVertex(%d)](%d) == m_vi_VertexColors[i_DistanceOneVertex(%d)](%d) \n", i_PresentVertex, m_vi_VertexColors[i_PresentVertex], i_DistanceOneVertex, m_vi_VertexColors[i_DistanceOneVertex]);
					if (Verbose < 2) return 1;
				}

				//For each Distance-Two Vertex, do ...
				for(k=m_vi_Vertices[i_DistanceOneVertex]; k<m_vi_Vertices[STEP_UP(i_DistanceOneVertex)]; k++)
				{
					i_DistanceTwoVertex = m_vi_Edges[k];

					if(i_DistanceTwoVertex == i_PresentVertex) continue; //We don't want to make a loop. Ignore this case

					if(m_vi_VertexColors[i_PresentVertex] == m_vi_VertexColors[i_DistanceTwoVertex]) {
						//Violate the requirement for DistanceTwoColoring(). Print error
						if (Verbose < 1) return 1;
						printf("D2 VIOLATION. m_vi_VertexColors[i_PresentVertex(%d)](%d) == m_vi_VertexColors[i_DistanceTwoVertex(%d)](%d) \n", i_PresentVertex, m_vi_VertexColors[i_PresentVertex], i_DistanceTwoVertex, m_vi_VertexColors[i_DistanceTwoVertex]);
						printf("\t i_PresentVertex(%d) and i_DistanceTwoVertex(%d) connected through i_DistanceOneVertex(%d) \n", i_PresentVertex, i_DistanceTwoVertex, i_DistanceOneVertex);
						if (Verbose < 2) return 1;
					}
				}
			}
		}
		return 0;
	}

}
