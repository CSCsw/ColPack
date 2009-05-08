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

#include "command_line_parameter_processor.h"

void createArgs(int argc, const char* argv[], vector<string>& arg) {
	for(int i=0;i<argc;i++) arg.push_back(argv[i]);
}

int findArg(string argument, vector<string>& arg) {
	for (unsigned int i=0; i<arg.size(); i++)
	{
		if (arg[i]==argument) return i;
	}
	return -1;
}
