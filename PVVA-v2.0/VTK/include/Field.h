#pragma once
#ifndef FIELD_H
#define FIELD_H

#include <gl/glut.h>
#include <vector>
#include <fstream>
#include <complex>
#include <String>
#include "Tools/GEMS_Memory.h"
#include "Tools/GraphTrans.h"
//#include "openGL/include/Gaussian.h"
#include "Tools/Vector3.h"
#include "../include/Source.h"

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>

#include <QtWidgets/QDialog>

class Field
{
	//Q_OBJECT
public:
	Field();
	Field(const Field & _field); // 重写构造函数
	Field& operator =(const Field & _field);
	//Field(Gaussian * const _gaussian); // 加入一个高斯源;

	~Field();

	void init();

	void freeMemory();
	void allocateMemory();

	void setNM(int N, int M);
	void setPlane(const GraphTrans & _graphTransPara, double _ds);

	void setField(Source const * _source);
	void setField(complex<double> ** _Ex, complex<double> ** _Ey, complex<double> ** _Ez,
		complex<double> ** _Hx, complex<double> ** _Hy, complex<double> ** _Hz);

	complex<double> ** getEx() const;
	complex<double> ** getEy() const;

	//得到场的参数
	void getSourcePara(GraphTrans & _graphTransPara, int & _N_width, int & _M_depth,
		double &_ds) const;

	void setShowPara(int _content, bool _isLinear, bool _isPhs);
	void getShowPara(int &_content, bool &_isLinear, bool &_isPhs);
	void set3D(bool _is3D);
	bool get3D() const;

	void calActor3D();
	void calActor();
	void resetActor();
	void resetActor3D();

	vtkSmartPointer<vtkImageActor> getActor() const;
	vtkSmartPointer<vtkActor> getActorArrow();
	vtkSmartPointer<vtkActor> getActor3D() const;

	double getWidth() const { return para[7]; }
	double getDepth() const { return para[8]; }

	void save(const std::string &fileName) const;
	//void draw() const;

private:
	void setDrawData(complex<double> ** data);
	void setRGB(float var) const;

private:
	double* para; // 模型参数0~6旋转参数 7 width 8 depth 

	Vector3 ** Plane;  // 平面各点的坐标（绝对坐标）

	complex<double> ** Ex, ** Ey, ** Ez;	// 场分量
	complex<double> ** Hx, ** Hy, ** Hz;

	int content;
	bool isLinear;
	bool isPhs;
	// 需要画图的内容 content 0~5表示Ex~H
	// isLinear 是否是线性
	// isPhs 是相位 还是幅度

	int N_width, M_depth;  //N_width = para[0] /ds

	double ds;
	
	bool isSource;
	vtkSmartPointer<vtkImageActor> actor;
	vtkSmartPointer<vtkActor> actorArrow;
	vtkSmartPointer<vtkActor> actor3D;
	bool isNew; // 如果是new则需要计算得到actor
	bool is3D;
};

#endif // !DRAW_H

