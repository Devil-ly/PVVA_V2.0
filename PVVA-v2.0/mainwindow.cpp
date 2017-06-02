#include "mainwindow.h"
#include <QApplication>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <QMessageBox.h>
#include <QButtonGroup>
#include <QGroupBox>
#include <vector>
#include <QTimer>
#include <QProgressDialog>
#include <QThread>
#include <QFileDialog>

#include "Qt/include/UnitWidget.h"

#include "Calculation/include/PVVA.h"
#include "qt/include/FieldThread.h"
#include "Tools/Vector3.h"
#include "qt/include/CalculateThread.h"
#include "qt/include/ImportWidget .h"


mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	//setWindowTitle(QString::fromLocal8Bit("����΢������ʵ����"));
	
	setWindowTitle(tr("PVVA-2.0")); // �������봰��
	setCentralWidget(&widget);
	resize(1200, 800);

	// ��ʼ��vtk����
	axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);//�޸�X������ɫΪ��ɫ  
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);//�޸�Y������ɫΪ��ɫ  
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);//�޸�Z������ɫΪ��ɫ  
	axes->SetConeRadius(0.1);
	axes->SetConeResolution(20);
	//axes->SetTotalLength(15, 15, 15); �޸�����ߴ�
    //axes->SetXAxisLabelText("X/U");
	//axes->SetYAxisLabelText("Y/V");
	//axes->SetZAxisLabelText("Z/N");
	
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);
	renderer->AddActor(axes);

	auto window = widget.GetRenderWindow();

	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(window);

	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);

	interactor->Initialize();
	window->AddRenderer(renderer);
	window->Render();

	// ����dock����
	leftWidget = new QDockWidget("Navigation", this);
	leftWidget->setFeatures(QDockWidget::DockWidgetMovable);
	leftWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	// treeWidget
	treeWidget = new QTreeWidget;
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
		this, SLOT(on_treeWidget_ContextMenuRequested(QPoint)));
	connect(treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
		this, SLOT(on_treeWidget_leftPressed(QTreeWidgetItem*, int)));
	treeWidget->setHeaderHidden(true);  // ���ر�ͷ
	leftWidget->setWidget(treeWidget);

	//RightWidget
	detailsDockWidget = new QDockWidget("Details", this);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	detailsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, detailsDockWidget);

	//treeWidget->setStyleSheet("QTreeView::branch {image:none;}"); 
	//QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(treeWidget, QStringList(QString("Gaussian_Thread")));
	//QTreeWidgetItem *imageItem2 = new QTreeWidgetItem(imageItem1, QStringList(QString("test2")));

	createTreeWidgetItem();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createRightMenu();
	createDetails();
	creareWindows();
	init();
	
}

mainWindow::~mainWindow()
{

}

void mainWindow::init()
{
	//glMainWidget->init();

	paraboloidNum = 0;
	LightSourceNum = 0;
	rectangleNum = 0;
	cylinderNum = 0;
	STLInputNum = 0;
	sphereNum = 0;
	FieldNum = 0;
	ArbitrarySurfaceNum = 0;
	ImportNum = 0;

	isSet = 0; // �����޸�

	//updateTreeWidgetItem();
	detailsWidget = NULL;
	leftSelectItem = NULL;
	tempActor = NULL;
	tempModleData = NULL;
	source = NULL;
	tempField = NULL;
	tempSource = NULL;
	tempRaySource = NULL;

	Unit_factor = 1;
}

