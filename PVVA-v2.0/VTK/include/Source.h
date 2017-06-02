#pragma once
#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include <complex>
#include <vector>
#include <QString>
#include "Tools/GEMS_Memory.h"
#include "Tools/Constant_Var.h"
#include "Tools/GraphTrans.h"

#define GAUSSIAN 0x1000
#define APERTURE 0x1001
#define RESFIELD 0x1002
//#define LIGHTSOURCE 0x1001
using namespace std;

class Source  // 待修改
{
public:
	Source(int _type = APERTURE);
	Source(const Source & _source);
	//Source(int type, float * _data);
	~Source();

	void setData(double * _data, int n);
	vector<double> getData() const;
	void setGraphTrans(const GraphTrans & graphtrans);
	GraphTrans getGraphTrans() const;
	void setFileAddress(const QString & file);
	void setNM(int _N, int _M);

	void getNM(int & N, int & M) const;
	void getPlaneData(double *para) const;
	void setDs(double _Ds);
	double getDs() const;
	void setUnit(double _unit);


	void getExEy(complex<double>** Ex_In, complex<double>** Ey_In) const;

private:

	void CreateGaussianSource(complex<double>** Ex_In, complex<double>** Ey_In) const;
	void Source::CreateGaussian(double r, double w0, double fre, double z0,
		double &Real, double &Imag) const;

	//double * data; // 保存数据信息
	/****************************************************************************
	*     APERTURE                         GAUSSIAN
	*  0 ~ 6 位置信息                    0 ~ 6 位置信息
	*  7 width 8 depth                   7 width 8 depth
	*  9 scale 10 phase			         9 z0   10 w0
	*  11 StartNum  12 Ds                11 fre   12 Ds
	*  13 datadef       
    *****************************************************************************/
	vector<double> data;
	
	string fileAddress; // 文件地址
	int N; 
	int M;
	int type;
	double factor;
	
};




#endif