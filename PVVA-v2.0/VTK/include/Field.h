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
	Field(const Field & _field); // ��д���캯��
	Field& operator =(const Field & _field);
	//Field(Gaussian * const _gaussian); // ����һ����˹Դ;

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

	//�õ����Ĳ���
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
	double* para; // ģ�Ͳ���0~6��ת���� 7 width 8 depth 

	Vector3 ** Plane;  // ƽ���������꣨�������꣩

	complex<double> ** Ex, ** Ey, ** Ez;	// ������
	complex<double> ** Hx, ** Hy, ** Hz;

	int content;
	bool isLinear;
	bool isPhs;
	// ��Ҫ��ͼ������ content 0~5��ʾEx~H
	// isLinear �Ƿ�������
	// isPhs ����λ ���Ƿ���

	int N_width, M_depth;  //N_width = para[0] /ds

	double ds;
	
	bool isSource;
	vtkSmartPointer<vtkImageActor> actor;
	vtkSmartPointer<vtkActor> actorArrow;
	vtkSmartPointer<vtkActor> actor3D;
	bool isNew; // �����new����Ҫ����õ�actor
	bool is3D;
};

#endif // !DRAW_H

