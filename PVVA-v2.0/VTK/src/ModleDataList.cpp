#include "VTK/include/ModleDataList.h"

ModleDataList::ModleDataList()
{
}

ModleDataList::~ModleDataList()
{
}

void ModleDataList::pushModle(const ModleData &modleData)
{
	data.push_back(modleData);
}

void ModleDataList::modifyModle(const ModleData & modleData)
{
	removeModle(modleData.getName(), modleData.getIndex());
	pushModle(modleData);
}

void ModleDataList::removeModle(int name, int index)
{
	list<ModleData>::iterator it;
	for (it = data.begin(); it != data.end(); it++)
	{
		if (name == it->getName() && index == it->getIndex())
		{
			data.erase(it);
			break;
		}
	}
}

void ModleDataList::removeModle_Actor(int name, int index, vtkSmartPointer<vtkRenderer> renderer)
{
	list<ModleData>::iterator it;
	for (it = data.begin(); it != data.end(); it++)
	{
		if (name == it->getName() && index == it->getIndex())
		{
			renderer->RemoveActor(it->getActor());
			data.erase(it);
			break;
		}
	}
}

void ModleDataList::findModle(int name, int index, ModleData &modleData) const
{
	list<ModleData>::const_iterator it;
	for (it = data.begin(); it != data.end(); it++)
		if (name == it->getName() && index == it->getIndex())
		{
			modleData = *it;
			return;
		}
}

vtkSmartPointer<vtkPolyData> ModleDataList::getPolyData(int name, int index, double ds) const
{
	list<ModleData>::const_iterator it;
	for (it = data.begin(); it != data.end(); it++)
	{
		if (name == it->getName() && index == it->getIndex())
			return it->getPolyData(ds);
	}
	//return vtkSmartPointer<vtkPolyData>();
}

vtkSmartPointer<vtkActor> ModleDataList::getActor(int name, int index)
{
	list<ModleData>::iterator it;
	for (it = data.begin(); it != data.end(); it++)
	{
		if (name == it->getName() && index == it->getIndex())
			return it->getActor();
	}
	//return vtkSmartPointer<vtkActor>();
}

void ModleDataList::draw(vtkSmartPointer<vtkRenderer> renderer)
{
	list<ModleData>::iterator it;
	for (it = data.begin(); it != data.end(); it++)
		renderer->AddActor(it->getActor());
}

bool ModleDataList::getTransparency(int name, int index) const
{
	list<ModleData>::const_iterator it;
	for (it = data.begin(); it != data.end(); it++)
		if (name == it->getName() && index == it->getIndex())
		{
			return it->getTransparent();
		}
	return false;
}

void ModleDataList::setTransparency(int name, int index, bool is)
{
	list<ModleData>::iterator it;
	for (it = data.begin(); it != data.end(); it++)
		if (name == it->getName() && index == it->getIndex())
		{
			 it->setTransparent(is);
			 return;
		}
}
