/*
*  图形的变换（平移和旋转）
*  
*  成员函数 平移的到某个点 旋转绕某个轴 和 角度
*/
#pragma once
#ifndef GRAPHTRANS_H
#define GRAPHTRANS_H
#include <fstream>
#include "Tools/Vector3.h"
using namespace std;
class GraphTrans
{
public:
	GraphTrans()
	{
		trans_x = 0;
		trans_y = 0;
		trans_z = 0;
		rotate_x = 1;
		rotate_y = 0;
		rotate_z = 0;
		rotate_theta = 0;
	}
	void setGraphTransPar(const float& _trans_x, const float& _trans_y, const float& _trans_z,
		const float& _rotate_x, const float& _rotate_y, const float& _rotate_z, const float& _rotate_theta)
	{
		trans_x = _trans_x;
		trans_y = _trans_y;
		trans_z = _trans_z;
		rotate_x = _rotate_x;
		rotate_y = _rotate_y;
		rotate_z = _rotate_z;
		rotate_theta = _rotate_theta;
	}
	void getGraphTransPar(float &_trans_x, float &_trans_y, float &_trans_z,
		float &_rotate_x, float &_rotate_y, float &_rotate_z, float &_rotate_theta) const
	{
		_trans_x = trans_x;
		_trans_y = trans_y;
		_trans_z = trans_z;
		_rotate_x = rotate_x;
		_rotate_y = rotate_y;
		_rotate_z = rotate_z;
		_rotate_theta = rotate_theta;
	}
	
	void normalization(float fator) //变为标准单位 m
	{
		trans_x *= fator;
		trans_y *= fator; 
		trans_z *= fator;
	} 

	float getTrans_x() const { return trans_x; }
	float getTrans_y() const { return trans_y; }
	float getTrans_z() const { return trans_z; }

	float getRotate_x() const { return rotate_x; }
	float getRotate_y() const { return rotate_y; }
	float getRotate_z() const { return rotate_z; }

	float getRotate_theta() const { return rotate_theta; }

	void updateRotate(Vector3 rotate, float theta)
	{
		rotate_x = rotate.x;
		rotate_y = rotate.y;
		rotate_z = rotate.z;
		rotate_theta = theta;
	}

	void updateTranslate(Vector3 tran)
	{
		trans_x = tran.x;
		trans_y = tran.y;
		trans_z = tran.z;
	}

	void updateGraphTransPar(Vector3 tran, float theta)
	{
		trans_x = tran.x;
		trans_y = tran.y;
		trans_z = tran.z;
		rotate_theta = theta;
	}

	void save(ofstream & savefile) const
	{
		savefile << trans_x << " "
			<< trans_y << " "
			<< trans_z << " "
			<< rotate_x << " "
			<< rotate_y << " "
			<< rotate_z << " "
			<< rotate_theta << endl;
	}

	void open(ifstream & readfile)
	{
		readfile >> trans_x >> trans_y >> trans_z >> rotate_x
			>> rotate_y >> rotate_z >> rotate_theta;

	}
private:
	float trans_x, trans_y, trans_z; // 平移的点
	float rotate_x, rotate_y, rotate_z; // 旋转轴
	float rotate_theta; // 旋转角度 右手定则
};

#endif // !GRAPHTRANS_H

