#include"calculation/include/pvva.h"
#include "calculation/include/Position3D.h"
#include "calculation/include/Matrix4D.h"
#include "calculation/include/Vector3D.h"

PVVA::PVVA()
{
	Initialization();
}

PVVA::~PVVA()
{
	//Free_2D(Ex_In);
	//Free_2D(Ey_In);

	Free_2D(Px);
	Free_2D(Py);
	Free_2D(Pz);
	if (EleData != NULL)
	{
		delete[] EleData;
		delete[] PointNormal;
		delete[] NodesXYZ;
	}

	Free_2D(Plane);
	Free_2D(n_Plane);
	Free_2D(R_Plane);
	Free_2D(InterVal_Plane);
	
	Free_2D(Ex1);
	Free_2D(Ey1);
	Free_2D(Ez1);
	Free_2D(Hx1);
	Free_2D(Hy1);
	Free_2D(Hz1);

	Free_2D(Ex_R);
	Free_2D(Ey_R);
	Free_2D(Ez_R);

	Free_2D(Ex_R_Inter);
	Free_2D(Ey_R_Inter);
	Free_2D(Ez_R_Inter);
}

void PVVA::Initialization()
{
	N = 0; M = 0;
	f = 10.65e9;
	lamda = 0; 
	k = 0;
	theta = 0;
	ds = 0; z0 = 0;
	unit_factor = 1;

	Ex_In = NULL;
	Ey_In = NULL;

	Px = NULL;
	Py = NULL;
	Pz = NULL;

	EleData = NULL;
	NodesXYZ = NULL;
	Plane = NULL;
	n_Plane = NULL;
	R_Plane = NULL;
	InterVal_Plane = NULL;

	Ex1 = NULL;
	Ey1 = NULL;
	Ez1 = NULL;
	Hx1 = NULL;;
	Hy1 = NULL;
	Hz1 = NULL;

	Ex_R = NULL;
	Ey_R = NULL;
	Ez_R = NULL;

	Ex_R_Inter = NULL;
	Ey_R_Inter = NULL;
	Ez_R_Inter = NULL;
}

void PVVA::setUnit(double factor)
{
	unit_factor = factor;
}

bool PVVA::IsIntersect(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, Vector3 &intersection, double &t)
{
	double u, v;
	// E1
	Vector3 E1 = v1 - v0;

	// E2
	Vector3 E2 = v2 - v0;

	// P
	Vector3 P = dir.Cross(E2);

	// determinant
	double det = E1.Dot(P);

	Vector3 T;
	T = orig - v0;

	// If determinant is near zero, ray lies in plane of triangle
	//if (det < 0.00000001 && det > -0.00000001)
	//	return false;

	// Calculate u and make sure u <= 1
	u = T.Dot(P);
	double fInvDet = 1.0f / det;
	u *= fInvDet;

	if (u < 0.0 || u > 1)
		return false;

	// Q
	Vector3 Q = T.Cross(E1);

	// Calculate v and make sure u + v <= 1
	v = dir.Dot(Q);
	v *= fInvDet;
	if (v < 0.0 || u + v > 1)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	t = E2.Dot(Q);
	t *= fInvDet;

	intersection = orig + dir * t;

	return true;
}

double PVVA::IntersectPlane(const Vector3 & orig, const Vector3 & dir, const Vector3 & Plane_org, const Vector3 & Plane_n, Vector3 & intersection)
{
	double temp1 = dir.Dot(Plane_n);
	if(temp1 == 0)
		return 0.0;
	double temp2 = Plane_org.Dot(Plane_n) - orig.Dot(Plane_n);
	double t = temp2 / temp1;
	intersection = orig + dir * t;
	return t;
}

double PVVA::IntersectPoint(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0, const Vector3 &E1, const Vector3 &E2, Vector3 &intersection)
{
	// P
	Vector3 P = dir.Cross(E2);

	// determinant
	double det = E1.Dot(P);

	double u, v, t;
	Vector3 T;
	T = orig - v0;

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 10e-10 && det > -10e-10)
		return 0;

	u = T.Dot(P);
	double fInvDet = 1.0f / det;
	u *= fInvDet;

	// Q
	Vector3 Q = T.Cross(E1);

	v = dir.Dot(Q);
	v *= fInvDet;

	// Calculate t, scale parameters, ray intersects triangle
	t = E2.Dot(Q);
	t *= fInvDet;

	intersection = orig + dir * t;
	return t;
}

void PVVA::Poynting()
{
	double tempx, tempy, tempz;
	double absx, absy, absz;
	//double sum = 0;

	// 源的坐标系转换到绝对坐标系（只求旋转）
	Vector3D RotateAsixSou(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(SourceGraphTrans.getRotate_theta(), RotateAsixSou);


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Px[i][j] = ConjugateMul(Ey1[i][j], Hz1[i][j]) - ConjugateMul(Ez1[i][j], Hy1[i][j]);
			Py[i][j] = ConjugateMul(Ez1[i][j], Hx1[i][j]) - ConjugateMul(Ex1[i][j], Hz1[i][j]);
			Pz[i][j] = ConjugateMul(Ex1[i][j], Hy1[i][j]) - ConjugateMul(Ey1[i][j], Hx1[i][j]);

			absx = Px[i][j].real();
			absy = Py[i][j].real();
			absz = Pz[i][j].real();

			n_Plane[i][j].set(absx, absy, absz);

			n_Plane[i][j] = rotatMatrixSou * n_Plane[i][j];
			n_Plane[i][j].Normalization();
		}
	}

	//cout << "prePower:  " << sum << endl;

	/*ofstream outfile1("Px.txt");
	ofstream outfile2("Py.txt");
	ofstream outfile3("Pz.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfile1 << Px[i][j].real() << " " << Px[i][j].imag() << "\n";
			outfile2 << Py[i][j].real() << " " << Py[i][j].imag() << "\n";
			outfile3 << Pz[i][j].real() << " " << Pz[i][j].imag() << "\n";
		}
	}
	outfile1.close();
	outfile2.close();
	outfile3.close();*/
}

void PVVA::getPlane(Vector3 **& _org, Vector3 **& _n) const
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			_org[i][j] = Plane[i][j];
			_n[i][j] = n_Plane[i][j];
		}
}

void PVVA::updateSource_n(Vector3 new_n)
{
	new_n.Normalization();
	if (new_n.x != 0 || new_n.y != 0 || new_n.z != 1)
	{
		Vector3 rotate_axis = Vector3(0, 0, 1).Cross(new_n); // 旋转轴
		double rotate_theta = acos(Vector3(0, 0, 1).Dot(new_n));
		rotate_theta = rotate_theta / Pi * 180;
		SourceGraphTrans.updateRotate(rotate_axis, rotate_theta);
	}
	else
	{
		SourceGraphTrans.updateRotate(Vector3(0, 0, 1), 0);
	}
}

void PVVA::calPointNormal()
{
	for (int i = 0; i < EleNum; i++)
	{
		Vector3 tempa = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N2];
		Vector3 tempb = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N3];
		//Vector3 tempb = NodesXYZ[EleData[i].N3] - NodesXYZ[EleData[i].N1];
		Vector3 n_light = tempa.Cross(tempb);  //法向量
		n_light.Normalization();
		PointNormal[EleData[i].N1] = PointNormal[EleData[i].N1] + n_light;
		PointNormal[EleData[i].N2] = PointNormal[EleData[i].N2] + n_light;
		PointNormal[EleData[i].N3] = PointNormal[EleData[i].N3] + n_light;
	}

	for (int i = 0; i < NodesNum; i++)
	{
		PointNormal[i].Normalization();
	}

}

