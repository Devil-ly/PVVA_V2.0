#include "../include/Source.h"

Source::Source(int _type)
{
	type = _type;
	factor = 1;
	//data = nullptr;
}

Source::Source(const Source & _source)
	:fileAddress(_source.fileAddress), factor(_source.factor),type(_source.type)
{
	data.resize(_source.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = _source.data[i];
}

Source::~Source()
{
	//if (data != nullptr)
	//	delete[] data;
	//data.clear();
}

void Source::setData(double * _data, int n)
{
	//if (data != nullptr)
	//	delete[] data;
	data.resize(n);
	//data = new double[n];
	for (int i = 0; i < n; i++)
		data[i] = _data[i];
	if (type == GAUSSIAN)
	{
		N = int(data[7] / data[12]) + 1;
		M = int(data[8] / data[12]) + 1;
	}
}

vector<double> Source::getData() const
{ 
	return data;
}

void Source::setGraphTrans(const GraphTrans & graphtrans)
{
	data[0] = graphtrans.getTrans_x();
	data[1] = graphtrans.getTrans_y();
	data[2] = graphtrans.getTrans_z();
	data[3] = graphtrans.getRotate_x();
	data[4] = graphtrans.getRotate_y();
	data[5] = graphtrans.getRotate_z();
	data[6] = graphtrans.getRotate_theta();
}

GraphTrans Source::getGraphTrans() const
{
	GraphTrans graphTrans;
	graphTrans.setGraphTransPar(data[0], data[1], data[2],
		data[3], data[4], data[5], data[6]);
	return graphTrans;
}

void Source::setFileAddress(const QString & file)
{
	fileAddress = file.toStdString();
}

void Source::setNM(int _N, int _M)
{
	N = _N;
	M = _M;
}

void Source::getNM(int & _N, int & _M) const
{
	//double ds
	_N = N; 
	_M = M; 
}

void Source::getPlaneData(double *para) const
{
	for (int i = 0; i < 9; i++)
		para[i] = data[i];
}

void Source::setDs(double _Ds)
{
	
}

double Source::getDs() const
{
	return data[12];
}

void Source::setUnit(double _unit)
{
	factor = _unit;
}

void Source::getExEy(complex<double>** Ex_In, complex<double>** Ey_In) const
{
	if (type == GAUSSIAN)
	{
		CreateGaussianSource(Ex_In, Ey_In);
	}
	else if (type == APERTURE)
	{
		ifstream file(fileAddress);
		int begin = int(data[11]) - 1;
		string temp;
		while (begin--)
			getline(file, temp);

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				double a1, p1, a2, p2, a3, p3;
				double temp;
				file >> temp >> temp >> temp;
				file >> a1 >> p1 >> a2 >> p2 >> a3 >> p3;
				Ex_In[i][j] = complex<double>(a1, p1);
				Ey_In[i][j] = complex<double>(a2, p2);
				/*Ex_In[i][j] = complex<double>(a1 * cos(p1 + data[10]), 
					a1 * sin(p1 + data[10]));
				Ey_In[i][j] = complex<double>(a2 * cos(p2 + data[10]),
					a2 * sin(p2 + data[10]));*/
			}
		}
		file.close();

	}
}

void Source::CreateGaussianSource(complex<double> ** Ex_In,
	complex<double> ** Ey_In) const
{
	//生成Ex_In, Ey_IN;
	//double z0_back = z0; // 目标距离
	//double w0_back = w0; // 束腰
	//double fre_back = fre; // 频率
	double z0 = data[9];
	double w0 = data[10];
	double fre = data[11];
	double ds = data[12];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			double tempX = (i - (N - 1) / 2) * ds * factor;
			double tempY = (j - (M - 1) / 2) * ds * factor;
			double tempr1;
			tempr1 = pow((tempX * tempX + tempY * tempY), 0.5);
			double temper, tempei;
			CreateGaussian(tempr1, w0 * factor, fre * 1e9, z0 * factor, temper, tempei);
			Ex_In[i][j] = complex<double>(0, 0);
			Ey_In[i][j] = complex<double>(temper, tempei);
		}
	}
	
	//ofstream outfile1("Ex_In.txt");
	/*ofstream outfile2("Ey_In.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfile2 << Ey_In[i][j].real() << " " << Ey_In[i][j].imag() << "\n";
			//outfile2 << 0 << " "<< 0 << " " << Ey_In[i][j].real() << " " << 0 << "\n";
		}
	}
	//outfile1.close();
	outfile2.close();*/
}

void Source::CreateGaussian(double r, double w0, double fre,
	double z0, double &Real, double &Imag) const {
	double lamda = C_Speed / fre;
	double k = 2 * Pi * fre / C_Speed;
	double w, Rdl, theta, r2, temp;

	w = w0 * pow((1 + pow(((lamda * z0) / (Pi * w0 * w0)), 2)), 0.5);
	Rdl = z0 / (z0 * z0 + pow((Pi * w0 * w0 / lamda), 2));
	theta = atan((lamda * z0) / (Pi * w0 * w0));
	r2 = r * r;
	temp = pow((2 / w / w / Pi), 0.5) * exp(-r2 / w / w);

	Real = temp * cos(theta - k * z0 - Pi * r2 / lamda * Rdl);
	Imag = temp * sin(theta - k * z0 - Pi * r2 / lamda * Rdl);
}