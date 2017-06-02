#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QComBobox>
#include <QstatusBar>
#include <QPushButton>
#include <QRadioButton>
#include <list>
#include <QProgressDialog>
#include <queue>

#include <vtkActor.h>
#include <vtkImageActor.h>
//#include <vtkActorCollection.h>

#include "VTK/include/ModleData.h"
#include "VTK/include/ModleDataList.h"
#include "VTK/include/Source.h"
#include "VTK/include/Field.h"
#include "VTK/include/RaySource.h"

#include "Qt/include/ParaboloidWidget.h"
#include "Qt/include/CylinderWidget.h"
#include "Qt/include/BooleanWidget.h"
#include "Qt/include/RectangleWidget.h"
#include "Qt/include/stlInputWidget.h"
#include "Qt/include/SphereWidget.h"
#include "Qt/include/GaussianWidget.h"
#include "Qt/include/WaitingDialog.h"
#include "Qt/include/CalculationWidget.h"
#include "Qt/include/ApertureFieldWidget.h"
#include "Qt/include/LightSourceWidget.h"
#include "Qt/include/ArbitrarySurfaceWidget.h"

#include <stack>
#include <unordered_map>
//#include "ui_mainwindow.h"

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = 0);
	~mainWindow();

private:
	void init();

	//************窗口设置*****************
	void createActions();
	void createMenus(); // 菜单
	void createToolBars();
	void createStatusBar();
	void createDetails();
	void creareWindows(); // 先创建好窗口 （由于没有解决delete的问题 采用这个方法避免）


	void createTreeWidgetItem(); // 创建tree
	//void updateTreeWidgetItem(); // 先销毁原来的tree 在创建一个新的tree

	void createRightMenu(); // 右键菜单

	// 添加子树
	void addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, int name, int index, int rightPar = 0);

	//************ vtk ********************
	void updateVtk(); // 刷新vtk显示
	void clearCheckActor();
 
private slots:

	void on_treeWidget_ContextMenuRequested(QPoint pos);// 右键菜单
	void on_treeWidget_leftPressed(QTreeWidgetItem *item, int column);

	void on_Details_FieldClicked(); //details Field 

	void showDetails();

	void createRectangle(); // 矩形
	void Rectangle(int index);

	void createParaboloid();    // 抛物面
	void Paraboloid(int index);

	void createArbitrarySurface(); // 任意面

	void createCylinder();   // 圆柱
	void Cylinder(int index);

	void createSphere();   //  球
	void Sphere(int index);

	void createBoolean();
	void Boolean(int index);
	
	void createSTLInput();  // stl
	void STLInput(int index);

	void createGaussian();      // 高斯
	void Gaussian(int index);

	void createCalculation();
	void ToCalculation(int index);

	void createApertureField(); // 口面场
	void ApertureField(int index);

	void createLightSource();  // 光线追踪
	void ToLightSource(int index);

	void SetGeometryPara();  //修改参数
	void DeleteGeometry(); // 删除模型
	void Field2light(); // 通过场生成光源
	void SaveField(); // 保存Field
	void SetTransparency(); // 设置成透明
	
	void showDetails(int name, int index);
	void createImportAction();
	void changeUnit();  // 修改单位
	void updateProgressDialog();

	void Gaussian_Thread();

	void receiveField(Field *field); // 接收计算后的结果

