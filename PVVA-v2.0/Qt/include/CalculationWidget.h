#ifndef CALCULATIONWIDGET_H
#define CALCULATIONWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "Qt/include/GraphTransWidget.h"


class CalculationWidget : public QDialog
{
	Q_OBJECT

public:
	CalculationWidget(QWidget *parent = 0);
	~CalculationWidget();

	void addAllSurface(QStringList _allSurfaceName);
	void addAllSources(QStringList _allSourcesName);

	void getSelSurfaces(int & _NumSel, int *& index); // 得到需要计算反射的面
	int getSelSources() const; // 得到需要计算反射的面
	bool getResRectangle(bool & Isdefualt, double * &data);  // 得到保存结果的面
	void setResRectangle(const double &data1, const double &data2);
	int getMeshSize() const; //得到剖分密度信息
	int getCalMod() const; //得到计算模式信息

	double getFre();

signals:
	/*************************
	* 0: 表示close 
	* 1: 表示OK
	* 2: 表示Check
	* 3: 表示cancel
	**************************/
	void sendData(int);

private slots:
    void buttonOk();
	void buttonCheck();
    void buttonCancel();
	void buttonSave();

	void selectLineEditChange(const QString & text);
	void monitorNumLineEditChange(const QString & text);
	void resultComboboxChange(int Num);
private:

	//page1
	QTabWidget * tabWidget;

	QGroupBox * paraGroupBox;
	QLabel * freLabel;
	QLineEdit * freLineEdit;
	QLabel *  sourceLabel;
	QComboBox * sourceCombobox;
	QLabel *  calModLabel; // 计算模式
	QComboBox * calModCombobox;
	QLabel *  MeshLabel;
	QComboBox * MeshCombobox;

	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceLabel;
	QLineEdit * surfaceLineEdit;

	QGroupBox * selectGroupBox;
	QLabel *  selectLabel;
	QComboBox * selectCombobox;
	QGridLayout * selectlayout;

	QGroupBox * resultGroupBox;
	QLabel *  resultLabel;
	QComboBox * resultCombobox;
	QLabel *  distanceLabel;
	QLabel * widthLabel;
	QLabel * depthLaebl;
	QLineEdit * distanceLineEdit;
	QLineEdit * widthLineEdit;
	QLineEdit * depthLineEdit;

	QLabel * saveLabel;
	QLineEdit * saveLineEdit;
	QPushButton * saveBtn;

	//page2
	QGroupBox * monitorNumGroupBox;
	QLabel *  monitorNumLabel;
	QLineEdit * monitorNumLineEdit;

	QGroupBox * monitorGroupBox;
	QLabel *  monitorLabel;
	QComboBox * monitorCombobox;
	QGridLayout * monitorlayout;


	QPushButton * okBtn;
	QPushButton * checkBtn;
	QPushButton * cancelBtn;


	int NumSel; // 选择需要反射的面的数量
	int NumMonitor; // 加入监视面的数量

	QStringList allSurfaceName; // 所有面数据的代号

	QStringList allRectangleName; // 所有矩形面数据的代号

};

#endif // CALCULATIONWIDGET_H
