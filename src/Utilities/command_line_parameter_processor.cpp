
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