void mainWindow::createActions()
{
	//**************�˵�*****************
	//����
	saveFileAction = new QAction(QIcon(tr("Qt/images/save.png")), tr("Save"), this);
	saveFileAction->setShortcut(tr("Ctrl +��s"));
	saveFileAction->setStatusTip(tr("Save a file"));
	connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	//��
	openFileAction = new QAction(QIcon(tr("Qt/images/open.png")), tr("Open"), this);
	openFileAction->setShortcut(tr("Ctrl +��O"));
	openFileAction->setStatusTip(tr("Open a file"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

	//�½�
	newFileAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	newFileAction->setShortcut(tr("Ctrl +��N"));
	newFileAction->setStatusTip(tr("New a file"));
	connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

	//Details
	detailsAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	detailsAction->setStatusTip(tr("Show Details"));
	connect(detailsAction, SIGNAL(triggered()), this, SLOT(showDetails()));

	//��ʼ���ӽ�
	viewAction = new QAction(QIcon(tr("Qt/images/view.png")), tr("View"), this);
	viewAction->setStatusTip(tr("Initialize the view"));
	connect(viewAction, SIGNAL(triggered()), this, SLOT(viewInitFile()));

	//�޸��ӽ�
	viewLabel = new QLabel(tr("View"));
	viewComboBox = new QComboBox;
	viewComboBox->addItem("View YZ plane(original)");
	viewComboBox->addItem("View XZ plane");
	viewComboBox->addItem("View XY plane");
	viewComboBox->addItem("View -YZ plane");
	viewComboBox->addItem("View -XZ plane");
	viewComboBox->addItem("View -XY plane");
	connect(viewComboBox, SIGNAL(activated(int)), this, SLOT(setView(int)));

	//**************ģ��*****************
	//����
	rectangleAction = new QAction(QIcon(tr("Qt/images/rectangle.png")), tr("Rectangle"), this);
	rectangleAction->setStatusTip(tr("Create a rectangle"));
	connect(rectangleAction, SIGNAL(triggered()), this, SLOT(createRectangle()));

	//������
	polygonAction = new QAction(QIcon(tr("Qt/images/polygon.png")), tr("Polygon"), this);
	polygonAction->setStatusTip(tr("Create a polygon"));
	connect(polygonAction, SIGNAL(triggered()), this, SLOT(createArbitrarySurface()));

	//������
	paraboloidAction = new QAction(QIcon(tr("Qt/images/paraboloid.png")), tr("Paraboloid"), this);
	paraboloidAction->setStatusTip(tr("Create a paraboloid"));
	connect(paraboloidAction, SIGNAL(triggered()), this, SLOT(createParaboloid()));

	//Բ����
	cylinderAction = new QAction(QIcon(tr("Qt/images/cylinder.png")), tr("Cylinder"), this);
	cylinderAction->setStatusTip(tr("Create a cylinder"));
	connect(cylinderAction, SIGNAL(triggered()), this, SLOT(createCylinder()));

	//��
	sphereAction = new QAction(QIcon(tr("Qt/images/sphere.png")), tr("Sphere"), this);
	sphereAction->setStatusTip(tr("Create a sphere"));
	connect(sphereAction, SIGNAL(triggered()), this, SLOT(createSphere()));

	//��������
	booleanAction = new QAction(QIcon(tr("Qt/images/boolean.png")), tr("Boolean"), this);
	booleanAction->setStatusTip(tr("Boolean operation"));
	connect(booleanAction, SIGNAL(triggered()), this, SLOT(createBoolean()));

	//STL
	STLInputAction = new QAction(QIcon(tr("Qt/images/stlInput.png")), tr("STL"), this);
	STLInputAction->setStatusTip(tr("STL Input"));
	connect(STLInputAction, SIGNAL(triggered()), this, SLOT(createSTLInput()));

	//����׷��Դ
	LightSourceAction = new QAction(QIcon(tr("Qt/images/LightSource.png")), tr("LightSource"), this);
	LightSourceAction->setStatusTip(tr("Create a LightSource"));
	connect(LightSourceAction, SIGNAL(triggered()), this, SLOT(createLightSource()));

	//���泡Դ
	ApertureFieldAction = new QAction(QIcon(tr("Qt/images/ApertureField.png")), tr("Aperture-Field"), this);
	ApertureFieldAction->setStatusTip(tr("Create a Aperture-Field source"));
	connect(ApertureFieldAction, SIGNAL(triggered()), this, SLOT(createApertureField()));

	//��˹Դ
	GaussianAction = new QAction(QIcon(tr("Qt/images/Gaussian.png")), tr("Gaussian"), this);
	GaussianAction->setStatusTip(tr("Create a Gaussian source"));
	connect(GaussianAction, SIGNAL(triggered()), this, SLOT(createGaussian()));

	//����
	CalculationAction = new QAction(QIcon(tr("Qt/images/calculation.png")), tr("Calculate"), this);
	CalculationAction->setStatusTip(tr("Calculate"));
	connect(CalculationAction, SIGNAL(triggered()), this, SLOT(createCalculation()));


	ImportAction = new QAction(QIcon(tr("Qt/images/import.png")), tr("Calculate"), this);
	ImportAction->setStatusTip(tr("Import"));
	connect(ImportAction, SIGNAL(triggered()), this, SLOT(createImportAction()));

}

void mainWindow::createStatusBar()
{
	locationLabel = new QLabel("    ");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());
	this->statusBar()->addWidget(locationLabel, 90);

	unitBtn = new QPushButton(tr("Unit: m"));
	connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	//connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	this->statusBar()->addWidget(unitBtn);
}

void mainWindow::createDetails()
{
	dimensionGroupBtn = new QButtonGroup();
	ThreeDBtn = new QRadioButton(tr("3D"));
	TwoDBtn = new QRadioButton(tr("2D"));
	dimensionGroupBtn->addButton(ThreeDBtn, 0);
	dimensionGroupBtn->addButton(TwoDBtn, 1);
	connect(ThreeDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(TwoDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * dimensionLayout = new QGridLayout;
	dimensionLayout->addWidget(ThreeDBtn, 0, 0);
	dimensionLayout->addWidget(TwoDBtn, 0, 1);

	QGroupBox * dimensionGroupBox = new QGroupBox;
	dimensionGroupBox->setTitle(tr("dimension"));
	dimensionGroupBox->setLayout(dimensionLayout);

	fieldGroupBtn = new QButtonGroup();
	ExBtn = new QRadioButton(tr("Ex"));
	EyBtn = new QRadioButton(tr("Ey"));
	EzBtn = new QRadioButton(tr("Ez"));
	HxBtn = new QRadioButton(tr("Hx"));
	HyBtn = new QRadioButton(tr("Hy"));
	HzBtn = new QRadioButton(tr("Hz"));

	fieldGroupBtn->addButton(ExBtn, 0);
	fieldGroupBtn->addButton(EyBtn, 1);
	fieldGroupBtn->addButton(EzBtn, 2);
	fieldGroupBtn->addButton(HxBtn, 3);
	fieldGroupBtn->addButton(HyBtn, 4);
	fieldGroupBtn->addButton(HzBtn, 5);

	connect(ExBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HxBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * fieldLayout = new QGridLayout;
	fieldLayout->addWidget(ExBtn, 0, 0);
	fieldLayout->addWidget(EyBtn, 1, 0);
	fieldLayout->addWidget(EzBtn, 2, 0);
	fieldLayout->addWidget(HxBtn, 0, 1);
	fieldLayout->addWidget(HyBtn, 1, 1);
	fieldLayout->addWidget(HzBtn, 2, 1);

	QGroupBox * fieldGroupBox = new QGroupBox;
	fieldGroupBox->setTitle(tr("field"));
	fieldGroupBox->setLayout(fieldLayout);

	// pmGroupBox
	pmGroupBtn = new QButtonGroup();
	magnitudeBtn = new QRadioButton(tr("magnitude"));
	phaseBtn = new QRadioButton(tr("phase"));

	pmGroupBtn->addButton(magnitudeBtn, 0);
	pmGroupBtn->addButton(phaseBtn, 1);

	connect(magnitudeBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(phaseBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * pmLayout = new QGridLayout;
	pmLayout->addWidget(magnitudeBtn, 0, 0);
	pmLayout->addWidget(phaseBtn, 0, 1);

	QGroupBox * pmGroupBox = new QGroupBox;
	pmGroupBox->setTitle(tr("parameter"));
	pmGroupBox->setLayout(pmLayout);

	// pmGroupBox
	powerGroupBtn = new QButtonGroup();
	linearBtn = new QRadioButton(tr("linear"));
	dbBtn = new QRadioButton(tr("dB"));
	powerGroupBtn->addButton(dbBtn, 0);
	powerGroupBtn->addButton(linearBtn, 1);


	QGridLayout * powerLayout = new QGridLayout;
	powerLayout->addWidget(linearBtn, 0, 0);
	powerLayout->addWidget(dbBtn, 0, 1);

	connect(linearBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(dbBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	// powerGroupBox
	QGroupBox * powerGroupBox = new QGroupBox;
	powerGroupBox->setTitle(tr("power"));
	powerGroupBox->setLayout(powerLayout);

	QVBoxLayout * boxLayout = new QVBoxLayout;
	boxLayout->addWidget(dimensionGroupBox);
	boxLayout->addWidget(fieldGroupBox);
	boxLayout->addWidget(pmGroupBox);
	boxLayout->addWidget(powerGroupBox);

	detailsWidget = new QWidget;
	detailsWidget->setLayout(boxLayout);
	detailsDockWidget->setWidget(detailsWidget);
	detailsDockWidget->close();
}

void mainWindow::creareWindows()
{
	LightSourceDialog = new LightSourceWidget(this);
	connect(LightSourceDialog, SIGNAL(sendData(int)),
		this, SLOT(ToLightSource(int)));
	LightSourceDialog->setWindowFlags(Qt::WindowStaysOnTopHint);

	ArbitrarySurfaceDialog = new ArbitrarySurfaceWidget(this);
	//connect(LightSourceDialog, SIGNAL(sendData(int)),
	//	this, SLOT(ToLightSource(int)));
}

void mainWindow::createMenus()
{
	// �ļ��˵�
	//this->menuBar()
	fileMenu = this->menuBar()->addMenu(tr("Files"));

	fileMenu->addAction(saveFileAction);
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(newFileAction);
	fileMenu->addSeparator();
	//fileMenu->addAction(LightSourceAction);

}

void mainWindow::createToolBars()
{
	//file ��
	fileTool = addToolBar("Files");
	fileTool->addAction(saveFileAction);
	fileTool->addAction(openFileAction);
	fileTool->addAction(newFileAction);
	fileTool->addAction(detailsAction);
	fileTool->addSeparator();
	fileTool->addAction(viewAction);
	fileTool->addSeparator();
	fileTool->addWidget(viewLabel);
	fileTool->addWidget(viewComboBox);
	//fileTool->addAction(LightSourceAction);

	// ģ����
	constructTool = addToolBar("Construct");
	constructTool->addAction(rectangleAction);
	constructTool->addAction(polygonAction);
	constructTool->addAction(paraboloidAction);
	constructTool->addAction(ImportAction);
	constructTool->addSeparator();
	constructTool->addAction(cylinderAction);
	constructTool->addAction(sphereAction);
	
	constructTool->addSeparator();
	constructTool->addAction(STLInputAction);
	constructTool->addAction(booleanAction);
	constructTool->addAction(LightSourceAction);
	constructTool->addAction(ApertureFieldAction);
	constructTool->addAction(GaussianAction);
	constructTool->addSeparator();
	constructTool->addAction(CalculationAction);
}

void mainWindow::createTreeWidgetItem()
{
	definitionsTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Definitions")));
	modelTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Model")));

	variablesTreeItem = new QTreeWidgetItem(QStringList(QString("Variables")));
	definitionsTreeItem->addChild(variablesTreeItem);

	geometryTreeItem = new QTreeWidgetItem(QStringList(QString("Geometry")));
	modelTreeItem->addChild(geometryTreeItem);

	lightTreeItem = new QTreeWidgetItem(QStringList(QString("Light")));
	modelTreeItem->addChild(lightTreeItem);

	sourceTreeItem = new QTreeWidgetItem(QStringList(QString("Field")));
	modelTreeItem->addChild(sourceTreeItem);

}

void mainWindow::createRightMenu()
{
	R_Tree_compenents_childMenu = new QMenu(this);
	R_BlankMenu = new QMenu(this);

	SetGeometryAction = new QAction(tr("Properties"), this);
	SetGeometryAction->setStatusTip(tr("Modifying parameters"));
	connect(SetGeometryAction, SIGNAL(triggered()), this, SLOT(SetGeometryPara()));

	DeleteGeometryAction = new QAction(tr("Delete"), this);
	DeleteGeometryAction->setStatusTip(tr("Delete parameters"));
	connect(DeleteGeometryAction, SIGNAL(triggered()), this, SLOT(DeleteGeometry()));

	Field2lightAction = new QAction(tr("Generation lightSource"), this);
	Field2lightAction->setStatusTip(tr("Generation lightSource"));
	connect(Field2lightAction, SIGNAL(triggered()), this, SLOT(Field2light()));

	SaveFieldAction = new QAction(tr("Save"), this);
	SaveFieldAction->setStatusTip(tr("Save Field"));
	connect(SaveFieldAction, SIGNAL(triggered()), this, SLOT(SaveField()));

	transparencyAction = new QAction(tr("Transparency"), this);
	transparencyAction->setStatusTip(tr("Transparency"));
	transparencyAction->setCheckable(true);
	transparencyAction->setChecked(true);
	connect(transparencyAction, SIGNAL(triggered()), this, SLOT(SetTransparency()));
}

void mainWindow::on_treeWidget_ContextMenuRequested(QPoint pos)
{
	rightSelectItem = treeWidget->itemAt(pos);
	if (rightSelectItem == NULL)
	{
		return;
	}
	QVariant var = rightSelectItem->data(0, Qt::UserRole);
	//ItemVar1 = SelectItem->data(1, Qt::UserRole);
	//ItemVar2 = SelectItem->data(2, Qt::UserRole);
	if (0 == var)      //data(...)���ص�data�Ѿ���֮ǰ�����ڵ�ʱ��setdata()���ú�  
	{
		//if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->addAction(transparencyAction);
			int varName = rightSelectItem->data(1, Qt::UserRole).toInt();
			int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
			transparencyAction->setChecked(modleDataList.getTransparency(varName, varIndex));
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);
		}
		//�˵����ֵ�λ��Ϊ��ǰ����λ��  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if(1 == var)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->addAction(Field2lightAction);
			R_Tree_compenents_childMenu->addAction(SaveFieldAction);

		}
		//�˵����ֵ�λ��Ϊ��ǰ����λ��  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if (var == 2)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);
		}
		//�˵����ֵ�λ��Ϊ��ǰ����λ��  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
}

void mainWindow::on_treeWidget_leftPressed(QTreeWidgetItem *item, int column)
{
	// ע���������itempressed�������ԣ�itemclicled���У������������Ҽ�����֪��Ϊʲô������
	if (item->parent() != NULL)     // ���ڵ�
		if (qApp->mouseButtons() == Qt::LeftButton)
		{
			//RectangleWidget dialog(this, isSet); 
			if (item->data(0, Qt::UserRole) == 1)
			{
				int name = item->data(1, Qt::UserRole).toInt();
				int index = item->data(2, Qt::UserRole).toInt();
				leftSelectItem = item;
				showDetails(name, index);
			}
	
		}
}

void mainWindow::on_Details_FieldClicked()
{
	int index = leftSelectItem->data(2, Qt::UserRole).toInt();
	int dim = dimensionGroupBtn->checkedId();
	if (dim == 0) // 3D
	{
		fieldHash[index].set3D(true);
		renderer->RemoveActor(fieldHash[index].getActor());
		renderer->RemoveActor(fieldHash[index].getActor3D());
		fieldHash[index].setShowPara(fieldGroupBtn->checkedId(), 
			powerGroupBtn->checkedId(), pmGroupBtn->checkedId());
		magnitudeBtn->setChecked(true);
		phaseBtn->setEnabled(false);
		magnitudeBtn->setEnabled(false);
		fieldHash[index].resetActor3D();
		renderer->AddActor(fieldHash[index].getActor3D());
	}
	else // 2D
	{
		fieldHash[index].set3D(false);
		phaseBtn->setEnabled(true);
		magnitudeBtn->setEnabled(true);

		int i = pmGroupBtn->checkedId();
		if (i == 0)  // ��λû��db��ʽ
		{
			linearBtn->setEnabled(true);
			dbBtn->setEnabled(true);
		}
		else
		{
			linearBtn->setEnabled(false);
			dbBtn->setEnabled(false);
		}
		fieldHash[index].setShowPara(fieldGroupBtn->checkedId(), powerGroupBtn->checkedId(), i);

		renderer->RemoveActor(fieldHash[index].getActor());
		renderer->RemoveActor(fieldHash[index].getActor3D());
		fieldHash[index].resetActor();
		renderer->AddActor(fieldHash[index].getActor());
	}
	
	updateVtk();	
}

void mainWindow::showDetails()
{
	detailsDockWidget->show();
}

void mainWindow::createRectangle() // ���޸�
{
	rectangleDialog = new RectangleWidget(this, isSet);

	rectangleDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(rectangleDialog, SIGNAL(sendData(int)),
		this, SLOT(Rectangle(int)));

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int recModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		rectangleDialog->setNameLineEdit(rightSelectItem->text(0));
		tempModleData = new ModleData;
		modleDataList.findModle(RECTANGLE, recModleIndex, *tempModleData);
		tempActor = tempModleData->getActor();
		rectangleDialog->setWidgetData(*tempModleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		rectangleDialog->setNameLineEdit("rectangle" + QString::number(rectangleNum + 1));

	rectangleDialog->show();
	Rectangle(5);

	
}

void mainWindow::Rectangle(int index)
{
	if (index == 5)
	{
		bool ok;
		if (tempModleData == NULL)
			tempModleData = new ModleData(RECTANGLE, rectangleNum, 9);

		ok = rectangleDialog->getWidgetData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);

	}
	else if (index == 1 || index == 2)
	{
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		modleDataList.pushModle(*tempModleData);
		
		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, rectangleDialog->getName(),
			"Qt/images/rectangle.png", RECTANGLE, rectangleNum);

		tempActor = NULL;

		rectangleNum++;
		if (index == 1)
			delete rectangleDialog;
		else
			Rectangle(5);
		//updateVtk();
	}
	else if (index == 3)
	{
		
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		
		modleDataList.modifyModle(*tempModleData);

		rightSelectItem->setText(0, rectangleDialog->getName()); // ��������������
		tempActor = NULL;

		delete rectangleDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete rectangleDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	updateVtk();
}

void mainWindow::createParaboloid()
{
	paraboloidDialog = new ParaboloidWidget(this, isSet);

	paraboloidDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(paraboloidDialog, SIGNAL(sendData(int)),
		this, SLOT(Paraboloid(int)));

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
	
		// ���������ó�ѡ�е�ֵ
		paraboloidDialog->setNameLineEdit(rightSelectItem->text(0));
		ModleData modleData;
		modleDataList.findModle(PARABOLOID, paraModleIndex, modleData);
		tempActor = modleData.getActor();
		paraboloidDialog->setWidgetData(modleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		paraboloidDialog->setNameLineEdit("paraboloid" + QString::number(paraboloidNum + 1));
	
	paraboloidDialog->show();
	Paraboloid(5);
	
}

void mainWindow::Paraboloid(int index)
{
	if (index == 5)
	{
		bool ok;
		if (tempModleData == NULL)
			tempModleData = new ModleData(PARABOLOID, paraboloidNum, 15);
		else
		{
			delete tempModleData;
			tempModleData = new ModleData(PARABOLOID, paraboloidNum, 15);
		}

		ok = paraboloidDialog->getWidgetData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);

		updateVtk();
	}
	else if (index == 1 || index == 2)
	{
		renderer->RemoveActor(tempActor);
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		modleDataList.pushModle(*tempModleData);
		renderer->AddActor(tempModleData->getActor());

		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, paraboloidDialog->getName(),
			"Qt/images/paraboloid.png", PARABOLOID, paraboloidNum);
		tempActor = NULL;
		paraboloidNum++;
		if (index == 1)
			paraboloidDialog->close();
			//delete paraboloidDialog;
		else
			Paraboloid(5);
	}
	else if (index == 3)
	{
		renderer->RemoveActor(tempActor);  // �Ƴ����е�actor
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		renderer->AddActor(tempModleData->getActor());
		modleDataList.modifyModle(*tempModleData);

		rightSelectItem->setText(0, paraboloidDialog->getName()); // ��������������
		updateVtk();
		tempActor = NULL;

		//delete paraboloidDialog;
		paraboloidDialog->close();
		delete tempModleData;
		tempModleData = NULL;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		//delete paraboloidDialog;
		paraboloidDialog->close();
		delete tempModleData;
		tempModleData = NULL;
	}
}

void mainWindow::createArbitrarySurface()
{
	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		ArbitrarySurfaceDialog->setNameLineEdit(rightSelectItem->text(0));
		ModleData modleData;
		modleDataList.findModle(ARBITRARYSURFACE, paraModleIndex, modleData);
		tempActor = modleData.getActor();
		ArbitrarySurfaceDialog->setWidgetData(modleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		ArbitrarySurfaceDialog->setNameLineEdit("ArbitrarySurface" + QString::number(ArbitrarySurfaceNum + 1));

	ArbitrarySurfaceDialog->show();
}

void mainWindow::createCylinder() // ���޸�
{
	cylinderDialog = new CylinderWidget(this, isSet);

	cylinderDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(cylinderDialog, SIGNAL(sendData(int)), 
		this, SLOT(Cylinder(int)));
	
	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		cylinderDialog->setNameLineEdit(rightSelectItem->text(0));
		tempModleData = new ModleData;
		modleDataList.findModle(CYLINDER, paraModleIndex, *tempModleData);
		tempActor = tempModleData->getActor();
		cylinderDialog->setWidgetData(*tempModleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		cylinderDialog->setNameLineEdit("cylinder" + QString::number(cylinderNum + 1));
	Cylinder(5);
	cylinderDialog->show();
}

void mainWindow::Cylinder(int index)
{
	if(index == 5) // ��ֵ�޸�
	{
		bool ok;
		if(tempModleData == NULL)
			tempModleData = new ModleData(CYLINDER, cylinderNum, 9);
			
		ok = cylinderDialog->getWidgetData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);

		//updateVtk();
	}
	else if (index == 1 || index == 2) // 1Ϊȷ�� 2Ϊadd
	{
		//renderer->RemoveActor(tempActor);
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		modleDataList.pushModle(*tempModleData);
        //renderer->AddActor(tempModleData->getActor());
		//renderer->AddActor(modleDataList.getActor(CYLINDER, cylinderNum));
		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, cylinderDialog->getName(),
			"Qt/images/cylinder.png", CYLINDER, cylinderNum);

		tempActor = NULL;

		cylinderNum++;
		if (index == 1)
			delete cylinderDialog;
		else
			Cylinder(5);
		//updateVtk();
	}
	else if (index == 3) // 3Ϊ�޸�ʱ��ȷ��
	{
		//renderer->RemoveActor(tempActor);  // �Ƴ����е�actor
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		//renderer->AddActor(tempModleData->getActor());
		modleDataList.modifyModle(*tempModleData);

		rightSelectItem->setText(0, cylinderDialog->getName()); // ��������������
		updateVtk();
		tempActor = NULL;
		
		delete cylinderDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	else if (index == 4) // �޸�ʱ�Ĺر�
	{

		renderer->RemoveActor(tempActor);
		delete tempModleData;
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		cylinderDialog->setNameLineEdit(rightSelectItem->text(0));
		tempModleData = new ModleData;
		modleDataList.findModle(CYLINDER, paraModleIndex, *tempModleData);
		tempActor = tempModleData->getActor();
		renderer->AddActor(tempActor);

		tempActor = NULL;
		delete cylinderDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete cylinderDialog;
		delete tempModleData;
		tempModleData = NULL;
		//updateVtk();
	}
	updateVtk();
}

void mainWindow::createSphere()
{
	sphereDialog = new SphereWidget(this, isSet);

	sphereDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(sphereDialog, SIGNAL(sendData(int)),
		this, SLOT(Sphere(int)));

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		sphereDialog->setNameLineEdit(rightSelectItem->text(0));
		tempModleData = new ModleData;
		modleDataList.findModle(SPHERE, paraModleIndex, *tempModleData);
		tempActor = tempModleData->getActor();
		sphereDialog->setWidgetData(*tempModleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		sphereDialog->setNameLineEdit("sphere" + QString::number(sphereNum + 1));
	Sphere(5);
	sphereDialog->show();
	
}

void mainWindow::Sphere(int index)
{
	if (index == 5)
	{
		bool ok;
		if (tempModleData == NULL)
			tempModleData = new ModleData(SPHERE, sphereNum, 8);

		ok = sphereDialog->getWidgetData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);

		//updateVtk();
	}
	else if (index == 1 || index == 2)
	{
		//renderer->RemoveActor(tempActor);
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		modleDataList.pushModle(*tempModleData);
		//renderer->AddActor(tempModleData->getActor());
		//renderer->AddActor(modleDataList.getActor(CYLINDER, cylinderNum));
		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, sphereDialog->getName(),
			"Qt/images/sphere.png", SPHERE, sphereNum);

		tempActor = NULL;

		cylinderNum++;
		if (index == 1)
			delete sphereDialog;
		else
			Sphere(5);
		//updateVtk();
	}
	else if (index == 3)
	{
		//renderer->RemoveActor(tempActor);  // �Ƴ����е�actor
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		//renderer->AddActor(tempModleData->getActor());
		modleDataList.modifyModle(*tempModleData);

		rightSelectItem->setText(0, sphereDialog->getName()); // ��������������
		updateVtk();
		tempActor = NULL;

		delete sphereDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete sphereDialog;
		delete tempModleData;
		tempModleData = NULL;
		//updateVtk();
	}
	updateVtk();
}

void mainWindow::createBoolean()
{
	int nChild = geometryTreeItem->childCount();
	if (nChild == 0)
	{
		QMessageBox::warning(NULL, "Warning", "Not find any model!");
		return;
	}
	booleanDialog = new BooleanWidget(this, isSet);

	QStringList allSurfaceName;

	for (int i = 0; i < nChild; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);

	booleanDialog->addAllSurface(allSurfaceName);
	//booleanDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(booleanDialog, SIGNAL(sendData(int)),
		this, SLOT(Boolean(int)));
	
	booleanDialog->show();
	Boolean(-1);
}

void mainWindow::Boolean(int index)
{
	if (index != -1 && index != 3)
	{
		tempActor = stackActor.top();
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		stackActor.pop();
		tempActor = stackActor.top();
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		stackActor.pop();
	}
	if (index == 5 || index == -1)
	{
		QTreeWidgetItem *tempTree = geometryTreeItem->child(booleanDialog->getModel1());
		int name1 = tempTree->data(1, Qt::UserRole).toInt();
		int index1 = tempTree->data(2, Qt::UserRole).toInt();

		tempTree = geometryTreeItem->child(booleanDialog->getModel2());
		int name2 = tempTree->data(1, Qt::UserRole).toInt();
		int index2 = tempTree->data(2, Qt::UserRole).toInt();

		tempActor = modleDataList.getActor(name1, index1);
		
		tempActor->GetProperty()->SetOpacity(0.3);
		tempActor->GetProperty()->SetColor(1, 0, 0);
		stackActor.push(tempActor);
		tempActor = modleDataList.getActor(name2, index2);
	
		tempActor->GetProperty()->SetOpacity(0.3);
		tempActor->GetProperty()->SetColor(1, 0, 0);
		stackActor.push(tempActor);


	}
	else if (index == 2 || index == 3)
	{
		QTreeWidgetItem *tempTree1 = geometryTreeItem->child(booleanDialog->getModel1());
		int name1 = tempTree1->data(1, Qt::UserRole).toInt();
		int index1 = tempTree1->data(2, Qt::UserRole).toInt();

		QTreeWidgetItem *tempTree2 = geometryTreeItem->child(booleanDialog->getModel2());
		int name2 = tempTree2->data(1, Qt::UserRole).toInt();
		int index2 = tempTree2->data(2, Qt::UserRole).toInt();

		ModleData modledata(BOOLEANOP, 0, 5);
		ModleData a, b;
		int operation = booleanDialog->getOperation();
		modleDataList.findModle(name1, index1, a);
		modleDataList.findModle(name2, index2, b);
		if (operation == 1 && (name1 == PARABOLOID || name2 == PARABOLOID))
			operation = 3;
		modledata.setBoolean(name1, index1, name2, index2,
			&a, &b, operation);
		renderer->AddActor(modledata.getActor());
		modleDataList.pushModle(modledata);

		QString nameOperation;
		if (operation == 0)
			nameOperation = "Union";
		else if (operation == 1|| operation == 3)
			nameOperation = "Intersection";
		else
			nameOperation = "Difference";

		addTreeChild(geometryTreeItem, nameOperation,
			"Qt/images/boolean.png", BOOLEANOP, 0);
		
		//geometryTreeItem->removeChild(tempTree1);

		tempActor = stackActor.top();
		tempActor->GetProperty()->SetOpacity(0);
		//tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		stackActor.pop();
		tempActor = stackActor.top();
		tempActor->GetProperty()->SetOpacity(0);
		//tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		stackActor.pop();

		//tempActor = NULL;
		delete booleanDialog;
		booleanDialog = NULL;
	}
	else
	{
		tempActor = NULL;
		delete booleanDialog;
		booleanDialog = NULL;
		//updateVtk();
	}

	updateVtk();
}

void mainWindow::createSTLInput()
{
	STLInputDialog = new STLInputWidget(this, isSet);
	STLInputDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(STLInputDialog, SIGNAL(sendData(int)),
		this, SLOT(STLInput(int)));
	
	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// ���������ó�ѡ�е�ֵ
		STLInputDialog->setNameLineEdit(rightSelectItem->text(0));
		tempModleData = new ModleData;
		modleDataList.findModle(STLINPUT, paraModleIndex, *tempModleData);
		tempActor = tempModleData->getActor();
		STLInputDialog->setWidgetData(*tempModleData);
		isSet = 0; // ��Ϊ�½�

	}
	else
		STLInputDialog->setNameLineEdit("STLInput" + QString::number(STLInputNum + 1));
	
	STLInputDialog->show();
}

void mainWindow::STLInput(int index)
{
	if (index == 5)
	{
		if (tempModleData == NULL)
			tempModleData = new ModleData(STLINPUT, STLInputNum, 7);

		STLInputDialog->getWidgetData(*tempModleData);
	
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);

		//updateVtk();
	}
	else if (index == 1 || index == 2)
	{
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		modleDataList.pushModle(*tempModleData);
	
		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, STLInputDialog->getName(),
			"Qt/images/stlInput.png", STLINPUT, STLInputNum);

		tempActor = NULL;

		STLInputNum++;
		if (index == 1)
			delete STLInputDialog;
		else
			STLInput(5);
	}
	else if (index == 3)
	{
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);

		modleDataList.modifyModle(*tempModleData);

		rightSelectItem->setText(0, STLInputDialog->getName()); // ��������������
		updateVtk();
		tempActor = NULL;

		delete STLInputDialog;
		delete tempModleData;
		tempModleData = NULL;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete STLInputDialog;
		delete tempModleData;
		tempModleData = NULL;
		//updateVtk();
	}
	updateVtk();
}

