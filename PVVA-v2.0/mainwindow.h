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

	//************��������*****************
	void createActions();
	void createMenus(); // �˵�
	void createToolBars();
	void createStatusBar();
	void createDetails();
	void creareWindows(); // �ȴ����ô��� ������û�н��delete������ ��������������⣩


	void createTreeWidgetItem(); // ����tree
	//void updateTreeWidgetItem(); // ������ԭ����tree �ڴ���һ���µ�tree

	void createRightMenu(); // �Ҽ��˵�

	// �������
	void addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, int name, int index, int rightPar = 0);

	//************ vtk ********************
	void updateVtk(); // ˢ��vtk��ʾ
	void clearCheckActor();
 
private slots:

	void on_treeWidget_ContextMenuRequested(QPoint pos);// �Ҽ��˵�
	void on_treeWidget_leftPressed(QTreeWidgetItem *item, int column);

	void on_Details_FieldClicked(); //details Field 

	void showDetails();

	void createRectangle(); // ����
	void Rectangle(int index);

	void createParaboloid();    // ������
	void Paraboloid(int index);

	void createArbitrarySurface(); // ������

	void createCylinder();   // Բ��
	void Cylinder(int index);

	void createSphere();   //  ��
	void Sphere(int index);

	void createBoolean();
	void Boolean(int index);
	
	void createSTLInput();  // stl
	void STLInput(int index);

	void createGaussian();      // ��˹
	void Gaussian(int index);

	void createCalculation();
	void ToCalculation(int index);

	void createApertureField(); // ���泡
	void ApertureField(int index);

	void createLightSource();  // ����׷��
	void ToLightSource(int index);

	void SetGeometryPara();  //�޸Ĳ���
	void DeleteGeometry(); // ɾ��ģ��
	void Field2light(); // ͨ�������ɹ�Դ
	void SaveField(); // ����Field
	void SetTransparency(); // ���ó�͸��
	
	void showDetails(int name, int index);
	void createImportAction();
	void changeUnit();  // �޸ĵ�λ
	void updateProgressDialog();

	void Gaussian_Thread();

	void receiveField(Field *field); // ���ռ����Ľ��

private:
	//**********VTK**********
	QVTKWidget widget; // vtk ��ʾ����
	vtkSmartPointer<vtkRenderWindowInteractor> interactor; // ����
	vtkSmartPointer<vtkRenderer> renderer; 
	vtkSmartPointer<vtkAxesActor> axes; // ����
	list<vtkActor*> actors; // ����������Ա

	QDockWidget * leftWidget; //����treeWidget�Ĵ���
	QTreeWidget * treeWidget;

	QDockWidget * detailsDockWidget;
	QWidget * detailsWidget;

	QLabel * locationLabel;  // ״̬����ǩ
	QPushButton * unitBtn;   // ��λ��ť

	//******TreeWidgetItem***
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * lightTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // �Ҽ�ѡ�еĽڵ�

	//QVariant ItemVar1, ItemVar2;   //�ڵ�����

	//**********Menu**********
	QMenu * fileMenu;  //�˵���

	//�Ҽ��˵�
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//**********ToolBar*******
	QToolBar * fileTool;   //������
	QToolBar * constructTool;  //ģ����

	//**********Action********
	//�ļ��˵���
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	QAction * viewAction;  // �ӽ�
	QLabel * viewLabel;
	QComboBox * viewComboBox;
	QAction * detailsAction;

	//ģ��
	QAction * rectangleAction; //����
	QAction * polygonAction; // ����͹�����
	QAction * paraboloidAction; // ������

	QAction * cylinderAction; // Բ����
	QAction * sphereAction; // ��

	QAction * booleanAction; // bool

	QAction * STLInputAction; // STL����
	QAction * ImportAction;

	//�Ҽ��˵�
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;
	QAction * SaveFieldAction;
	QAction * Field2lightAction; // �����ɹ�Դ
	QAction * transparencyAction;

	//Դ
	QAction * LightSourceAction;  //����׷��Դ
	QAction * ApertureFieldAction;//���泡Դ
	QAction * GaussianAction;     //��˹��Դ

	//����
	QAction * CalculationAction;     //����

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

	//******��¼����ģ�͵ĸ���********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int cylinderNum;
	int STLInputNum;
	int sphereNum;
	int FieldNum;
	int ArbitrarySurfaceNum;
	int ImportNum;
	int isSet; // �Ƿ����޸� 0Ϊ���� 1Ϊ��

	ModleDataList modleDataList; // ����ģ������

	//******  Դ  ********
	Source * source;

	//list<Field> fieldList;
	unordered_map<int, Field> fieldHash;
	unordered_map<int, RaySource*> raySourceHash;
	//Field * field;
	double Unit_factor;

	//******���ִ��ڵ�ָ��********
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

	vtkSmartPointer<vtkActor> tempActor; //���ڱ��潨ģʱ����ʱactor
	ModleData *tempModleData; //���ڱ��潨ģʱ����ʱModleData
	vtkSmartPointer<vtkImageActor> tempImageActor;
	Field *tempField;
	Source * tempSource;
	RaySource * tempRaySource;

	stack<vtkActor*> stackActor;
	stack<vtkActor*> stackCheckActor; // ���ڱ���checkʱ��actor

	QProgressDialog* progDlg;
	int currentValue;
	QTimer *timer;
	//QThread* thread;
	WaitingDialog *waitingDlg;
	int tempIndex;

	// ����ʱ������
	queue<QString> calName;
};

#endif // MAINWINDOW_H
