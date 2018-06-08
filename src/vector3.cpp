#include "vector3.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

inline double ran()
{
	return double(rand() % RAND_MAX) / RAND_MAX;
}

Vector3 operator + ( const Vector3& A , const Vector3& B )
{
	return Vector3( A.x + B.x , A.y + B.y , A.z + B.z );
}

Vector3 operator - ( const Vector3& A , const Vector3& B )
{
	return Vector3( A.x - B.x , A.y - B.y , A.z - B.z );
}

Vector3 operator * ( const Vector3& A , const double& k )
{
	return Vector3( A.x * k , A.y * k , A.z * k );
}

Vector3 operator / ( const Vector3& A , const double& k )
{
	return Vector3( A.x / k , A.y / k , A.z / k );
}

Vector3& operator += ( Vector3& A , const Vector3& B )
{
	A = A + B;
	return A;
}

Vector3& operator -= ( Vector3& A , const Vector3& B )
{
	A = A - B;
	return A;
}

Vector3& operator *= ( Vector3& A , const double& k )
{
	A = A * k;
	return A;
}

Vector3& operator += ( Vector3& A , const double& k )
{
	A = A / k;
	return A;
}

Vector3 operator - ( const Vector3& A )
{
	return Vector3( -A.x , -A.y , -A.z );
}

ssm& operator>>(ssm& in, Vector3& v)
{
	in >> v.x >> v.y >> v.z;
	return in;
}

double Vector3::Dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

double Vector3::Norm() const
{
	return sqrt(Norm2());
}

double Vector3::Norm2() const
{
	return x * x + y * y + z * z;
}

double Vector3::Dist(const Vector3& v) const
{
	return sqrt(Dist2(v));
}

double Vector3::Dist2(const Vector3& v) const
{
	return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z);
}

double Vector3::GetMax() const
{
	return fmax(fabs(x), fmax(fabs(y), fabs(z)));
}

double& Vector3::Project(int axis)
{
	if(axis == 1)	return x;
	if(axis == 2)	return y;
	if(axis == 3)	return z;
}

double Vector3::Project(int axis) const
{
	if(axis == 1)	return x;
	if(axis == 2)	return y;
	if(axis == 3)	return z;
}

Vector3 Vector3::Regular() const
{
	return Vector3(fmin(x,1.0) ,fmin(y,1.0) ,fmin(z,1.0));
}

Vector3 Vector3::Unit() const
{
	double len = Norm();
	return Vector3(x / len, y / len, z / len);
}

bool Vector3::IsZero() const
{
	if(fabs(x) > EPS)	return false;
	if(fabs(y) > EPS)	return false;
	if(fabs(z) > EPS)	return false;
	return true;
}

Vector3 Vector3::GetAnVerticalVector() const
{
	Vector3 ret = this->Cross(Vector3( 0 , 0 , 1 ));
	if ( ret.IsZero() ) ret = Vector3( 1 , 0 , 0 );
		else ret = ret.Unit();
	return ret;
}

Vector3 Vector3::Diffuse() const
{
	srand((unsigned int)time(0));
	Vector3 Vert = GetAnVerticalVector();
	double theta = acos( sqrt( ran() ) );
	double phi = ran() * 2 * PI;
	return Rotate( Vert , theta ).Rotate( *this , phi );
}

Vector3 Vector3::Rotate( Vector3 axis , double theta ) const
{
	Vector3 ret;
	double cost = cos( theta );
	double sint = sin( theta );
	ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
	ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
	ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
	ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
	ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
	ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
	ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
	ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
	ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
	return ret;
}
