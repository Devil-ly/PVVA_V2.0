#include "Qt/include/ArbitrarySurfaceWidget.h"

ArbitrarySurfaceWidget::ArbitrarySurfaceWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create Arbitrary surface"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/rectangle_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);
	
	//dimGroupBox
	a0label = new QLabel(tr("a0"));
	a1label = new QLabel(tr("a1"));
	a2label = new QLabel(tr("a2"));
	a3label = new QLabel(tr("a3"));
	a4label = new QLabel(tr("a4"));
	a5label = new QLabel(tr("a5"));
	a6label = new QLabel(tr("a6"));
	a7label = new QLabel(tr("a7"));
	a8label = new QLabel(tr("a8"));
	a9label = new QLabel(tr("a9"));

	a0LineEidt = new QLineEdit;
	a1LineEidt = new QLineEdit;
	a2LineEidt = new QLineEdit;
	a3LineEidt = new QLineEdit;
	a4LineEidt = new QLineEdit;
	a5LineEidt = new QLineEdit;
	a6LineEidt = new QLineEdit;
	a7LineEidt = new QLineEdit;
	a8LineEidt = new QLineEdit;
	a9LineEidt = new QLineEdit;

	//widthLineEidt->setText(tr("0.5"));
	//depthLineEidt->setText(tr("0.5"));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(a0label, 0, 0);
	layout3->addWidget(a0LineEidt, 0, 1);
	layout3->addWidget(a1label, 0, 2);
	layout3->addWidget(a1LineEidt, 0, 3);
	layout3->addWidget(a2label, 1, 0);
	layout3->addWidget(a2LineEidt, 1, 1);
	layout3->addWidget(a3label, 1, 2);
	layout3->addWidget(a3LineEidt, 1, 3);
	layout3->addWidget(a4label, 2, 0);
	layout3->addWidget(a4LineEidt, 2, 1);
	layout3->addWidget(a5label, 2, 2);
	layout3->addWidget(a5LineEidt, 2, 3);
	layout3->addWidget(a6label, 3, 0);
	layout3->addWidget(a6LineEidt, 3, 1);
	layout3->addWidget(a7label, 3, 2);
	layout3->addWidget(a7LineEidt, 3, 3);
	layout3->addWidget(a8label, 4, 0);
	layout3->addWidget(a8LineEidt, 4, 1);
	layout3->addWidget(a9label, 4, 2);
	layout3->addWidget(a9LineEidt, 4, 3);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	xminlabel = new QLabel(tr("xmin"));
	yminlabel = new QLabel(tr("ymin"));
	zminlabel = new QLabel(tr("zmin"));
	xmaxlabel = new QLabel(tr("xmax"));
	ymaxlabel = new QLabel(tr("ymax"));
	zmaxlabel = new QLabel(tr("zmax"));

	xminLineEidt = new QLineEdit;
	yminLineEidt = new QLineEdit;
	zminLineEidt = new QLineEdit;
	xmaxLineEidt = new QLineEdit;
	ymaxLineEidt = new QLineEdit;
	zmaxLineEidt = new QLineEdit;

	QGridLayout * layoutRange = new QGridLayout;
	layoutRange->addWidget(xminlabel, 0, 0);
	layoutRange->addWidget(xminLineEidt, 0, 1);
	layoutRange->addWidget(xmaxlabel, 0, 2);
	layoutRange->addWidget(xmaxLineEidt, 0, 3);
	layoutRange->addWidget(yminlabel, 1, 0);
	layoutRange->addWidget(yminLineEidt, 1, 1);
	layoutRange->addWidget(ymaxlabel, 1, 2);
	layoutRange->addWidget(ymaxLineEidt, 1, 3);
	layoutRange->addWidget(zminlabel, 2, 0);
	layoutRange->addWidget(zminLineEidt, 2, 1);
	layoutRange->addWidget(zmaxlabel, 2, 2);
	layoutRange->addWidget(zmaxLineEidt, 2, 3);

	RangeGroupBox = new QGroupBox;
	RangeGroupBox->setTitle(tr("Range"));
	RangeGroupBox->setLayout(layoutRange);

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addWidget(RangeGroupBox);
	tabLayout1->addLayout(layout4);

	QWidget * widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	QWidget * widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");


	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));


	QGridLayout * layout5 = new QGridLayout;
	addBtn(layout5, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layout5);

}

ArbitrarySurfaceWidget::~ArbitrarySurfaceWidget()
{
	
}

void ArbitrarySurfaceWidget::setWidgetData(const ModleData & _modleData)
{
	float const * para = _modleData.getData();
	a0LineEidt->setText(QString::number(para[7]));
	a1LineEidt->setText(QString::number(para[8]));
	a2LineEidt->setText(QString::number(para[9]));
	a3LineEidt->setText(QString::number(para[10]));
	a4LineEidt->setText(QString::number(para[11]));
	a5LineEidt->setText(QString::number(para[12]));
	a6LineEidt->setText(QString::number(para[13]));
	a7LineEidt->setText(QString::number(para[14]));
	a8LineEidt->setText(QString::number(para[15]));
	a9LineEidt->setText(QString::number(para[16]));
	xminLineEidt->setText(QString::number(para[17]));
	xmaxLineEidt->setText(QString::number(para[18]));
	yminLineEidt->setText(QString::number(para[19]));
	ymaxLineEidt->setText(QString::number(para[20]));
	zminLineEidt->setText(QString::number(para[21]));
	zmaxLineEidt->setText(QString::number(para[22]));
	setGraphTransData(_modleData.getGraphTrans());
}

bool ArbitrarySurfaceWidget::getWidgetData(ModleData & _modleData) const
{
	bool ok, ok_back;
	float * para = new float[23];
	para[7] = a0LineEidt->text().toFloat(&ok_back);
	para[8] = a1LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[9] = a2LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[10] = a3LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[11] = a4LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[12] = a5LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[13] = a6LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[14] = a7LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[15] = a8LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[16] = a9LineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[17] = xminLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[18] = xmaxLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[19] = yminLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[20] = ymaxLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[21] = zminLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;
	para[22] = zmaxLineEidt->text().toFloat(&ok);
	ok = ok && ok_back;

	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	_modleData.setData(para);
	_modleData.setGraphTrans(graphTransPara);
	return ok;
}

QString ArbitrarySurfaceWidget::getName() const
{
	return getGraphTransName();
}


