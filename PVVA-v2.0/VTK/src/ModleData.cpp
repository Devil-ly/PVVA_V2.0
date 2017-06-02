#include "VTK/include/ModleData.h"
#include <vtkSmartPointer.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellArray.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkPlaneSource.h>
#include <vtkTriangleFilter.h>
#include <vtkCylinderSource.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkSTLReader.h>
#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkLineSource.h>
#include <vtkConeSource.h>
#include <vtkTriangle.h>

#include <fstream>
#include "Calculation/include/Matrix4D.h"
#include "Calculation/include/Vector3D.h"

ModleData::ModleData()
{
	name = -1;
	index = -1;
	dataSize = 7;
	data = nullptr;
	isNew = true;
	isTransparent = false;
	namefile = "";
}

ModleData::ModleData(int _name, int _index, int _dataSize)
{
	name = _name;
	index = _index;
	dataSize = _dataSize;
	data = nullptr;
	isNew = true;
	namefile = "";
	isTransparent = false;
}

ModleData::ModleData(const ModleData & _ModleData)
:name(_ModleData.name), index(_ModleData.index), dataSize(_ModleData.dataSize)
,actor(_ModleData.actor),isNew(_ModleData.isNew), namefile(_ModleData.namefile),
isTransparent(_ModleData.isTransparent), polyData(_ModleData.polyData)
{
	if (_ModleData.data != nullptr)
	{
		data = new float[dataSize];
		for (int i = 0; i < dataSize; i++)
			data[i] = _ModleData.data[i];
	}
	else
		data = nullptr;	
}

ModleData::~ModleData()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}

ModleData& ModleData::operator=(const ModleData & _ModleData)
{
	if (this != &_ModleData)
	{
		name = _ModleData.name;
		index = _ModleData.index;
		dataSize = _ModleData.dataSize;
		actor = _ModleData.actor;
		isNew = _ModleData.isNew;
		namefile = _ModleData.namefile;
		isTransparent = _ModleData.isTransparent;
		if (data != nullptr)
			delete[] data;
		if (_ModleData.data != nullptr)
		{
			data = new float[dataSize];
			for (int i = 0; i < dataSize; i++)
				data[i] = _ModleData.data[i];
		}
		else
			data = nullptr;
	}
	return *this;
}

void ModleData::setData(float * _data)
{
	if (data != nullptr)
		delete[] data;
	data = _data;
	isNew = true;
}

void ModleData::setNameFile(const string & file)
{
	namefile = file;
	isNew = true;
}

void ModleData::setGraphTrans(const GraphTrans & graphtrans)
{
	data[0] = graphtrans.getTrans_x();
	data[1] = graphtrans.getTrans_y();
	data[2] = graphtrans.getTrans_z();
	data[3] = graphtrans.getRotate_x();
	data[4] = graphtrans.getRotate_y();
	data[5] = graphtrans.getRotate_z();
	data[6] = graphtrans.getRotate_theta();
	isNew = true;
}

void ModleData::setBoolean(int _name1, int _index1, int _name2, int _index2, 
	ModleData *a, ModleData *b, int operation)
{
	data = new float[5];
	data[0] = _name1;
	data[1] = _index1;
	data[2] = _name2;
	data[3] = _index2;
	data[4] = operation;
	isNew = true;
	boolean(a, b, operation);
	/*
	//可视化,设置mapper、actor
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//    mapper->SetInputConnection(contourFilter->GetOutputPort());
	mapper->SetInputData(boolean(a, b, operation));
	mapper->ScalarVisibilityOff();

	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	isNew = false; // 已计算
   */
}

void ModleData::setName(int _name)
{
	name = _name;
}

int ModleData::getName() const
{
	return name;
}

int ModleData::getIndex() const
{
	return index;
}

int ModleData::getDataSize() const
{
	return dataSize;
}

float * ModleData::getData() const
{
	return data;
}

void ModleData::setTransparent(bool is)
{
	isTransparent = is;
}

bool ModleData::getTransparent() const
{
	return isTransparent;
}

string ModleData::getNameFile() const
{
	return namefile;
}

GraphTrans ModleData::getGraphTrans() const
{
	GraphTrans graphTrans;
	graphTrans.setGraphTransPar(data[0], data[1], data[2],
		data[3], data[4], data[5], data[6]);
	return graphTrans;
}

vtkSmartPointer<vtkActor> ModleData::getActor()
{
	if (isNew)
	{
		//可视化,设置mapper、actor
		vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		//    mapper->SetInputConnection(contourFilter->GetOutputPort());
		mapper->SetInputData(getPolyData(0));
		mapper->ScalarVisibilityOff();

		actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
		isNew = false; // 已计算
	}
	
	return actor;
}

