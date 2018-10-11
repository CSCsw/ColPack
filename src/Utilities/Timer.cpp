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

//using namespace std;

#include "Definitions.h"

#include "Timer.h"

namespace ColPack
{
	//Public Constructor 4351
	Timer::Timer()
	{

	}


	//Public Destructor 4352
	Timer::~Timer()
	{

	}


	//Public Function 4354
	void Timer::Start()
	{

#ifdef SYSTEM_TIME

		ct_BeginTimer = times(&tms_BeginTimer);

#else

		ct_BeginTimer = clock();

#endif

	}

	//Public Function 4355
	void Timer::Stop()
	{

#ifdef SYSTEM_TIME

		ct_EndTimer = times(&tms_EndTimer);

#else

		ct_EndTimer = clock();

#endif

	}

	//Public Function 4356
	double Timer::GetWallTime()
	{

#ifdef SYSTEM_TIME

		return (double)(ct_EndTimer - ct_BeginTimer) / CLK_TCK;

#else

		return (double)(ct_EndTimer - ct_BeginTimer) / CLOCKS_PER_SEC;

#endif

	}

	//Public Function 4357
	double Timer::GetProcessorTime()
	{

#ifdef SYSTEM_TIME

		double t_UserTime = (double) (tms_EndTimer.tms_utime - tms_BeginTimer.tms_utime) / CLK_TCK;
		double t_SystemTime = (double) (tms_EndTimer.tms_stime - tms_BeginTimer.tms_stime) / CLK_TCK;

		return(t_UserTime + t_SystemTime);

#else

		return(_UNKNOWN);

#endif

	}

	//Public Function 4358
	double Timer::GetUserProcessorTime()
	{

#ifdef SYSTEM_TIME

		double t_UserTime = (double)(tms_EndTimer.tms_utime - tms_BeginTimer.tms_utime) / CLK_TCK;

		return(t_UserTime);

#else

		return(_UNKNOWN);

#endif

	}

	//Public Function 4359
	double Timer::GetSystemProcessorTime()
	{

#ifdef SYSTEM_TIME

		double t_SystemTime = (double)(tms_EndTimer.tms_stime - tms_BeginTimer.tms_stime) / CLK_TCK;

		return(t_SystemTime);

#else

		return(_UNKNOWN);

#endif

	}
}
