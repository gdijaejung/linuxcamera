#pragma once


namespace common
{
	struct Triangle
	{
		Triangle();
		Triangle( const Vector3& v1, const Vector3& v2, const Vector3& v3 );

		void Create( const Vector3& vA, const Vector3& vB, const Vector3& vC );
		bool Intersect(	const Vector3& vOrig, const Vector3& vDir,
			float* pfT = 0, float* pfU = 0, float* pfV = 0) const;
		float Distance( const Vector3& vPos )  const;

		Vector3 a, b, c;
	};

}