vtkSmartPointer<vtkActor> ModleData::getLineActor(const Vector3 & p0, const Vector3 & p1)
{
	vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
	line->SetPoint1(p0.x, p0.y, p0.z);
	line->SetPoint2(p1.x, p1.y, p1.z);

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(line->GetOutputPort());
	//mapper->SetInputData(line-);
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actorline = vtkSmartPointer<vtkActor>::New();
	actorline->SetMapper(mapper);
	actorline->GetProperty()->SetColor(1, 0, 0);
	return actorline;
}

vtkSmartPointer<vtkActor> ModleData::getRectangleActor(const GraphTrans & graphTrans, float w, float d)
{
	w /=  2.0;
	d /=  2.0;
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
	//plane->SetCenter(0, 0, 0);
	plane->SetOrigin(-w, -d, 0);
	plane->SetPoint1(w, -d, 0);
	plane->SetPoint2(-w, d, 0);
	plane->Update();

	vtkSmartPointer<vtkPolyData> polyData = plane->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(graphTrans.getTrans_x(), graphTrans.getTrans_y(), graphTrans.getTrans_z());
	transform->RotateWXYZ(graphTrans.getRotate_theta(), graphTrans.getRotate_x(),
		graphTrans.getRotate_y(), graphTrans.getRotate_z());

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(line->GetOutputPort());
	mapper->SetInputData(TransFilter->GetOutput());
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actorline = vtkSmartPointer<vtkActor>::New();
	actorline->SetMapper(mapper);
	actorline->GetProperty()->SetColor(1, 0, 1);
	actorline->GetProperty()->SetOpacity(0.4);
	return actorline;
}

vtkSmartPointer<vtkPolyData> ModleData::getPolyData(double ds) const
{
	if (name == PARABOLOID)
	{
		//return drawCustom(ds);
		return drawParaboloid(ds);
	}
	else if (name == RECTANGLE)
	{
		return drawRectangle(ds);
	}
	else if (name == CYLINDER)
	{
		return drawCylinder(ds);
	}
	else if (name == STLINPUT)
	{
		return drawSTLInput();
	}
	else if (name == SPHERE)
	{
		return drawSphere(ds);
	}
	else if (name == ARROW)
	{
		return drawArrow();
	}
	else if (name == REVERSECONE)
	{
		return drawReverseCone();
	}
	else if (name == BOOLEANOP)
	{
		return polyData;
	}
	else if (name == IMPORT)
	{
		return polyData;
	}
}

void ModleData::readImport()
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkTriangle> p1 = vtkSmartPointer<vtkTriangle>::New();
	vtkSmartPointer<vtkCellArray>pLineCell = vtkCellArray::New();

	double x, y, z;
	ifstream file(namefile);
	int counter = 0;
	for (int i = 0; i < data[7]; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			file >> x >> y >> z;
			points->InsertNextPoint(x, y, z);
		}
		p1->GetPointIds()->SetId(0, counter++);
		p1->GetPointIds()->SetId(1, counter++);
		p1->GetPointIds()->SetId(2, counter++);
		pLineCell->InsertNextCell(p1);
	}
	file.close();
	polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points); //获得网格模型中的几何数据：点集  
	polyData->SetStrips(pLineCell);
}

vtkSmartPointer<vtkPolyData> ModleData::drawParaboloid(double ds) const
{
	float radius = data[7];
	float temp = -4 * data[8]; // data[7] 表示焦距
	vtkSmartPointer<vtkQuadric>quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(1, 1, 0, 0, 0, 0, 0, 0, temp, 0);

	//二次函数采样分辨率
	vtkSmartPointer<vtkSampleFunction>sample = vtkSmartPointer<vtkSampleFunction>::New();
	if (ds == 0)
		sample->SetSampleDimensions(40, 40, 20);
	else
		sample->SetSampleDimensions(int(radius / ds) * 2, 
			int(radius / ds) * 2,int(-temp / ds) * 2); // 采样点和ds有关
	sample->SetImplicitFunction(quadric);
	//double xmin = -radius * 2, xmax = radius * 2, ymin = -radius * 2,
	//	ymax = radius * 2, zmin = 1.0f / temp, zmax = -radius / temp + 1.0f / temp;

	double xmin = data[9], ymin = data[10], 
		zmin = 1.0f / temp+ data[11],
		xmax = data[12], ymax = data[13], 
		zmax = -radius / temp + data[14];

	sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);
	//sample->SetModelBounds(-0.5, 0.5, -0.5, 0.5, zmin, zmax);
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(sample->GetOutputPort());
	contourFilter->GenerateValues(1, 1, 1);
	contourFilter->Update();

	vtkSmartPointer<vtkPolyData> polyData = contourFilter->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	transform->Translate(0, 0, -zmin);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	//polyData
	return polyData;

}

