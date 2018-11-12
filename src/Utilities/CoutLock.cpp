
#include "Definitions.h"

#include "CoutLock.h"

namespace ColPack
{
#ifdef _OPENMP
	omp_lock_t CoutLock::coutLock;
#endif

	int CoutLock::unset()
	{
#ifdef _OPENMP
		omp_unset_lock(&CoutLock::coutLock);
#endif
		return 0;
	}
	int CoutLock::set()
	{
#ifdef _OPENMP
		omp_set_lock(&CoutLock::coutLock);
#endif
		return 0;
	}
}
