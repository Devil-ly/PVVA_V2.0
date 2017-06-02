#include "Qt/include/GaussianWidget.h"
#include <QFileDialog>

GaussianWidget::GaussianWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create Gaussian"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/rectangle_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	widthlabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEdit = new QLineEdit;
	depthLineEdit = new QLineEdit;

	widthLineEdit->setText(tr("1"));
	depthLineEdit->setText(tr("1"));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(widthlabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEdit, 0, 1);
	layout3->addWidget(depthLineEdit, 1, 1);

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

	// page3
	imgGroupBox = new QGroupBox;
	addDefGroupBox(imgGroupBox, "Qt/images/Gaussian_def.png");

	//sourcegroup
	z0label = new QLabel(tr("Target distance(z0)"));
	w0label = new QLabel(tr("Gaussian waist(w0)"));
	frelabel = new QLabel(tr("Frequency  (GHz)"));
	dslabel = new QLabel(tr("Sampling Precision"));

	z0LineEdit = new QLineEdit;
	w0LineEdit = new QLineEdit;
	freLineEdit = new QLineEdit;
	DsLineEdit = new QLineEdit;

	z0LineEdit->setText("0");
	w0LineEdit->setText("0.04");
	freLineEdit->setText("10");
	DsLineEdit->setText("0.005");
	connect(z0LineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(w0LineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(freLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(DsLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layoutSou = new QGridLayout;
	layoutSou->addWidget(z0label, 0, 0);
	layoutSou->addWidget(w0label, 1, 0);
	layoutSou->addWidget(frelabel, 2, 0);
	layoutSou->addWidget(dslabel, 3, 0);
	layoutSou->addWidget(z0LineEdit, 0, 1);
	layoutSou->addWidget(w0LineEdit, 1, 1);
	layoutSou->addWidget(freLineEdit, 2, 1);
	layoutSou->addWidget(DsLineEdit, 3, 1);

	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Source"));
	sourceGroupBox->setLayout(layoutSou);

	//tabLayout2
	QVBoxLayout * tabLayout2; // page1
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addWidget(sourceGroupBox);

	QWidget * widget3 = new QWidget;
	widget3->setLayout(tabLayout2);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Excitation"));


	QGridLayout * layoutbtn = new QGridLayout;
	addBtn(layoutbtn, wayButton);
	setAddBtn();

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);/**/

}

GaussianWidget::~GaussianWidget()
{
	
}

void GaussianWidget::setWidgetData(Source const * _Source)
{
	vector<double> para = _Source->getData();

	widthLineEdit->setText(QString::number(para[7]));
	depthLineEdit->setText(QString::number(para[8]));
	//GraphTrans test = _Source->getGraphTrans();
	//setGraphTransData(test);

	z0LineEdit->setText(QString::number(para[9]));
	w0LineEdit->setText(QString::number(para[10]));
	freLineEdit->setText(QString::number(para[11]));
	DsLineEdit->setText(QString::number(para[12]));
	
	//setGraphTransName(_name);
}

bool GaussianWidget::getWidgetData(Source * _Source) const
{
	bool ok, ok_back;
	double * para = new double[13];
	para[7] = widthLineEdit->text().toFloat(&ok_back);
	para[8] = depthLineEdit->text().toFloat(&ok);
	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	para[9] = z0LineEdit->text().toDouble(&ok_back);
	ok = ok && ok_back;

	para[10] = w0LineEdit->text().toDouble(&ok_back);
	ok = ok && ok_back;

	para[11] = freLineEdit->text().toDouble(&ok_back);
	ok = ok && ok_back;

	para[12] = DsLineEdit->text().toDouble(&ok_back);
	ok = ok && ok_back;
	
	_Source->setData(para,13);
	_Source->setGraphTrans(graphTransPara);
	delete para;
	//QString name = getGraphTransName();
	//_GaussianPtr->setGaussian(para, graphTransPara, z0, w0, fre, ds, name);
	return ok;
}

QString GaussianWidget::getName() const
{
	return getGraphTransName();
}
