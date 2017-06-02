#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H

#include <QThread>

#include "Calculation/include/PVVA.h"
#include "VTK/include/ModleDataList.h"

class CalculateThread : public QThread
{
	Q_OBJECT

public:
	CalculateThread(double _fre, double _Unit_factor, int *_nameSel,int *_indexSel,
		int _Num, ModleDataList *_modleDataList, Field* _fieldSource, double _dis,
		double _width, double _depth, int _meshsize, int _calmodel)
	{
		fre = _fre;
		Unit_factor = _Unit_factor;
		nameSel = _nameSel;
		indexSel = _indexSel;
		modleDataList = _modleDataList;
		Num = _Num;
		fieldSource = _fieldSource;
		dis = _dis;
		width = _width;
		depth = _depth;
		meshsize = _meshsize;
		calmodel = _calmodel;
	}
	//~CalculateThread();
signals:
	void sendField(Field* field);
protected:
	void run();

private:
	int *nameSel;
	int *indexSel;
	ModleDataList *modleDataList;
	int Num;
	Field* fieldSource;
	double fre;
	double Unit_factor;
	double dis; //最后传播距离
	double width; // 重新定义后的长宽
	double depth;
	int meshsize;// 剖分信息 
	int calmodel;// 计算模式信息 剖分优先还是解析解优先
	
};

#endif // CalculateThread_H