void PVVA::InterVal()
{
    test_Plane = Allocate_2D(test_Plane, N, M);
	double tempx, tempz;
	int tempn, tempm;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			tempx = R_Plane[i][j].x - Plane[0][0].x;
			tempz = R_Plane[i][j].z - Plane[0][0].z;
			test_Plane[i][j].x = -tempx * cos(theta_Source) - tempz * sin(theta_Source);
			test_Plane[i][j].y = R_Plane[i][j].y - Plane[0][0].y;

			test_Plane[i][j].x = (test_Plane[i][j].x) / ds;
			test_Plane[i][j].y = (test_Plane[i][j].y) / ds;

		}
	}
	double a, b, c, d;
	double sum;
	for (int i = 1; i < N - 1; i++)
	{
		for (int j = 1; j < M - 1; j++)
		{
			Ex_In[i][j] = 0;
			Ey_In[i][j] = 0;
			//Ez_R_Inter[i][j] = 0;

			a = pow((double(i) - test_Plane[i][j].x), 2) + pow((double(j) - test_Plane[i][j].y), 2);
			b = pow((double(i) - test_Plane[i + 1][j].x), 2) + pow((double(j) - test_Plane[i + 1][j].y), 2);
			c = pow((double(i) - test_Plane[i + 1][j + 1].x), 2) + pow((double(j) - test_Plane[i + 1][j + 1].y), 2);
			d = pow((double(i) - test_Plane[i][j + 1].x), 2) + pow((double(j) - test_Plane[i][j + 1].y), 2);
			sum = 1 / (a*b*c + a*b*d + a*d*c + b*c*d);
			Ex_In[i][j] = (Ex_R[i][j] * b*c*d + Ex_R[i + 1][j] * a*d*c + Ex_R[i + 1][j + 1] * a*b*d + Ex_R[i][j + 1] * a*b*c) * sum;
			Ey_In[i][j] = (Ey_R[i][j] * b*c*d + Ey_R[i + 1][j] * a*d*c + Ey_R[i + 1][j + 1] * a*b*d + Ey_R[i][j + 1] * a*b*c) * sum;
		}
	}


	Vector3D RotateAsix(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(SourceGraphTrans.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(SourceGraphTrans.getTrans_x(),
		SourceGraphTrans.getTrans_y(), SourceGraphTrans.getTrans_z());


	// 设置源各个点的位置
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			Position3D tempPoint((i - (N - 1) / 2) * ds, (j - (M - 1) / 2) * ds, 0);
			tempPoint = translateMatrix * rotatMatrix * tempPoint;
			Plane[i][j].set(tempPoint.X(), tempPoint.Y(), tempPoint.Z());
		}

	Free_2D(test_Plane);
	/*
	ofstream outfile1("Ex_R_Inter_M.txt");
	ofstream outfile2("Ey_R_Inter_M.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfile1 << Ex_R_Inter[i][j].real() << " " << Ex_R_Inter[i][j].imag() << "\n";
			outfile2 << Ey_R_Inter[i][j].real() << " " << Ey_R_Inter[i][j].imag() << "\n";
		}
	}
	outfile1.close();
	outfile2.close();*/

	/*ofstream outfilex1("Ex2.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfilex1 << Ey_In[i][j].real() << " " << Ey_In[i][j].imag() << "\n";

		}
	}
	outfilex1.close();

	//updateSource_n(n_Source);
	
	Vector3 temp(0, 0, 1);
	for (int i = 0; i < N; i++)     // 更新平面位置
		for (int j = 0; j < M; j++)
		{
			Plane[i][j] = Plane[i][j] + temp * z1;
		}

	*/
	//Free_2D(Interpolation);
}

void PVVA::Result(double dis)
{
	CalPlane(dis);
	/*
	ofstream outfilex1("Ex2_M.txt");
	ofstream outfilex2("Ey2_M.txt");
	ofstream outfilex3("Ez2_M.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfilex1 << Ex1[i][j].real() << " " << Ex1[i][j].imag() << "\n";
			outfilex2 << Ey1[i][j].real() << " " << Ey1[i][j].imag() << "\n";
			outfilex3 << Ez1[i][j].real() << " " << Ez1[i][j].imag() << "\n";
		}
	}
	outfilex1.close();
	outfilex2.close();
	outfilex3.close();*/
}

