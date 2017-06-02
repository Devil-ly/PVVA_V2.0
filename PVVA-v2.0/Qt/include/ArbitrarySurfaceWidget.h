#ifndef ARBITRARYSURFACEWIDGET_H
#define ARBITRARYSURFACEWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/ModleData.h>


class ArbitrarySurfaceWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	ArbitrarySurfaceWidget(QWidget *parent = 0, int wayButton = 0);
	~ArbitrarySurfaceWidget();

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

	QLabel * a0label;
	QLabel * a1label;
	QLabel * a2label;
	QLabel * a3label;
	QLabel * a4label;
	QLabel * a5label;
	QLabel * a6label;
	QLabel * a7label;
	QLabel * a8label;
	QLabel * a9label;

	QLineEdit * a0LineEidt;
	QLineEdit * a1LineEidt;
	QLineEdit * a2LineEidt;
	QLineEdit * a3LineEidt;
	QLineEdit * a4LineEidt;
	QLineEdit * a5LineEidt;
	QLineEdit * a6LineEidt;
	QLineEdit * a7LineEidt;
	QLineEdit * a8LineEidt;
	QLineEdit * a9LineEidt;

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


	QLabel * label;
	QLineEdit * RecLineEidt;

};

#endif // ArbitrarySurfaceWidget_H
