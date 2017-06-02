#include "Qt/include/LightSourceWidget.h"
#include <QMessageBox>

LightSourceWidget::LightSourceWidget(QWidget *parent, int wayButton)

{
	setWindowTitle(tr("Create LightSource"));

	// page1
	//defGroupBox
	combobox = new QComboBox;
	combobox->addItem(tr("Rectangle"));
	combobox->addItem(tr("Custom"));
	combobox->addItem(tr("Point"));
	combobox->addItem(tr("Select"));
	
	connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange(int)));

	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap("Qt/images/rectangle_coor.png"));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(combobox);
	layout1->addWidget(imgLlabel);

	defGroupBox = new QGroupBox;
	defGroupBox->setLayout(layout1);
	defGroupBox->setTitle(tr("Definition methods"));

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);
	
	//dimGroupBox
	radiuslabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	widthLineEidt->setText(tr("1"));
	depthLineEidt->setText(tr("1"));
	connect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	layoutName = new QGridLayout;
	addNameLabel(layoutName);

	sourceCombobox = new QComboBox;
	QGridLayout * layoutsource = new QGridLayout;
	layoutsource->addWidget(sourceCombobox);
	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Select Source"));
	sourceGroupBox->setLayout(layoutsource);
	sourceGroupBox->hide();

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addLayout(layoutName);
	tabLayout1->addWidget(sourceGroupBox);

	widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	//imgGroupBox
	//QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");

	//page3
	//img2GroupBox
	img2GroupBox = new QGroupBox;
	addDefGroupBox(img2GroupBox, "Qt/images/rectangle_coor.png");

	//LightNumGroupBox
	var1label = new QLabel(tr("Width interval"));
	var2label = new QLabel(tr("Depth interval"));

	var1LineEidt = new QLineEdit;
	var2LineEidt = new QLineEdit;
	connect(var1LineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));
	connect(var2LineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));

	var1LineEidt->setText(tr("0.1"));
	var2LineEidt->setText(tr("0.1"));

	QGridLayout * layout9 = new QGridLayout;
	layout9->addWidget(var1label, 0, 0);
	layout9->addWidget(var2label, 1, 0);
	layout9->addWidget(var1LineEidt, 0, 1);
	layout9->addWidget(var2LineEidt, 1, 1);

	LightNumGroupBox = new QGroupBox;
	LightNumGroupBox->setTitle(tr("Ray numbers"));
	LightNumGroupBox->setLayout(layout9);

	// lengthGroupBox
	lengthlabel = new QLabel(tr("length"));

	lengthLineEidt = new QLineEdit;
	lengthLineEidt->setText(tr("1"));
	connect(lengthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));

	QGridLayout * layoutlength = new QGridLayout;
	layoutlength->addWidget(lengthlabel, 0, 0);
	layoutlength->addWidget(lengthLineEidt, 0, 1);
	
	lengthGroupBox = new QGroupBox;
	lengthGroupBox->setTitle(tr("Length of ray after reflection"));
	lengthGroupBox->setLayout(layoutlength);

	QVBoxLayout * tabLayout3;
	tabLayout3 = new QVBoxLayout;
	tabLayout3->addWidget(img2GroupBox);
	tabLayout3->addWidget(LightNumGroupBox);
	tabLayout3->addWidget(lengthGroupBox);


	widget3 = new QWidget;
	widget3->setLayout(tabLayout3);

	//page4
	//img3GroupBox
	img3GroupBox = new QGroupBox;
	addDefGroupBox(img3GroupBox, "Qt/images/LightSource_SelSurface.png");

	//surfaceNumGroupBox
	surfaceNumlabel = new QLabel(tr("Num of Surfaces"));
	selectLineEidt = new QLineEdit;
	selectLineEidt->setText(tr("3"));

	connect(selectLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(selectLineEidtChange(QString)));

	QGridLayout * layout10 = new QGridLayout;
	layout10->addWidget(surfaceNumlabel, 0, 0);
	layout10->addWidget(selectLineEidt, 0, 1);

	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to trace"));
	surfaceNumGroupBox->setLayout(layout10);
	
	//selectGroupBox
	NumSel = 3;
	selectlayout = new QGridLayout;
	selectcombobox = new QComboBox[NumSel];
	selectlabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
	
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}

	selectGroupBox = new QGroupBox;
	selectGroupBox->setTitle(tr("Select surfaces"));
	selectGroupBox->setLayout(selectlayout);

	selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(img3GroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(selectGroupBox);

	selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Rays"));
	tabWidget->addTab(selectWidget, tr("Surfaces"));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetChange(int)));


	QGridLayout * layoutbt = new QGridLayout;
	addBtn(layoutbt, wayButton);
	setAddBtn();

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt); 

}

LightSourceWidget::~LightSourceWidget()
{
	delete[]selectcombobox;
	delete[]selectlabel;
}