void PVVA::CalAmplitude()
{
	Vector3 tempA1, tempA2;
	Vector3 tempB1, tempB2;
	Vector3 tempC1, tempC2;
	Vector3 tempD1, tempD2;
	Vector3 tempn1, tempn2; // 该点的法向量
	double tempcos1, tempcos2;
	double PreSquare, PostSquare;
	double tempratio;
	double LAE, TAE;
	
	//中心部分四个顶点
	for (int i = 1; i < N - 2; i++)
	{
		for (int j = 1; j < M - 2; j++)
		{
			// 反射前投影面积
			tempA1 = Plane[i - 1][j];
			tempB1 = Plane[i][j - 1];
			tempC1 = Plane[i + 1][j];
			tempD1 = Plane[i][j + 1];
			tempn1 = (tempB1 - Plane[i][j]).Cross(tempC1 - Plane[i][j]);

			if (tempn1.Length() == 0 || n_Plane[i][j].Length() == 0)
				tempcos1 = 1;
			else
				tempcos1 = tempn1.Dot(n_Plane[i][j]) / tempn1.Length()
				/ n_Plane[i][j].Length();
		
			PreSquare = abs(tempcos1) * CalSquare(tempA1, tempB1, tempC1, tempD1);
			//PreSquare =  CalSquare(tempA1, tempB1, tempC1, tempD1);
			LAE = pow((Ex1[i][j].real() * Ex1[i][j].real() + Ex1[i][j].imag() * Ex1[i][j].imag()
				+ Ey1[i][j].real() * Ey1[i][j].real() + Ey1[i][j].imag() * Ey1[i][j].imag()
				+ Ez1[i][j].real() * Ez1[i][j].real() + Ez1[i][j].imag() * Ez1[i][j].imag()), 0.5);
			// 反射后投影面积
			tempA2 = R_Plane[i - 1][j];
			tempB2 = R_Plane[i][j - 1];
			tempC2 = R_Plane[i + 1][j];
			tempD2 = R_Plane[i][j + 1];
			tempn2 = (tempB2 - R_Plane[i][j]).Cross(tempC2 - R_Plane[i][j]);

			if (n_Source.Length() == 0 || R_Plane[i][j].Length() == 0)
				tempcos2 = 1;
			else
				tempcos2 = n_Source.Dot(R_Plane[i][j]) / n_Source.Length()
				/ R_Plane[i][j].Length();

			TAE = pow((Ex_R[i][j].real() * Ex_R[i][j].real() + Ex_R[i][j].imag() * Ex_R[i][j].imag()
				+ Ey_R[i][j].real() * Ey_R[i][j].real() + Ey_R[i][j].imag() * Ey_R[i][j].imag()
				+ Ez_R[i][j].real() * Ez_R[i][j].real() + Ez_R[i][j].imag() * Ez_R[i][j].imag()), 0.5);

			PostSquare = abs(tempcos2) * CalSquare(tempA2, tempB2, tempC2, tempD2);
			//PostSquare = CalSquare(tempA2, tempB2, tempC2, tempD2);
			if (PostSquare == 0)
				PostSquare = 1;
			if (TAE == 0)
				TAE = 1;
			tempratio = pow(abs(PreSquare / PostSquare), 0.5) * LAE / TAE;

			Ex_R[i][j] = Ex_R[i][j] * tempratio;
			Ey_R[i][j] = Ey_R[i][j] * tempratio;
			Ez_R[i][j] = Ez_R[i][j] * tempratio;
		}
	}
	//边界部分三个点
	  //上边界
	for (int i = 0; i < N - 2 ; i++)
	{
		// 反射前投影面积
		if (i == 0)
		{
			tempA1 = Plane[i][0];
			tempA2 = R_Plane[i][0];
		}
		else
		{
			tempA1 = Plane[i - 1][0];
			tempA2 = R_Plane[i - 1][0];
		}

		tempC1 = Plane[i + 1][0];
		tempD1 = Plane[i][1];
		tempn1 = (tempD1 - Plane[i][0]).Cross(tempC1 - Plane[i][0]);
		tempcos1 = tempn1.Dot(n_Plane[i][0]) / tempn1.Length() / n_Plane[i][0].Length();

		LAE = pow((Ex1[i][0].real() * Ex1[i][0].real() + Ex1[i][0].imag() * Ex1[i][0].imag()
			+ Ey1[i][0].real() * Ey1[i][0].real() + Ey1[i][0].imag() * Ey1[i][0].imag()
			+ Ez1[i][0].real() * Ez1[i][0].real() + Ez1[i][0].imag() * Ez1[i][0].imag()), 0.5);

		//PreSquare = abs(tempcos1) * CalSquare(tempA1, tempC1, tempD1);
		PreSquare =  CalSquare(tempA1, tempC1, tempD1);

		// 反射后投影面积
		tempC2 = R_Plane[i + 1][0];
		tempD2 = R_Plane[i][1];
		tempn2 = (tempD2 - R_Plane[i][0]).Cross(tempC2 - R_Plane[i][0]);
		tempcos2 = tempn2.Dot(R_Plane[i][0]) / tempn2.Length() / R_Plane[i][0].Length();

		TAE = pow((Ex_R[i][0].real() * Ex_R[i][0].real() + Ex_R[i][0].imag() * Ex_R[i][0].imag()
			+ Ey_R[i][0].real() * Ey_R[i][0].real() + Ey_R[i][0].imag() * Ey_R[i][0].imag()
			+ Ez_R[i][0].real() * Ez_R[i][0].real() + Ez_R[i][0].imag() * Ez_R[i][0].imag()), 0.5);

		//PostSquare = abs(tempcos2) * CalSquare(tempA2, tempC2, tempD2);
		PostSquare =  CalSquare(tempA2, tempC2, tempD2);
		if (PostSquare == 0)
			PostSquare = 1;
		if (TAE == 0)
			TAE = 1;
		tempratio = pow(abs(PreSquare / PostSquare), 0.5) * LAE / TAE;

		Ex_R[i][0] = Ex_R[i][0] * tempratio;
		Ey_R[i][0] = Ey_R[i][0] * tempratio;
		Ez_R[i][0] = Ez_R[i][0] * tempratio;
	}
	  //下边界
	for (int i = 1; i < N - 1; i++)
	{
		// 反射前投影面积
		if (i == N - 1)
		{
			tempC1 = Plane[i][N - 1];
			tempC2 = R_Plane[i][N - 1];
		}
		else
		{
			tempC1 = Plane[i + 1][N - 1];
			tempC2 = R_Plane[i + 1][N - 1];
		}
		tempA1 = Plane[i - 1][N - 1];
		tempB1 = Plane[i][N - 2];
		
		tempn1 = (tempA1 - Plane[i][N - 1]).Cross(tempB1 - Plane[i][N - 1]);
		tempcos1 = tempn1.Dot(n_Plane[i][N - 1]) / tempn1.Length() / n_Plane[i][N - 1].Length();

		LAE = pow((Ex1[i][N - 1].real() * Ex1[i][N - 1].real() + Ex1[i][N - 1].imag() * Ex1[i][N - 1].imag()
			+ Ey1[i][N - 1].real() * Ey1[i][N - 1].real() + Ey1[i][N - 1].imag() * Ey1[i][N - 1].imag()
			+ Ez1[i][N - 1].real() * Ez1[i][N - 1].real() + Ez1[i][N - 1].imag() * Ez1[i][N - 1].imag()), 0.5);

		PreSquare = abs(tempcos1) * CalSquare(tempA1, tempB1, tempC1);

		// 反射后投影面积
		tempA2 = R_Plane[i - 1][N - 1];
		tempB2 = R_Plane[i][N - 2];
		
		tempn2 = (tempA2 - R_Plane[i][N - 1]).Cross(tempB2 - R_Plane[i][N - 1]);
		if (R_Plane[i][N - 1].Length() == 0 || tempn2.Length() == 0)
			tempcos2 = 1;
		else
			tempcos2 = tempn2.Dot(R_Plane[i][N - 1]) / tempn2.Length() 
			/ R_Plane[i][N - 1].Length();


		TAE = pow((Ex_R[i][N - 1].real() * Ex_R[i][N - 1].real() + Ex_R[i][N - 1].imag() * Ex_R[i][N - 1].imag()
			+ Ey_R[i][N - 1].real() * Ey_R[i][N - 1].real() + Ey_R[i][N - 1].imag() * Ey_R[i][N - 1].imag()
			+ Ez_R[i][N - 1].real() * Ez_R[i][N - 1].real() + Ez_R[i][N - 1].imag() * Ez_R[i][N - 1].imag()), 0.5);

		PostSquare = abs(tempcos2) * CalSquare(tempA2, tempB2, tempC2);
		if (PostSquare == 0)
			PostSquare = 1;
		if (TAE == 0)
			TAE = 1;
		tempratio = pow(abs(PreSquare / PostSquare), 0.5) * LAE / TAE;

		Ex_R[i][N - 1] = Ex_R[i][N - 1] * tempratio;
		Ey_R[i][N - 1] = Ey_R[i][N - 1] * tempratio;
		Ez_R[i][N - 1] = Ez_R[i][N - 1] * tempratio;
	}
	  //左边界
	for (int j = 1; j < N - 1; j++)
	{
		// 反射前投影面积
		if (j == N - 1)
		{
			tempD1 = Plane[0][j];
			tempD2 = R_Plane[0][j];
		}
		else
		{
			tempD1 = Plane[0][j + 1];
			tempD2 = R_Plane[0][j + 1];
		}
		tempB1 = Plane[0][j - 1];
		tempC1 = Plane[1][j];
		
		tempn1 = (tempB1 - Plane[0][j]).Cross(tempC1 - Plane[0][j]);
		if (tempn1.Length() == 0 || n_Plane[0][j].Length())
			tempcos1 = 1;
		else 
			tempcos1 = tempn1.Dot(n_Plane[0][j]) / tempn1.Length()
			/ n_Plane[0][j].Length();

		LAE = pow((Ex1[0][j].real() * Ex1[0][j].real() + Ex1[0][j].imag() * Ex1[0][j].imag()
			+ Ey1[0][j].real() * Ey1[0][j].real() + Ey1[0][j].imag() * Ey1[0][j].imag()
			+ Ez1[0][j].real() * Ez1[0][j].real() + Ez1[0][j].imag() * Ez1[0][j].imag()), 0.5);

		//PreSquare = abs(tempcos1) * CalSquare(tempB1, tempC1, tempD1);
		PreSquare = CalSquare(tempB1, tempC1, tempD1);
		// 反射后投影面积
		tempB2 = R_Plane[0][j - 1];
		tempC2 = R_Plane[1][j];
		
		tempn2 = (tempB2 - R_Plane[0][j]).Cross(tempC2 - R_Plane[0][j]);
		if (R_Plane[0][j].Length() == 0 || tempn2.Length() == 0)
			tempcos2 = 1;
		else
			tempcos2 = tempn2.Dot(R_Plane[0][j]) / tempn2.Length() / R_Plane[0][j].Length();


		TAE = pow((Ex_R[0][j].real() * Ex_R[0][j].real() + Ex_R[0][j].imag() * Ex_R[0][j].imag()
			+ Ey_R[0][j].real() * Ey_R[0][j].real() + Ey_R[0][j].imag() * Ey_R[0][j].imag()
			+ Ez_R[0][j].real() * Ez_R[0][j].real() + Ez_R[0][j].imag() * Ez_R[0][j].imag()), 0.5);

		//PostSquare = abs(tempcos2) * CalSquare(tempB2, tempC2, tempD2);
		PostSquare = CalSquare(tempB2, tempC2, tempD2);
		if (PostSquare == 0)
			PostSquare = 1;
		if (TAE == 0)
			TAE = 1;
		tempratio = pow(abs(PreSquare / PostSquare), 0.5) * LAE / TAE;

		Ex_R[0][j] = Ex_R[0][j] * tempratio;
		Ey_R[0][j] = Ey_R[0][j] * tempratio;
		Ez_R[0][j] = Ez_R[0][j] * tempratio;
	}
	  //右边界
	for (int j = 0; j < N - 2; j++)
	{
		// 反射前投影面积
		if (j == 0)
		{
			tempB1 = Plane[N - 1][j];
			tempB2 = R_Plane[N - 1][j];
		}
		else
		{
			tempB1 = Plane[N - 1][j - 1];
			tempB2 = R_Plane[N - 1][j - 1];
		}
		tempA1 = Plane[N - 2][j];
		
		tempD1 = Plane[N - 1][j + 1];
		tempn1 = (tempD1 - Plane[N - 1][j]).Cross(tempA1 - Plane[N - 1][j]);
		tempcos1 = tempn1.Dot(n_Plane[N - 1][j]) / tempn1.Length() / n_Plane[N - 1][j].Length();

		LAE = pow((Ex1[N - 1][j].real() * Ex1[N - 1][j].real() + Ex1[N - 1][j].imag() * Ex1[N - 1][j].imag()
			+ Ey1[N - 1][j].real() * Ey1[N - 1][j].real() + Ey1[N - 1][j].imag() * Ey1[N - 1][j].imag()
			+ Ez1[N - 1][j].real() * Ez1[N - 1][j].real() + Ez1[N - 1][j].imag() * Ez1[N - 1][j].imag()), 0.5);

		//PreSquare = abs(tempcos1) * CalSquare(tempD1, tempA1, tempB1);
		PreSquare = CalSquare(tempD1, tempA1, tempB1);
		// 反射后投影面积
		tempA2 = R_Plane[N - 2][j];
		
		tempD2 = R_Plane[N - 1][j + 1];
		tempn2 = (tempD2 - R_Plane[N - 1][j]).Cross(tempA2 - R_Plane[N - 1][j]);
		tempcos2 = tempn2.Dot(R_Plane[N - 1][j]) / tempn2.Length() / R_Plane[N - 1][j].Length();

		TAE = pow((Ex_R[N - 1][j].real() * Ex_R[N - 1][j].real() + Ex_R[N - 1][j].imag() * Ex_R[N - 1][j].imag()
			+ Ey_R[N - 1][j].real() * Ey_R[N - 1][j].real() + Ey_R[N - 1][j].imag() * Ey_R[N - 1][j].imag()
			+ Ez_R[N - 1][j].real() * Ez_R[N - 1][j].real() + Ez_R[N - 1][j].imag() * Ez_R[N - 1][j].imag()), 0.5);

		//PostSquare = abs(tempcos2) * CalSquare(tempD2, tempA2, tempB2
		PostSquare = CalSquare(tempD2, tempA2, tempB2);
		if (PostSquare == 0)
			PostSquare = 1;
		if (TAE == 0)
			TAE = 1;
		tempratio = pow(abs(PreSquare / PostSquare), 0.5) * LAE / TAE;

		Ex_R[N - 1][j] = Ex_R[N - 1][j] * tempratio;
		Ey_R[N - 1][j] = Ey_R[N - 1][j] * tempratio;
		Ez_R[N - 1][j] = Ez_R[N - 1][j] * tempratio;
	}
}

