#ifndef LIGHTSOURCEWIDGET_H
#define LIGHTSOURCEWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <list>

#include "Qt/include/GraphTransWidget.h"
#include "VTK/include/RaySource.h"
#include "VTK/include/ModleData.h"

using namespace std;

class LightSourceWidget : public GraphTransWidget
{
	Q_OBJECT
public:
	LightSourceWidget(QWidget *parent = 0, int wayButton = 0);
	~LightSourceWidget();

	//set and get Data
	//void setWidgetData(const LightSource  &_LightSource);
	bool getWidgetData(RaySource & _LightSource);

	void addAllSurface(QStringList _allSurfaceName);
	void addAllSources(QStringList _allSourcesName);
	int getSelSources() const;

	float getLength() const;

	//set and get 选择面的数据
	void getSelSurfaces(int & _NumSel, int *& index);
	void setSelSurfaces(int _NumSel);
	bool getRectData(ModleData & _modleData) const;

	int getType() const;
	QString getName() const;

	void init();

private slots:
    void comboChange(int index);
	void selectLineEidtChange(const QString & text);
	void tabWidgetChange(int index);
	void Page3Change(QString); // 3页的内容改变

private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;

	QComboBox * combobox;

	QLabel * imgLlabel;

	QLabel * radiuslabel;
	QLabel * depthlabel;

	QLineEdit * widthLineEidt;
	QLineEdit * depthLineEidt;

	QGridLayout * layoutName;
	QWidget * widget1;
	QWidget * widget2;

	QGroupBox * sourceGroupBox;// 只有读入已存在的源才用
	QComboBox * sourceCombobox; 

	//page3
	QGroupBox * img2GroupBox;
	QGroupBox * LightNumGroupBox;
	QGroupBox * lengthGroupBox;

	QLabel * imgLlabel2;

	QLabel * var1label;
	QLabel * var2label;

	QLineEdit * var1LineEidt;
	QLineEdit * var2LineEidt;

	QLabel * lengthlabel;
	QLineEdit * lengthLineEidt;
	QWidget * widget3;

	//page4
	QGroupBox * img3GroupBox;
	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceNumlabel;

	QGroupBox * selectGroupBox;
	QGridLayout * selectlayout;

	QLabel *  selectlabel;
	QLineEdit * selectLineEidt;
	QComboBox * selectcombobox;

	QVBoxLayout * selectTabLayout;
	QWidget * selectWidget;
	//QComboBox * combobox;

	QStringList allSurfaceName; // 所有面数据的代号

	int NumSel;
};

#endif // LightSourceWidget_H
