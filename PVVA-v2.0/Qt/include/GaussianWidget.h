#ifndef GAUSSIANWIDGET_H
#define GAUSSIANWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "Qt/include/GraphTransWidget.h"
#include "VTK/include/Source.h"
//#include "Tools/Gaussian.h"


class GaussianWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	GaussianWidget(QWidget *parent = 0, int wayButton = 0);
	~GaussianWidget();

	void setWidgetData(Source const * _Source);
	bool getWidgetData(Source * _Source) const;

	QString getName() const;

private slots:
  

private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;


	QLabel * widthlabel;
	QLabel * depthlabel;

	QLineEdit * widthLineEdit;
	QLineEdit * depthLineEdit;

	QLabel * label;
	QLineEdit * RecLineEdit;

	//page3
	QGroupBox * imgGroupBox;

	QGroupBox * sourceGroupBox;
	QLabel * z0label;
	QLabel * w0label;
	QLabel * frelabel;
	QLabel * dslabel;

	QLineEdit * z0LineEdit;
	QLineEdit * w0LineEdit;
	QLineEdit * freLineEdit;
	QLineEdit * DsLineEdit;
	QLineEdit * dsLineEdit;

};

#endif // RECTANGLEWIDGET_H
