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
	
	//Public Destructor 3702
	BipartiteGraphBicoloringInterface::~BipartiteGraphBicoloringInterface()
	{
		BipartiteGraphBicoloring::Clear();
	}

	//Virtual Function 3703
	void BipartiteGraphBicoloringInterface::Clear()
	{
		BipartiteGraphBicoloring::Clear();

		return;
	}

	
	//Virtual Function 3704
	void BipartiteGraphBicoloringInterface::Reset()
	{
		BipartiteGraphBicoloring::Reset();

		return;
	}
	
	//Public Function 3708
	int BipartiteGraphBicoloringInterface::NaturalOrderingExplicitCoveringModifiedStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = NaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3709
	int BipartiteGraphBicoloringInterface::LargestFirstOrderingExplicitCoveringModifiedStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = LargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}

	
	//Public Function 3710
	int BipartiteGraphBicoloringInterface::DynamicLargestFirstOrderingExplicitCoveringModifiedStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DynamicLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}


		return(i_ColoringStatus);

	}

	
	//Public Function 3711
	int BipartiteGraphBicoloringInterface::SmallestLastOrderingExplicitCoveringModifiedStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = SmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}
	
	//Public Function 3712
	int BipartiteGraphBicoloringInterface::IncidenceDegreeOrderingExplicitCoveringModifiedStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = IncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3713
	int BipartiteGraphBicoloringInterface::NaturalOrderingExplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = NaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}

	
	//Public Function 3714
	int BipartiteGraphBicoloringInterface::LargestFirstOrderingExplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = LargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}

	
	//Public Function 3715
	int BipartiteGraphBicoloringInterface::DynamicLargestFirstOrderingExplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DynamicLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}

	
	//Public Function 3716
	int BipartiteGraphBicoloringInterface::SmallestLastOrderingExplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = SmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3717
	int BipartiteGraphBicoloringInterface::IncidenceDegreeOrderingExplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = IncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ExplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Explicit Cover Modified Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	//Public Function 3723
	int BipartiteGraphBicoloringInterface::NaturalOrderingImplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = NaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}
	
	//Public Function 3724
	int BipartiteGraphBicoloringInterface::LargestFirstOrderingImplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = LargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);

	}
	
	//Public Function 3725
	int BipartiteGraphBicoloringInterface::DynamicLargestFirstOrderingImplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DynamicLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 3726
	int BipartiteGraphBicoloringInterface::SmallestLastOrderingImplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;
		
		m_T_Timer.Start();

		i_OrderingStatus = SmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringStarBicoloring();

		m_T_Timer.Stop();
		
		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 3727
	int BipartiteGraphBicoloringInterface::IncidenceDegreeOrderingImplicitCoveringStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = IncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}
	
	//Public Function 3738
	int BipartiteGraphBicoloringInterface::NaturalOrderingImplicitCoveringGreedyStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = NaturalOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3739
	int BipartiteGraphBicoloringInterface::LargestFirstOrderingImplicitCoveringGreedyStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = LargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3740
	int BipartiteGraphBicoloringInterface::DynamicLargestFirstOrderingImplicitCoveringGreedyStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DynamicLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3741
	int BipartiteGraphBicoloringInterface::SmallestLastOrderingImplicitCoveringGreedyStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = SmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Public Function 3742
	int BipartiteGraphBicoloringInterface::IncidenceDegreeOrderingImplicitCoveringGreedyStarBicoloring()
	{
		int i_OrderingStatus, i_ColoringStatus, i_CheckingStatus;

		m_T_Timer.Start();

		i_OrderingStatus = IncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_CheckingStatus = CheckStarBicoloring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_CheckingStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Implicit Cover Greedy Star Bicoloring Check Failed : "<<i_CheckingStatus;

			if(i_CheckingStatus > 1)
			{
				cerr<<" Violations"<<endl;
			}
			else
			{
				cerr<<" Violation"<<endl;
			}

			cerr<<endl;

			return(i_CheckingStatus);
		}

		return(i_ColoringStatus);
	}

	void BipartiteGraphBicoloringInterface::GenerateSeedJacobian(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount, double*** dp3_LeftSeed, int *ip1_LeftSeedRowCount, int *ip1_LeftSeedColumnCount, double*** dp3_RightSeed, int *ip1_RightSeedRowCount, int *ip1_RightSeedColumnCount, string s_BicoloringVariant, string s_OrderingVariant) {
		//Clear (Re-initialize) the bipartite graph
		Clear();

		//Read the sparsity pattern of the given Jacobian matrix (compressed sparse rows format)
		//and create the corresponding bipartite graph
		BuildBPGraph_ADOLC(uip2_JacobianSparsityPattern, i_RowCount, i_ColumnCount);

		//Color the graph based on the specified ordering and (Star) Bicoloring
		Bicoloring(s_BicoloringVariant, s_OrderingVariant);

		//From the coloring information, create and return the Left and Right seed matrices
		*dp3_LeftSeed = GetLeftSeedMatrix(ip1_LeftSeedRowCount, ip1_LeftSeedColumnCount);
		*dp3_RightSeed = GetRightSeedMatrix(ip1_RightSeedRowCount, ip1_RightSeedColumnCount);
	}

	int BipartiteGraphBicoloringInterface::Bicoloring(string s_BicoloringVariant, string s_OrderingVariant) {
		m_T_Timer.Start();
		int i_OrderingStatus = OrderVertices(s_OrderingVariant);
		m_T_Timer.Stop();
		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<s_OrderingVariant<<" Ordering Failed";
			cerr<<endl;

			return(1);
		}

		s_BicoloringVariant = toUpper(s_BicoloringVariant);
		m_T_Timer.Start();

		int i_ColoringStatus;
		if(s_BicoloringVariant == "IMPLICIT_COVERING__STAR_BICOLORING") {
			i_ColoringStatus = ImplicitCoveringStarBicoloring();
		} else if (s_BicoloringVariant == "EXPLICIT_COVERING__STAR_BICOLORING") {
			i_ColoringStatus = ExplicitCoveringStarBicoloring();
		} else if (s_BicoloringVariant == "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING") {
			i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();
		} else if (s_BicoloringVariant == "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING") {
			i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();
		} else {
			cout<<" Unknown Bicoloring Method "<<s_BicoloringVariant<<". Please use a legal Method."<<endl;
			m_T_Timer.Stop();
			m_d_ColoringTime = m_T_Timer.GetWallTime();
			return (_FALSE);
		}
		
		m_T_Timer.Stop();
		m_d_ColoringTime = m_T_Timer.GetWallTime();
		return(i_ColoringStatus);
	}

	BipartiteGraphBicoloringInterface::BipartiteGraphBicoloringInterface(string s_InputFile, string s_fileFormat, string s_OrderingVariant, string s_BicoloringVariant) {
		Clear();

		if (s_InputFile == "WAIT") return;
		
		//---------READ INPUT FILE-------------
		ReadBipartiteGraph(s_InputFile, s_fileFormat);

		if (s_OrderingVariant == "WAIT") return;

		//---------ORDERING-------------
		m_T_Timer.Start();

		int i_OrderingStatus = OrderVertices(s_OrderingVariant);

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl<<"*ERROR: "<<s_OrderingVariant<<" Ordering Failed"<<endl;
			return;
		}

		if (s_BicoloringVariant == "WAIT") return;

		//---------COLORING-------------
		s_BicoloringVariant = toUpper(s_BicoloringVariant);
		m_T_Timer.Start();

		int i_ColoringStatus;
		if(s_BicoloringVariant == "IMPLICIT_COVERING__STAR_BICOLORING") {
			i_ColoringStatus = ImplicitCoveringStarBicoloring();
		} else if (s_BicoloringVariant == "EXPLICIT_COVERING__STAR_BICOLORING") {
			i_ColoringStatus = ExplicitCoveringStarBicoloring();
		} else if (s_BicoloringVariant == "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING") {
			i_ColoringStatus = ExplicitCoveringModifiedStarBicoloring();
		} else if (s_BicoloringVariant == "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING") {
			i_ColoringStatus = ImplicitCoveringGreedyStarBicoloring();
		} else {
			cout<<" Unknown Bicoloring Method "<<s_BicoloringVariant<<". Please use a legal Method."<<endl;
			m_T_Timer.Stop();
			m_d_ColoringTime = m_T_Timer.GetWallTime();
			return;
		}
		
		m_T_Timer.Stop();
		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return;
	}
}