vtkSmartPointer<vtkPolyData> ModleData::drawRectangle(double ds)const
{
	float w = data[7] / 2.0;
	float d = data[8] / 2.0;
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
	//plane->SetCenter(0, 0, 0);
	plane->SetOrigin(-w, -d, 0);
	plane->SetPoint1(w, -d, 0);
	plane->SetPoint2(-w, d, 0);
	plane->Update();

	vtkSmartPointer<vtkPolyData> polyData = plane->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();
	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(polyData);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter->Update();
	polyData = triangleFilter->GetOutput();
	return polyData;
}

vtkSmartPointer<vtkPolyData> ModleData::drawCylinder(double ds) const
{
	float r = data[7];
	float h = data[8];
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	//plane->SetCenter(0, 0, 0);
	cylinder->SetHeight(h);
	cylinder->SetRadius(r);
	cylinder->SetCenter(0, h / 2, 0);
	cylinder->SetResolution(40);
	cylinder->Update();

	vtkSmartPointer<vtkPolyData> polyData = cylinder->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)

	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	transform->RotateWXYZ(90, 1, 0, 0);
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(polyData);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter->Update();
	polyData = triangleFilter->GetOutput();

	return polyData;
}

vtkSmartPointer<vtkPolyData> ModleData::drawSphere(double ds) const
{
	float r = data[7];
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	//plane->SetCenter(0, 0, 0);
	
	sphere->SetRadius(r);
	sphere->SetCenter(data[0], data[1], data[2]);
	sphere->SetThetaResolution(40);
	sphere->SetPhiResolution(40);
	sphere->Update();

	vtkSmartPointer<vtkPolyData> polyData = sphere->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)

	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(polyData);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter->Update();
	polyData = triangleFilter->GetOutput();

	return polyData;
}

vtkSmartPointer<vtkPolyData> ModleData::drawArrow() const
{
	vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
	//plane->SetCenter(0, 0, 0);
	arrow->SetTipLength(0.1);
	arrow->SetTipRadius(0.02);
	arrow->SetShaftRadius(0.01);
	arrow->Update();

	vtkSmartPointer<vtkPolyData> polyData = arrow->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)

	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	//polyData = 
	return TransFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> ModleData::drawSTLInput() const
{
	vtkSmartPointer<vtkSTLReader> reader =
		vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(namefile.c_str());
	reader->Update();

	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(reader->GetOutput());
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();

	return TransFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> ModleData::drawReverseCone() const
{
	vtkSmartPointer<vtkConeSource> cone = 
		vtkSmartPointer<vtkConeSource>::New();
	cone->SetAngle(data[7]);
	cone->SetResolution(20);
	cone->Update();

	vtkSmartPointer<vtkPolyData> polyData = cone->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	transform->Translate(0, 0, 0.5);
	transform->RotateWXYZ(90, 0, 1, 0);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	return polyData;
}

vtkSmartPointer<vtkPolyData> ModleData::drawCustom(double ds) const
{
	float radius = data[7];
	float temp = -4 * data[8]; // data[7] 表示焦距

	int last = 0;
	int now = 0;
	int couter = 0;
	ds = 0.01;
	int N = radius / ds * 2;
	int M = radius / ds * 2;

	double *a = new double[10];
	a[0] = 1; a[1] = 1; a[2] = 0;
	a[3] = 0; a[4] = 0; a[5] = 0;
	a[6] = 0; a[7] = 0; a[8] = temp;
	a[9] = 0;

	double x, y, z;
	double zmin = 0, zmax = -radius / temp;

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkTriangle> p1 = vtkSmartPointer<vtkTriangle>::New();
	vtkSmartPointer<vtkCellArray>pLineCell = vtkCellArray::New();

	for (int i = 0; i < N; i++)
	{
		x = i * ds - radius;
		for (int j = 0; j < M; j++)
		{
			y = j * ds - radius;
			if (getZ(a, x, y, zmin, zmax,z))
			{
				points->InsertNextPoint(x,y,z);
				now++;
				couter++;
			}
		}
		if (last > 0)
		{
			int temp_couter = couter - now - last;
			int j = 0, k = 0;
			for (; j < last - 1 && k < now; j++, k++)
			{

				p1->GetPointIds()->SetId(0, temp_couter + j);
				p1->GetPointIds()->SetId(1, temp_couter + j + 1);
				p1->GetPointIds()->SetId(2, temp_couter + last + k);
				pLineCell->InsertNextCell(p1);
			}
			while (j < last - 1)
			{
				p1->GetPointIds()->SetId(0, temp_couter + j);
				p1->GetPointIds()->SetId(1, temp_couter + j + 1);
				p1->GetPointIds()->SetId(2, temp_couter + last + k - 1);
				pLineCell->InsertNextCell(p1);
				j++;
			}

			j = 1, k = 0;
			for (; j < last && k < now - 1; j++, k++)
			{

				p1->GetPointIds()->SetId(0, temp_couter + j);
				p1->GetPointIds()->SetId(1, temp_couter + last + k);
				p1->GetPointIds()->SetId(2, temp_couter + last + k + 1);
				pLineCell->InsertNextCell(p1);
			}
			while (k < now - 1)
			{
				p1->GetPointIds()->SetId(0, temp_couter + j - 1);
				p1->GetPointIds()->SetId(1, temp_couter + last + k);
				p1->GetPointIds()->SetId(2, temp_couter + last + k + 1);
				pLineCell->InsertNextCell(p1);
				k++;
			}
		}
		last = now;
		now = 0;
		
	}
	vtkSmartPointer<vtkPolyData>pointsData = vtkSmartPointer<vtkPolyData>::New();
	pointsData->SetPoints(points); //获得网格模型中的几何数据：点集  
	pointsData->SetStrips(pLineCell);
	return pointsData;
}

void ModleData::transformModle(vtkSmartPointer<vtkPolyData> polyData)
{
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);

	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	//actor->GetProperty()->SetColor(0, 0, 1);
}

