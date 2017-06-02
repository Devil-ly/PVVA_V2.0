#include "Qt\include\BooleanWidget.h"

BooleanWidget::BooleanWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	setWindowTitle(tr("Boolean Operation"));

	Label1 = new QLabel(tr("Model1 "));
	Label2 = new QLabel(tr("Model2 "));
	LabelOp = new QLabel(tr("Operation "));

	selectCombobox1 = new QComboBox;
    selectCombobox2 = new QComboBox;

	connect(selectCombobox1, SIGNAL(activated(int)),
		this, SLOT(selectComboboxChange(int)));
	connect(selectCombobox2, SIGNAL(activated(int)),
		this, SLOT(selectComboboxChange(int)));

	opCombobox = new QComboBox;
	opCombobox->addItem(tr("Union"));
	opCombobox->addItem(tr("Intersection"));
	opCombobox->addItem(tr("Difference"));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(Label1, 0, 0);
	layout->addWidget(Label2, 1, 0);
	layout->addWidget(LabelOp, 2, 0);

	layout->addWidget(selectCombobox1, 0, 1);
	layout->addWidget(selectCombobox2, 1, 1);
	layout->addWidget(opCombobox, 2, 1);

	defGroupBox = new QGroupBox;
	defGroupBox->setTitle(tr("Define"));
	defGroupBox->setLayout(layout);

	btnOk = new QPushButton(tr("OK"));
	btnApply = new QPushButton(tr("Apply"));
	btnClose = new QPushButton(tr("Close"));

	connect(btnOk, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(btnApply, SIGNAL(clicked()), this, SLOT(buttonApply()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(buttonClose()));

	QGridLayout *layoutBtn = new QGridLayout;
	layoutBtn->addWidget(btnOk, 0, 0);
	layoutBtn->addWidget(btnApply, 0, 1);
	layoutBtn->addWidget(btnClose, 0, 2);

	QVBoxLayout *layoutGB = new QVBoxLayout(this);
	layoutGB->addWidget(defGroupBox);
	layoutGB->addLayout(layoutBtn);
}

BooleanWidget::~BooleanWidget()
{
}

void BooleanWidget::addAllSurface(QStringList _allSurfaceName)
{
	//allSurfaceID = _allSurfaceID;
	allSurfaceName = _allSurfaceName;
	selectCombobox1->addItems(allSurfaceName);
	selectCombobox2->addItems(allSurfaceName);
}

int BooleanWidget::getModel1()
{
	return selectCombobox1->currentIndex();
}

int BooleanWidget::getModel2()
{
	return selectCombobox2->currentIndex();
}

int BooleanWidget::getOperation()
{
	return opCombobox->currentIndex();;
}

void BooleanWidget::buttonApply()
{
	emit sendData(5);

}

void BooleanWidget::buttonOk()
{
	emit sendData(3);
}

void BooleanWidget::buttonClose()
{
	emit sendData(0);

}

void BooleanWidget::closeEvent(QCloseEvent * event)
{
	emit sendData(0);
}

void BooleanWidget::selectComboboxChange(int x)
{
	sendData(5);
}