private:
	//**********VTK**********
	QVTKWidget widget; // vtk 显示窗口
	vtkSmartPointer<vtkRenderWindowInteractor> interactor; // 交互
	vtkSmartPointer<vtkRenderer> renderer; 
	vtkSmartPointer<vtkAxesActor> axes; // 坐标
	list<vtkActor*> actors; // 保存所有演员

	QDockWidget * leftWidget; //放置treeWidget的窗口
	QTreeWidget * treeWidget;

	QDockWidget * detailsDockWidget;
	QWidget * detailsWidget;

	QLabel * locationLabel;  // 状态栏标签
	QPushButton * unitBtn;   // 单位按钮

	//******TreeWidgetItem***
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * lightTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // 右键选中的节点

	//QVariant ItemVar1, ItemVar2;   //节点属性

	//**********Menu**********
	QMenu * fileMenu;  //菜单栏

	//右键菜单
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//**********ToolBar*******
	QToolBar * fileTool;   //工具栏
	QToolBar * constructTool;  //模型栏

	//**********Action********
	//文件菜单项
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	QAction * viewAction;  // 视角
	QLabel * viewLabel;
	QComboBox * viewComboBox;
	QAction * detailsAction;

	//模型
	QAction * rectangleAction; //矩形
	QAction * polygonAction; // 任意凸多边形
	QAction * paraboloidAction; // 抛物面

	QAction * cylinderAction; // 圆柱体
	QAction * sphereAction; // 球

	QAction * booleanAction; // bool

	QAction * STLInputAction; // STL输入
	QAction * ImportAction;

	//右键菜单
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;
	QAction * SaveFieldAction;
	QAction * Field2lightAction; // 场生成光源
	QAction * transparencyAction;

	//源
	QAction * LightSourceAction;  //光线追踪源
	QAction * ApertureFieldAction;//口面场源
	QAction * GaussianAction;     //高斯场源

	//仿真
	QAction * CalculationAction;     //计算

	//****** Details********
	QButtonGroup * dimensionGroupBtn;
	QRadioButton * ThreeDBtn;
	QRadioButton * TwoDBtn;
	QButtonGroup * fieldGroupBtn;
	QRadioButton * ExBtn;
	QRadioButton * EyBtn;
	QRadioButton * EzBtn;
	QRadioButton * HxBtn;
	QRadioButton * HyBtn;
	QRadioButton * HzBtn;
	QButtonGroup * pmGroupBtn;
	QRadioButton * magnitudeBtn;
	QRadioButton * phaseBtn;
	QButtonGroup * powerGroupBtn;
	QRadioButton * linearBtn;
	QRadioButton * dbBtn;

	//******记录各个模型的个数********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int cylinderNum;
	int STLInputNum;
	int sphereNum;
	int FieldNum;
	int ArbitrarySurfaceNum;
	int ImportNum;
	int isSet; // 是否是修改 0为不是 1为是

	ModleDataList modleDataList; // 所有模型数据

	//******  源  ********
	Source * source;

	//list<Field> fieldList;
	unordered_map<int, Field> fieldHash;
	unordered_map<int, RaySource*> raySourceHash;
	//Field * field;
	double Unit_factor;

	//******各种窗口的指针********
	ParaboloidWidget * paraboloidDialog;
	CylinderWidget * cylinderDialog;
	BooleanWidget * booleanDialog;
	RectangleWidget * rectangleDialog;
	STLInputWidget * STLInputDialog;
	SphereWidget * sphereDialog;
	CalculationWidget * calculationDialog;
	GaussianWidget * GaussianDialog;
	ApertureFieldWidget *ApertureFieldDialog;
	LightSourceWidget * LightSourceDialog;
	ArbitrarySurfaceWidget * ArbitrarySurfaceDialog;

	vtkSmartPointer<vtkActor> tempActor; //用于保存建模时的临时actor
	ModleData *tempModleData; //用于保存建模时的临时ModleData
	vtkSmartPointer<vtkImageActor> tempImageActor;
	Field *tempField;
	Source * tempSource;
	RaySource * tempRaySource;

	stack<vtkActor*> stackActor;
	stack<vtkActor*> stackCheckActor; // 用于保存check时的actor

	QProgressDialog* progDlg;
	int currentValue;
	QTimer *timer;
	//QThread* thread;
	WaitingDialog *waitingDlg;
	int tempIndex;

	// 计算时的名字
	queue<QString> calName;
};

#endif // MAINWINDOW_H