vtkSmartPointer<vtkPolyData> ModleData::boolean(ModleData * a, ModleData * b, int operation)
{
	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
		vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
	if (operation == 0)
	{
		booleanOperation->SetOperationToUnion();
	}
	else if (operation == 1)
	{
		booleanOperation->SetOperationToIntersection();
	}
	else if (operation == 2)
	{
		booleanOperation->SetOperationToDifference();
	}
	else if (operation == 3)
	{
		booleanOperation->SetOperationToIntersection();

		booleanOperation->SetInputData(0, a->getPolyData(0.01)); // set the input data  
		booleanOperation->SetInputData(1, b->getPolyData(0.01));
		booleanOperation->Update();

		vtkSmartPointer<vtkPolyData> polyData = booleanOperation->GetOutput();

		float * surfaceData;
		if (a->getName() == PARABOLOID)
			surfaceData = a->getData();
		else
			surfaceData = b->getData();

		float temp = 4 * surfaceData[8]; // data[7] 表示焦距

		// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
		Vector3D RotateAsix(surfaceData[3], surfaceData[4], surfaceData[5]);
		Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-surfaceData[6], RotateAsix);
		//Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(-surfaceData[0], -surfaceData[1], -surfaceData[2]);

		Vector3D rotatTranslate(surfaceData[0], surfaceData[1], surfaceData[2]);
		rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
		Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));
		
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkTriangle> p1 = vtkSmartPointer<vtkTriangle>::New();
		vtkSmartPointer<vtkCellArray>pLineCell = vtkCellArray::New();

		int EleNum = polyData->GetNumberOfCells();
		int couter = 0; // 计数器
		for (int i = 0; i < EleNum; i++)
		{
			vtkIdList * p = polyData->GetCell(i)->GetPointIds();
			bool ok = true;
			for (int j = 0; j < 3; j++)
			{
				double * temppoint = polyData->GetPoint(p->GetId(j));
				Vector3 point = R_translateMatrix*R_rotatMatrix*
					Vector3(temppoint[0], temppoint[1], temppoint[2]);
				double tempz = (point.x * point.x + point.y * point.y) / temp;
				tempz = abs(tempz - point.z);
				if (tempz > 0.01)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				points->InsertNextPoint(polyData->GetPoint(p->GetId(0)));
				points->InsertNextPoint(polyData->GetPoint(p->GetId(1)));
				points->InsertNextPoint(polyData->GetPoint(p->GetId(2)));
				p1->GetPointIds()->SetId(0, couter++);
				p1->GetPointIds()->SetId(1, couter++);
				p1->GetPointIds()->SetId(2, couter++);
				pLineCell->InsertNextCell(p1);
			}		
		}

		this->polyData = vtkSmartPointer<vtkPolyData>::New();
		this->polyData->SetPoints(points); //获得网格模型中的几何数据：点集  
		this->polyData->SetStrips(pLineCell);

		return this->polyData;
	}
	booleanOperation->SetInputData(0, a->getPolyData()); // set the input data  
	booleanOperation->SetInputData(1, b->getPolyData());
	booleanOperation->Update();

	this->polyData = booleanOperation->GetOutput();
	return booleanOperation->GetOutput();
}

