#ifndef ParaboloidWidget_H
#define ParaboloidWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/ModleData.h>
//#include "openGL/include/Surface.h"

class ParaboloidWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	ParaboloidWidget(QWidget *parent = 0, int wayButton = 0);
	~ParaboloidWidget();

	void setWidgetData(const ModleData & _modleData);
	bool getWidgetData(ModleData & _modleData) const;
	QString getName() const;

private:
	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;
	QGroupBox * RangeGroupBox;

	QLabel * radiuslabel;
	QLabel * depthlabel;

	QLineEdit * radiusLineEidt;
	QLineEdit * depthLineEidt;

	QLabel * label;
	QLineEdit * nameLineEidt;

	QLabel * xminlabel;
	QLabel * yminlabel;
	QLabel * zminlabel;
	QLabel * xmaxlabel;
	QLabel * ymaxlabel;
	QLabel * zmaxlabel;

	QLineEdit * xminLineEidt;
	QLineEdit * yminLineEidt;
	QLineEdit * zminLineEidt;
	QLineEdit * xmaxLineEidt;
	QLineEdit * ymaxLineEidt;
	QLineEdit * zmaxLineEidt;
};

#endif // ParaboloidWidget_H
