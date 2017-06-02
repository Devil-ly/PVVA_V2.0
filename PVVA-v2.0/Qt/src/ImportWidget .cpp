#include "..\include\ImportWidget .h"
#include <QFileDialog>

ImportWidget::ImportWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	setWindowTitle(tr("Import Custom triangle surface source"));
	numLabel = new QLabel(tr("Num of Triangle"));
	numLineEdit = new QLineEdit;
	fileLineEdit = new QLineEdit;
	fileBtn = new QPushButton(tr("Open"));

	okBtn = new QPushButton(tr("OK"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(fileBtn, SIGNAL(clicked()), this, SLOT(buttonOpen()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(numLabel, 0, 0);
	layout->addWidget(numLineEdit, 0, 1);
	layout->addWidget(fileLineEdit, 1, 0);
	layout->addWidget(fileBtn, 1, 1);
	layout->addWidget(okBtn, 2, 0);
	layout->addWidget(cancelBtn, 2, 1);
	returnNum = 0;
}

ImportWidget::~ImportWidget()
{
}

int ImportWidget::Num()
{
	return returnNum;
}

void ImportWidget::buttonCancel()
{
	returnNum = 0;
	close();
}

void ImportWidget::buttonOpen()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the STL"),
		"",
		tr("*.txt"));
	if (!filename.isEmpty())
	{
		fileLineEdit->setText(filename);
	}
}

void ImportWidget::buttonOk()
{
	returnNum = 1;
	close();
}

bool ImportWidget::getWidgetData(ModleData & _modleData) const
{
	bool ok;
	float * para = new float[8];
	para[7] = numLineEdit->text().toInt(&ok);

	_modleData.setData(para);
	_modleData.setNameFile(fileLineEdit->text().toStdString());

	return ok;
}