void LightSourceWidget::comboChange(int index)
{
	tabWidget->removeTab(1);
	tabWidget->removeTab(2);
	tabWidget->removeTab(3);

	if (index == 1)
	{
		radiuslabel->setText(tr("Width(W)"));
		depthlabel->setText(tr("Depth(D)"));
		depthlabel->show();
		depthLineEidt->show();
		var1label->setText(tr("Width interval"));
		var2label->setText(tr("Depth interval"));
		var2label->show();
		var2LineEidt->show();
		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
	}
	else if (index == 0)
	{
		radiuslabel->setText(tr("Width(W)"));
		depthlabel->setText(tr("Depth(D)"));
		depthlabel->show();
		depthLineEidt->show();
		var1label->setText(tr("Width interval"));
		var2label->setText(tr("Depth interval"));
		var2label->show();
		var2LineEidt->show();

		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
	}
	else if(index == 2)
	{
		radiuslabel->setText(tr("Angle(A)"));
		widthLineEidt->setText(tr("30"));
		depthlabel->hide();
		depthLineEidt->hide();
		var1label->setText(tr("Theta interval"));
		var2label->hide();
		var2LineEidt->hide();

		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
		//setText(tr("Phi interval"));
	}
	else
	{
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
		LightNumGroupBox->hide();
		baseGroupBox->hide();
		dimGroupBox->hide();
		setNameHide(true);
		sourceGroupBox->show();
		emit sendData(5);
		return;
	}

	LightNumGroupBox->show();
	baseGroupBox->show();
	dimGroupBox->show();
	setNameHide(false);
	sourceGroupBox->hide();

	emit sendData(5);
}

bool LightSourceWidget::getWidgetData(RaySource & _LightSource)
{
	float *data = new float[12];
	int type;
	GraphTrans graphTransPara;

	bool ok, ok_back;

	data[7] = widthLineEidt->text().toFloat(&ok);
	data[8] = depthLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	data[9] = var1LineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;
	data[10] = var2LineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	type = combobox->currentIndex();

	data[11] = lengthLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	_LightSource.setData(data);
	_LightSource.setGraphTrans(graphTransPara);
	_LightSource.setType(type);
	delete[] data;
	return ok;
}

void LightSourceWidget::addAllSurface(QStringList _allSurfaceName)
{
	allSurfaceName = _allSurfaceName;
	for (int i = 0; i < NumSel; i++)
	{
		selectcombobox[i].clear();
		selectcombobox[i].addItems(allSurfaceName);
	}
}

void LightSourceWidget::addAllSources(QStringList _allSourcesName)
{
	sourceCombobox->clear();
	sourceCombobox->addItems(_allSourcesName);
}

int LightSourceWidget::getSelSources() const
{
	return sourceCombobox->currentIndex();
}

float LightSourceWidget::getLength() const
{
	return lengthLineEidt->text().toFloat();
}

void LightSourceWidget::getSelSurfaces(int & _NumSel, int *& index)
{
	_NumSel = NumSel;

	_NumSel = NumSel;
	index = new int[NumSel];
	for (int i = 0; i < NumSel; i++)
		index[i] = selectcombobox[i].currentIndex();	
}

void LightSourceWidget::setSelSurfaces(int _NumSel)
{
	NumSel = _NumSel;
	delete[]selectcombobox;
	delete[]selectlabel;
	selectcombobox = new QComboBox[_NumSel];
	selectlabel = new QLabel[_NumSel];
	for (int i = 0; i < _NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}
	selectLineEidt->setText(QString::number(_NumSel));
}

void LightSourceWidget::selectLineEidtChange(const QString & text)
{
	bool ok;
	NumSel = text.toInt(&ok);
	if (ok)
	{
		delete[]selectcombobox;
		delete[]selectlabel;
		selectcombobox = new QComboBox[NumSel];
		selectlabel = new QLabel[NumSel];
		for (int i = 0; i < NumSel; i++)
		{
			selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
			selectcombobox[i].addItems(allSurfaceName);
			selectlayout->addWidget(&selectlabel[i], i, 0);
			selectlayout->addWidget(&selectcombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
	}	
}

void LightSourceWidget::tabWidgetChange(int index)
{
	if(index == 0 || index == 1)
		emit sendData(5);
	else
		emit sendData(7);
}

void LightSourceWidget::Page3Change(QString)
{
	emit sendData(7);
}

bool LightSourceWidget::getRectData(ModleData & _modleData) const
{
	bool ok, ok_back;
	float * para = new float[9];
	para[7] = widthLineEidt->text().toFloat(&ok_back);
	para[8] = depthLineEidt->text().toFloat(&ok);

	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	_modleData.setData(para);
	_modleData.setGraphTrans(graphTransPara);
	return ok;
}

int LightSourceWidget::getType() const
{
	return combobox->currentIndex();
}

QString LightSourceWidget::getName() const
{
	return getGraphTransName();
}

void LightSourceWidget::init()
{
	tabWidget->setCurrentIndex(0);
	combobox->setCurrentIndex(0);
	comboChange(0);
}
