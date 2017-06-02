#pragma once
#ifndef RAYSOURCE_H
#define RAYSOURCE_H

#include <vector>
#include "Tools/GraphTrans.h"
#include "Tools/Vector3.h"
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include "VTK/include/ModleData.h"
#include <list>
#include <Calculation/include/RayTrace.h>

using namespace std;

class RaySource: public RayTrace
{
public:
	RaySource();
	~RaySource();
	RaySource & operator = (const RaySource & right);

	void setGraphTrans(const GraphTrans & graphtrans);
	void setData(float * _data);
	void setType(int _type);
	void setNM(int _N, int _M);
	void setLength(float dis); // 设置长度

	void getPlane(Vector3 **&_org, Vector3 **&_n);
	void copyPlane(); // 将起点数据放入计算循环中
	void calRayTrace();
	void setSurface(ModleData * modleData); // 传入解析的二次曲面
	void setSurface(vtkSmartPointer<vtkPolyData> polyData); // 传入三角面源

	void calRayActor(); // 计算Ray
	vtkSmartPointer<vtkActor> getRayActor();
	list<vtkActor*> getActors();

	void clearActor();
	void clearVector3(); // 计算完后delete
	
private:

	/****************************************************************************                       
	*  0 ~ 6 位置信息 
	*  7 width    8 depth(矩形和导入时)        7 angle    8 无意义 (点源时）                 
	*  9 widthGap 10 depthGap(矩形和导入时)	   9 thetaGap 10 phiGap(点源时)        
	*  11 length                               11 length                
	*****************************************************************************/
	vector<float> data;
	int type;// 0 矩形 2 点源 1 导入  3 Field 生成

	Vector3 **org; // 起始点
	Vector3 **n; // 起始方向

	//计算过程的临时
	Vector3 **tempOrg; // 起始点
	Vector3 **tempN; // 起始方向

	vtkSmartPointer<vtkActor> RayActor; 
	list<vtkActor*> actors; // 保存追踪后的光线
	int N, M;
};

#endif