#include "Qt/include/ParaboloidWidget.h"
#include <vector>

ParaboloidWidget::ParaboloidWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create paraboloid"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	radiuslabel = new QLabel(tr("Radius(R)"));
	depthlabel = new QLabel(tr("Focal depth(F)"));

	radiusLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	connect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	radiusLineEidt->setText(tr("1.0"));
	depthLineEidt->setText(tr("0.5"));

	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(radiusLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);

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

	xminLineEidt->setText(tr("-1.0"));
	yminLineEidt->setText(tr("-1.0"));
	zminLineEidt->setText(tr("0.0"));
	xmaxLineEidt->setText(tr("1.0"));
	ymaxLineEidt->setText(tr("1.0"));
	zmaxLineEidt->setText(tr("1.0"));

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
	//imgGroupBox
	QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Paraboloid_coor.png");

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	QGridLayout * layoutbt = new QGridLayout;

	addBtn(layoutbt, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt);

}

ParaboloidWidget::~ParaboloidWidget()
{

}

void ParaboloidWidget::setWidgetData(const ModleData & _modleData)
{
	float const * para = _modleData.getData();
	radiusLineEidt->setText(QString::number(para[7]));
	depthLineEidt->setText(QString::number(para[8]));
	xminLineEidt->setText(QString::number(para[9]));
	yminLineEidt->setText(QString::number(para[10]));
	zminLineEidt->setText(QString::number(para[11]));
	xmaxLineEidt->setText(QString::number(para[12]));
	ymaxLineEidt->setText(QString::number(para[13]));
	zmaxLineEidt->setText(QString::number(para[14]));
	setGraphTransData(_modleData.getGraphTrans());
	//setGraphTransName(_surfacePtr->getName());
}

bool ParaboloidWidget::getWidgetData(ModleData & _modleData) const
{
	bool ok, ok_back;
	float * para = new float[15];
	//vector <float> para;
	para[7] = radiusLineEidt->text().toFloat(&ok);
    para[8] = depthLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[9] = xminLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[10] = yminLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[11] = zminLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[12] = xmaxLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[13] = ymaxLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	para[14] = zmaxLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;
	_modleData.setData(para);
	_modleData.setGraphTrans(graphTransPara);

	//QString name = getGraphTransName();
	//_surface.setSurfaceData(para, graphTransPara, name);
	return ok;
}

QString ParaboloidWidget::getName() const
{
	return getGraphTransName();
}