void mainWindow::createGaussian()
{
	GaussianDialog = new GaussianWidget(this, isSet);
	//delete GaussianDialog;
	GaussianDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(GaussianDialog, SIGNAL(sendData(int)),
		this, SLOT(Gaussian(int)));

	GaussianDialog->show();
	
	if (isSet == 1)
	{
		GaussianDialog->setWidgetData(source);
		GaussianDialog->setNameLineEdit("Gaussian-Field");
		isSet = 0;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();

		tempImageActor = fieldHash[Index].getActor();
		//fieldHash[Index].getActor3D()->GetProperty()->SetOpacity(0.4);
		//fieldList.begin()->getActor3D()->GetProperty()->SetOpacity(0.4);

	}
	else
		GaussianDialog->setNameLineEdit("Gaussian-Field"+QString::number(FieldNum+1));

}

void mainWindow::Gaussian(int index)
{
	if (index == 2 || index == 6) // ��ʾ���Ӧ��
	{
		tempIndex = 2;
		if (tempSource != NULL)
			delete tempSource;
		tempSource = new Source(GAUSSIAN);

		bool ok;
		ok = GaussianDialog->getWidgetData(tempSource);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		
		tempSource->setUnit(Unit_factor);
		if (tempField != NULL)
			delete tempField;
		tempField = new Field;

		tempField->setField(tempSource);
		tempField->set3D(false);

		renderer->RemoveActor(tempImageActor);
		tempField->calActor();
		tempImageActor = tempField->getActor();

		renderer->AddActor(tempImageActor);

/*
		FieldThread *fieldThd = new FieldThread(tempField);
		connect(fieldThd, SIGNAL(finished()), this, SLOT(Gaussian_Thread()));
		connect(fieldThd, SIGNAL(finished()), fieldThd, SLOT(deleteLater()));
		fieldThd->start();
		
		progDlg = new QProgressDialog();
		progDlg->setWindowTitle("Please wait...");
		progDlg->setFixedWidth(200);
		progDlg->setRange(0, 100);
		progDlg->setWindowFlags(Qt::WindowStaysOnTopHint);
		progDlg->show();
		timer = new QTimer();
		currentValue = 0;
		progDlg->setValue(currentValue);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressDialog()));
		timer->start(50); */
	
	}
	else if (index == 1)  // ���ȷ��
	{	
		if (tempIndex != 2)// ���Ӧ�ú������޸ıȲ����м���
		{
			Gaussian(2);	
			tempIndex = 1;
		}
		
		tempIndex = 1;

		fieldHash[FieldNum] = *tempField;
		addTreeChild(sourceTreeItem, GaussianDialog->getName(),
			"Qt/images/Gaussian.png", GAUSSIAN, FieldNum, 1);
		source = new Source(*tempSource);
		
		delete tempSource;
		tempSource = NULL;
		FieldNum++;
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;


	}
	else if (index == 4)  // ��ʾ�޸�ʱ�Ĺر� �ָ����޸�ǰ��״̬
	{
		tempIndex = 4;
		renderer->RemoveActor(tempImageActor);

		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		renderer->AddActor(fieldHash[Index].getActor());
		//fieldHash[Index].getActor()->GetProperty()->SetOpacity(1);

		delete tempSource;
		tempSource = NULL;
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
	}
	else if (index == 3)   // �޸�ʱ��ȷ��
	{
		if (tempIndex != 2) // ���Ӧ�ú������޸ıȲ����м���
		{
			Gaussian(2);
			tempIndex = 3;
		}

		tempIndex = 3;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempImageActor = fieldHash[Index].getActor();
		renderer->RemoveActor(tempImageActor);

		fieldHash[Index] = *tempField;

		source = new Source(*tempSource);
		delete tempSource;
		tempSource = NULL;
		//FieldNum = 1;
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;
	

	}
	else if (index == 5)
	{
		tempIndex = 5;
		return;
	}
	else
	{
		renderer->RemoveActor(tempActor);
		renderer->RemoveActor(tempImageActor);
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
	}
	updateVtk();
}