Vector3 PVVA::ReflectLight(Vector3 a, Vector3 n)
{
	//先单位化
	double absa = pow(a.Dot(a), 0.5);
	double absn = pow(n.Dot(n), 0.5);
	a = a * (1 / absa);
	n = n * (1 / absn);
	double I = 2 * n.Dot(a);
	if (I < 0)
		I = -I;
	else
		a = Vector3(0.0, 0.0, 0.0) - a;
		
	return n * I + a;
}

void PVVA::CalReflectExyz(const Vector3 & n, const complex<double>& Ex, const complex<double>& Ey, const complex<double>& Ez, complex<double>& Ex_out, complex<double>& Ey_out, complex<double>& Ez_out)
{
	complex<double> ne(n.x * Ex.real() + n.y * Ey.real() + n.z * Ez.real(),
		n.x * Ex.imag() + n.y * Ey.imag() + n.z * Ez.imag());
	complex<double> tempx = 2 * n.x * ne;
	complex<double> tempy = 2 * n.y * ne;
	complex<double> tempz = 2 * n.z * ne;

	Ex_out = tempx - Ex;
	Ey_out = tempy - Ey;
	Ez_out = tempz - Ez;
}

double PVVA::CalSquare(const Vector3 & A, const Vector3 & B, const Vector3 & C, const Vector3 & D) const
{
	Vector3 AB = B - A;
	Vector3 AC = C - A;
	Vector3 AD = D - A;
	Vector3 tempS1 = AB.Cross(AC);
	Vector3 tempS2 = AC.Cross(AD);
	double tempS = tempS1.Length() + tempS2.Length();
	return tempS / 2;
}
double PVVA::CalSquare(const Vector3 & A, const Vector3 & B, const Vector3 & C) const
{
	Vector3 AB = B - A;
	Vector3 AC = C - A;
	Vector3 tempS1 = AB.Cross(AC);
	double tempS = tempS1.Length();
	return tempS / 2;
}

void PVVA::ReadDat(string ProjName)
{
	cout << "Reading the dat" << endl;

	ifstream infile;
	infile.open(ProjName + ".dat");
	assert(infile.is_open());
	string buff;        //行数据
	vector<string> LineDate;  //分割行后临时保存数据

	JumpLine(6, infile);
	getline(infile, buff);
	Split(buff, " ", LineDate);
	EleNum = atoi(LineDate[0].c_str());
	NodesNum = atoi(LineDate[1].c_str());
	NodesXYZ = new Vector3[NodesNum];
	EleData = new Element[EleNum];
	LineDate.clear();

	JumpLine(5, infile);          //读xyz坐标值
	for (int i = 0; i < NodesNum; i++)
	{
		getline(infile, buff);
		Split(buff, " ", LineDate);
		NodesXYZ[i].x = atof(LineDate[2].c_str());
		NodesXYZ[i].y = atof(LineDate[3].c_str());
		NodesXYZ[i].z = atof(LineDate[4].c_str());
		LineDate.clear();
	}

	JumpLine(5, infile);
	for (int i = 0; i < EleNum; i++)   //读元素联系的8个点以及材料代号
	{
		getline(infile, buff);
		Split(buff, " ", LineDate);
		EleData[i].N1 = atof(LineDate[2].c_str());
		EleData[i].N2 = atof(LineDate[3].c_str());
		EleData[i].N3 = atof(LineDate[4].c_str());
		//EleDate[i].Mat = atof(LineDate[5].c_str());
		LineDate.clear();
	}

	infile.close();

	cout << "Reading the dat Completed" << endl;
}

void PVVA::AllocateMemory()
{
	//坡印廷矢量
	Px = Allocate_2D(Px, N, M);
	Py = Allocate_2D(Py, N, M);
	Pz = Allocate_2D(Pz, N, M);

	//源
	Ex_In = Allocate_2D(Ex_In, N, M);
	Ey_In = Allocate_2D(Ey_In, N, M);

	//传播后的电磁场
	Ex1 = Allocate_2D(Ex1, N, M);
	Ey1 = Allocate_2D(Ey1, N, M);
	Ez1 = Allocate_2D(Ez1, N, M);

	Hx1 = Allocate_2D(Hx1, N, M);
	Hy1 = Allocate_2D(Hy1, N, M);
	Hz1 = Allocate_2D(Hz1, N, M);

	//反射后的电场
	Ex_R = Allocate_2D(Ex_R, N, M);
	Ey_R = Allocate_2D(Ey_R, N, M);
	Ez_R = Allocate_2D(Ez_R, N, M);

	// 平面位置（反射先，反射后）和各点的法向量
	n_Plane = Allocate_2D(n_Plane, N, M);
	Plane = Allocate_2D(Plane, N, M);
	R_Plane = Allocate_2D(R_Plane, N, M);
	Plane_org = Allocate_2D(Plane_org, N, M);

	//差值后的Exyz
	InterVal_Plane = Allocate_2D(InterVal_Plane, N, M);
	Ex_R_Inter = Allocate_2D(Ex_R_Inter, N, M);
	Ey_R_Inter = Allocate_2D(Ey_R_Inter, N, M);
	Ez_R_Inter = Allocate_2D(Ez_R_Inter, N, M);
}

