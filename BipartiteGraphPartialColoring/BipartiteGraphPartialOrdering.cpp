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
	//Private Function 2301
	int BipartiteGraphPartialOrdering::CheckVertexOrdering(string s_VertexOrderingVariant)
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

	
	//Public Constructor 2351
	BipartiteGraphPartialOrdering::BipartiteGraphPartialOrdering()
	{
		Clear();
	}

	
	//Public Destructor 2352
	BipartiteGraphPartialOrdering::~BipartiteGraphPartialOrdering()
	{
		Clear();
	}

	
	//Virtual Function 2353
	void BipartiteGraphPartialOrdering::Clear()
	{
		BipartiteGraphInputOutput::Clear();

		m_d_OrderingTime = _UNKNOWN;

		m_s_VertexOrderingVariant.clear();

		m_vi_OrderedVertices.clear();

		return;
	}

	
	//Virtual Function 2354
	void BipartiteGraphPartialOrdering::Reset()
	{
		m_d_OrderingTime = _UNKNOWN;

		m_s_VertexOrderingVariant.clear();

		m_vi_OrderedVertices.clear();

		return;
	}

	
	//Public Function 2355
	int BipartiteGraphPartialOrdering::RowNaturalOrdering()
	{
		if(CheckVertexOrdering("ROW_NATURAL"))
		{
			return(_TRUE);
		}
		
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());

		m_vi_OrderedVertices.clear();

		for(int i = 0; i<i_LeftVertexCount; i++)
		{
			m_vi_OrderedVertices.push_back(i);
		}
		
		return(_TRUE);
	}

	
	//Public Function 2356
	int BipartiteGraphPartialOrdering::ColumnNaturalOrdering()
	{
		if(CheckVertexOrdering("COLUMN_NATURAL"))
		{
			return(_TRUE);
		}
		
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());
		int i_RightVertexCount = STEP_DOWN((signed) m_vi_RightVertices.size());
		
		m_vi_OrderedVertices.clear();

		for(int i = 0; i < i_RightVertexCount; i++)
		{
			m_vi_OrderedVertices.push_back(i + i_LeftVertexCount);
		}
		
		return(_TRUE);
	}
	
	//Public Function 2357
	int BipartiteGraphPartialOrdering::RowLargestFirstOrdering()
	{
		if(CheckVertexOrdering("ROW_LARGEST_FIRST"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_DegreeCount;
		int i_VertexNumber, i_Current;
		vector<int> vi_Visited;
		vector< vector<int> > vi_Bucket;
	    
		m_i_MaximumVertexDegree = 0;
		i_VertexNumber = (int)m_vi_LeftVertices.size () - 1;
		vi_Bucket.resize ( i_VertexNumber );
		vi_Visited.resize ( i_VertexNumber, -1 );

		// enter code here
		for ( i=0; i<i_VertexNumber; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_DegreeCount = 0;
			// let's loop from mvi_RightVertices[i] to mvi_RightVertices[i+1] for the i'th column
			for ( j=m_vi_LeftVertices [i]; j<m_vi_LeftVertices [i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for ( k=m_vi_RightVertices [i_Current]; k<m_vi_RightVertices [i_Current+1]; ++k )
				{
					// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
					
					if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					{
						++i_DegreeCount;
						// vi_VistedNodes.push_back ( m_vi_Edges [k] );
						vi_Visited [m_vi_Edges [k]] = i;
					}
				}
			}

			vi_Bucket [i_DegreeCount].push_back ( i );
			
			if ( m_i_MaximumVertexDegree < i_DegreeCount )
			{
				m_i_MaximumVertexDegree = i_DegreeCount;
			}
		}
	    
		// clear the vertexorder
		m_vi_OrderedVertices.clear ();
		// take the bucket and place it in the vertexorder
		for ( i=m_i_MaximumVertexDegree; i>=0; --i )
		{
			// j = size of the bucket
			// get the size of the i-th bucket
			j = (int)vi_Bucket [i].size ();
			// place it into vertex ordering
			for ( k=0; k<j; ++k )
			{
				m_vi_OrderedVertices.push_back ( vi_Bucket [i][k] );
			}
		}
	    
		return(_TRUE);
}


	
	//Public Function 2358
	int BipartiteGraphPartialOrdering::ColumnLargestFirstOrdering()
	{
		if(CheckVertexOrdering("COLUMN_LARGEST_FIRST"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_DegreeCount;
		int i_VertexNumber, i_Current;
		vector<int> vi_Visited;
		vector< vector<int> > vi_Bucket;
	    
		m_i_MaximumVertexDegree = 0;
		i_VertexNumber = (int)m_vi_RightVertices.size () - 1;
		vi_Bucket.resize ( i_VertexNumber );
		vi_Visited.resize ( i_VertexNumber, -1 );
	    
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());

		// enter code here
		for ( i=0; i<i_VertexNumber; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_DegreeCount = 0;
			// let's loop from mvi_RightVertices[i] to mvi_RightVertices[i+1] for the i'th column
			for ( j=m_vi_RightVertices [i]; j<m_vi_RightVertices [i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for ( k=m_vi_LeftVertices [i_Current]; k<m_vi_LeftVertices [i_Current+1]; ++k )
				{
				// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
				
					if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					{
						++i_DegreeCount;
						// vi_VistedNodes.push_back ( m_vi_Edges [k] );
						vi_Visited [m_vi_Edges [k]] = i;
					}
				}
			}
			vi_Bucket [i_DegreeCount].push_back ( i );
			
			if ( m_i_MaximumVertexDegree < i_DegreeCount )
			{
				m_i_MaximumVertexDegree = i_DegreeCount;
			}
		}
	    
		// clear the vertexorder
		m_vi_OrderedVertices.clear ();
		// take the bucket and place it in the vertexorder

		for ( i=m_i_MaximumVertexDegree; i>=0; --i )
		{
			// j = size of the bucket
			// get the size of the i-th bucket
			j = (int)vi_Bucket [i].size ();
			// place it into vertex ordering
			for ( k=0; k<j; ++k )
			{
				m_vi_OrderedVertices.push_back ( vi_Bucket [i][k] + i_LeftVertexCount);
			}
		}
	    
		return(_TRUE);
	}


	
	//Public Function 2359
	int BipartiteGraphPartialOrdering::RowSmallestLastOrdering()
	{
		if(CheckVertexOrdering("ROW_SMALLEST_LAST"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_Current;
		int i_SelectedVertex, i_SelectedVertexCount;
		int i_VertexCount, i_InducedVertexDegreeCount;
		int i_VertexCountMinus1; // = i_VertexCount - 1, used when inserting selected vertices into m_vi_OrderedVertices 
		int i_HighestInducedVertexDegree, i_InducedVertexDegree;
		vector<int> vi_InducedVertexDegree;
		vector<int> vi_Visited;
		vector< list<int> > vli_GroupedInducedVertexDegree;
		vector< list<int>::iterator > vlit_VertexLocation;
	    
		// initialize

		i_SelectedVertex = _UNKNOWN;
		i_VertexCount = (int)m_vi_LeftVertices.size () - 1;
		i_VertexCountMinus1 = i_VertexCount - 1;
		vli_GroupedInducedVertexDegree.resize ( i_VertexCount );
		i_HighestInducedVertexDegree = 0;
		vi_Visited.resize ( i_VertexCount, -1 );
		m_vi_OrderedVertices.clear();
		m_vi_OrderedVertices.resize(i_VertexCount, _UNKNOWN);
	    
		for ( i=0; i<i_VertexCount; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_InducedVertexDegree = 0;
			// let's loop from mvi_LeftVertices[i] to mvi_LeftVertices[i+1] for the i'th column
			for ( j=m_vi_LeftVertices[i]; j<m_vi_LeftVertices[i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for (k=m_vi_RightVertices[i_Current]; k<m_vi_RightVertices[i_Current+1]; ++k)
					{
					// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
					
					if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					{
						++i_InducedVertexDegree;
						// vi_VistedNodes.push_back ( m_vi_Edges [k] );
						vi_Visited [m_vi_Edges [k]] = i;
					}
				}
			}

			vi_InducedVertexDegree.push_back ( i_InducedVertexDegree );
			vli_GroupedInducedVertexDegree [i_InducedVertexDegree].push_front ( i );
			vlit_VertexLocation.push_back ( vli_GroupedInducedVertexDegree [i_InducedVertexDegree].begin () );
			
			if ( i_HighestInducedVertexDegree < i_InducedVertexDegree )
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}
	    
		i_SelectedVertexCount = 0;
		// first clear the visited nodes
		vi_Visited.clear ();
		vi_Visited.resize ( i_VertexCount, -1 );
		// end clear nodes
	    
		while ( i_SelectedVertexCount < i_VertexCount )
		{
			// selecte first item from the bucket
			for ( i=0; i<(i_HighestInducedVertexDegree+1); ++i )
			{
				i_InducedVertexDegreeCount = (int)vli_GroupedInducedVertexDegree[i].size ();
			    
				if ( i_InducedVertexDegreeCount != 0 )
				{
					i_SelectedVertex = vli_GroupedInducedVertexDegree[i].front ();
					break;
				}
			}
			// end select first nonzero item from the bucket
			
			// go to the neighbors of i_SelectedVertex and decrease the degrees by 1
			for ( i=m_vi_LeftVertices [i_SelectedVertex]; i<m_vi_LeftVertices [i_SelectedVertex+1]; ++i )
			{
				// which Row element is Col (i_SelectedVertex) pointing to?
				i_Current = m_vi_Edges [i];
				// go to each neighbor of Col (i_SelectedVertex) and decrease the degree by 1
				for ( j=m_vi_RightVertices [i_Current]; j<m_vi_RightVertices [i_Current+1]; ++j )
				{
					// note: m_vi_Edges [j] is the neighbor of Col (i_SelectedVertex)
					// make sure it's not pointing back to itself, or if we already visited this node
					if ( m_vi_Edges [j] == i_SelectedVertex || vi_Visited [m_vi_Edges [j]] == i_SelectedVertex )
					{
						// it is pointed to itself or already visited
						continue;
					}
				
					// now check to make sure that the neighbor's degree isn't UNKNOWN
					if ( vi_InducedVertexDegree [m_vi_Edges [j]] == -1 )
					{
						// our neighbor doesn't have any neighbors, so skip it.
						continue;
					}
					
					// update that we visited this
					vi_Visited [m_vi_Edges [j]] = i_SelectedVertex;
					// end up update that we visited this
					
					// now let's tell the neighbor that we are going to remove i_SelectedVertex from the list
					// first remove the neighbor from the list as a temp solution
					vli_GroupedInducedVertexDegree [ vi_InducedVertexDegree [m_vi_Edges [j]] ].erase ( vlit_VertexLocation [m_vi_Edges [j]] );
					// next we tell the neighbor to reduce it's degree by 1
					--vi_InducedVertexDegree [m_vi_Edges [j]];
					// now, "push one degree up" for the neighbor on the bucket
					vli_GroupedInducedVertexDegree [vi_InducedVertexDegree[m_vi_Edges [j]]].push_front ( m_vi_Edges [j] );
					// now update the location for the neighbor
					vlit_VertexLocation [m_vi_Edges[j]] = vli_GroupedInducedVertexDegree [vi_InducedVertexDegree[m_vi_Edges [j]]].begin ();
				}
			}
			// end of go to the neighbors of i_SelectedVertex and decrease the degrees by 1
			
			// remove selected vertex from the bucket by iterator pointing to that degree
			vli_GroupedInducedVertexDegree [vi_InducedVertexDegree [i_SelectedVertex]].erase ( vlit_VertexLocation [i_SelectedVertex] );
			// now say that the degree of that is unknown
			vi_InducedVertexDegree [i_SelectedVertex] = -1;
			
			// insert the selected vertex to the front of the vertex order
			//insert at the beginning (push_front)? BAD!!!! due to moving
			//m_vi_OrderedVertices.insert ( m_vi_OrderedVertices.begin (), i_SelectedVertex );

			//Alternative method for the above (commented) line
//printf("m_vi_OrderedVertices[i_VertexCountMinus1 (%d) - i_SelectedVertexCount (%d)] (%d) = i_SelectedVertex (%d) \n", i_VertexCountMinus1, i_SelectedVertexCount, i_VertexCountMinus1 - i_SelectedVertexCount, i_SelectedVertex);
			m_vi_OrderedVertices[i_VertexCountMinus1 - i_SelectedVertexCount] = i_SelectedVertex;
			
			// go to next one
			++i_SelectedVertexCount;
		}
	    
		return(_TRUE);
	}

	
	//Public Function 2360
	int BipartiteGraphPartialOrdering::ColumnSmallestLastOrdering()
	{
		if(CheckVertexOrdering("COLUMN_SMALLEST_LAST"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_Current;
		int i_SelectedVertex, i_SelectedVertexCount;
		int i_VertexCount, i_InducedVertexDegreeCount;
		int i_VertexCountMinus1; // = i_VertexCount - 1, used when inserting selected vertices into m_vi_OrderedVertices 
		int i_HighestInducedVertexDegree, i_InducedVertexDegree;
		vector<int> vi_InducedVertexDegree;
		vector<int> vi_Visited;
		vector< list<int> > vli_GroupedInducedVertexDegree;
		vector< list<int>::iterator > vlit_VertexLocation;
	    
		// initialize
		i_SelectedVertex = _UNKNOWN;
		i_VertexCount = (int)m_vi_RightVertices.size () - 1;
		i_VertexCountMinus1 = i_VertexCount - 1;
		vli_GroupedInducedVertexDegree.resize ( i_VertexCount );
		i_HighestInducedVertexDegree = 0;
		vi_Visited.resize ( i_VertexCount, -1 );
		m_vi_OrderedVertices.clear();
		m_vi_OrderedVertices.resize(i_VertexCount, _UNKNOWN);
	    
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());

		for ( i=0; i<i_VertexCount; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_InducedVertexDegree = 0;
			// let's loop from mvi_RightVertices[i] to mvi_RightVertices[i+1] for the i'th column
			for ( j=m_vi_RightVertices [i]; j<m_vi_RightVertices [i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for ( k=m_vi_LeftVertices [i_Current]; k<m_vi_LeftVertices [i_Current+1]; ++k )
				{
				// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
				
					if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					{
						++i_InducedVertexDegree;
						// vi_VistedNodes.push_back ( m_vi_Edges [k] );
						vi_Visited [m_vi_Edges [k]] = i;
					}
				}
			}
			vi_InducedVertexDegree.push_back ( i_InducedVertexDegree );
			vli_GroupedInducedVertexDegree [i_InducedVertexDegree].push_front ( i );
			vlit_VertexLocation.push_back ( vli_GroupedInducedVertexDegree [i_InducedVertexDegree].begin () );
		
			if ( i_HighestInducedVertexDegree < i_InducedVertexDegree )
			{
				i_HighestInducedVertexDegree = i_InducedVertexDegree;
			}
		}
	    
		i_SelectedVertexCount = 0;
		// first clear the visited nodes
		vi_Visited.clear ();
		vi_Visited.resize ( i_VertexCount, -1 );
		// end clear nodes
	    
		while ( i_SelectedVertexCount < i_VertexCount )
		{
			// selecte first item from the bucket
			for ( i=0; i<(i_HighestInducedVertexDegree+1); ++i )
			{
				i_InducedVertexDegreeCount = (int)vli_GroupedInducedVertexDegree [i].size ();
			    
				if ( i_InducedVertexDegreeCount != 0 )
				{
				i_SelectedVertex = vli_GroupedInducedVertexDegree [i].front ();
				break;
				}
			}
			// end select first nonzero item from the bucket
		
			// go to the neighbors of i_SelectedVertex and decrease the degrees by 1
			for ( i=m_vi_RightVertices [i_SelectedVertex]; i<m_vi_RightVertices [i_SelectedVertex+1]; ++i )
			{
				// which Row element is Col (i_SelectedVertex) pointing to?
				i_Current = m_vi_Edges [i];
				// go to each neighbor of Col (i_SelectedVertex) and decrease the degree by 1
				for ( j=m_vi_LeftVertices [i_Current]; j<m_vi_LeftVertices [i_Current+1]; ++j )
				{
					// note: m_vi_Edges [j] is the neighbor of Col (i_SelectedVertex)
					// make sure it's not pointing back to itself, or if we already visited this node
					if ( m_vi_Edges [j] == i_SelectedVertex || vi_Visited [m_vi_Edges [j]] == i_SelectedVertex )
					{
						// it is pointed to itself or already visited
						continue;
					}
			
					// now check to make sure that the neighbor's degree isn't UNKNOWN
					if ( vi_InducedVertexDegree [m_vi_Edges [j]] == -1 )
					{
						// our neighbor doesn't have any neighbors, so skip it.
						continue;
					}
			
					// update that we visited this
					vi_Visited [m_vi_Edges [j]] = i_SelectedVertex;
					// end up update that we visited this
					
					// now let's tell the neighbor that we are going to remove i_SelectedVertex from the list
					// first remove the neighbor from the list as a temp solution
					vli_GroupedInducedVertexDegree [ vi_InducedVertexDegree [m_vi_Edges [j]] ].erase ( vlit_VertexLocation [m_vi_Edges [j]] );
					// next we tell the neighbor to reduce it's degree by 1
					--vi_InducedVertexDegree [m_vi_Edges [j]];
					// now, "push one degree up" for the neighbor on the bucket
					vli_GroupedInducedVertexDegree [vi_InducedVertexDegree [m_vi_Edges [j]]].push_front ( m_vi_Edges [j] );
					// now update the location for the neighbor
					vlit_VertexLocation [m_vi_Edges [j]] = vli_GroupedInducedVertexDegree [vi_InducedVertexDegree [m_vi_Edges [j]]].begin ();
				}
			}
			// end of go to the neighbors of i_SelectedVertex and decrease the degrees by 1
			
			// remove selected vertex from the bucket by iterator pointing to that degree
			vli_GroupedInducedVertexDegree [vi_InducedVertexDegree [i_SelectedVertex]].erase ( vlit_VertexLocation [i_SelectedVertex] );
			// now say that the degree of that is unknown
			vi_InducedVertexDegree [i_SelectedVertex] = -1;

			// insert the selected vertex to the front of the vertex order
			//insert at the beginning (push_front)? BAD!!!! due to moving
			//m_vi_OrderedVertices.insert ( m_vi_OrderedVertices.begin (), i_SelectedVertex + i_LeftVertexCount);

			//Alternative method for the above (commented) line
//printf("m_vi_OrderedVertices[i_VertexCountMinus1 (%d) - i_SelectedVertexCount (%d)] (%d) = i_SelectedVertex (%d) \n", i_VertexCountMinus1, i_SelectedVertexCount, i_VertexCountMinus1 - i_SelectedVertexCount, i_SelectedVertex);
			m_vi_OrderedVertices[i_VertexCountMinus1 - i_SelectedVertexCount] = i_SelectedVertex + i_LeftVertexCount;
			
			// go to next one
			++i_SelectedVertexCount;
		}
	    
		return(_TRUE);
	}


	
	//Public Function 2361
	int BipartiteGraphPartialOrdering::RowIncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("ROW_INCIDENCE_DEGREE"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_Current;
		int i_HighestDegreeVertex, m_i_MaximumVertexDegree;
		int i_VertexCount, i_VertexDegree, i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;
		int i_SelectedVertex, i_SelectedVertexCount;
		vector<int> vi_IncidenceVertexDegree;
		vector<int> vi_Visited;
		vector< list<int> > vli_GroupedIncidenceVertexDegree;
		vector< list<int>::iterator > vlit_VertexLocation;
	    
		// initialize
		i_SelectedVertex = _UNKNOWN;
		i_VertexCount = (int)m_vi_LeftVertices.size () - 1;
		vli_GroupedIncidenceVertexDegree.resize ( i_VertexCount );
		i_HighestDegreeVertex = -1;
		m_i_MaximumVertexDegree = -1;
		i_IncidenceVertexDegree = 0;
		vi_Visited.resize ( i_VertexCount, -1 );
		m_vi_OrderedVertices.clear();
	    
		// enter code here
		for ( i=0; i<i_VertexCount; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_VertexDegree = 0;
			// let's loop from mvi_RightVertices[i] to mvi_RightVertices[i+1] for the i'th column
			for ( j=m_vi_LeftVertices [i]; j<m_vi_LeftVertices [i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for ( k=m_vi_RightVertices [i_Current]; k<m_vi_RightVertices [i_Current+1]; ++k )
				{
				// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
				
					if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					{
						++i_VertexDegree;
						// vi_VistedNodes.push_back ( m_vi_Edges [k] );
						vi_Visited [m_vi_Edges [k]] = i;
					}
				}
			}
			vi_IncidenceVertexDegree.push_back ( i_IncidenceVertexDegree );
			vli_GroupedIncidenceVertexDegree [i_IncidenceVertexDegree].push_front ( i );
			vlit_VertexLocation.push_back ( vli_GroupedIncidenceVertexDegree [i_IncidenceVertexDegree].begin () );
			
			if ( m_i_MaximumVertexDegree < i_VertexDegree )
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
				i_HighestDegreeVertex = i;
			}
		}
	    
		// initialize more things for the bucket "moving"
		i_SelectedVertexCount = 0;
		vi_Visited.clear ();
		vi_Visited.resize ( i_VertexCount, -1 );
	    
		while ( i_SelectedVertexCount < i_VertexCount )
		{
			// select the vertex with the highest degree
			if ( i_SelectedVertexCount == 0 )
			{
				i_SelectedVertex = 0;
			}
			else
			{
				for ( i=m_i_MaximumVertexDegree; i>=0; i-- )
				{
					i_IncidenceVertexDegreeCount = (int)vli_GroupedIncidenceVertexDegree [i].size ();
				
					if ( i_IncidenceVertexDegreeCount != 0 )
					{
						i_SelectedVertex = vli_GroupedIncidenceVertexDegree [i].front ();
						break;
					}
				}
			}
			// end select the vertex with the highest degree
		
			// tell the neighbors of i_SelectedVertex that we are removing it
			for ( i=m_vi_LeftVertices [i_SelectedVertex]; i<m_vi_LeftVertices [i_SelectedVertex+1]; ++i )
			{
				i_Current = m_vi_Edges [i];
		    
				for ( j=m_vi_RightVertices [i_Current]; j<m_vi_RightVertices [i_Current+1]; ++j )
				{
					// now check if the degree of i_SelectedVertex's neighbor isn't unknow to us
					if ( vi_IncidenceVertexDegree [m_vi_Edges [j]] == -1 )
					{
						// i_SelectedVertex's neighbor's degree is unknown. skip!
						continue;
					}
			
					// note: m_vi_Edges [j] = neighbor of i_SelectedVertex
					// first check if we are pointing to itself or if we already visited a neighbor
					if ( m_vi_Edges [j] == i_SelectedVertex || vi_Visited [m_vi_Edges [j]] == i_SelectedVertex )
					{
						// we already visited or pointing to itself. skip.
						continue;
					}
			
					// now update that we are visiting m_vi_Edges [j]
					vi_Visited [m_vi_Edges [j]] = i_SelectedVertex;
					// end now update that we are visiting m_vi_Edges [j]
					
					// remove the neighbor temp.
					vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].erase ( vlit_VertexLocation [m_vi_Edges [j]] );
					// now we tell that neighbor to increase its degree by 1
					++vi_IncidenceVertexDegree [m_vi_Edges [j]];
					// place the neighbor in 1 degree up in the bucket
					vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].push_front ( m_vi_Edges [j] );
					// update the location of the now moved neighbor
					vlit_VertexLocation [m_vi_Edges [j]] = vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].begin ();
				}
			}
			// end tell the neighbors of i_SelectedVertex that we are removing it
			
			// now we remove i_SelectedVertex from the bucket
			vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [i_SelectedVertex]].erase ( vlit_VertexLocation [i_SelectedVertex] );
			// now we say that i_SelectedVertex's degree is unknown to us
			vi_IncidenceVertexDegree [i_SelectedVertex] = -1;
			// now we push i_SelectedVertex to the back or VertexOrder
			m_vi_OrderedVertices.push_back ( i_SelectedVertex );
			// loop it again
			++i_SelectedVertexCount;
		}
	    
		return(_TRUE);

	}


	
	//Public Function 2362
	int BipartiteGraphPartialOrdering::ColumnIncidenceDegreeOrdering()
	{
		if(CheckVertexOrdering("COLUMN_INCIDENCE_DEGREE"))
		{
			return(_TRUE);
		}

		int i, j, k;
		int i_Current;
		int i_HighestDegreeVertex, m_i_MaximumVertexDegree;
		int i_VertexCount, i_VertexDegree, i_IncidenceVertexDegree, i_IncidenceVertexDegreeCount;
		int i_SelectedVertex, i_SelectedVertexCount;
		vector<int> vi_IncidenceVertexDegree;
		vector<int> vi_Visited;
		vector< list<int> > vli_GroupedIncidenceVertexDegree;
		vector< list<int>::iterator > vlit_VertexLocation;
	    
		// initialize
		i_SelectedVertex = _UNKNOWN;
		i_VertexCount = (int)m_vi_RightVertices.size () - 1;
		vli_GroupedIncidenceVertexDegree.resize ( i_VertexCount );
		i_HighestDegreeVertex = -1;
		m_i_MaximumVertexDegree = -1;
		i_IncidenceVertexDegree = 0;
		vi_Visited.resize ( i_VertexCount, -1 );
		m_vi_OrderedVertices.clear();
	    
		int i_LeftVertexCount = STEP_DOWN((signed) m_vi_LeftVertices.size());

		// enter code here
		for ( i=0; i<i_VertexCount; ++i )
		{
			// clear the visted nodes
			//vi_VistedNodes.clear ();
			// reset the degree count
			i_VertexDegree = 0;
			// let's loop from mvi_RightVertices[i] to mvi_RightVertices[i+1] for the i'th column
			for ( j=m_vi_RightVertices [i]; j<m_vi_RightVertices [i+1]; ++j )
			{
				i_Current = m_vi_Edges [j];
			    
				for ( k=m_vi_LeftVertices [i_Current]; k<m_vi_LeftVertices [i_Current+1]; ++k )
				{
					// b_visited = visitedAlready ( vi_VistedNodes, m_vi_Edges [k] );
				
					 if ( m_vi_Edges [k] != i && vi_Visited [m_vi_Edges [k]] != i )
					 {
						 ++i_VertexDegree;
						 // vi_VistedNodes.push_back ( m_vi_Edges [k] );
						 vi_Visited [m_vi_Edges [k]] = i;
					 }
				}
			}
			vi_IncidenceVertexDegree.push_back ( i_IncidenceVertexDegree );
			vli_GroupedIncidenceVertexDegree [i_IncidenceVertexDegree].push_front ( i );
			vlit_VertexLocation.push_back ( vli_GroupedIncidenceVertexDegree [i_IncidenceVertexDegree].begin () );
			
			if ( m_i_MaximumVertexDegree < i_VertexDegree )
			{
				m_i_MaximumVertexDegree = i_VertexDegree;
				i_HighestDegreeVertex = i;
			}
		}
	    
		// initialize more things for the bucket "moving"
		i_SelectedVertexCount = 0;
		vi_Visited.clear ();
		vi_Visited.resize ( i_VertexCount, -1 );
	    
		while ( i_SelectedVertexCount < i_VertexCount )
		{
			// select the vertex with the highest degree
			if ( i_SelectedVertexCount == 0 )
			{
				i_SelectedVertex = 0;
			}
			else
			{
				for ( i=m_i_MaximumVertexDegree; i>=0; i-- )
				{
					i_IncidenceVertexDegreeCount = (int)vli_GroupedIncidenceVertexDegree [i].size ();
					
					if ( i_IncidenceVertexDegreeCount != 0 )
					{
						i_SelectedVertex = vli_GroupedIncidenceVertexDegree [i].front ();
						break;
					}
				}
			}
			// end select the vertex with the highest degree
	     
			// tell the neighbors of i_SelectedVertex that we are removing it
			for ( i=m_vi_RightVertices [i_SelectedVertex]; i<m_vi_RightVertices [i_SelectedVertex+1]; ++i )
			{
				i_Current = m_vi_Edges [i];
			    
				for ( j=m_vi_LeftVertices [i_Current]; j<m_vi_LeftVertices [i_Current+1]; ++j )
				{
					// now check if the degree of i_SelectedVertex's neighbor isn't unknow to us
					if ( vi_IncidenceVertexDegree [m_vi_Edges [j]] == -1 )
					{
						// i_SelectedVertex's neighbor's degree is unknown. skip!
						continue;
					}
				
					// note: m_vi_Edges [j] = neighbor of i_SelectedVertex
					// first check if we are pointing to itself or if we already visited a neighbor
					if ( m_vi_Edges [j] == i_SelectedVertex || vi_Visited [m_vi_Edges [j]] == i_SelectedVertex )
					{
						// we already visited or pointing to itself. skip.
						continue;
					}
			
					// now update that we are visiting m_vi_Edges [j]
					vi_Visited [m_vi_Edges [j]] = i_SelectedVertex;
					// end now update that we are visiting m_vi_Edges [j]
					
					// remove the neighbor temp.
					vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].erase ( vlit_VertexLocation [m_vi_Edges [j]] );
					// now we tell that neighbor to increase its degree by 1
					++vi_IncidenceVertexDegree [m_vi_Edges [j]];
					// place the neighbor in 1 degree up in the bucket
					vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].push_front ( m_vi_Edges [j] );
					// update the location of the now moved neighbor
					vlit_VertexLocation [m_vi_Edges [j]] = vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [m_vi_Edges [j]]].begin ();
				}
			}
			// end tell the neighbors of i_SelectedVertex that we are removing it
			
			// now we remove i_SelectedVertex from the bucket
			vli_GroupedIncidenceVertexDegree [vi_IncidenceVertexDegree [i_SelectedVertex]].erase ( vlit_VertexLocation [i_SelectedVertex] );
			// now we say that i_SelectedVertex's degree is unknown to us
			vi_IncidenceVertexDegree [i_SelectedVertex] = -1;
			// now we push i_SelectedVertex to the back or VertexOrder
			m_vi_OrderedVertices.push_back ( i_SelectedVertex + i_LeftVertexCount );
			// loop it again
			++i_SelectedVertexCount;
		}
	    
		return(_TRUE);
	}

	
	//Public Function 2363
	string BipartiteGraphPartialOrdering::GetVertexOrderingVariant()
	{
		if(m_s_VertexOrderingVariant.compare("ROW_NATURAL") == 0)
		{
			return("Row Natural");
		}
		else
		if(m_s_VertexOrderingVariant.compare("COLUMN_NATURAL") == 0)
		{
			return("Column Natural");
		}
		else
		if(m_s_VertexOrderingVariant.compare("ROW_LARGEST_FIRST") == 0)
		{
			return("Row Largest First");
		}
		else
		if(m_s_VertexOrderingVariant.compare("COLUMN_LARGEST_FIRST") == 0)
		{
			return("Column Largest First");
		}
		else
		if(m_s_VertexOrderingVariant.compare("ROW_SMALLEST_LAST") == 0)
		{
			return("Row Smallest Last");
		}
		else
		if(m_s_VertexOrderingVariant.compare("COLUMN_SMALLEST_LAST") == 0)
		{
			return("Column Smallest Last");
		}
		else
		if(m_s_VertexOrderingVariant.compare("ROW_INCIDENCE_DEGREE") == 0)
		{
			return("Row Incidence Degree");
		}
		else
		if(m_s_VertexOrderingVariant.compare("COLUMN_INCIDENCE_DEGREE") == 0)
		{
			return("Column Incidence Degree");
		}
		else
		{
			return("Unknown");
		}
	}
	
	//Public Function 2364
	void BipartiteGraphPartialOrdering::GetOrderedVertices(vector<int> &output)
	{
		output = (m_vi_OrderedVertices);
	}

	int BipartiteGraphPartialOrdering::OrderVertices(string s_OrderingVariant, string s_ColoringVariant) {
		s_ColoringVariant = toUpper(s_ColoringVariant);
		s_OrderingVariant = toUpper(s_OrderingVariant);

		if(s_ColoringVariant == "LEFT_PARTIAL_DISTANCE_TWO")
		{
			if((s_OrderingVariant.compare("NATURAL") == 0))
			{
				return(RowNaturalOrdering());
			}
			else
			if((s_OrderingVariant.compare("LARGEST_FIRST") == 0))
			{
				return(RowLargestFirstOrdering());
			}
			else
			if((s_OrderingVariant.compare("SMALLEST_LAST") == 0))
			{
				return(RowSmallestLastOrdering());
			}
			else
			if((s_OrderingVariant.compare("INCIDENCE_DEGREE") == 0))
			{
				return(RowIncidenceDegreeOrdering());
			}
			else
			{
				cerr<<endl;
				cerr<<"Unknown Ordering Method";
				cerr<<endl;
			}
		}
		else
		if(s_ColoringVariant == "RIGHT_PARTIAL_DISTANCE_TWO")
		{
			if((s_OrderingVariant.compare("NATURAL") == 0))
			{
				return(ColumnNaturalOrdering());
			}
			else
			if((s_OrderingVariant.compare("LARGEST_FIRST") == 0))
			{
				return(ColumnLargestFirstOrdering());
			}
			else
			if((s_OrderingVariant.compare("SMALLEST_LAST") == 0))
			{
				return(ColumnSmallestLastOrdering());
			}
			else
			if((s_OrderingVariant.compare("INCIDENCE_DEGREE") == 0))
			{
				return(ColumnIncidenceDegreeOrdering());
			}
			else
			{
				cerr<<endl;
				cerr<<"Unknown Ordering Method";
				cerr<<endl;
			}
		}
		else
		{
			cerr<<endl;
			cerr<<"Invalid s_ColoringVariant = \""<<s_ColoringVariant<<"\", must be either \"RIGHT_PARTIAL_DISTANCE_TWO\" or \"LEFT_PARTIAL_DISTANCE_TWO\".";
			cerr<<endl;
		}

		return(_TRUE);
	}

	
}