void mainWindow::createCalculation()
{
	// ����Ƿ���Լ���
	if (FieldNum == 0)
	{
		QMessageBox::warning(NULL, "Warning", "Please add a source first!");
		return;
	}
	calculationDialog = new CalculationWidget(this);
	connect(calculationDialog, SIGNAL(sendData(int)),
		this, SLOT(ToCalculation(int)));
	//calculationDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�

	// �ҵ����е���
	int nChild1 = geometryTreeItem->childCount();
	int nChild2 = sourceTreeItem->childCount();
	QStringList allSurfaceName;
	QStringList allSourcesName;
	//vector<int> treeChildIndexOfRec;

	for (int i = 0; i < nChild1; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);
	
	for (int i = 0; i < nChild2; i++)
		allSourcesName << sourceTreeItem->child(i)->text(0);

	calculationDialog->addAllSurface(allSurfaceName);
	calculationDialog->addAllSources(allSourcesName);
	//calculationDialog->addAllRectangles(allRectangleName);
	calculationDialog->setResRectangle(fieldHash.begin()->second.getWidth(), 
		fieldHash.begin()->second.getDepth());
	
	calculationDialog->show();
	//delete calculationDialog;
	//dialog.show();

}

void mainWindow::ToCalculation(int index)
{
	if (index == 1)
	{
		// ��ȡ������Ϣ
		int NumSurface, NumRectangle;
		int* treeChildIndex;
		int sourceIndex;

		// �ʷ���Ϣ 
		int meshsize = calculationDialog->getMeshSize();

		// ����ģʽ��Ϣ �ʷ����Ȼ��ǽ���������
		int calmodel = calculationDialog->getCalMod();

	    // �����
		calculationDialog->getSelSurfaces(NumSurface, treeChildIndex);
		sourceIndex = calculationDialog->getSelSources();

		// �õ����һ�������Ϣ
		double *data = new double[3];
		double dis, width, depth;
		bool isdefualt;
		if (!calculationDialog->getResRectangle(isdefualt, data))
		{
			QMessageBox::warning(NULL, "Warning", "Please add a source first!");
			return;
		}
		if (isdefualt)  // �ж��Ƿ�ΪĬ��
		{
			dis = 1;
			width = fieldHash[sourceIndex].getWidth();
			depth = fieldHash[sourceIndex].getDepth();
		}
		else
		{
			dis = data[0];
			width = data[1];
			depth = data[2];
		}

		double fre = calculationDialog->getFre() * 1e9;

		int *nameSel = new int[NumSurface];
		int *indexSel = new int[NumSurface];

		QString name;
		sourceIndex = sourceTreeItem->child(sourceIndex)->data(2, Qt::UserRole).toInt();
		name = sourceTreeItem->child(sourceIndex)->text(0);
		name += tr("_");
		for (int i = 0; i < NumSurface; i++)
		{
			nameSel[i] = geometryTreeItem->child(treeChildIndex[i])->data(1, Qt::UserRole).toInt();
			indexSel[i] = geometryTreeItem->child(treeChildIndex[i])->data(2, Qt::UserRole).toInt();
			name += geometryTreeItem->child(treeChildIndex[i])->text(0);
			name += tr("_");
		}
		name += QString::number(dis);
		calName.push(name);
		CalculateThread *calThr = new CalculateThread(
			fre,                     // Ƶ��
			Unit_factor,             // ������
			nameSel, indexSel,       // ѡ��ķ�������Ϣ 
			NumSurface,              // ѡ��ķ��������� 
			&modleDataList,          // ����ģ������
			&fieldHash[sourceIndex], // ��ʼԴ
			dis, width, depth,       // �õ������һ�������Ϣ
			meshsize,                // �ʷ��ܶ���Ϣ 
			calmodel                 // ����ģʽ��Ϣ �ʷ����Ȼ��ǽ���������
		);

		delete[] treeChildIndex;

		connect(calThr, SIGNAL(sendField(Field*)), this, SLOT(receiveField(Field*)));
		connect(calThr, SIGNAL(finished()), calThr, SLOT(deleteLater()));
		calThr->start();

		progDlg = new QProgressDialog();
		progDlg->setWindowTitle("Please wait...");
		progDlg->setFixedWidth(200);
		progDlg->setRange(0, 100);
		progDlg->setWindowFlags(Qt::WindowStaysOnTopHint);
		progDlg->show();
		timer = new QTimer();
		currentValue = 0;
		progDlg->setValue(currentValue);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressDialog()));
		timer->start(50);

	}
	else if (index == 2) // check 
	{
		clearCheckActor();
	
		// ��ȡ������Ϣ
		int NumSurface, NumRectangle;

		// ����ģʽ��Ϣ �ʷ����Ȼ��ǽ���������
		int calmodel = calculationDialog->getCalMod();

		int* treeChildIndex;

		//modleID ResRectangle;  // �����
		calculationDialog->getSelSurfaces(NumSurface, treeChildIndex);
		int sourceIndex = calculationDialog->getSelSources();
		sourceIndex = sourceTreeItem->child(sourceIndex)->data(2, Qt::UserRole).toInt();

		double fre = calculationDialog->getFre() * 1e9;
		PVVA pvva;
		// ���õ�λ
		pvva.setUnit(Unit_factor);
		// ����Ƶ��
		pvva.setFre(fre);
		// ����Դ�������ڴ�
		pvva.setSource(&fieldHash[sourceIndex]); //?
		Vector3 point1, n_point1;
		Vector3 point2, n_point2;
		pvva.getSourcePoint(point2, n_point2);
		for (int i = 0; i < NumSurface; i++)
		{
			int nameSel = geometryTreeItem->child(treeChildIndex[i])->data(1, Qt::UserRole).toInt();
			int indexSel = geometryTreeItem->child(treeChildIndex[i])->data(2, Qt::UserRole).toInt();

			PVVA pvva1;
			// ���õ�λ
			pvva1.setUnit(Unit_factor);
			// ����Ƶ��
			pvva1.setFre(fre);
			pvva1.setSourcePoint(point2, n_point2);
			point1 = point2;
			n_point1 = n_point2;

			pvva1.setModle(modleDataList.getPolyData(nameSel, indexSel));
			if (!pvva1.CheckModle(point2, n_point2))
			{
				QString a = "There is no intersection between the ";
				a += QString::number(i + 1);
				a += "th surface and the source. \n Please check it!";
				QMessageBox::warning(NULL, "Warning", a);
				clearCheckActor();
				return;
			}
			tempActor = ModleData::getLineActor(point1, point2);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		double *data = new double[3];
		bool isdefualt;
		GraphTrans graphTrans = PVVA::getSourceGraphTrans(n_point2);
		if (!calculationDialog->getResRectangle(isdefualt, data))
		{
			QMessageBox::warning(NULL, "Warning", "Please add a source first!");
			clearCheckActor();
			return;
		}
		if (isdefualt) // Ĭ��ֵ
		{	
			tempActor = ModleData::getLineActor(point2, point2 + n_point2);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
			graphTrans.updateTranslate(point2 + n_point2);
			tempActor = ModleData::getRectangleActor(graphTrans, fieldHash[sourceIndex].getWidth(),
				fieldHash[sourceIndex].getDepth());
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		else // �û��Զ���
		{
			tempActor = ModleData::getLineActor(point2, point2 + n_point2*data[0]);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
			graphTrans.updateTranslate(point2 + n_point2*data[0]);
			tempActor = ModleData::getRectangleActor(graphTrans, data[1], data[2]);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		delete[] data;
		tempActor = NULL;
		updateVtk();
		QMessageBox::warning(NULL, "OK", "OK");

	}
}

void mainWindow::createApertureField()
{
	ApertureFieldDialog = new ApertureFieldWidget(this);

	ApertureFieldDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // �Ӵ��ڱ����ö�
	connect(ApertureFieldDialog, SIGNAL(sendData(int)),
		this, SLOT(ApertureField(int)));

	if (isSet == 1)
	{

		isSet = 0;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();		

	}
	else
	{
		ApertureFieldDialog->setNameLineEdit("Aperture-Field"
			+ QString::number(FieldNum + 1));
		ApertureField(5);
	}

	ApertureFieldDialog->show();

}

void mainWindow::ApertureField(int index)
{
	if (index == 5) // �������
	{
		bool ok;
		if (tempModleData == NULL)
			tempModleData = new ModleData(RECTANGLE, rectangleNum, 9);

		ok = ApertureFieldDialog->getRectData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);
	}
	else if (index == 2) // ���Ӧ��
	{
		tempIndex = 2;
		if (tempSource != NULL)
			delete tempSource;
		tempSource = new Source(APERTURE);

		bool ok;
		ok = ApertureFieldDialog->getWidgetData(tempSource);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		tempSource->setUnit(Unit_factor);
		if (tempField != NULL)
			delete tempField;
		tempField = new Field;

		tempField->setField(tempSource);

		renderer->RemoveActor(tempActor);
		tempField->calActor();
		renderer->AddActor(tempField->getActor());
	}
	else if (index == 1)  // ���ȷ��
	{
		if (tempIndex != 2) // δ���Ӧ�ð�ť 
		{
			ApertureField(2);
			tempIndex = 1;
		}
		fieldHash[FieldNum] = *tempField;
		addTreeChild(sourceTreeItem, ApertureFieldDialog->getName(),
			"Qt/images/Aperture.png", APERTURE, FieldNum, 1);
		source = new Source(*tempSource);

		delete tempSource;
		tempSource = NULL;
		FieldNum++;
		delete tempField;
		tempField = NULL;
		ApertureFieldDialog->close();
		ApertureFieldDialog = NULL;
		tempActor = NULL;

	}
	updateVtk();
}

void mainWindow::createLightSource()
{
	//�õ����е���
	QStringList allSurfaceName;
	int nChild1 = geometryTreeItem->childCount();
	for (int i = 0; i < nChild1; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);
	//�õ����еĹ�Դ
	int nChild2 = lightTreeItem->childCount();
	QStringList allSourcesName;
	for (int i = 0; i < nChild2; i++)
		allSourcesName << lightTreeItem->child(i)->text(0);

	LightSourceDialog->addAllSurface(allSurfaceName);
	LightSourceDialog->addAllSources(allSourcesName);
	LightSourceDialog->setNameLineEdit("LightSource" + 
		QString::number(LightSourceNum+1));

	LightSourceDialog->init();
	LightSourceDialog->show();
	ToLightSource(5); // ���ȴ���һ����ʱ����
}

void mainWindow::ToLightSource(int index)
{
	if (index == 5) // ������λ�׶�Σ���Դ��
	{
		bool ok;
		if (LightSourceDialog->getType() == 2)  // ������
		{
			if (tempModleData != NULL)
				delete tempModleData;
			tempModleData = new ModleData(REVERSECONE, rectangleNum, 9);
		}
		else if (LightSourceDialog->getType() == 3)  //�Ѿ�����Դ
		{
			renderer->RemoveActor(tempActor);
			updateVtk();
			return;
		}
		else
		{
			if (tempModleData != NULL)
				delete tempModleData;
			tempModleData = new ModleData(RECTANGLE, rectangleNum, 9);
		}


		ok = LightSourceDialog->getRectData(*tempModleData);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(0.4);
		tempActor->GetProperty()->SetColor(0, 0, 1);
		renderer->AddActor(tempActor);
	}
	else if (index == 7) // �������߸���������ʱ��ʾ����
	{
		if (LightSourceDialog->getType() == 3)
			return;
		tempIndex = 7;
		bool ok;
		if (tempRaySource == NULL)
			tempRaySource = new RaySource;

		ok = LightSourceDialog->getWidgetData(*tempRaySource);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		
		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		tempRaySource->calRayTrace();
		tempRaySource->copyPlane();
		tempRaySource->calRayActor();
		tempActor = tempRaySource->getRayActor();
		renderer->AddActor(tempActor);
	}
	else if (index == 2) // ���Ӧ��
	{
		tempIndex = 2;
		// ��ȡ������Ϣ
		if (LightSourceDialog->getType() == 3)
		{
			int SelSource = geometryTreeItem->child(LightSourceDialog->
				getSelSources())->data(2, Qt::UserRole).toInt();
			tempRaySource = raySourceHash[SelSource];
			tempActor = tempRaySource->getRayActor();
			tempRaySource->setLength(LightSourceDialog->getLength());
			list<vtkActor*> listactor = tempRaySource->getActors();
			for (auto x : listactor)
			{
				stackActor.push(x);
			}
		}

		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		int NumSurface;
		int* treeChildIndex;
		LightSourceDialog->getSelSurfaces(NumSurface, treeChildIndex);
		tempRaySource->clearActor();
		tempRaySource->copyPlane();
		int nameSel, indexSel;
		for (int i = 0; i < NumSurface; i++)
		{
			nameSel = geometryTreeItem->child(treeChildIndex[i])->
				data(1, Qt::UserRole).toInt();
			indexSel = geometryTreeItem->child(treeChildIndex[i])->
				data(2, Qt::UserRole).toInt();
			if (nameSel == PARABOLOID)
			{
				ModleData modleData;
				modleDataList.findModle(nameSel, indexSel, modleData);
				tempRaySource->setSurface(&modleData);
			}
			else
			{
				tempRaySource->setSurface(modleDataList.getPolyData(nameSel, indexSel));
			}
		}
		list<vtkActor*> listactor = tempRaySource->getActors();
		for (auto x : listactor)
		{
			renderer->AddActor(x);
			stackActor.push(x);
		}
		tempRaySource->calRayActor();
		tempActor = tempRaySource->getRayActor();
		renderer->AddActor(tempActor);
	}
	else if (index == 1)
	{
		if (tempIndex != 2) // δ���Ӧ�ð�ť 
		{
			ToLightSource(2);
		}
		tempIndex = 1;
		if (LightSourceDialog->getType() != 3)
		{
			raySourceHash[LightSourceNum] = tempRaySource;
			addTreeChild(lightTreeItem, LightSourceDialog->getName(),
				"Qt/images/lightSource.png", LIGHTSOURCE, LightSourceNum, 2);

			LightSourceNum++;
		}
		
		tempRaySource = NULL;
		tempActor = NULL;
		while (!stackActor.empty())
		{
			stackActor.pop();
		}

		LightSourceDialog->close();

		tempActor = NULL;
		if (tempModleData != NULL)
		{
			delete tempModleData;
			tempModleData = NULL;
		}
	}
	else
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		if (tempModleData != NULL)
		{
			delete tempModleData;
			tempModleData = NULL;
		}
		//if (tempRaySource != NULL)
		//{
			//delete tempRaySource;
		//	tempRaySource = NULL;
		//}
		LightSourceDialog->close();
	}
	updateVtk();
}

