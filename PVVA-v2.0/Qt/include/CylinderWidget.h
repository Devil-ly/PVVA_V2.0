#ifndef CYLINDERWIDGET_H
#define CYLINDERWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/ModleData.h>


class CylinderWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	CylinderWidget(QWidget *parent = 0, int wayButton = 0);
	~CylinderWidget();

	void setWidgetData(const ModleData & _modleData);
	bool getWidgetData(ModleData & _modleData) const;
	QString getName() const;

signals:
	//void sendData();//信号，用来向父窗体传值

private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;


	QLabel * widthlabel;
	QLabel * depthlabel;

	QLineEdit * widthLineEidt;
	QLineEdit * depthLineEidt;

	QLabel * label;
	QLineEdit * RecLineEidt;

};

#endif // CylinderWidget_H
