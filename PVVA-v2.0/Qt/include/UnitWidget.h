#ifndef UNITWIDGET_H
#define UNITWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>


class UnitWidget : public QDialog
{
	Q_OBJECT

public:
	UnitWidget(QWidget *parent = 0, int wayButton = 0);
	~UnitWidget();

	double getUnitFactor();
	int Num();

private slots:
    void buttonOk();
    void buttonCancel();
private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * unitGroupBox;

	QRadioButton * mmRadioButton;
	QRadioButton * cmRadioButton;
	QRadioButton * mRadioButton;

	QPushButton * okBtn;
	QPushButton * cancelBtn;

	int returnNum;

};

#endif // UNITWIDGET_H
