#include "qt/include/FieldThread.h"

void FieldThread::run()
{
	workField->calActor3D();
	workField->getActor3D();
}