void mainWindow::SetGeometryPara()  //�޸Ĳ���
{
	isSet = 1;
	QVariant var = rightSelectItem->data(1, Qt::UserRole);
	if (var == PARABOLOID)   createParaboloid();
	//else if (ItemVar1 == LIGHTSOURCE)   createLightSource();
	else if (var == RECTANGLE)   createRectangle();
	//else if (ItemVar1 == APERTUREFIELDE)   createApertureField();
	else if (var == GAUSSIAN)   createGaussian();
	else if (var == CYLINDER)   createCylinder();
	else if (var == STLINPUT)   createSTLInput();
	else if (var == SPHERE)   createSphere();

}

void mainWindow::DeleteGeometry()
{
	QVariant varName = rightSelectItem->data(1, Qt::UserRole);
	QVariant varIndex = rightSelectItem->data(2, Qt::UserRole);
	if (varName == PARABOLOID || varName == RECTANGLE || varName == CYLINDER 
		|| varName == STLINPUT || varName == SPHERE) 
	{
		geometryTreeItem->removeChild(rightSelectItem);
		// ɾ��ѡ���ֵ
		//ModleData modleData;
		modleDataList.removeModle_Actor(varName.toInt(), varIndex.toInt(), renderer);
		//renderer->RemoveActor(atcor)
		//modleDataList.removeModel(varName.toInt(), varIndex.toInt());
	}
	else if (varName == GAUSSIAN)
	{
		sourceTreeItem->removeChild(rightSelectItem);
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		renderer->RemoveActor(fieldHash[Index].getActor3D());
		renderer->RemoveActor(fieldHash[Index].getActor());
		fieldHash.erase(Index);
	}
	else if (varName == LIGHTSOURCE)
	{
		lightTreeItem->removeChild(rightSelectItem);
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempRaySource = raySourceHash[Index];
		raySourceHash.erase(Index);
		list<vtkActor*> listactor = tempRaySource->getActors();
		for (auto x : listactor)
		{
			renderer->RemoveActor(x);
		}
		renderer->RemoveActor(tempRaySource->getRayActor());
		// delete tempRaySource;  // �ڴ�й© ��ʱ��������ô���
		tempRaySource = NULL;
	}
	if (leftSelectItem != NULL)
	{
		QVariant leftVarName = leftSelectItem->data(1, Qt::UserRole);
		QVariant leftVarIndex = leftSelectItem->data(2, Qt::UserRole);
		if (leftVarName == varName && leftVarIndex == varIndex)
		{
			if (detailsWidget != NULL)
			{
				delete detailsWidget;
				detailsWidget = NULL;
				delete leftSelectItem;
				leftSelectItem = NULL;
			}
		}
	}
	
	/*
	else if (ItemVar1 == LIGHTSOURCE_NAME) // ��Դ
	{
		sourceTreeItem->removeChild(SelectItem);
		// ɾ��ѡ���ֵ

		LightSource_Data.erase(ItemVar2.toInt());
	}

	else if (ItemVar1 == APERTUREFIELD_NAME) // ���泡
	{
		sourceTreeItem->removeChild(SelectItem);
		delete ApertureField_Data;
		FieldNum = 0;
	}

	else if (ItemVar1 == GAUSSIAN_NAME) // ��˹��
	{
		sourceTreeItem->removeChild(SelectItem);
		delete Gaussian_Data;
		glMainWidget->updateField(NULL, false);
		FieldList.clear();
		FieldNum = 0;
	}*/
	updateVtk();// ����opengl
}

