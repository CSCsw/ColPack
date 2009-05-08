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
	
	//Public Destructor 2602
	BipartiteGraphPartialColoringInterface::~BipartiteGraphPartialColoringInterface()
	{
		BipartiteGraphPartialColoring::Clear();
	}

	//Public Function 2603
	void BipartiteGraphPartialColoringInterface::Clear()
	{
		BipartiteGraphPartialColoring::Clear();

		return;
	}

	
	//Public Function 2604
	void BipartiteGraphPartialColoringInterface::Reset()
	{
		BipartiteGraphPartialColoring::Reset();

		return;
	}

	
	//Public Function 2605
	int BipartiteGraphPartialColoringInterface::RowNaturalOrderingPartialDistanceTwoRowColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = RowNaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoRowColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2606
	int BipartiteGraphPartialColoringInterface::RowLargestFirstOrderingPartialDistanceTwoRowColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = RowLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoRowColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2607
	int BipartiteGraphPartialColoringInterface::RowSmallestLastOrderingPartialDistanceTwoRowColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = RowSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoRowColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2608
	int BipartiteGraphPartialColoringInterface::RowIncidenceDegreeOrderingPartialDistanceTwoRowColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = RowIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoRowColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Row Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2609
	int BipartiteGraphPartialColoringInterface::ColumnNaturalOrderingPartialDistanceTwoColumnColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = ColumnNaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoColumnColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 2610
	int BipartiteGraphPartialColoringInterface::ColumnLargestFirstOrderingPartialDistanceTwoColumnColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = ColumnLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoColumnColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2611
	int BipartiteGraphPartialColoringInterface::ColumnSmallestLastOrderingPartialDistanceTwoColumnColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = ColumnSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoColumnColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 2612
	int BipartiteGraphPartialColoringInterface::ColumnIncidenceDegreeOrderingPartialDistanceTwoColumnColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = ColumnIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = PartialDistanceTwoColumnColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Column Partial Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}

	void BipartiteGraphPartialColoringInterface::GenerateSeedJacobian(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount, double*** dp3_seed, int *ip1_SeedRowCount, int *ip1_SeedColumnCount, string s_ColoringVariant, string s_OrderingVariant) {
		//Clear (Re-initialize) the bipartite graph
		Clear();

		//Read the sparsity pattern of the given Jacobian matrix (compressed sparse rows format)
		//and create the corresponding bipartite graph
		BuildBPGraph_ADOLC(uip2_JacobianSparsityPattern, i_RowCount, i_ColumnCount);

		//Do Partial-Distance-Two-Coloring the bipartite graph with the specified ordering
		PartialDistanceTwoColoring(s_ColoringVariant,s_OrderingVariant);

		//Create the seed matrix from the coloring information
		*dp3_seed = GetSeedMatrix(ip1_SeedRowCount, ip1_SeedColumnCount);
	}

	int BipartiteGraphPartialColoringInterface::PartialDistanceTwoColoring(string s_ColoringVariant, string s_OrderingVariant) {
		m_T_Timer.Start();
		int i_OrderingStatus = OrderVertices(s_OrderingVariant, s_ColoringVariant);
		m_T_Timer.Stop();
		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<s_OrderingVariant<<" Ordering Failed";
			cerr<<endl;

			return(1);
		}

		s_ColoringVariant = toUpper(s_ColoringVariant);
		m_T_Timer.Start();

		int i_ColoringStatus;
		if(s_ColoringVariant == "RIGHT_PARTIAL_DISTANCE_TWO") {
			i_ColoringStatus = PartialDistanceTwoColumnColoring();
		} else if (s_ColoringVariant == "LEFT_PARTIAL_DISTANCE_TWO") {
			i_ColoringStatus = PartialDistanceTwoRowColoring();
		} else {
			cout<<" Unknown Partial Distance Two Coloring Method "<<s_ColoringVariant<<". Please use a legal Method."<<endl;
			m_T_Timer.Stop();
			m_d_ColoringTime = m_T_Timer.GetWallTime();
			return (_FALSE);
		}
		
		m_T_Timer.Stop();
		m_d_ColoringTime = m_T_Timer.GetWallTime();
		return(i_ColoringStatus);
	}

	BipartiteGraphPartialColoringInterface::BipartiteGraphPartialColoringInterface(string s_InputFile, string s_fileFormat, string s_OrderingVariant, string s_ColoringVariant) {
		Clear();

		if (s_InputFile == "WAIT") return;
		
		//---------READ INPUT FILE-------------
		ReadBipartiteGraph(s_InputFile, s_fileFormat);

		if (s_OrderingVariant == "WAIT") return;

		//---------ORDERING-------------
		m_T_Timer.Start();

		int i_OrderingStatus = OrderVertices(s_OrderingVariant, s_ColoringVariant);

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl<<"*ERROR: "<<s_OrderingVariant<<" Ordering Failed"<<endl;
			return;
		}

		if (s_ColoringVariant == "WAIT") return;

		//---------COLORING-------------
		s_ColoringVariant = toUpper(s_ColoringVariant);
		m_T_Timer.Start();

		int i_ColoringStatus;
		if(s_ColoringVariant == "RIGHT_PARTIAL_DISTANCE_TWO") {
			i_ColoringStatus = PartialDistanceTwoColumnColoring();
		} else if (s_ColoringVariant == "LEFT_PARTIAL_DISTANCE_TWO") {
			i_ColoringStatus = PartialDistanceTwoRowColoring();
		} else {
			cout<<" Unknown Partial Distance Two Coloring Method "<<s_ColoringVariant<<". Please use a legal Method."<<endl;
			m_T_Timer.Stop();
			m_d_ColoringTime = m_T_Timer.GetWallTime();
			return;
		}
		
		m_T_Timer.Stop();
		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return;
	}
}
