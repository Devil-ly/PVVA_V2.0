#include "Qt/include/CalculationWidget.h"
#include <QMessageBox>
#include <QFileDialog>

CalculationWidget::CalculationWidget(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Calculation"));
	
	//paraGroupBox
	freLabel = new QLabel(tr("Frequency(GHz)"));
	freLineEdit = new QLineEdit;
	freLineEdit->setText(tr("10"));

	sourceLabel = new QLabel(tr("start from source"));
	sourceCombobox = new QComboBox;

	calModLabel = new QLabel(tr("Calculation model"));   // 计算模式 解析解优先还是剖分优先
	calModCombobox = new QComboBox;
	calModCombobox->addItem(tr("Mesh priority"));
	calModCombobox->addItem(tr("Analysis priority"));
	calModCombobox->addItem(tr("Custom"));
	calModCombobox->setCurrentIndex(2);

	MeshLabel = new QLabel(tr("Mesh size"));
	MeshCombobox = new QComboBox;
	MeshCombobox->addItem(tr("Fine"));
	MeshCombobox->addItem(tr("Standard"));
	MeshCombobox->addItem(tr("Coarse"));
	MeshCombobox->setCurrentIndex(1);

	QGridLayout * layoutpara = new QGridLayout;
	layoutpara->addWidget(freLabel, 0, 0);
	layoutpara->addWidget(freLineEdit, 0, 1);
	layoutpara->addWidget(sourceLabel, 1, 0);
	layoutpara->addWidget(sourceCombobox, 1, 1);
	layoutpara->addWidget(calModLabel, 2, 0);
	layoutpara->addWidget(calModCombobox, 2, 1);
	layoutpara->addWidget(MeshLabel, 3, 0);
	layoutpara->addWidget(MeshCombobox, 3, 1);

	paraGroupBox = new QGroupBox;
	paraGroupBox->setTitle(tr("Parameters"));
	paraGroupBox->setLayout(layoutpara);

	//surfaceNumGroupBox
	surfaceLabel = new QLabel(tr("Num of Surfaces"));
	surfaceLineEdit = new QLineEdit;
	surfaceLineEdit->setText(tr("3"));

	connect(surfaceLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(selectLineEditChange(QString)));

	QGridLayout * layoutSel = new QGridLayout;
	layoutSel->addWidget(surfaceLabel, 0, 0);
	layoutSel->addWidget(surfaceLineEdit, 0, 1);
	
	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to calculate"));
	surfaceNumGroupBox->setLayout(layoutSel);

	//selectGroupBox
	NumSel = 3;
	selectlayout = new QGridLayout;
	selectCombobox = new QComboBox[NumSel];
	selectLabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		selectLabel[i].setText(tr("Surface ") + QString::number(i + 1));

		selectCombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectLabel[i], i, 0);
		selectlayout->addWidget(&selectCombobox[i], i, 1);
	}

	selectGroupBox = new QGroupBox;
	selectGroupBox->setTitle(tr("Select surfaces"));
	selectGroupBox->setLayout(selectlayout);

	//resultGroupBox
	resultLabel = new QLabel(tr("Num of Surfaces"));
	resultCombobox = new QComboBox;
	resultCombobox->addItem(tr("Default"));
	resultCombobox->addItem(tr("Custom"));
	connect(resultCombobox, SIGNAL(activated(int)), this, SLOT(resultComboboxChange(int)));

	distanceLabel = new QLabel(tr("Propagation distance"));
	widthLabel = new QLabel(tr("Width"));
	depthLaebl = new QLabel(tr("Depth"));
	distanceLineEdit = new QLineEdit;
	widthLineEdit = new QLineEdit;
	depthLineEdit = new QLineEdit;
	distanceLineEdit->setEnabled(false);
	widthLineEdit->setEnabled(false);
	depthLineEdit->setEnabled(false);

	QGridLayout * layoutRect = new QGridLayout;
	layoutRect->addWidget(resultLabel, 0, 0);
	layoutRect->addWidget(resultCombobox, 0, 1);
	layoutRect->addWidget(distanceLabel, 1, 0);
	layoutRect->addWidget(distanceLineEdit, 1, 1);
	layoutRect->addWidget(widthLabel, 2, 0);
	layoutRect->addWidget(widthLineEdit, 2, 1);
	layoutRect->addWidget(depthLaebl, 3, 0);
	layoutRect->addWidget(depthLineEdit, 3, 1);

	saveLabel = new QLabel(tr("Output address"));
	saveLineEdit = new QLineEdit;
	saveBtn = new QPushButton(tr("Browse..."));
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(buttonSave()));

	QGridLayout * layoutSave = new QGridLayout;
	layoutSave->addWidget(saveLabel, 0, 0);
	layoutSave->addWidget(saveLineEdit, 0, 1);
	layoutSave->addWidget(saveBtn, 0, 2);

	QVBoxLayout * resultLayout = new QVBoxLayout;
	resultLayout->addLayout(layoutRect);
	resultLayout->addLayout(layoutSave);

	resultGroupBox = new QGroupBox;
	resultGroupBox->setTitle(tr("Result"));
	resultGroupBox->setLayout(resultLayout);

	QVBoxLayout * selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(paraGroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(selectGroupBox);
	selectTabLayout->addWidget(resultGroupBox);

	QWidget * selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	// page 2
	//surfaceNumGroupBox
	/*monitorNumLabel = new QLabel(tr("Num of Rectangles"));
	monitorNumLineEdit = new QLineEdit;
	monitorNumLineEdit->setText(tr("3"));

	connect(monitorNumLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(monitorNumLineEditChange(QString)));

	QGridLayout * layoutMonitorNum = new QGridLayout;
	layoutMonitorNum->addWidget(monitorNumLabel, 0, 0);
	layoutMonitorNum->addWidget(monitorNumLineEdit, 0, 1);

	monitorNumGroupBox = new QGroupBox;
	monitorNumGroupBox->setTitle(tr("Add rectangle to watch (only rectangle)"));
	monitorNumGroupBox->setLayout(layoutMonitorNum);

	//selectGroupBox
	
	NumMonitor = 3;
	monitorlayout = new QGridLayout;
	monitorCombobox = new QComboBox[NumSel];
	monitorLabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		monitorLabel[i].setText(tr("Rectangle ") + QString::number(i + 1));
		monitorCombobox[i].addItems(allSurfaceName);
		monitorlayout->addWidget(&monitorLabel[i], i, 0);
		monitorlayout->addWidget(&monitorCombobox[i], i, 1);
	}

	monitorGroupBox = new QGroupBox;
	monitorGroupBox->setTitle(tr("Select rectangles"));
	monitorGroupBox->setLayout(monitorlayout);

	QVBoxLayout * monitorTabLayout = new QVBoxLayout;
	monitorTabLayout->addWidget(monitorNumGroupBox);
	monitorTabLayout->addWidget(monitorGroupBox);

	QWidget * monitorWidget = new QWidget;
	monitorWidget->setLayout(monitorTabLayout);*/

	tabWidget = new QTabWidget;
	tabWidget->addTab(selectWidget, tr("Surfaces"));
	//tabWidget->addTab(monitorWidget, tr("Monitors"));

	okBtn = new QPushButton(tr("OK"));
	checkBtn = new QPushButton(tr("Check"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(checkBtn, SIGNAL(clicked()), this, SLOT(buttonCheck()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));
	QHBoxLayout * layoutbtn = new QHBoxLayout;
	layoutbtn->addSpacing(100);
	layoutbtn->addWidget(okBtn, 0);
	layoutbtn->addWidget(checkBtn, 0);
	layoutbtn->addWidget(cancelBtn, 0);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);

	//returnNum = -1;
}

CalculationWidget::~CalculationWidget()
{
	delete[]selectCombobox;
	delete[]selectLabel;
	//delete[]monitorCombobox;
	//delete[]monitorLabel;
}

void CalculationWidget::buttonCancel()
{
	close();
}

void CalculationWidget::buttonSave()
{
	QString filename = QFileDialog::getExistingDirectory(this, 
		"Select the path to save output...", "./");
	if (!filename.isEmpty())
	{
		saveLineEdit->setText(filename);
	}
}

void CalculationWidget::buttonOk()
{
	//returnNum = 1;
	//close();
	sendData(1);
}

void CalculationWidget::buttonCheck()
{
	//returnNum = 1;
	//close();
	sendData(2);
}

void CalculationWidget::selectLineEditChange(const QString & text)
{
	bool ok;
	NumSel = text.toInt(&ok);
	if (ok)
	{
		delete[]selectCombobox;
		delete[]selectLabel;
		selectCombobox = new QComboBox[NumSel];
		selectLabel = new QLabel[NumSel];
		for (int i = 0; i < NumSel; i++)
		{
			selectLabel[i].setText(tr("Surface ") + QString::number(i + 1));
			selectCombobox[i].addItems(allSurfaceName);
			selectlayout->addWidget(&selectLabel[i], i, 0);
			selectlayout->addWidget(&selectCombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
		surfaceLineEdit->setText(QString::number(NumSel));
	}
}

void CalculationWidget::monitorNumLineEditChange(const QString & text)
{
	bool ok;
	NumMonitor = text.toInt(&ok);
	if (ok)
	{
		delete[]monitorCombobox;
		delete[]monitorLabel;
		monitorCombobox = new QComboBox[NumMonitor];
		monitorLabel = new QLabel[NumMonitor];
		for (int i = 0; i < NumMonitor; i++)
		{
			monitorLabel[i].setText(tr("Rectangle ") + QString::number(i + 1));
			monitorCombobox[i].addItems(allSurfaceName);
			monitorlayout->addWidget(&monitorLabel[i], i, 0);
			monitorlayout->addWidget(&monitorCombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
		monitorNumLineEdit->setText(QString::number(NumMonitor));
	}
}

void CalculationWidget::resultComboboxChange(int Num)
{
	if (Num == 0) 
	{
		distanceLineEdit->setEnabled(false);
		widthLineEdit->setEnabled(false);
		depthLineEdit->setEnabled(false);
	}
	else
	{
		distanceLineEdit->setEnabled(true);
		widthLineEdit->setEnabled(true);
		depthLineEdit->setEnabled(true);
	}
}

void CalculationWidget::addAllSurface(QStringList _allSurfaceName)
{
	//allSurfaceID = _allSurfaceID;
	allSurfaceName = _allSurfaceName;
	for (int i = 0; i < NumSel; i++)
	{
		selectCombobox[i].addItems(allSurfaceName);
	}
}

void CalculationWidget::addAllSources(QStringList _allSourcesName)
{
	sourceCombobox->addItems(_allSourcesName);
}

void CalculationWidget::getSelSurfaces(int &_NumSel, int* & index)
{
	_NumSel = NumSel;
	//_SelSurf.clear();
	index = new int[NumSel];
	for (int i = 0; i < NumSel; i++)
		index[i] = selectCombobox[i].currentIndex();
	//index = index;
}

int CalculationWidget::getSelSources() const
{
	return sourceCombobox->currentIndex();
}

void CalculationWidget::setResRectangle(const double &data1, const double &data2)
{
	distanceLineEdit->setText(tr("1"));
	widthLineEdit->setText(QString::number(data1));
	depthLineEdit->setText(QString::number(data2));
}

int CalculationWidget::getMeshSize() const
{
	return MeshCombobox->currentIndex();
}

int CalculationWidget::getCalMod() const
{
	return calModCombobox->currentIndex();
}

bool CalculationWidget::getResRectangle(bool & Isdefualt, double * &data)
{
	if (resultCombobox->currentIndex() == 0)
	{
		Isdefualt = true;
		return true;
	}
	else
	{
		Isdefualt = false;
		bool ok = false;
		bool ok1 = false;

		data[0] = distanceLineEdit->text().toDouble(&ok);
		data[1] = widthLineEdit->text().toDouble(&ok1);
		ok = ok & ok1;
		data[2] = depthLineEdit->text().toDouble(&ok1);
		ok = ok & ok1;
		return ok;
		//index = rectCombobox->currentIndex();

		//list<modleID>::const_iterator iter = allRectangleID.begin();
		//for (int j = 0; j < tempNumSel; j++)
		//	iter++;
		//_SelRect= *iter;		
	}
}

double CalculationWidget::getFre()
{
	return freLineEdit->text().toDouble();
}
