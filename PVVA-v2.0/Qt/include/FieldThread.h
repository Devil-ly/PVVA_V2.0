#ifndef FIELDTHREAD_H
#define FIELDTHREAD_H

#include <QThread>

#include <VTK/include/Field.h>

class FieldThread : public QThread
{
	Q_OBJECT

public:
	FieldThread(Field *_workField)
	{
		workField = _workField;
	}
	//~FieldThread();


protected:
	void run();
private:
	Field *workField;

	
};

#endif // FieldThread_H