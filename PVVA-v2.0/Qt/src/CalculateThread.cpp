#include "qt/include/CalculateThread.h"

void CalculateThread::run()
{
	double Mesh;
	if (meshsize == 0)
		Mesh = 0.005;
	else if (meshsize == 2)
		Mesh = 0.02;
	else
		Mesh = 0.01;
	//workField->getActor3D();
	// **********开始计算***********
	PVVA pvva;
	// 设置单位
	pvva.setUnit(Unit_factor);
	// 设置频率
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(fieldSource);
	if (calmodel == 2)
	{
		ModleData tempData;
		modleDataList->findModle(nameSel[0], indexSel[0], tempData);
		pvva.setModle(modleDataList->getPolyData(nameSel[1], indexSel[1], Mesh));
		pvva.setAnalytic_Modle(&tempData);
		pvva.Analytic_CalZ0Theta();
		pvva.Analytic_Mesh_Reflect();
		pvva.InterVal();
	}
	else
	{
		for (int i = 0; i < Num; i++)
		{
			if (calmodel == 1 && nameSel[i] == PARABOLOID)// 解析解优先
			{
				ModleData tempData;
				modleDataList->findModle(nameSel[i], indexSel[i], tempData);
				pvva.setAnalytic_Modle(&tempData);
				pvva.Analytic_CalZ0Theta();
				pvva.Analytic_Reflect();
			}
			else
			{
				pvva.setModle(modleDataList->getPolyData(nameSel[i], indexSel[i], Mesh));
				pvva.CalZ0Theta();
				pvva.Reflect();
			}
			pvva.InterVal();
		}
	}

	pvva.Result(dis);
	Field *tempField = new Field;
	pvva.getField(*tempField);
	emit sendField(tempField);
	delete[] nameSel;
	delete[] indexSel;
}