void PVVA::setSource(const Field* _field)
{
	complex<double> **Ex_temp = _field->getEx();
	complex<double> **Ey_temp = _field->getEy();

	_field->getSourcePara(SourceGraphTrans, N, M, ds);
	SourceGraphTrans.normalization(unit_factor);
	ds = ds * unit_factor;

	AllocateMemory();

	for(int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			Ex_In[i][j] = Ex_temp[i][j];
			Ey_In[i][j] = Ey_temp[i][j];
		}

	Vector3D RotateAsix(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(SourceGraphTrans.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(SourceGraphTrans.getTrans_x(),
		SourceGraphTrans.getTrans_y(), SourceGraphTrans.getTrans_z());


	// 设置源各个点的位置
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			Position3D tempPoint((i - (N - 1) / 2) * ds, (j - (M - 1) / 2) * ds, 0);
			tempPoint = translateMatrix * rotatMatrix * tempPoint;
			Plane[i][j].set(tempPoint.X(), tempPoint.Y(), tempPoint.Z());
		}

	// 设置源的方向
	Position3D tempPoint(0, 0, 1);
	tempPoint = rotatMatrix * tempPoint;
	n_Source.set(tempPoint.X(), tempPoint.Y(), tempPoint.Z());
	n_Source.Normalization();
	
	// 平面的中点
	Position3D tempPoint1(0, 0, 0);
	tempPoint1 = translateMatrix * rotatMatrix * tempPoint1;
	Org_Source.set(tempPoint1.X(), tempPoint1.Y(), tempPoint1.Z());	

//	updateSource_n(Vector3(1, 1, 0));
}

void PVVA::getSourcePoint(Vector3 & interPoint, Vector3 & n_Point) const
{
	interPoint = Org_Source;
	n_Point = n_Source;
}

void PVVA::setSourcePoint(const Vector3 & interPoint, const Vector3 & n_Point)
{
	Org_Source = interPoint;
	n_Source = n_Point;
}

GraphTrans PVVA::getSourceGraphTrans(const Vector3 & n_Point)
{
	Vector3 a = n_Point;
	GraphTrans res;
	a.Normalization();
	if (a.x != 0 || a.y != 0 || a.z != 1)
	{
		Vector3 rotate_axis = Vector3(0, 0, 1).Cross(a); // 旋转轴
		double rotate_theta = acos(Vector3(0, 0, 1).Dot(a));
		rotate_theta = rotate_theta / Pi * 180;
	
		res.updateRotate(rotate_axis, rotate_theta);
	}
	else
	{
		res.updateRotate(Vector3(0, 0, 1), 0);
	}
	return res;
}

void PVVA::setFre(double _fre)
{
	f = _fre;
	lamda = C_Speed / f;
	k = 2 * Pi * f / C_Speed;
}

void PVVA::setModle(vtkSmartPointer<vtkPolyData> polyData)
{
	
	NodesNum = polyData->GetNumberOfPoints();
	EleNum = polyData->GetNumberOfCells();
	//NodesNum = 5;
	//EleNum = 4;
	if (NodesXYZ != NULL && EleData != NULL)
	{
		delete[] NodesXYZ;
		delete[] EleData;
	}

	NodesXYZ = new Vector3[NodesNum];
	PointNormal = new Vector3[NodesNum];
	EleData = new Element[EleNum];


for (int i = 0; i < NodesNum; i++)
	{
		double * p = polyData->GetPoint(i);
		NodesXYZ[i].x = p[0];
		NodesXYZ[i].y = p[1];
		NodesXYZ[i].z = p[2];
		PointNormal[i].set(0, 0, 0);
	}

	for (int i = 0; i < EleNum; i++)
	{
		vtkIdList * p = polyData->GetCell(i)->GetPointIds();
		EleData[i].N1 = p->GetId(0);
		EleData[i].N2 = p->GetId(1);
		EleData[i].N3 = p->GetId(2);
	}
	calPointNormal();
}

void PVVA::setAnalytic_Modle(ModleData * modeldata)
{
	int num = modeldata->getDataSize();
	float *temp = modeldata->getData();
	Analytic_data.resize(num);
	for (int i = 0; i < num; i++)
	{
		Analytic_data[i] = temp[i];
	}
}

bool PVVA::CheckModle(Vector3 & interPoint, Vector3 & n_Point)
{
	//Vector3 InterPoint;
	double temp;
	bool isOK = false;
	int i = 0;
	for (; i < EleNum; i++)
	{
		if (IsIntersect(Org_Source, n_Source, NodesXYZ[EleData[i].N1],
			NodesXYZ[EleData[i].N2], NodesXYZ[EleData[i].N3], interPoint, temp))
		{
			if (temp >= 0)
			{
				isOK = true;
				break;
			}

		}

	}
	if (isOK)
	{
		Vector3 n_light = calSurfaceNormal(EleData[i], interPoint);
		n_light.Normalization();

		n_Point = ReflectLight(n_Source, n_light);
	}

	return isOK;
}

void PVVA::getField(Field & _field)
{
	_field.setNM(N, M);
	_field.setPlane(SourceGraphTrans, ds);
	_field.setField(Ex1, Ey1, Ez1, Hx1, Hy1, Hz1);
	_field.setShowPara(1, 1, 0);

}

void PVVA::DatePretreat()
{
	double tempx, tempy, tempz;
	if ((N % 2 == 1) && (M % 2 == 1))   //判断N M 是否为奇数
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				tempx = Org_Source.x - cos(theta_Source) * (i- (N - 1) / 2) * ds;
				tempy = (j - (M - 1) / 2) * ds + Org_Source.y;
				tempz = Org_Source.z - sin(theta_Source) * (i - (N - 1) / 2) * ds;
				Plane[i][j].set(tempx, tempy, tempz);
			}
		}//endloop
	}
}

void PVVA::CalZ0Theta()
{
	Vector3 InterPoint;
	double temp;
	int i = 0;
	for (; i < EleNum; i++)
	{
		if (IsIntersect(Org_Source, n_Source, NodesXYZ[EleData[i].N1],
			NodesXYZ[EleData[i].N2], NodesXYZ[EleData[i].N3], InterPoint, temp))
			break;
	}

	//计算平移的距离（此时传播方向没有改变）
	z0 = CalDistance(Org_Source, InterPoint);
	//z0 = 1.2;
	CalPlane(z0);

	//计算反射后的传播方向（传播方向改变）
	/*Vector3 tempa = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N2];
	Vector3 tempb = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N3];
	tempa.Normalization();
	tempb.Normalization();
	Vector3 n_light = tempa.Cross(tempb);  //法向量
	n_light.Normalization();*/

	Vector3 n_light = calSurfaceNormal(EleData[i], InterPoint);
	n_light.Normalization();

	n_Source = ReflectLight(n_Source, n_light);

	Poynting();
	//z0 = 0.525;
	//cout << "Distance of moving " << z0 << endl;
}

void PVVA::Analytic_CalZ0Theta()
{
	// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
	Vector3D RotateAsix(Analytic_data[3], Analytic_data[4], Analytic_data[5]);
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-Analytic_data[6], RotateAsix);
	Vector3D rotatTranslate(Analytic_data[0], Analytic_data[1], Analytic_data[2]);
	rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));


	// 模型的相对坐标系转到世界坐标矩阵
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(Analytic_data[6], RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(Analytic_data[0], Analytic_data[1], Analytic_data[2]);

	Vector3 tempa = R_translateMatrix * R_rotatMatrix * Org_Source;
	Vector3 tempb = R_rotatMatrix * n_Source; // 向量只用求旋转

	Vector3 InterPoint;
	double temp;

	Analytic_Paraboloid_InterPoint(tempa, tempb, Analytic_data[8],
		Analytic_data[7], temp, InterPoint);

	Vector3 n_light = Vector3(-InterPoint.x * 2, -InterPoint.y * 2,
		Analytic_data[8] * 4);

	//计算平移的距离（此时传播方向没有改变）
	InterPoint = translateMatrix * rotatMatrix * InterPoint;
	z0 = CalDistance(Org_Source, InterPoint);

	CalPlane(z0);

	n_light.Normalization();
	n_light = rotatMatrix * n_light;
	n_Source = ReflectLight(n_Source, n_light);
	Poynting();
}

