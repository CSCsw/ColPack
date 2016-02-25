/* Notes:
 * - This code will crash if the matrix only has patterns (no value)
//*/

#include "ColPackHeaders.h"

using namespace ColPack;
using namespace std;



#include "extra.h" //This .h file contains functions that are used in the below examples:
					//ReadMM(), MatrixMultiplication...(), Times2Plus1point5(), displayMatrix() and displayCompressedRowMatrix()
#include "stat.h"

int main(int argc, const char* argv[]) {
	vector<string> Orderings;
	Orderings.push_back("NATURAL");
	Orderings.push_back("LARGEST_FIRST");
	Orderings.push_back("DYNAMIC_LARGEST_FIRST");
	Orderings.push_back("SMALLEST_LAST");
	Orderings.push_back("INCIDENCE_DEGREE");
	Orderings.push_back("RANDOM");

	vector<string> Colorings;
	Colorings.push_back("EXPLICIT_COVERING__STAR_BICOLORING");
	Colorings.push_back("EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING");
	Colorings.push_back("IMPLICIT_COVERING__STAR_BICOLORING");
	Colorings.push_back("IMPLICIT_COVERING__GREEDY_STAR_BICOLORING");

	map<string, bool> stat_flags;
	stat_flags["output_append"]=true;
	stat_flags["NumberOfColors"]=true;
	stat_flags["Time"]=true;

	toFileBiC("/home/nguyend/Desktop/Duck/Research/Prog/graph/MM_collection/", "test1", Orderings,  Colorings,  stat_flags );

	return 0;
}
