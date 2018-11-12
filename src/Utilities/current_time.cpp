
#include <iostream>
#include <ctime>

using namespace std;

#include "current_time.h"

void current_time() {
  time_t curr=time(0);
  cout << "Current time is: " << ctime(&curr) <<endl;
}