void mainWindow::Field2light()
{
	int Index = rightSelectItem->data(2, Qt::UserRole).toInt();

	PVVA pvva;
	// ���õ�λ
	pvva.setUnit(Unit_factor);
	// ����Դ�������ڴ�
	pvva.setSource(&fieldHash[Index]); //?
	pvva.CalPlane(0.01); // ��ǰ�ƽ�0.01�׺ü�����Ӧ͢ʸ��
	pvva.Poynting();

	GraphTrans  _graphTransPara;
	int N_width; int M_depth;
	double ds;
	fieldHash[Index].getSourcePara(_graphTransPara, N_width, M_depth, ds);
	if (tempRaySource == NULL)
		tempRaySource = new RaySource;
	tempRaySource->setNM(N_width, M_depth);
	tempRaySource->setGraphTrans(_graphTransPara);
	tempRaySource->setType(3);
	tempRaySource->setLength(1.0f);
	Vector3 ** _org; Vector3 ** _n;
	tempRaySource->getPlane(_org, _n); // �õ�plane��ָ��

	pvva.getPlane(_org, _n);// �޸�plane���ڴ�
	tempRaySource->copyPlane();
	tempRaySource->calRayActor();
	renderer->AddActor(tempRaySource->getRayActor());

	raySourceHash[LightSourceNum] = tempRaySource;
	addTreeChild(lightTreeItem, "By "+ rightSelectItem->text(0),
		"Qt/images/lightSource.png", LIGHTSOURCE, LightSourceNum, 2);

	LightSourceNum++;
	tempRaySource = NULL;

	updateVtk();
}

