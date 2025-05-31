

#include <stdlib.h>

#include "ProjectCommon.h"
// @overloaded
void SafeExit(int _Code){
	crun::ProjectCommon::ChCP(0, 0);
	exit(_Code);
}