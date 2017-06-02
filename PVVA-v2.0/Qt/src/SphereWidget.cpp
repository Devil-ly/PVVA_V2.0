#include "Qt/include/SphereWidget.h"

SphereWidget::SphereWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create sphere"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/sphere_coor.png");


	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	
	//dimGroupBox
	widthlabel = new QLabel(tr("Radius(R)"));

	widthLineEidt = new QLineEdit;
	connect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	widthLineEidt->setText(tr("0.5"));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(widthlabel, 0, 0);
	layout3->addWidget(widthLineEidt, 0, 1);


	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
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

SphereWidget::~SphereWidget()
{
	
}

void SphereWidget::setWidgetData(const ModleData & _modleData)
{
	float const * para = _modleData.getData();
	widthLineEidt->setText(QString::number(para[7]));

	setGraphTransData(_modleData.getGraphTrans());
	//setGraphTransName(_surfacePtr->getName());
}

bool SphereWidget::getWidgetData(ModleData & _modleData) const
{
	bool ok, ok_back;
	float * para = new float[8];
	para[7] = widthLineEidt->text().toFloat(&ok_back);

	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	_modleData.setData(para);
	_modleData.setGraphTrans(graphTransPara);
	//QString name = getGraphTransName();
	//_surface.setSurfaceData(para, graphTransPara, name);
	return ok;
}

QString SphereWidget::getName() const
{
	return getGraphTransName();
}