void mainWindow::SaveField()
{
	QString filename = QFileDialog::getExistingDirectory(this,
		"Select the path to save field...", "./");
	if (!filename.isEmpty())
	{
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		fieldHash[Index].save(filename.toStdString() + "//" +
			rightSelectItem->text(0).toStdString());
	}
}

void mainWindow::SetTransparency()
{
	int varName = rightSelectItem->data(1, Qt::UserRole).toInt();
	int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
	if (transparencyAction->isChecked()) //  ѡ���Ƿ�͸��
	{
		modleDataList.getActor(varName, varIndex)->GetProperty()->SetOpacity(0.4);
		modleDataList.setTransparency(varName, varIndex, true);
	}
	else
	{
		modleDataList.getActor(varName, varIndex)->GetProperty()->SetOpacity(1);
		modleDataList.setTransparency(varName, varIndex, false);

	}

	updateVtk();
}

void mainWindow::showDetails(int name, int index)
{
	if (name == GAUSSIAN || name == APERTURE || name == RESFIELD)
	{
		int index = leftSelectItem->data(2, Qt::UserRole).toInt();
		int content; bool isLinear; bool isPhs;
		fieldHash[index].getShowPara(content, isLinear, isPhs);

		switch (fieldHash[index].get3D())
		{
		case 0:
			TwoDBtn->setChecked(true);
			phaseBtn->setEnabled(true);
			magnitudeBtn->setEnabled(true);
			break;
		case 1:
			ThreeDBtn->setChecked(true);
			magnitudeBtn->setChecked(true);
			phaseBtn->setEnabled(false);
			magnitudeBtn->setEnabled(false);
			break;
		default:
			break;
		}

		switch (content)
		{
		case 0:
			ExBtn->setChecked(true);
			break;
		case 1:
			EyBtn->setChecked(true);
			break;
		case 2:
			EzBtn->setChecked(true);
			break;
		case 3:
			HxBtn->setChecked(true);
			break;
		case 4:
			HyBtn->setChecked(true);
			break;
		case 5:
			HzBtn->setChecked(true);
			break;
		default:
			break;
		}

		switch (isPhs)
		{
		case true:
			phaseBtn->setChecked(true);
			linearBtn->setEnabled(false);
			dbBtn->setEnabled(false);
			break;
		case false:
			magnitudeBtn->setChecked(true);
			linearBtn->setEnabled(true);
			dbBtn->setEnabled(true);
			break;
		default:
			break;
		}
	
		switch (isLinear)
		{
		case true:
			linearBtn->setChecked(true);
			break;
		case false:
			dbBtn->setChecked(true);
			break;
		default:
			break;
		}

		if (name == GAUSSIAN || name == APERTURE)  // �����Դ EzHxHyHz ���õ�
		{
			EzBtn->setEnabled(false);
			HxBtn->setEnabled(false);
			HyBtn->setEnabled(false);
			HzBtn->setEnabled(false);
		}
		else
		{
			EzBtn->setEnabled(true);
			HxBtn->setEnabled(true);
			HyBtn->setEnabled(true);
			HzBtn->setEnabled(true);
		}
	}
	showDetails();
}

