#include "Qt/include/GraphTransWidget.h"

GraphTransWidget::GraphTransWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	//setWindowTitle(tr("Create paraboloid"));

	
	//createWidget();
	this->wayButton = wayButton;
}

GraphTransWidget::~GraphTransWidget()
{

}

void GraphTransWidget::addNameLabel(QGridLayout * layout)
{
	// label
	label = new QLabel(tr("Label"));
	nameLineEidt = new QLineEdit;
	nameLineEidt->setText(tr("Paraboloid"));  // ´ýÐÞ¸Ä

	layout->addWidget(label, 0, 0);
	layout->addWidget(nameLineEidt, 0, 1);

}

void GraphTransWidget::addDefGroupBox(QGroupBox * _defGroupBox, QString filename)
{
	// page1
	//defGroupBox
	QLabel * imgLlabel;
	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap(filename));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(imgLlabel);

	//defGroupBox = new QGroupBox;
	_defGroupBox->setLayout(layout1);
	_defGroupBox->setTitle(tr("Definition methods"));
}

void GraphTransWidget::addBaseGroupBox(QGroupBox * _baseGroupBox)
{
	//baseGroupBox
	Ulabel = new QLabel(tr("U"));
	Vlabel = new QLabel(tr("V"));
	Nlabel = new QLabel(tr("N"));

	ULineEidt = new QLineEdit;
	VLineEidt = new QLineEdit;
	NLineEidt = new QLineEdit;

	ULineEidt->setText(tr("0.0"));
	VLineEidt->setText(tr("0.0"));
	NLineEidt->setText(tr("0.0"));

	connect(ULineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(VLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(NLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layout2 = new QGridLayout;
	layout2->addWidget(Ulabel, 0, 0);
	layout2->addWidget(Vlabel, 1, 0);
	layout2->addWidget(Nlabel, 2, 0);
	layout2->addWidget(ULineEidt, 0, 1);
	layout2->addWidget(VLineEidt, 1, 1);
	layout2->addWidget(NLineEidt, 2, 1);

	_baseGroupBox->setTitle(tr("Definition methods"));
	_baseGroupBox->setLayout(layout2);
}

void GraphTransWidget::addRotateWidget(QWidget * RotateWidget, QString filename)
{
	// page2
	//imgGroupBox
	imgLlabel1 = new QLabel;
	imgLlabel1->setPixmap(QPixmap(filename));

	QGridLayout * layout5 = new QGridLayout;
	layout5->addWidget(imgLlabel1);

	imgGroupBox = new QGroupBox;
	imgGroupBox->setLayout(layout5);
	imgGroupBox->setTitle(tr("Definition methods"));

	//RotateGroupBox
	xlabel = new QLabel(tr("X"));
	ylabel = new QLabel(tr("Y"));
	zlabel = new QLabel(tr("Z"));

	xRotateLineEidt = new QLineEdit;
	yRotateLineEidt = new QLineEdit;
	zRotateLineEidt = new QLineEdit;
	connect(xRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(yRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(zRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	xRotateLineEidt->setText(tr("1.0"));
	yRotateLineEidt->setText(tr("0.0"));
	zRotateLineEidt->setText(tr("0.0"));

	QGridLayout * layout6 = new QGridLayout;
	layout6->addWidget(xlabel, 0, 0);
	layout6->addWidget(ylabel, 1, 0);
	layout6->addWidget(zlabel, 2, 0);
	layout6->addWidget(xRotateLineEidt, 0, 1);
	layout6->addWidget(yRotateLineEidt, 1, 1);
	layout6->addWidget(zRotateLineEidt, 2, 1);

	RotateGroupBox = new QGroupBox;
	RotateGroupBox->setTitle(tr("Axis of rotation"));
	RotateGroupBox->setLayout(layout6);

	//thetaGroupBox
	thetalabel = new QLabel(tr("degree"));
	thetaLineEidt = new QLineEdit;
	thetaLineEidt->setText(tr("0.0"));

	connect(thetaLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layout7 = new QGridLayout;
	layout7->addWidget(thetalabel, 0, 0);
	layout7->addWidget(thetaLineEidt, 0, 1);

	thetaGroupBox = new QGroupBox;
	thetaGroupBox->setTitle(tr("Rotation angle"));
	thetaGroupBox->setLayout(layout7);

	QVBoxLayout * tabLayout2;
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addWidget(RotateGroupBox);
	tabLayout2->addWidget(thetaGroupBox);

	RotateWidget->setLayout(tabLayout2);
}

void GraphTransWidget::addBtn(QGridLayout * _layoutbt, int wayButton)
{
	this->wayButton = wayButton;
	if (wayButton == 1)
	{
		createbtn = new QPushButton(tr("Ok"));
		addbtn = new QPushButton(tr("Apply"));
		closebtn = new QPushButton(tr("Cancle"));

		connect(createbtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
		connect(addbtn, SIGNAL(clicked()), this, SLOT(buttonApply()));
		connect(closebtn, SIGNAL(clicked()), this, SLOT(buttonClose()));
	}
	else
	{
		createbtn = new QPushButton(tr("Create"));
		addbtn = new QPushButton(tr("Add"));
		closebtn = new QPushButton(tr("Close"));

		connect(createbtn, SIGNAL(clicked()), this, SLOT(buttonCreate()));
		connect(addbtn, SIGNAL(clicked()), this, SLOT(buttonAdd()));
		connect(closebtn, SIGNAL(clicked()), this, SLOT(buttonClose()));
	}

	_layoutbt->addWidget(createbtn, 0, 0);
	_layoutbt->addWidget(addbtn, 0, 1);
	_layoutbt->addWidget(closebtn, 0, 2);
}

void GraphTransWidget::buttonApply()
{
	emit sendData(6);
	
}

void GraphTransWidget::buttonOk()
{
	emit sendData(3);
}

void GraphTransWidget::on_GraphChange(QString t)
{
	bool ok = false;
	t.toFloat(&ok);
	if (!ok)
		return;
	emit sendData(5);
}

GraphTrans GraphTransWidget::getGraphTransData(bool &ok) const
{
	bool ok1, ok2, ok3, ok4, ok5, ok6, ok7;
	GraphTrans graphTransPara;
	graphTransPara.setGraphTransPar(ULineEidt->text().toFloat(&ok1), VLineEidt->text().toFloat(&ok2),
		NLineEidt->text().toFloat(&ok3), xRotateLineEidt->text().toFloat(&ok4), yRotateLineEidt->text().toFloat(&ok5),
		zRotateLineEidt->text().toFloat(&ok6), thetaLineEidt->text().toFloat(&ok7));
	ok = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7;
	return graphTransPara;	
}

void GraphTransWidget::setGraphTransData(const GraphTrans& graphTransPara)
{

	ULineEidt->setText(QString::number(graphTransPara.getTrans_x()));
	VLineEidt->setText(QString::number(graphTransPara.getTrans_y()));
	NLineEidt->setText(QString::number(graphTransPara.getTrans_z()));

	xRotateLineEidt->setText(QString::number(graphTransPara.getRotate_x()));
	yRotateLineEidt->setText(QString::number(graphTransPara.getRotate_y()));
	zRotateLineEidt->setText(QString::number(graphTransPara.getRotate_z()));

	thetaLineEidt->setText(QString::number(graphTransPara.getRotate_theta()));

}

QString GraphTransWidget::getGraphTransName() const
{
	return nameLineEidt->text();
}

void GraphTransWidget::setGraphTransName(QString name)
{
	nameLineEidt->setText(name);
}

void GraphTransWidget::setNameLineEdit(QString name)
{
	nameLineEidt->setText(name);
}

int GraphTransWidget::Num()
{
	return 0;
}

void GraphTransWidget::setAddBtn()
{
	addbtn->setText(tr("Apply"));
}

void GraphTransWidget::setNameHide(bool ok)
{
	if (ok)
	{
		label->hide();
		nameLineEidt->hide();
	}
	else
	{
		label->show();
		nameLineEidt->show();
	}
}

void GraphTransWidget::closeEvent(QCloseEvent * event)
{
	if (wayButton == 1)
		emit sendData(4);
	else
		emit sendData(0);
}

void GraphTransWidget::buttonAdd()
{
	emit sendData(2);	
}

void GraphTransWidget::buttonCreate()
{
	//returnNum = 1;
	emit sendData(1);
	//close();
}

void GraphTransWidget::buttonClose()
{
	if(wayButton ==1)
		emit sendData(4);
	else
	    emit sendData(0);
	//returnNum = 0;
	//close();
}