void PVVA::CalPlane(double dis)
{
	//cout << "Calculating plane wave propagation " << endl;
	//theta = Pi / 12;
	//double theta1 = Pi / 6;
	Calculation cal(f, dis, N, M);
	cal.Setds(ds);
	cal.SetInput(Ex_In, Ey_In);
	
	cal.StartCal();
	cal.output(Ex1, Ey1, Ez1, Hx1, Hy1, Hz1); // 计算平面传播

	for (int i = 0; i < N; i++)     // 更新平面位置
		for (int j = 0; j < M; j++)
		{
			Plane[i][j] = Plane[i][j] + n_Source * dis;
		}

	Org_Source = Org_Source + n_Source * dis;
	SourceGraphTrans.updateTranslate(Org_Source);
	//n_Source.set(0, 0, -1);

	/*
	ofstream outfilex1("Ex1.txt");
	ofstream outfilex2("Ey1.txt");
	ofstream outfilex3("Ez1.txt");
	ofstream outfilex4("Hx1.txt");
	ofstream outfilex5("Hy1.txt");
	ofstream outfilex6("Hz1.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfilex1 << Ex1[i][j].real() << " " << Ex1[i][j].imag() << "\n";
			outfilex2 << Ey1[i][j].real() << " " << Ey1[i][j].imag() << "\n";
			outfilex3 << Ez1[i][j].real() << " " << Ez1[i][j].imag() << "\n";
			outfilex4 << Hx1[i][j].real() << " " << Hx1[i][j].imag() << "\n";
			outfilex5 << Hy1[i][j].real() << " " << Hy1[i][j].imag() << "\n";
			outfilex6 << Hz1[i][j].real() << " " << Hz1[i][j].imag() << "\n";
		}
		
	}
	outfilex1.close();
	outfilex2.close();
	outfilex3.close();
	outfilex4.close();
	outfilex5.close();
	outfilex6.close();*/
	
	//cout << "Calculating plane wave propagation Completed" << endl;
}

void PVVA::Reflect()  
{
	//cout << "Calculating Reflecting surface" << endl;
	
	// 遍历每一个点 N * M * EleNum 
	Vector3 InterPoint;
	Vector3 Reflight, n_light; // 反射光线 和 法向量
	Vector3 n_light_Plane;  // 相对于平面的法向量

	// 绝对坐标系转换到源的坐标系（只求旋转）
	Vector3D RotateAsixSou(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(-SourceGraphTrans.getRotate_theta(), RotateAsixSou);

	Vector3 tempa, tempb;

	double tempx, tempz, tempy;  // 坐标转化临时变量
	double dir_t;  // 坡印廷矢量方向的系数
	double plane_t;
	double d1, d2; //虚拟路径
	double tempphase; // 相位
	complex<double> tempejphase; // = exp(j*phase)

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Ex_R[i][j] = 0;
			Ey_R[i][j] = 0;
			Ez_R[i][j] = 0;
			R_Plane[i][j] = 0;
			for (int k = 0; k < EleNum; k++)  //求与反射面的交点
			{
				if (IsIntersect(Plane[i][j], n_Plane[i][j], NodesXYZ[EleData[k].N1],
					NodesXYZ[EleData[k].N2], NodesXYZ[EleData[k].N3], InterPoint, dir_t))
				{
					//if (!Analytic_Paraboloid_InterPoint(Plane[i][j], n_Plane[i][j], 2.25,
					//	10, dir_t, InterPoint))  // 没有交点
					//	continue;
					//n_light = Vector3(-InterPoint.x * 2, -InterPoint.y * 2, 9);

					//tempa = NodesXYZ[EleData[k].N1] - NodesXYZ[EleData[k].N2];
					//tempb = NodesXYZ[EleData[k].N1] - NodesXYZ[EleData[k].N3];
					//n_light = tempa.Cross(tempb);  //法向量

					n_light = calSurfaceNormal(EleData[k], InterPoint);
					n_light.Normalization();
					
					Reflight = ReflectLight(n_Plane[i][j], n_light);   // 反射光线
					n_Plane[i][j] = Reflight;
					
					// 将反射面法向量转化到源坐标系上(先转换到绝对坐标系)
					n_light_Plane = rotatMatrixSou * n_light;
					n_light_Plane.Normalization();  // 单位化

					CalReflectExyz(n_light_Plane, Ex1[i][j], Ey1[i][j],
						Ez1[i][j], Ex_R[i][j], Ey_R[i][j], Ez_R[i][j]); // 只做极化变换

					if (dir_t == 0.0)  // 平面与反射面相交
					{
						R_Plane[i][j] = InterPoint;
					}
					else if (dir_t > 0.0)  // 平面在反射面的前面
					{
						plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
						d2 = CalDistance(InterPoint, R_Plane[i][j]);
						d1 = CalDistance(InterPoint, Plane[i][j]);
						
						if (plane_t > 0.0)  // 虚拟面2在反射面的前面
							tempphase = -(d1 + d2) / lamda * 2 * Pi;
						else
							tempphase = -(d1 - d2) / lamda * 2 * Pi;
						//tempphase = -(d1 + d2) / lamda * 2 * Pi;
						tempejphase = complex <double>(cos(tempphase), sin(tempphase));
						Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase);  // 只做相位变换
						Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
						Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
					}
					else   // 平面在反射面的后面
					{
						plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
						d2 = CalDistance(InterPoint, R_Plane[i][j]);
						d1 = CalDistance(InterPoint, Plane[i][j]);

						if (plane_t < 0.0)  // 虚拟面2在反射面的后面
							tempphase = (d1 + d2) / lamda * 2 * Pi;
						else
							tempphase = (d1 - d2) / lamda * 2 * Pi;
						tempejphase = complex <double>(cos(tempphase), sin(tempphase));
						Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase); // 只做相位变换
						Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
						Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
					}
					break;
				}
			}
			

		}
	} // endloop

	//源的传播方向改变
	updateSource_n(n_Source);

	CalAmplitude();  // 只做幅度变换
	/*
	ofstream outfile1("Ex_out.txt");
	ofstream outfile2("Ey_out.txt");
	ofstream outfile3("Ez_out.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfile1 << Ex_R[i][j].real() << " " << Ex_R[i][j].imag() << "\n";
			outfile2 << Ey_R[i][j].real() << " " << Ey_R[i][j].imag() << "\n";
			outfile3 << Ez_R[i][j].real() << " " << Ez_R[i][j].imag() << "\n";
		}
	}
	outfile1.close();
	outfile2.close();
	outfile3.close();*/

}

void PVVA::Analytic_Reflect()
{
	// 遍历每一个点 N * M * EleNum 
	Vector3 InterPoint;
	Vector3 Reflight, n_light; // 反射光线 和 法向量
	Vector3 n_light_Plane;  // 相对于平面的法向量

	// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
	Vector3D RotateAsix(Analytic_data[3], Analytic_data[4], Analytic_data[5]);
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-Analytic_data[6], RotateAsix);
	Vector3D rotatTranslate(Analytic_data[0], Analytic_data[1], Analytic_data[2]);
	rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));


	// 模型的相对坐标系转到世界坐标矩阵
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(Analytic_data[6], RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(Analytic_data[0], Analytic_data[1], Analytic_data[2]);


	// 绝对坐标系转换到源的坐标系（只求旋转）
	Vector3D RotateAsixSou(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(-SourceGraphTrans.getRotate_theta(), RotateAsixSou);

	Vector3 tempa, tempb;

	double tempx, tempz, tempy;  // 坐标转化临时变量
	double dir_t;  // 坡印廷矢量方向的系数
	double plane_t;
	double d1, d2; //虚拟路径
	double tempphase; // 相位
	complex<double> tempejphase; // = exp(j*phase)

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Ex_R[i][j] = 0;
			Ey_R[i][j] = 0;
			Ez_R[i][j] = 0;
			R_Plane[i][j] = 0;

			tempa = R_translateMatrix * R_rotatMatrix * Plane[i][j];
			tempb = R_rotatMatrix * n_Plane[i][j]; // 向量只用求旋转

			if (!Analytic_Paraboloid_InterPoint(tempa, tempb,
				Analytic_data[8], Analytic_data[7], dir_t, InterPoint))  // 没有交点
				continue;
			n_light = Vector3(-InterPoint.x * 2, -InterPoint.y * 2, 
				Analytic_data[8] * 4);

			n_light.Normalization();
			n_light = rotatMatrix * n_light;

			Reflight = ReflectLight(n_Plane[i][j], n_light);   // 反射光线
			n_Plane[i][j] = Reflight;
			InterPoint = translateMatrix * rotatMatrix * InterPoint;

			// 将反射面法向量转化到源坐标系上(先转换到绝对坐标系)
			n_light_Plane = rotatMatrixSou * n_light;
			n_light_Plane.Normalization();  // 单位化

			CalReflectExyz(n_light_Plane, Ex1[i][j], Ey1[i][j],
				Ez1[i][j], Ex_R[i][j], Ey_R[i][j], Ez_R[i][j]); // 只做极化变换

			if (dir_t == 0.0)  // 平面与反射面相交
			{
				R_Plane[i][j] = InterPoint;
			}
			else if (dir_t > 0.0)  // 平面在反射面的前面
			{
				plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				d1 = CalDistance(InterPoint, Plane[i][j]);

				if (plane_t > 0.0)  // 虚拟面2在反射面的前面
					tempphase = -(d1 + d2) / lamda * 2 * Pi;
				else
					tempphase = -(d1 - d2) / lamda * 2 * Pi;
				//tempphase = -(d1 + d2) / lamda * 2 * Pi;
				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase);  // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}
			else   // 平面在反射面的后面
			{
				plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				d1 = CalDistance(InterPoint, Plane[i][j]);

				if (plane_t < 0.0)  // 虚拟面2在反射面的后面
					tempphase = (d1 + d2) / lamda * 2 * Pi;
				else
					tempphase = (d1 - d2) / lamda * 2 * Pi;
				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase); // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}

		}
	} // endloop

	  //源的传播方向改变
	updateSource_n(n_Source);

	CalAmplitude();  // 只做幅度变换
}

