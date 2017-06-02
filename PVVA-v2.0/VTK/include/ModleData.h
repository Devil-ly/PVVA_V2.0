#pragma once
/***************************************
保存模型数据
data[0]~data[6] 保存transfer信息
模型信息在data[7]后
***************************************/

#ifndef MODLEDATA_H
#define MODLEDATA_H

#define PARABOLOID 0x00
#define LIGHTSOURCE 0x01
#define RECTANGLE 0x02
#define CYLINDER 0x03
#define BOOLEANOP 0x04
#define STLINPUT 0x05
#define SPHERE 0x06
#define ARROW 0x07 
#define REVERSECONE 0x08 // 倒置的cone
#define ARBITRARYSURFACE 0x09
#define IMPORT 0x0A

//#define APERTUREFIELD 0x03
//#define GAUSSIAN 0x04

#include "Tools/GraphTrans.h"
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <string>
#include "Tools/Vector3.h"
#include "Calculation/include/RayTrace.h"

class ModleData: public RayTrace
{
public:
	ModleData();
	ModleData(int _name, int _index, int _dataSize);
	ModleData(const ModleData & _ModleData); // 重写构造函数
	~ModleData();

	ModleData& operator = (const ModleData & _ModleData);

	void setData(float * _data); // 加入一个元素
	void setNameFile(const string & file);
	void setGraphTrans(const GraphTrans & graphtrans);

	void setBoolean(int _name1, int _index1, int _name2, int _index2,
		ModleData *a, ModleData *b, int operation);

	void setName(int _name); 
	int getName() const;
	int getIndex() const;

	int getDataSize() const;
	float * getData() const;

	void setTransparent(bool is = false); // 设置透明
	bool getTransparent() const; // 设置透明
	
	string getNameFile() const;
	GraphTrans getGraphTrans() const;

	vtkSmartPointer<vtkActor> getActor();
	static vtkSmartPointer<vtkActor> getLineActor(const Vector3 & p0, const Vector3 & p1);
	static vtkSmartPointer<vtkActor> getRectangleActor(const GraphTrans & graphTrans, float w, float d);

	//得到图形的几何数据 ds为分辨率 在画图的情况较小为0.05
	vtkSmartPointer<vtkPolyData> getPolyData(double ds = 0.05) const;
	
	void readImport();
private:
	vtkSmartPointer<vtkPolyData> drawParaboloid(double ds) const;
	vtkSmartPointer<vtkPolyData> drawRectangle(double ds) const;
	vtkSmartPointer<vtkPolyData> drawCylinder(double ds) const;
	vtkSmartPointer<vtkPolyData> drawSphere(double ds) const;
	vtkSmartPointer<vtkPolyData> drawArrow() const;
	vtkSmartPointer<vtkPolyData> drawSTLInput() const;
	vtkSmartPointer<vtkPolyData> drawReverseCone() const;
	vtkSmartPointer<vtkPolyData> drawCustom(double ds) const;

	void transformModle(vtkSmartPointer<vtkPolyData> polyData);

	// 布尔运算 operation = 0 表示交集
	vtkSmartPointer<vtkPolyData> boolean(ModleData *a, ModleData *b, int operation);

	int name;  // 模型名称
	int index; // 模型编号

	//GraphTrans* graphTrans;

	int dataSize; // 数据长度
	float * data;
	string namefile;

	vtkSmartPointer<vtkActor> actor; //  显示数据
	vtkSmartPointer<vtkPolyData> polyData;   // 计算数据

	bool isNew; // 如果是new则需要计算得到actor

	bool isTransparent; // 是否透明
};

#endif // !MODLEDATA_H