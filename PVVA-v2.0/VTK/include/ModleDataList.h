#pragma once
/***************************************
保存所有模型数据
提供模型数据的增删改查功能
***************************************/

#ifndef MODLEDATALIST_H
#define MODLEDATALIST_H

#include <list>
#include "VTK/include/ModleData.h"
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>

using namespace std;

class ModleDataList
{
public:
	ModleDataList();
	~ModleDataList();

	void pushModle(const ModleData &modleData); // 添加一个modle
	void modifyModle(const ModleData &modleData); // 修改一个modle
	void removeModle(int name, int index); // 删除一个modle
	void removeModle_Actor(int name, int index, vtkSmartPointer<vtkRenderer> renderer);
	void findModle(int name, int index, ModleData &modleData) const;
	vtkSmartPointer<vtkPolyData> getPolyData(int name, int index, double ds = 0.05) const; // 得到模型几何数据
	vtkSmartPointer<vtkActor> getActor(int name, int index);

	void draw(vtkSmartPointer<vtkRenderer> renderer);
	bool getTransparency(int name, int index) const; //得到模型的透明度
	void setTransparency(int name, int index, bool is);

private:
	list<ModleData> data;
};
#endif // !MODLEDATALIST_H