void PVVA::Analytic_Mesh_Reflect()
{
	// 遍历每一个点 N * M * EleNum 
	Vector3 InterPoint;
	Vector3 Reflight, n_light; // 反射光线 和 法向量
	Vector3 n_light_Plane;  // 相对于平面的法向量

							// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
	Vector3D RotateAsix(Analytic_data[3], Analytic_data[4], Analytic_data[5]);
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-Analytic_data[6], RotateAsix);
	Vector3D rotatTranslate(Analytic_data[0], Analytic_data[1], Analytic_data[2]);
	rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));


	// 模型的相对坐标系转到世界坐标矩阵
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(Analytic_data[6], RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(Analytic_data[0], Analytic_data[1], Analytic_data[2]);


	// 绝对坐标系转换到源的坐标系（只求旋转）
	Vector3D RotateAsixSou(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(-SourceGraphTrans.getRotate_theta(), RotateAsixSou);

	Vector3 tempa, tempb;

	double tempx, tempz, tempy;  // 坐标转化临时变量
	double dir_t;  // 坡印廷矢量方向的系数
	double plane_t;
	double d1, d2; //虚拟路径
	double tempphase; // 相位
	complex<double> tempejphase; // = exp(j*phase)
	bool isHave = true;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Ex_R[i][j] = 0;
			Ey_R[i][j] = 0;
			Ez_R[i][j] = 0;
			R_Plane[i][j] = 0;

			tempa = R_translateMatrix * R_rotatMatrix * Plane[i][j];
			tempb = R_rotatMatrix * n_Plane[i][j]; // 向量只用求旋转

			if (!Analytic_Paraboloid_InterPoint(tempa, tempb,
				Analytic_data[8], Analytic_data[7], dir_t, InterPoint))
			{
				for (int k = 0; k < EleNum; k++)  //求与反射面的交点
				{
					if (IsIntersect(Plane[i][j], n_Plane[i][j], NodesXYZ[EleData[k].N1],
						NodesXYZ[EleData[k].N2], NodesXYZ[EleData[k].N3], InterPoint, dir_t))
					{
						n_light = calSurfaceNormal(EleData[k], InterPoint);
						n_light.Normalization();

						Reflight = ReflectLight(n_Plane[i][j], n_light);   // 反射光线
						n_Plane[i][j] = Reflight;

						// 将反射面法向量转化到源坐标系上(先转换到绝对坐标系)
						n_light_Plane = rotatMatrixSou * n_light;
						n_light_Plane.Normalization();  // 单位化
						break;
					}
					isHave = false;
				}
				if (!isHave)
				{
					continue;
					isHave = true;
				}

			}
			else
			{
				n_light = Vector3(-InterPoint.x * 2, -InterPoint.y * 2,
					Analytic_data[8] * 4);

				n_light.Normalization();
				n_light = rotatMatrix * n_light;

				Reflight = ReflectLight(n_Plane[i][j], n_light);   // 反射光线
				n_Plane[i][j] = Reflight;
				InterPoint = translateMatrix * rotatMatrix * InterPoint;

				// 将反射面法向量转化到源坐标系上(先转换到绝对坐标系)
				n_light_Plane = rotatMatrixSou * n_light;
				n_light_Plane.Normalization();  // 单位化
			}

			
			CalReflectExyz(n_light_Plane, Ex1[i][j], Ey1[i][j],
				Ez1[i][j], Ex_R[i][j], Ey_R[i][j], Ez_R[i][j]); // 只做极化变换

			if (dir_t == 0.0)  // 平面与反射面相交
			{
				R_Plane[i][j] = InterPoint;
			}
			else if (dir_t > 0.0)  // 平面在反射面的前面
			{
				plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				d1 = CalDistance(InterPoint, Plane[i][j]);

				if (plane_t > 0.0)  // 虚拟面2在反射面的前面
					tempphase = -(d1 + d2) / lamda * 2 * Pi;
				else
					tempphase = -(d1 - d2) / lamda * 2 * Pi;
				//tempphase = -(d1 + d2) / lamda * 2 * Pi;
				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase);  // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}
			else   // 平面在反射面的后面
			{
				plane_t = IntersectPlane(InterPoint, Reflight, Org_Source, n_Source, R_Plane[i][j]);
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				d1 = CalDistance(InterPoint, Plane[i][j]);

				if (plane_t < 0.0)  // 虚拟面2在反射面的后面
					tempphase = (d1 + d2) / lamda * 2 * Pi;
				else
					tempphase = (d1 - d2) / lamda * 2 * Pi;
				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase); // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}

		}
	} // endloop

	  //源的传播方向改变
	updateSource_n(n_Source);

	CalAmplitude();  // 只做幅度变换
}

Vector3 PVVA::calSurfaceNormal(const Element &_EleData, Vector3 interPoint)
{
	double a = CalDistance(NodesXYZ[_EleData.N1], interPoint);
	double b = CalDistance(NodesXYZ[_EleData.N2], interPoint);
	double c = CalDistance(NodesXYZ[_EleData.N3], interPoint);
	a *= a; b *= b; c *= c;
	double sum = 1 / (a*b + b*c + a*c);
	return (PointNormal[_EleData.N1] * b*c + PointNormal[_EleData.N2] * a*c +
		PointNormal[_EleData.N3] * b*a) * sum;
}

complex<double> PVVA::ConjugateMul(const complex<double> &A, const complex<double> &B) const
{
	return complex<double>(A.real() * B.real() + A.imag() * B.imag(),
		-A.real() * B.imag() + A.imag() * B.real());
}

complex<double> PVVA::ComMul(const complex<double>& A, const complex<double>& B) const
{
	return complex<double>(A.real() * B.real() - A.imag() * B.imag(),
		A.real() * B.imag() + A.imag() * B.real());
}

