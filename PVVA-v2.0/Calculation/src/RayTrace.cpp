#include "Calculation/include/RayTrace.h"


void RayTrace::Reflect_Ray(double * a, Vector3 &n, Vector3 org,
	Vector3 &interPoint, double xmin, double xmax,
	double ymin, double ymax, double zmin, double zmax)
{
	double t;
	if (Ray_CurvedSurface(a, n, org, t, interPoint))
	{
		if (xmin <= interPoint.x && interPoint.x <= xmax &&
			ymin <= interPoint.y && interPoint.y <= ymax &&
			zmin <= interPoint.z && interPoint.z <= zmax) // 判断是否在给出的区间内
		{
			double x = 2 * a[0] * interPoint.x + a[3] * interPoint.y + a[5] * interPoint.z + a[6];
			double y = 2 * a[1] * interPoint.y + a[3] * interPoint.x + a[4] * interPoint.z + a[7];
			double z = 2 * a[2] * interPoint.z + a[4] * interPoint.y + a[5] * interPoint.x + a[8];
			Vector3 tempn(x, y, z);
			if (tempn.Dot(n) > 0)
				tempn.set(-x, -y, -z);
			n = ReflectLight(n, tempn);
		}
		else
			interPoint = org; // 让交点等于起点 方向不变 避免对无交点时特殊处理
	}
}

// F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z +
// a5*x*z + a6*x + a7*y + a8*z + a9. 与 射线 org + t * n 的交点
bool RayTrace::Ray_CurvedSurface(double * a, Vector3 n, Vector3 org,
	double & t, Vector3 &interPoint)
{
	double x0 = org.x, y0 = org.y, z0 = org.z;
	double x1 = n.x, y1 = n.y, z1 = n.z;

	double A = a[0] * x1 * x1 + a[1] * y1 * y1 + a[2] * z1 * z1 + a[3] * x1 * y1 +
		a[4] * z1 * y1 + a[5] * x1 * z1;
	double B = 2 * a[0] * x1 * x0 + 2 * a[1] * y1 * y0 + 2 * a[2] * z1 * z0 +
		a[3] * (x0 * y1 + x1 * y0) + a[4] * (z0 * y1 + z1 * y0) + a[5] * (z0 * x1 + z1 * x0) + 
		a[6] * x1 + a[7] * y1 + a[8] * z1;
	double C = a[0] * x0 * x0 + a[1] * y0 * y0 + a[2] * z0 * z0 +
		a[3] * x0 * y0 + a[4] * z0 * y0 + a[5] * x0 * z0 +
		a[6] * x0 + a[7] * y0 + a[8] * z0 + a[9];

	if (A != 0)
	{
		double temp = B * B - 4 * A * C;
		if (temp >= 0)
			temp = pow(temp, 0.5);
		else
			return false;

		double tempt1, tempt2;
		tempt1 = (-B + temp) / 2.0 / A;
		tempt2 = (-B - temp) / 2.0 / A; // 求根公式的两个解

		if (tempt1 >= 0 && tempt2 >= 0) // 都大于等于0 取小的
		{
			if (tempt1 > tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
		else if (tempt1 < 0 && tempt2 < 0) // 都小于0 无解
		{
			return false;
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
		t = -C / B;

	if(t < 0)
		return false;
	else
	{
		interPoint.set(x0 + x1 * t, y0 + y1 * t, z0 + z1 * t);
		return true;
	}

}

bool RayTrace::IsIntersect(const Vector3 & orig, const Vector3 & dir, const Vector3 & v0, const Vector3 & v1, const Vector3 & v2, Vector3 & intersection, double & t)
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

Vector3 RayTrace::ReflectLight(Vector3 a, Vector3 n)
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

double RayTrace::CalDistance(const Vector3 &a, const Vector3 &b) const
{
	return pow(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2), 0.5);
}

bool RayTrace::getZ(double * a, double x, double y, double zmin, double zmax, double &z) const
{
	double A = a[2];
	double B = a[4] * y + a[5] * x + a[8];
	double C = a[0] * x*x + a[1] * y*y + a[3] * x*y +
		a[6] * x + a[7] * y + a[9];

	if (A != 0)
	{
		double temp = B * B - 4 * A * C;
		if (temp >= 0)
			temp = pow(temp, 0.5);
		else
			return false;

		double tempt1, tempt2;
		tempt1 = (-B + temp) / 2.0 / A;
		tempt2 = (-B - temp) / 2.0 / A; // 求根公式的两个解

		if (tempt1 >= zmin && tempt1 <= zmax &&
			tempt2 >= zmin && tempt2 <= zmax) // 都在范围内取大值
		{
			if (tempt1 > tempt2)
				z = tempt2;
			else
				z = tempt1;
		}
		else if (tempt1 >= zmin && tempt1 <= zmax)
			z = tempt1;
		else if (tempt2 >= zmin && tempt2 <= zmax)
			z = tempt2;
		else
			return false;

	}
	else                          // 只有一个交点，与法线平行
	{
		if (B == 0)
			return false;
		z = -C / B;
		if (z < zmin || z > zmax)
			return false;
	}

	return true;
}


