#ifndef SPHEREWIDGET_H
#define SPHEREWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/ModleData.h>


class SphereWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	SphereWidget(QWidget *parent = 0, int wayButton = 0);
	~SphereWidget();

	void setWidgetData(const ModleData & _modleData);
	bool getWidgetData(ModleData & _modleData) const;
	QString getName() const;

private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;


	QLabel * widthlabel;

	QLineEdit * widthLineEidt;


	QLabel * label;
	QLineEdit * RecLineEidt;

};

#endif // SphereWidget_H