void mainWindow::createImportAction()
{
	ImportWidget ImportDialog;
	ImportDialog.exec();
	if (ImportDialog.Num() == 1)
	{
		if (tempModleData == NULL)
			tempModleData = new ModleData(IMPORT, ImportNum, 8);
		ImportDialog.getWidgetData(*tempModleData);
		tempModleData->readImport();
		tempActor = tempModleData->getActor();
		tempActor->GetProperty()->SetOpacity(1);
		tempActor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		renderer->AddActor(tempActor);

		modleDataList.pushModle(*tempModleData);

		delete tempModleData;
		tempModleData = NULL;

		addTreeChild(geometryTreeItem, tr("IMPORT") + QString::number(ImportNum+1),
			"Qt/images/IMPORT.png", IMPORT, ImportNum);

		tempActor = NULL;

		ImportNum++;
	}
	updateVtk();
}

void mainWindow::changeUnit()
{
	UnitWidget * dialog;
	if (unitBtn->text() == "Unit: mm")
		dialog = new UnitWidget(this, 0);
	else if (unitBtn->text() == "Unit: cm")
		dialog = new UnitWidget(this, 1);
	else if (unitBtn->text() == "Unit: m")
		dialog = new UnitWidget(this, 2);
	dialog->exec();
	if (dialog->Num() != -1)
	{
		Unit_factor = dialog->getUnitFactor();
		switch (dialog->Num())
		{
		case 1:
			unitBtn->setText(tr("Unit: mm"));
			break;
		case 2:
			unitBtn->setText(tr("Unit: cm"));
			break;
		case 3:
			unitBtn->setText(tr("Unit: m"));
			break;
		default:
			break;
		}

	}
}

void mainWindow::updateProgressDialog()
{
	currentValue+=2;
	if (currentValue >= 100)
		currentValue = 0;
	progDlg->setValue(currentValue);
	QCoreApplication::processEvents();//������涳��
	if (progDlg->wasCanceled())
		progDlg->setHidden(true);//���ضԻ���
}

void mainWindow::Gaussian_Thread()
{
	tempActor = tempField->getActor3D();

	renderer->AddActor(tempActor);
	updateVtk();

	
	
	if (tempIndex == 1)
	{
		fieldHash[FieldNum] = *tempField;

		addTreeChild(sourceTreeItem, GaussianDialog->getName(),
			"Qt/images/Gaussian.png", GAUSSIAN, FieldNum,1);
		source = new Source(*tempSource);

		delete tempSource;
		tempSource = NULL;
		FieldNum ++;
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;
	}
	else if (tempIndex == 3)
	{
		//renderer->RemoveActor(tempActor); // ����ʾ��ͷ
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = fieldHash[Index].getActor3D();
		renderer->RemoveActor(tempActor);
		fieldHash[Index] = *tempField;

		source = new Source(*tempSource);
		delete tempSource;
		tempSource = NULL;

		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;
	}

}

void mainWindow::receiveField(Field * field)
{
	timer->stop();//ֹͣ��ʱ��
	if (currentValue != 100)
		currentValue = 100;
	progDlg->setValue(currentValue);//���ȴﵽ���ֵ
	delete progDlg;//�رս��ȶԻ���
	delete timer;

	field->set3D(false);
	field->calActor();
	renderer->AddActor(field->getActor());

	fieldHash[FieldNum] = *field;
	
	updateVtk();
	delete field;
	QString name = calName.front();
	calName.pop();
	addTreeChild(sourceTreeItem, name, "Qt/images/Field.png", RESFIELD, FieldNum++, 1);
	FieldNum++;
}

void mainWindow::addTreeChild(QTreeWidgetItem *treeFather, QString treeName,
	QString iconFile, int name, int index, int rightPar)
{
	QTreeWidgetItem *child;
	child = new QTreeWidgetItem;
	child->setText(0, treeName);
	child->setIcon(0, QIcon(iconFile));

	QVariant var0(rightPar);        //�����Ҽ�����
	QVariant var1(name);
	QVariant var2(index);
	child->setData(0, Qt::UserRole, var0);
	child->setData(1, Qt::UserRole, var1);
	child->setData(2, Qt::UserRole, var2);
	treeFather->addChild(child);

	modelTreeItem->setExpanded(true);
	treeFather->setExpanded(true);
}

void mainWindow::updateVtk()
{
	auto window = widget.GetRenderWindow();

	window->AddRenderer(renderer);
	window->Render();
}

void mainWindow::clearCheckActor()
{
	while (!stackCheckActor.empty())
	{
		renderer->RemoveActor(stackCheckActor.top());
		stackCheckActor.pop();
	}
}
