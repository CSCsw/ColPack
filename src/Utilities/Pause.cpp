
//Special pause that work on both Windows and UNIX for both C and C++
#include "Pause.h"

using namespace std;

void Pause()
{
		printf("Press enter to continue ...");
		getchar();
}
