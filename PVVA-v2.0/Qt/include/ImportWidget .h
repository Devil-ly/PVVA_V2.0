#ifndef ImportWidget_H
#define ImportWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <VTK/include/ModleData.h>

class ImportWidget : public QDialog
{
	Q_OBJECT

public:
	ImportWidget(QWidget *parent = 0, int wayButton = 0);
	~ImportWidget();
	int Num();
	bool getWidgetData(ModleData & _modleData) const;
private slots:
    void buttonOk();
    void buttonCancel();
	void buttonOpen();
private:

	//page1
	QLabel *numLabel;
	QLineEdit *numLineEdit;

	QLineEdit *fileLineEdit;
	QPushButton *fileBtn;

	QPushButton *okBtn;
	QPushButton *cancelBtn;
	int returnNum;

};

#endif // ImportWidget_H
