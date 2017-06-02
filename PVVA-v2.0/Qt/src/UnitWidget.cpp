#include "..\include\UnitWidget.h"

UnitWidget::UnitWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	setWindowTitle(tr("Modle unit"));

	mmRadioButton = new QRadioButton(tr("millimeter (mm)"));
	cmRadioButton = new QRadioButton(tr("centimeter (cm)"));
	mRadioButton = new QRadioButton(tr("meter (m)"));

	switch (wayButton)
	{
	case 0:
		mmRadioButton->setChecked(true);
		break;
	case 1:
		cmRadioButton->setChecked(true);
		break;
	case 2:
		mRadioButton->setChecked(true);
		break;
	default:
		break;
	}

	okBtn = new QPushButton(tr("OK"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));

	QGridLayout * layoutbtn = new QGridLayout;
	layoutbtn->addWidget(okBtn, 0, 0);
	layoutbtn->addWidget(cancelBtn, 0, 1);

	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(mmRadioButton);
	layout->addWidget(cmRadioButton);
	layout->addWidget(mRadioButton);
	layout->addSpacing(1);
	layout->addLayout(layoutbtn);

	unitGroupBox = new QGroupBox;
	unitGroupBox->setLayout(layout);
	unitGroupBox->setTitle(tr("Unit"));	

	QGridLayout * layoutgb = new QGridLayout(this);
	layoutgb->addWidget(unitGroupBox);

	returnNum = -1;
}

UnitWidget::~UnitWidget()
{
}

double UnitWidget::getUnitFactor()
{
	if (mmRadioButton->isChecked())
	{
		returnNum = 1;
		return 1.0 / 1000;
	}

	else if (cmRadioButton->isChecked())
	{
		returnNum = 2;
		return 1.0 / 100;
	}
	else if (mRadioButton->isChecked())
	{
		returnNum = 3;
		return 1.0;
	}
}

int UnitWidget::Num()
{
	return returnNum;
}

void UnitWidget::buttonCancel()
{
	close();
}

void UnitWidget::buttonOk()
{
	returnNum = 1;
	close();
}