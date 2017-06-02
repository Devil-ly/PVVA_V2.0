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
	void setLength(float dis); // ���ó���

	void getPlane(Vector3 **&_org, Vector3 **&_n);
	void copyPlane(); // ��������ݷ������ѭ����
	void calRayTrace();
	void setSurface(ModleData * modleData); // ��������Ķ�������
	void setSurface(vtkSmartPointer<vtkPolyData> polyData); // ����������Դ

	void calRayActor(); // ����Ray
	vtkSmartPointer<vtkActor> getRayActor();
	list<vtkActor*> getActors();

	void clearActor();
	void clearVector3(); // �������delete
	
private:

	/****************************************************************************                       
	*  0 ~ 6 λ����Ϣ 
	*  7 width    8 depth(���κ͵���ʱ)        7 angle    8 ������ (��Դʱ��                 
	*  9 widthGap 10 depthGap(���κ͵���ʱ)	   9 thetaGap 10 phiGap(��Դʱ)        
	*  11 length                               11 length                
	*****************************************************************************/
	vector<float> data;
	int type;// 0 ���� 2 ��Դ 1 ����  3 Field ����

	Vector3 **org; // ��ʼ��
	Vector3 **n; // ��ʼ����

	//������̵���ʱ
	Vector3 **tempOrg; // ��ʼ��
	Vector3 **tempN; // ��ʼ����

	vtkSmartPointer<vtkActor> RayActor; 
	list<vtkActor*> actors; // ����׷�ٺ�Ĺ���
	int N, M;
};

#endif