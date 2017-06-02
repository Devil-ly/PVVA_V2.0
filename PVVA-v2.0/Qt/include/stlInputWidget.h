#ifndef STLINPUTWIDGET_H
#define STLINPUTWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/ModleData.h>


class STLInputWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	STLInputWidget(QWidget *parent = 0, int wayButton = 0);
	~STLInputWidget();

	void setWidgetData(const ModleData & _modleData);
	bool getWidgetData(ModleData & _modleData) const;
	QString getName() const;

private slots:
    void buttonSTL();

private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;

	QLineEdit * STLLineEdit;
	QPushButton * STLBtn;

	QLabel * label;
	QLineEdit * RecLineEidt;

};

#endif // STLInputWidget_H
