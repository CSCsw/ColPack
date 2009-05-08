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
	
	//Private Function 1502
	int GraphColoringInterface::NaturalOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Private Function 1503
	int GraphColoringInterface::NaturalOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		return(i_ColoringStatus);
	}

	
	//Private Function 1504
	int GraphColoringInterface::NaturalOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;
		
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

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1505
	int GraphColoringInterface::NaturalOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1506
	int GraphColoringInterface::NaturalOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1507
	int GraphColoringInterface::NaturalOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1508
	int GraphColoringInterface::NaturalOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1509
	int GraphColoringInterface::LargestFirstOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1510
	int GraphColoringInterface::LargestFirstOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = LargestFirstOrdering();

		m_T_Timer.Start();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1511
	int GraphColoringInterface::LargestFirstOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1512
	int GraphColoringInterface::LargestFirstOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1513
	int GraphColoringInterface::LargestFirstOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1514
	int GraphColoringInterface::LargestFirstOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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
			
		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1515
	int GraphColoringInterface::LargestFirstOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1516
	int GraphColoringInterface::DynamicLargestFirstOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1517
	int GraphColoringInterface::DynamicLargestFirstOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1517
	int GraphColoringInterface::DynamicLargestFirstOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1518
	int GraphColoringInterface::DynamicLargestFirstOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1519
	int GraphColoringInterface::DynamicLargestFirstOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1520
	int GraphColoringInterface::DynamicLargestFirstOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1521
	int GraphColoringInterface::DynamicLargestFirstOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1522
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1523
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1524
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1525
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1526
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1527
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1528
	int GraphColoringInterface::DistanceTwoLargestFirstOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1529
	int GraphColoringInterface::SmallestLastOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1530
	int GraphColoringInterface::SmallestLastOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1531
	int GraphColoringInterface::SmallestLastOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1532
	int GraphColoringInterface::SmallestLastOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1533
	int GraphColoringInterface::SmallestLastOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1534
	int GraphColoringInterface::SmallestLastOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1535
	int GraphColoringInterface::SmallestLastOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1536
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1537
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1538
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1539
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1540
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1541
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1542
	int GraphColoringInterface::DistanceTwoSmallestLastOrderingTriangularColoring()
	{	
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1543
	int GraphColoringInterface::IncidenceDegreeOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1544
	int GraphColoringInterface::IncidenceDegreeOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1545
	int GraphColoringInterface::IncidenceDegreeOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = NaiveStarColoring();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1546
	int GraphColoringInterface::IncidenceDegreeOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1547
	int GraphColoringInterface::IncidenceDegreeOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1548
	int GraphColoringInterface::IncidenceDegreeOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = AcyclicColoring();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1549
	int GraphColoringInterface::IncidenceDegreeOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

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

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1550
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingDistanceOneColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1551
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingDistanceTwoColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1552
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingNaiveStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1553
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingRestrictedStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1554
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingStarColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = StarColoring();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1556
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingAcyclicColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Private Function 1557
	int GraphColoringInterface::DistanceTwoIncidenceDegreeOrderingTriangularColoring()
	{
		int i_OrderingStatus, i_ColoringStatus;

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();
		
		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}

		m_T_Timer.Start();

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Private Function 1558
	int GraphColoringInterface::CompareVertexColors()
	{
		int i_ColoringStatus;

		int i_StarColoringCheckStatus, i_AcyclicColoringCheckStatus;


		m_s_VertexColoringVariant = "ALL";

		//Distance One Coloring

		m_T_Timer.Start();

		i_ColoringStatus = DistanceOneColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}
		
		FileVertexColoringMetrics();

		//Distance Two Coloring

		m_T_Timer.Start();

		i_ColoringStatus = DistanceTwoColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		//Naive Star Coloring

		m_T_Timer.Start();

		i_ColoringStatus = NaiveStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_StarColoringCheckStatus =	CheckStarColoring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_StarColoringCheckStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Naive Star Coloring Check Failed";
			cerr<<endl;

			return(i_StarColoringCheckStatus);
		}

		FileVertexColoringMetrics();

		//Restricted Star Coloring

		m_T_Timer.Start();

		i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_StarColoringCheckStatus =	CheckStarColoring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_StarColoringCheckStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Restricted Star Coloring Check Failed";
			cerr<<endl;

			return(i_StarColoringCheckStatus);
		}

		FileVertexColoringMetrics();

		//Star Coloring

		m_T_Timer.Start();

		i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_StarColoringCheckStatus =	CheckStarColoring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_StarColoringCheckStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Star Coloring Check Failed";
			cerr<<endl;

			return(i_StarColoringCheckStatus);
		}

		FileVertexColoringMetrics();

		//Acyclic Coloring

		m_T_Timer.Start();

		i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		m_T_Timer.Start();

		i_AcyclicColoringCheckStatus =	CheckAcyclicColoring();

		m_T_Timer.Stop();

		m_d_CheckingTime = m_T_Timer.GetWallTime();

		if(i_AcyclicColoringCheckStatus != _FALSE)
		{
			cerr<<endl;
			cerr<<"Acyclic Coloring Check Failed";
			cerr<<endl;

			return(i_AcyclicColoringCheckStatus);
		}

		FileVertexColoringMetrics();

		//Triangular Coloring

		m_T_Timer.Start();

		i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}


	
	//Public Destructor 1602
	GraphColoringInterface::~GraphColoringInterface()
	{
		Clear();
	}
	
	//Virtual Function 1603
	void GraphColoringInterface::Clear()
	{
		GraphColoring::Clear();

		return;
	}
	
	//Public Function 1604
	int GraphColoringInterface::DistanceOneColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = DistanceOneColoring();
		
		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1605
	int GraphColoringInterface::DistanceTwoColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = DistanceTwoColoring();
	
		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1606
	int GraphColoringInterface::NaiveStarColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = NaiveStarColoring();
		
		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1607
	int GraphColoringInterface::RestrictedStarColoring(string s_OrderingVariant)
	{
		m_T_Timer.Start();

		int i_OrderingVariant = OrderVertices(s_OrderingVariant);

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingVariant != _TRUE)
		{
			cerr<<endl;
			cerr<<s_OrderingVariant<<" Ordering Failed";
			cerr<<endl;

			return(_TRUE);
		}

		m_T_Timer.Start();

		int i_ColoringStatus = RestrictedStarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1608
	int GraphColoringInterface::StarColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = StarColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1609
	int GraphColoringInterface::AcyclicColoring_ForIndirectRecovery(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = GraphColoring::AcyclicColoring_ForIndirectRecovery();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1609
	int GraphColoringInterface::AcyclicColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = AcyclicColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}
	
	//Public Function 1610
	int GraphColoringInterface::TriangularColoring(string s_OrderingVariant)
	{
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

		m_T_Timer.Start();

		int i_ColoringStatus = TriangularColoring();

		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return(i_ColoringStatus);
	}

	
	//Public Function 1611
	int GraphColoringInterface::CompareVertexColorsWithNaturalOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Natural Ordering

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
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1612
	int GraphColoringInterface::CompareVertexColorsWithLargestFirstOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Largest First Ordering

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
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1613
	int GraphColoringInterface::CompareVertexColorsWithDynamicLargestFirstOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Dynamic Largest First Ordering

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
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1614
	int GraphColoringInterface::CompareVertexColorsWithDistanceTwoLargestFirstOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Distance Two Largest First Ordering

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoLargestFirstOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1615
	int GraphColoringInterface::CompareVertexColorsWithSmallestLastOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Smallest Last Ordering

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

		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1616
	int GraphColoringInterface::CompareVertexColorsWithDistanceTwoSmallestLastOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Distance Two Smallest Last Ordering

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoSmallestLastOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1617
	int GraphColoringInterface::CompareVertexColorsWithIncidenceDegreeOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Incidence Degree Ordering

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
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}

	
	//Public Function 1618
	int GraphColoringInterface::CompareVertexColorsWithDistanceTwoIncidenceDegreeOrdering()
	{
		int i_OrderingStatus, i_ColoringStatus;

		//Distance Two Incidence Degree Ordering

		m_T_Timer.Start();

		i_OrderingStatus = DistanceTwoIncidenceDegreeOrdering();

		m_T_Timer.Stop();

		m_d_OrderingTime = m_T_Timer.GetWallTime();

		if(i_OrderingStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Failed";
			cerr<<endl;

			return(i_OrderingStatus);
		}
		
		i_ColoringStatus = CompareVertexColors();

		return(i_ColoringStatus);
	}	
	
	//Public Function 1619
	int GraphColoringInterface::CompareVertexOrdersWithDistanceOneColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingDistanceOneColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingDistanceOneColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Distance One Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}
	
	//Public Function 1620
	int GraphColoringInterface::CompareVertexOrdersWithDistanceTwoColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingDistanceTwoColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingDistanceTwoColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Distance Two Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}
	
	//Public Function 1621
	int GraphColoringInterface::CompareVertexOrdersWithNaiveStarColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingNaiveStarColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingNaiveStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Naive Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}

	
	//Public Function 1622
	int GraphColoringInterface::CompareVertexOrdersWithRestrictedStarColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingRestrictedStarColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingRestrictedStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Restricted Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}
	
	//Public Function 1623
	int GraphColoringInterface::CompareVertexOrdersWithStarColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingStarColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingStarColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Star Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}

	
	//Public Function 1624
	int GraphColoringInterface::CompareVertexOrdersWithAcyclicColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingAcyclicColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingAcyclicColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Acyclic Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}


	
	//Public Function 1625
	int GraphColoringInterface::CompareVertexOrdersWithTriangularColoring()
	{	
		int i_ColoringStatus;

		m_s_VertexOrderingVariant = "ALL";

		i_ColoringStatus = NaturalOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Natural Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = LargestFirstOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Largest First Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		/*
		i_ColoringStatus = DynamicLargestFirstOrderingTriangularColoring();
		
		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Dynamic Largest First Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();
		*/

		i_ColoringStatus = DistanceTwoLargestFirstOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Largest First Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = SmallestLastOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Smallest Last Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoSmallestLastOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Smallest Last Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = IncidenceDegreeOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Incidence Degree Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		i_ColoringStatus = DistanceTwoIncidenceDegreeOrderingTriangularColoring();

		if(i_ColoringStatus != _TRUE)
		{
			cerr<<endl;
			cerr<<"Distance Two Incidence Degree Ordering Triangular Coloring Failed";
			cerr<<endl;

			return(i_ColoringStatus);
		}

		FileVertexColoringMetrics();

		return(i_ColoringStatus);
	}

	void GraphColoringInterface::GenerateSeedHessian(unsigned int ** uip2_HessianSparsityPattern, int i_RowCount, double*** dp3_seed, int *ip1_SeedRowCount, int *ip1_SeedColumnCount, string s_ColoringVariant, string s_OrderingVariant) {
		//Clear (Re-initialize) the graph
		Clear();

		//Read the sparsity pattern of the given Hessian matrix (compressed sparse rows format)
		//and create the corresponding graph
		BuildGraph_ADOLC(uip2_HessianSparsityPattern, i_RowCount);
		//PrintGraphStructure();

		//Color the bipartite graph with the specified ordering
		if (s_ColoringVariant=="STAR"
			|| s_ColoringVariant=="RESTRICTED_STAR"
			|| s_ColoringVariant=="ACYCLIC_FOR_INDIRECT_RECOVERY") 
		{
			Coloring(s_ColoringVariant, s_OrderingVariant);
		}
		else {
			cerr<<"Error: Unrecognized coloring method."<<endl;
			return;
		}

		//Create the seed matrix from the coloring information
		*dp3_seed = GetSeedMatrix(ip1_SeedRowCount, ip1_SeedColumnCount);

		/*
		PrintVertexColors();
		PrintVertexColoringMetrics();
		double **Seed = *dp3_seed;
		int rows = GetVertexCount();
		int cols = GetVertexColorCount(); 
		cout<<"Seed matrix: ("<<rows<<","<<cols<<")"<<endl;
		for(int i=0; i<rows; i++) {
			for(int j=0; j<cols; j++) {
				cout<<setw(6)<<Seed[i][j];
			}
			cout<<endl;
		}
		//*/
	}

	void GraphColoringInterface::PrintInducedVertexDegrees(int i, int i_HighestInducedVertexDegree, vector< list<int> > &vli_GroupedInducedVertexDegrees) {

		int k;

		list<int>::iterator lit_ListIterator;

		cout<<endl;
		cout<<"DEBUG 5103 | Hessian Evaluation | Induced Vertex Degrees | Set "<<STEP_UP(i)<<endl;
		cout<<endl;

		for(int j=0; j<STEP_UP(i_HighestInducedVertexDegree); j++)
		{
			int i_SetSize = (signed) vli_GroupedInducedVertexDegrees[j].size();

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

	}

	int GraphColoringInterface::Coloring(string s_ColoringVariant, string s_OrderingVariant) {
		if(s_ColoringVariant == "DISTANCE_ONE") {
			return DistanceOneColoring(s_OrderingVariant);
		} else if (s_ColoringVariant == "ACYCLIC") {
			return AcyclicColoring(s_OrderingVariant);
		} else if (s_ColoringVariant == "ACYCLIC_FOR_INDIRECT_RECOVERY") {
			return AcyclicColoring_ForIndirectRecovery(s_OrderingVariant);
		} else if (s_ColoringVariant == "STAR") {
			return StarColoring(s_OrderingVariant);
		} else if (s_ColoringVariant == "RESTRICTED_STAR") {
			return RestrictedStarColoring(s_OrderingVariant);
		} else if (s_ColoringVariant == "DISTANCE_TWO") {
			return DistanceTwoColoring(s_OrderingVariant);
		} else {
			cout<<" Unknown Coloring Method "<<s_ColoringVariant<<". Please use a legal Coloring Method."<<endl;
			return (_FALSE);
		}

		return (_TRUE);
	}

	GraphColoringInterface::GraphColoringInterface(string s_InputFile, string s_fileFormat, string s_OrderingVariant, string s_ColoringVariant)
	{
		Clear();

		if (s_InputFile == "WAIT") return;
		
		//---------READ INPUT FILE-------------
		ReadAdjacencyGraph(s_InputFile, s_fileFormat);

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

		if (s_ColoringVariant == "WAIT") return;

		//---------COLORING-------------
		m_T_Timer.Start();

		if(s_ColoringVariant == "DISTANCE_ONE") DistanceOneColoring();
		else if (s_ColoringVariant == "ACYCLIC") AcyclicColoring();
		else if (s_ColoringVariant == "STAR") StarColoring();
		else if (s_ColoringVariant == "RESTRICTED_STAR") RestrictedStarColoring();
		else if (s_ColoringVariant == "DISTANCE_TWO") DistanceTwoColoring();
		else {
			cerr<<endl<<"*ERROR: Unknown Coloring Method "<<s_ColoringVariant<<". Please use a legal Coloring Method."<<endl;
			return;
		}
		
		m_T_Timer.Stop();

		m_d_ColoringTime = m_T_Timer.GetWallTime();

		return;
	}
}