void PVVA::Split(const string & src, const string & separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator, start);
		if (index != string::npos)
		{
			substring = str.substr(start, index - start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator, index);
			if (start == string::npos) return;
		}
	} while (index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

void PVVA::JumpLine(int N, ifstream & infile)
{
	{
		string buff;
		for (int i = 0; i < N; i++)
		{
			getline(infile, buff);
		}
	}
}

double PVVA::CalDistance(const Vector3 &a, const Vector3 &b) const
{
	return pow(pow((a.x - b.x), 2) + pow((a.y - b.y), 2)+ pow((a.z - b.z), 2), 0.5);
}

bool PVVA::Analytic_Paraboloid_InterPoint(const Vector3 & O, const Vector3 & n, double Focus,
	double r, double &t, Vector3 & InterPoint)
{
	double x0 = O.x, y0 = O.y, z0 = O.z;
	double u = n.x, v = n.y, w = n.z;
	double p = Focus * 2;
	double a = u * u + v * v;
	double b = 2 * v * y0 + 2 * u * x0 - 2 * p * w;
	double c = x0 * x0 + y0 * y0 - 2 * p * z0;
	double x, y, z;
	if (a != 0)
	{
		double temp = b * b - 4 * a * c;
		if (temp >= 0)
			temp = pow(temp, 0.5);
		else
			return false;

		double tempt1, tempt2;
		tempt1 = (-b + temp) / 2.0 / a;
		tempt2 = (-b - temp) / 2.0 / a; // 求根公式的两个解

		if (tempt1 >= 0 && tempt2 >= 0) // 都大于等于0 取小的
		{
			if (tempt1 > tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
		else if (tempt1 < 0 && tempt2 < 0) // 都小于0 取大的（绝对值大的）
		{
			if (tempt1 < tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
		else                           // 取正值
		{
			if (tempt1 < tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
	}
	else                          // 只有一个交点，与法线平行
		t = -c / b;

	x = x0 + u * t;
	y = y0 + v * t;
	z = z0 + w * t;
	InterPoint.set(x, y, z);

	if (Analytic_data[9] <= x && x <= Analytic_data[12] &&
		Analytic_data[10] <= y && y <= Analytic_data[13] && 
		Analytic_data[11] <= z && z <= Analytic_data[14])
		return true;
	else
		return false;
	/*
	double _r = x * x + y * y;
	if (_r < r * r)
		return true;
	else
		return false;*/
}

/*void PVVA::Analytic_Reflect()
{
	// ********** 坐标转换 ************ 
	// 绝对坐标系转换到反射面的坐标系
	Vector3D RotateAsix(ModleGraphTrans.getRotate_x(), ModleGraphTrans.getRotate_y(),
		ModleGraphTrans.getRotate_z());
	Matrix4D rotatMatrix1 = Matrix4D::getRotateMatrix(-ModleGraphTrans.getRotate_theta(), RotateAsix);

	Vector3D rotatTranslate(ModleGraphTrans.getTrans_x(), ModleGraphTrans.getTrans_y(),
		ModleGraphTrans.getTrans_z());
	rotatTranslate = rotatMatrix1 * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D translateMatrix1 = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));

	// 反射面的坐标系转换到绝对坐标系
	Matrix4D rotatMatrix2 = Matrix4D::getRotateMatrix(ModleGraphTrans.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix2 = Matrix4D::getTranslateMatrix(ModleGraphTrans.getTrans_x(),
		ModleGraphTrans.getTrans_y(), ModleGraphTrans.getTrans_z());


	// 绝对坐标系转换到源的坐标系（只求旋转）
	Vector3D RotateAsixSou(SourceGraphTrans.getRotate_x(), SourceGraphTrans.getRotate_y(),
		SourceGraphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(-SourceGraphTrans.getRotate_theta(), RotateAsixSou);


	Vector3 InterPoint;
	Vector3 Reflight, n_light; // 反射光线 和 法向量
	Vector3 n_light_Plane;  // 相对于平面的法向量

	Vector3 v0_Plane = Plane[(N - 1) / 2][(M - 1) / 2];
	Vector3 v1_Plane = Plane[0][0];
	Vector3 v2_Plane = Plane[0][M - 1];  // 用于计算反射回来与平面的交点  // 用于计算反射回来与平面的交点

	v0_Plane = translateMatrix1 * rotatMatrix1 * v0_Plane;
	v1_Plane = translateMatrix1 * rotatMatrix1 * v1_Plane;
	v2_Plane = translateMatrix1 * rotatMatrix1 * v2_Plane;

	Vector3 E1 = v1_Plane - v0_Plane;
	Vector3 E2 = v2_Plane - v0_Plane;

	double tempx, tempz, tempy;  // 坐标转化临时变量
	double dir_t;  // 坡印廷矢量方向的系数
	double d1, d2, d3; //虚拟路径
	double tempphase; // 相位
	complex<double> tempejphase; // = exp(j*phase)

	Vector3 Plane_back;
	Vector3 n_Plane_back;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Plane_back = translateMatrix1 * rotatMatrix1 * Plane[i][j]; // 备份变换前的坐标
			n_Plane_back = rotatMatrix1 * n_Plane[i][j]; // 对向量 坐标变换只旋转

			Ex_R[i][j] = 0;
			Ey_R[i][j] = 0;
			Ez_R[i][j] = 0;

			if (!Analytic_Paraboloid_InterPoint(Plane_back, n_Plane_back, ModlePara[0],
				ModlePara[1], dir_t, InterPoint))  // 没有交点
				continue;
		
			n_light = Vector3(-InterPoint.x * 2, -InterPoint.y * 2, 2);
			Reflight = ReflectLight(n_Plane_back, n_light);   // 反射光线
			Plane_org[i][j] = Reflight;

			// 将反射面法向量转化到源坐标系上(先转换到绝对坐标系)
			n_light_Plane = rotatMatrixSou * rotatMatrix2 * n_light;
			n_light_Plane.Normalization();  // 单位化

			CalReflectExyz(n_light_Plane, Ex1[i][j], Ey1[i][j], Ez1[i][j], Ex_R[i][j], Ey_R[i][j], Ez_R[i][j]); // 只做极化变换

			if (dir_t == 0)  // 平面与反射面相交
			{
				R_Plane[i][j] = InterPoint;
			}
			else if (dir_t > 0)  // 平面在反射面的前面
			{
				d1 = CalDistance(InterPoint, Plane_back);
				d2 = abs(IntersectPoint(InterPoint, Reflight, v0_Plane, E1, E2, R_Plane[i][j]));
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				tempphase = -(d1 + d2) / lamda * 2 * Pi;

				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase);  // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}
			else   // 平面在反射面的后面
			{
				d2 = abs(IntersectPoint(InterPoint, Reflight, v0_Plane, E1, E2, R_Plane[i][j]));
				d2 = CalDistance(InterPoint, R_Plane[i][j]);
				d1 = CalDistance(InterPoint, Plane_back);

				tempphase = (d1 + d2) / lamda * 2 * Pi;

				tempejphase = complex <double>(cos(tempphase), sin(tempphase));
				Ex_R[i][j] = ComMul(Ex_R[i][j], tempejphase); // 只做相位变换
				Ey_R[i][j] = ComMul(Ey_R[i][j], tempejphase);
				Ez_R[i][j] = ComMul(Ez_R[i][j], tempejphase);
			}

			// 反射面的坐标系转换到绝对坐标系
			R_Plane[i][j] = translateMatrix2 * rotatMatrix2 * R_Plane[i][j];
		}
	} // end loop!

	CalAmplitude();

	
	ofstream outfile1("Ex_out1.txt");
	ofstream outfile2("Ey_out1.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			outfile1 << Ex_R[i][j].real() << " " << Ex_R[i][j].imag() << "\n";
			outfile2 << Ey_R[i][j].real() << " " << Ey_R[i][j].imag() << "\n";
		}
	}
	outfile1.close();
	outfile2.close();
}
*/
void PVVA::CreateGaussian(double r, double z, double w0, double f, double &Real, double &Imag) {
	double lamda = C_Speed / f;
	double k = 2 * Pi * f / C_Speed;
	double w, Rdl, theta, r2, temp;

	w = w0 * pow((1 + pow(((lamda * z) / (Pi * w0 * w0)), 2)), 0.5);
	Rdl = z / (z * z + pow((Pi * w0 * w0 / lamda), 2));
	theta = atan((lamda * z) / (Pi * w0 * w0));
	r2 = r * r;
	temp = pow((2 / w / w / Pi), 0.5) * exp(-r2 / w / w);

	Real = temp * cos(theta - k * z - Pi * r2 / lamda * Rdl);
	Imag = temp * sin(theta - k * z - Pi * r2 / lamda * Rdl);
}

bool PVVA::InterVal_IsInsect(const Vector2 &A, const Vector2 &B, const Vector2 &C, const Vector2 &D)
{
	Vector2 b = B - A; //向量b
	Vector2 d = D - C;
	Vector2 c = C - A;

	Vector2 d_V = d.Vertical(); //d的垂线

	double t;
	if (b.dot(d_V) == 0)
		return false;
	else
	{
		t = c.dot(d_V) / b.dot(d_V);
		if (t < 0 || t >1)
			return false;
		Vector2 b_V = b.Vertical();
		double u = c.dot(b_V) / b.dot(d_V);
		if (u < 0 || u > 1)
			return false;
		else
			return true;
	}

}