#ifndef BOOLEANWIDGET_H
#define BOOLEANWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>  
#include <QComboBox>

#include "Tools/GraphTrans.h"

class BooleanWidget : public QDialog
{
	Q_OBJECT

public:
	BooleanWidget(QWidget *parent = 0, int wayButton = 0);
	~BooleanWidget();

	void addAllSurface(QStringList _allSurfaceName);

	int getModel1();
	int getModel2();
	int getOperation();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void buttonOk();
    void buttonClose();
    void buttonApply();
	void selectComboboxChange(int);

signals:
	
	void sendData(int);

private:
	QLabel * Label1;
	QLabel * Label2;
	QComboBox * selectCombobox1;
	QComboBox * selectCombobox2;

	QLabel * LabelOp;
	QComboBox * opCombobox;

	QGroupBox * defGroupBox;

	QPushButton *btnOk;
	QPushButton *btnApply;
	QPushButton *btnClose;

	QStringList allSurfaceName; // 所有面数据的代号
	
};

#endif // BOOLEANWIDGET_H