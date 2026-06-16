#include <Novice.h>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <imgui.h>
#include <algorithm>

const char kWindowTitle[] = "GC1A_11_ヨ_カンリン_タイトル";

struct mousePosition {
	int x;
	int y;
};

mousePosition mousePos;
mousePosition prevMousePos;

struct Vector3 {
	float x;
	float y;
	float z;
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 halfSize;
	Vector3 orientation[3];
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Line{
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertices[3];
};

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Multiply(const Vector3& v, float scalar) {
	Vector3 result{};
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return result;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0.0f) {
		return Vector3{0.0f, 0.0f, 0.0f}; // 長さがゼロの場合はゼロベクトルを返す
	}
	return Multiply(v, 1.0f / length);
}

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

void PrintVector3(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x+ kColumnWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x+ kColumnWidth*2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x+ kColumnWidth*3, y, "%s",label);
}

struct Matrix4x4 {
	float m[4][4];
};

Matrix4x4 Add(const Matrix4x4& v1, const Matrix4x4& v2) {
	Matrix4x4 result{};
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			result.m[i][j] = v1.m[i][j] + v2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Subtract(const Matrix4x4& v1, const Matrix4x4& v2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = v1.m[i][j] - v2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& v1, const Matrix4x4& v2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] =
				v1.m[i][0] * v2.m[0][j] +
				v1.m[i][1] * v2.m[1][j] +
				v1.m[i][2] * v2.m[2][j] +
				v1.m[i][3] * v2.m[3][j];
		}
	}
	return result;
}


Matrix4x4 Inverse(const Matrix4x4& v) {
	
	Matrix4x4 inv{};
	float m[16] = { 
		v.m[0][0],v.m[0][1],v.m[0][2],v.m[0][3],
		v.m[1][0],v.m[1][1],v.m[1][2],v.m[1][3],
		v.m[2][0],v.m[2][1],v.m[2][2],v.m[2][3],
		v.m[3][0],v.m[3][1],v.m[3][2],v.m[3][3]
	}; 

	float invOut[16];

	invOut[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	invOut[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	invOut[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	invOut[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	invOut[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	invOut[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	invOut[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	invOut[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	invOut[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	invOut[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	invOut[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	invOut[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	invOut[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	invOut[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	invOut[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	invOut[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	float det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];

	det = 1.0f / det;

	for (int i = 0; i < 16; i++) {
		invOut[i] *= det;
	}

	// 回填到 Matrix4x4
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			inv.m[i][j] = invOut[i * 4 + j];
		}
	}

	return inv;
	//return result;
}

Matrix4x4 Transpose(const Matrix4x4& v) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = v.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1.0f;
	}
	return result;
}



void MatrixScreenPrint(int x, int y, const Matrix4x4& v, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; col++) {
			Novice::ScreenPrintf(x + col * kColumnWidth, y + (row+1) * kRowHeight, "%6.02f", v.m[row][col]);
		}
	}
	Novice::ScreenPrintf(x , y, "%s", label);
}



Matrix4x4 MakeTranslateMatrix(const Vector3& translation) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[3][0] = translation.x;
	result.m[3][1] = translation.y;
	result.m[3][2] = translation.z;
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	return result;
}

Matrix4x4 MakeRotationXMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotationYMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotationZMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	return result;
}


Vector3 Transform(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result;

	float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	// 透視除法（很重要，尤其是 Projection）
	if (w != 0.0f) {
		x /= w;
		y /= w;
		z /= w;
	}

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
	Matrix4x4 matScale = MakeScaleMatrix(scale);
	Matrix4x4 matRotX = MakeRotationXMatrix(rotation.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rotation.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rotation.z);
	Matrix4x4 matTrans = MakeTranslateMatrix(translation);

	Matrix4x4 matWorld = Multiply(matScale, matRotX);
	matWorld = Multiply(matWorld, matRotY);
	matWorld = Multiply(matWorld, matRotZ);
	matWorld = Multiply(matWorld, matTrans);

	return matWorld;
}

Matrix4x4 MakeOrthoGraphicMatrix(float left, float top, float right, float bottom, float nearZ, float farZ) {
	Matrix4x4 result{};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farZ - nearZ);
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -(nearZ) / (farZ - nearZ);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearZ, float farZ) {
	Matrix4x4 result{};
	float f = 1.0f / tanf(fovY / 2.0f);
	result.m[0][0] = f / aspect;
	result.m[1][1] = f;
	result.m[2][2] = (farZ) / (farZ -nearZ );
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(farZ * nearZ) / ( farZ-nearZ );
	return result;
}

Matrix4x4 MakeViewportMatrix(float Left, float Top, float width, float height,float MaxD,float MinD) {
	Matrix4x4 result{};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f; // Y軸を反転
	result.m[2][2] = MaxD-MinD;
	result.m[3][0] = Left + width / 2.0f;
	result.m[3][1] = Top + height / 2.0f;
	result.m[3][2] = MinD;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float lengthSq = Dot(v2, v2);
	if (lengthSq == 0.0f) {
		return Vector3{ 0.0f, 0.0f, 0.0f }; // ゼロベクトルを返す
	}
	float scalar = dot / lengthSq;
	return Multiply(v2, scalar);
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 toPoint = Subtract(point, segment.origin);
	Vector3 direction = Normalize(segment.diff);
	return Add(segment.origin, Multiply(direction, Dot(toPoint, direction)));
}

Vector3 Perpendiculusar(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return Normalize(Vector3{ -vector.y, vector.x, 0.0f });
	}
	else {
		return Normalize(Vector3{ 0.0f, -vector.z, vector.y });
	}
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;
	const int kSubDivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2) / float(kSubDivision);

	for (int xIndex = 0; xIndex <= kSubDivision; xIndex++) {
		float x = -kGridHalfWidth + kGridEvery * xIndex;
		Vector3 start = Transform({ x, 0.0f, -kGridHalfWidth }, viewProjectionMatrix);
		Vector3 end = Transform({ x, 0.0f, kGridHalfWidth }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), BLACK);
	}

	for (int zIndex = 0; zIndex <= kSubDivision; zIndex++) {
		float z = -kGridHalfWidth + kGridEvery * zIndex;
		Vector3 start = Transform({ -kGridHalfWidth, 0.0f, z }, viewProjectionMatrix);
		Vector3 end = Transform({ kGridHalfWidth, 0.0f, z }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), BLACK);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	const int kSubdivision = 20;
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);
	const float kLatEvery = float(M_PI) / float(kSubdivision);

	for (int latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float latA = -float(M_PI) / 2.0f + kLatEvery * latIndex;
		float latB = -float(M_PI) / 2.0f + kLatEvery * (latIndex + 1);
		for (int lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			float lonA = kLonEvery * lonIndex;
			float lonC = kLonEvery * (lonIndex + 1);
			Vector3 pointOnSphereA;
			pointOnSphereA.x = sphere.center.x + sphere.radius * std::cos(latA) * std::cos(lonA);
			pointOnSphereA.y = sphere.center.y + sphere.radius * std::sin(latA);
			pointOnSphereA.z = sphere.center.z + sphere.radius * std::cos(latA) * std::sin(lonA);
			

			Vector3 pointOnSphereB;
			pointOnSphereB.x = sphere.center.x + sphere.radius * std::cos(latB) * std::cos(lonA);
			pointOnSphereB.y = sphere.center.y + sphere.radius * std::sin(latB);
			pointOnSphereB.z = sphere.center.z + sphere.radius * std::cos(latB) * std::sin(lonA);
			

			Vector3 pointOnSphereC;
			pointOnSphereC.x = sphere.center.x + sphere.radius * std::cos(latA) * std::cos(lonC);
			pointOnSphereC.y = sphere.center.y + sphere.radius * std::sin(latA);
			pointOnSphereC.z = sphere.center.z + sphere.radius * std::cos(latA) * std::sin(lonC);
			


			Vector3 screenA = Transform(Transform(pointOnSphereA, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(pointOnSphereB, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(pointOnSphereC, viewProjectionMatrix), viewportMatrix);


			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);
		}
	}
}

void DrawPlane(
	const Plane& plane,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	int color)
{
	Vector3 normal = Normalize(plane.normal);

	Vector3 center = Multiply(normal, plane.distance);

	Vector3 perpendiculusars[4];
	perpendiculusars[0] = Normalize(Perpendiculusar(normal));
	perpendiculusars[1] = Multiply(perpendiculusars[0], -1.0f);

	perpendiculusars[2] = Normalize(Cross(normal, perpendiculusars[0]));
	perpendiculusars[3] = Multiply(perpendiculusars[2], -1.0f);

	Vector3 points[4];

	for (int index = 0; index < 4; ++index)
	{
		Vector3 extend = Multiply(perpendiculusars[index], 2.0f);
		Vector3 point = Add(center, extend);

		points[index] =
			Transform(
				Transform(point, viewProjectionMatrix),
				viewportMatrix
			);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	Vector3 screenVertices[3];
	for (int i = 0; i < 3; i++) {
		screenVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), color);
	Novice::DrawLine(int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), color);
	Novice::DrawLine(int(screenVertices[2].x), int(screenVertices[2].y), int(screenVertices[0].x), int(screenVertices[0].y), color);
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	Vector3 vertices[8] = {
		{aabb.min.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.max.z},
		{aabb.min.x, aabb.max.y, aabb.max.z}
	};
	for (int i = 0; i < 8; i++) {
		vertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}
	int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},
		{4, 5}, {5, 6}, {6, 7}, {7, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};
	for (int i = 0; i < 12; i++) {
		Vector3 start = vertices[edges[i][0]];
		Vector3 end = vertices[edges[i][1]];
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}
}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	Vector3 vertices[8];
	for (int i = 0; i < 8; i++) {
		Vector3 localVertex = {
			(i & 1) ? obb.halfSize.x : -obb.halfSize.x,
			(i & 2) ? obb.halfSize.y : -obb.halfSize.y,
			(i & 4) ? obb.halfSize.z : -obb.halfSize.z
		};
		Vector3 worldVertex = Add(obb.center, Add(Multiply(obb.orientation[0], localVertex.x), Add(Multiply(obb.orientation[1], localVertex.y), Multiply(obb.orientation[2], localVertex.z))));
		vertices[i] = Transform(Transform(worldVertex, viewProjectionMatrix), viewportMatrix);
	}
	int edges[12][2] = {
		 {0, 1}, {1, 3}, {3, 2}, {2, 0},
		{4, 5}, {5, 7}, {7, 6}, {6, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};
	for (int i = 0; i < 12; i++) {
		Vector3 start = vertices[edges[i][0]];
		Vector3 end = vertices[edges[i][1]];
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}
}

bool BxBCollision(const Sphere& sphere1, const Sphere& sphere2) {
	float distanceSq = (sphere1.center.x - sphere2.center.x) * (sphere1.center.x - sphere2.center.x) +
		(sphere1.center.y - sphere2.center.y) * (sphere1.center.y - sphere2.center.y) +
		(sphere1.center.z - sphere2.center.z) * (sphere1.center.z - sphere2.center.z);
	float radiusSum = sphere1.radius + sphere2.radius;
	if (distanceSq > radiusSum * radiusSum) {
		return false;
	}
	else
	{
		return true;
	}
}

bool BxPCollision(const Sphere& sphere, const Plane& plane)
{
	Vector3 normal = Normalize(plane.normal);

	float distance =
		Dot(normal, sphere.center) - plane.distance;

	return std::abs(distance) <= sphere.radius;
}

bool SxPCollision(const Segment& segment, const Plane& plane)
{
	Vector3 normal = Normalize(plane.normal);

	float dot = Dot(segment.diff, normal);

	// 線段和平面平行
	if (dot == 0.0f)
	{
		return false;
	}

	float t =
		(plane.distance - Dot(segment.origin, normal))
		/ dot;

	// t 在 0~1 之間，代表碰撞點在線段範圍內
	return 0.0f <= t && t <= 1.0f;
}

bool TxSCollision(const Triangle& triangle, const Segment& segment)
{
	//邊向量
	Vector3 e01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 e02 = Subtract(triangle.vertices[2], triangle.vertices[0]);
	//法線
	Vector3 normal = Normalize(Cross(e01, e02));

	// 線段方向向量t
	float denominator = Dot(normal, segment.diff);

	if (fabs(denominator) < 0.0001f)
	{
		// 平行
		return false;
	}

	float t =
		Dot(normal, Subtract(triangle.vertices[0], segment.origin))
		/ denominator;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	//P
	Vector3 p = Add(segment.origin, Multiply(segment.diff, t));


	Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[0]));
	Vector3 cross02 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[1]));
	Vector3 cross03 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[2]));

	if (Dot(normal, cross01) >= 0 && Dot(normal, cross02) >= 0 && Dot(normal, cross03) >= 0)
	{
		// 碰撞
		return true;
	}
	else
	{
		return false;
	}

}

bool AABBCollision(const AABB& aabb1, const AABB& aabb2)
{
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}

bool AABBxBCollision(const AABB& aabb, const Sphere& sphere)
{
	float closestX = std::clamp(sphere.center.x, aabb.min.x, aabb.max.x);
	float closestY = std::clamp(sphere.center.y, aabb.min.y, aabb.max.y);
	float closestZ = std::clamp(sphere.center.z, aabb.min.z, aabb.max.z);
	float distanceSq = (closestX - sphere.center.x) * (closestX - sphere.center.x) +
		(closestY - sphere.center.y) * (closestY - sphere.center.y) +
		(closestZ - sphere.center.z) * (closestZ - sphere.center.z);
	return distanceSq <= sphere.radius * sphere.radius;
}

bool AABBxSCollision(const AABB& aabb, const Segment& segment)
{
	float txMin =
		(aabb.min.x - segment.origin.x) /
		segment.diff.x;

	float txMax =
		(aabb.max.x - segment.origin.x) /
		segment.diff.x;

	if (txMin > txMax) {
		std::swap(txMin, txMax);
	}

	float tyMin =
		(aabb.min.y - segment.origin.y) /
		segment.diff.y;

	float tyMax =
		(aabb.max.y - segment.origin.y) /
		segment.diff.y;

	if (tyMin > tyMax) {
		std::swap(tyMin, tyMax);
	}

	float tzMin =
		(aabb.min.z - segment.origin.z) /
		segment.diff.z;

	float tzMax =
		(aabb.max.z - segment.origin.z) /
		segment.diff.z;

	if (tzMin > tzMax) {
		std::swap(tzMin, tzMax);
	}

	float tNear =
		(std::max)({ txMin, tyMin, tzMin });

	float tFar =
		(std::min)({ txMax, tyMax, tzMax });

	return
		tNear <= tFar &&
		tFar >= 0.0f &&
		tNear <= 1.0f;
}

bool OBBxBCollision(const OBB& obb, const Sphere& sphere)
{
	Vector3 diff = Subtract(sphere.center, obb.center);

	Vector3 closestPoint = obb.center;

	for (int i = 0; i < 3; ++i) {
		float distance = Dot(diff, obb.orientation[i]);

		if (i == 0) {
			distance = std::clamp(distance, -obb.halfSize.x, obb.halfSize.x);
		}
		else if (i == 1) {
			distance = std::clamp(distance, -obb.halfSize.y, obb.halfSize.y);
		}
		else {
			distance = std::clamp(distance, -obb.halfSize.z, obb.halfSize.z);
		}

		closestPoint = Add(
			closestPoint,
			Multiply(obb.orientation[i], distance)
		);
	}

	Vector3 difference = Subtract(sphere.center, closestPoint);

	return Dot(difference, difference) <= sphere.radius * sphere.radius;
}

Vector3 ToLocalPoint(const Vector3& point, const OBB& obb)
{
	Vector3 p = Subtract(point, obb.center);

	return {
		Dot(p, obb.orientation[0]),
		Dot(p, obb.orientation[1]),
		Dot(p, obb.orientation[2])
	};
}

bool OBBxSCollision(const OBB& obb, const Segment& segment)
{
	Segment localSegment;
	localSegment.origin = ToLocalPoint(segment.origin, obb);
	localSegment.diff = {
		Dot(segment.diff, obb.orientation[0]),
		Dot(segment.diff, obb.orientation[1]),
		Dot(segment.diff, obb.orientation[2])
	};

	AABB localAABB;
	localAABB.min = Multiply(obb.halfSize, -1.0f);
	localAABB.max = obb.halfSize;

	return AABBxSCollision(localAABB, localSegment);
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.0f, -10.0f };
	Vector3 cameraRotation{ 0.0f, 0.0f, 0.0f };

	Vector3 cameraTarget{ 0.0f, 0.0f, 0.0f };
	float cameraTheta = 0.0f;
	float cameraRadius = 10.0f;
	float cameraHeight = 3.0f;
	float cameraPhi = 0.0f;

	//AABB aabb1{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f } };
	OBB obb{
		{ -1.0f, 0.0f, 0.0f },
        {0.5f, 0.5f, 0.5f},{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},} };
	Sphere sphere{ { 0.0f, 1.0f, 0.0f }, 0.5f };

	Segment segment{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.5f, -2.0f } };
	Vector3 point{ -1.5f,0.6f,0.6f };
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point,segment);

	int kWindowWidth = 1280;
	int kWindowHeight = 720;
	const Vector3 kLocalVertices[3] =
	{
		{  0.0f,  1.0f, 0.0f },
	    { -1.0f, -1.0f, 0.0f },
	    {  1.0f, -1.0f, 0.0f },  // 左下
	};
	
	
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		
		prevMousePos = mousePos;
		Novice::GetMousePosition(&mousePos.x, &mousePos.y);

		///
		/// ↓更新処理ここから
		///
		
		Novice::GetMousePosition(&mousePos.x, &mousePos.y);

		Novice::ScreenPrintf(0, 0, "Mouse Position: (%d, %d)", int(mousePos.x), int(mousePos.y));
		Novice::ScreenPrintf(0, 20, "Previous Mouse Position: (%d, %d)", int(prevMousePos.x), int(prevMousePos.y));

		
		if (keys[DIK_RIGHT])
		{
			cameraPosition.x += 0.1f;	
		}
		if(keys[DIK_LEFT])
		{
			cameraPosition.x -= 0.1f;
		}
		if (keys[DIK_UP])
		{
			cameraPosition.y += 0.1f;
		}
		if (keys[DIK_DOWN])
		{
			cameraPosition.y -= 0.1f;
		}

		if (keys[DIK_K])
		{
			cameraTheta += 0.02f;

		}
		if (keys[DIK_L])
		{
			cameraTheta -= 0.02f;
		}

		
		if (Novice::IsPressMouse(1))
		{
			cameraTheta += float(mousePos.x - prevMousePos.x) * 0.01f;
			cameraPhi += float(mousePos.y - prevMousePos.y) * 0.01f;

			cameraPhi = std::clamp(cameraPhi, -1.4f, 1.4f);

			cameraPosition.x =
				cameraTarget.x +
				cameraRadius *
				std::cos(cameraPhi) *
				std::sin(cameraTheta);

			cameraPosition.y =
				cameraTarget.y +
				cameraRadius *
				std::sin(cameraPhi);

			cameraPosition.z =
				cameraTarget.z -
				cameraRadius *
				std::cos(cameraPhi) *
				std::cos(cameraTheta);

			cameraRotation.x = cameraPhi;
			cameraRotation.y = -cameraTheta;
			cameraRotation.z = 0.0f;
		}
		

		if (keys[DIK_D])
		{
			translate.x += 0.1f;
		}
		if (keys[DIK_A])
		{
			translate.x -= 0.1f;
		}
		if (keys[DIK_W])
		{
			translate.z += 0.1f;
		}
		if (keys[DIK_S])
		{
			translate.z -= 0.1f;
		}
		
		if (keys[DIK_R])
		{
			rotate={ 0.0f,0.0f,0.0f };
			translate={ 0.0f, 0.0f, 0.0f };
			cameraPosition={ 0.0f, 1.0f, -10.0f };
			cameraRotation={ 0.0f, 0.0f, 0.0f };

			cameraTarget={ 0.0f, 0.0f, 0.0f };
			cameraTheta = 0.0f;
			cameraRadius = 10.0f;
			cameraHeight = 3.0f;
			cameraPhi = 0.0f;
		}
		
		
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotation, cameraPosition);
		
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		/*aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);

		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);*/

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f , float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
	
		prevMousePos = mousePos;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

	
	/*	int segmentColor = WHITE;

		(IsCollision(triangle, segment)) ? segmentColor = RED : segmentColor = WHITE;*/

		

		DrawSphere({ cameraTarget,0.01f }, worldViewProjectionMatrix, viewportMatrix, WHITE); // カメラターゲットを描画

		//DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, AABBxSCollision(aabb1, segment) ? RED : WHITE);
		DrawSegment(segment, worldViewProjectionMatrix, viewportMatrix, OBBxSCollision(obb, segment) ? RED : WHITE);
		DrawOBB(obb, worldViewProjectionMatrix, viewportMatrix, OBBxSCollision(obb, segment) ? RED : WHITE);
		//DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, OBBxBCollision(obb, sphere) ? RED : WHITE);
	

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		
		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("Camera Position", &cameraPosition.x, 0.1f);
		ImGui::DragFloat3("Camera Rotation", &cameraRotation.x, 0.01f);
		ImGui::DragFloat3("Camera Target", &cameraTarget.x, 0.01f);
		/*ImGui::DragFloat3("AABB1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("AABB1.max", &aabb1.max.x, 0.01f);*/
		ImGui::DragFloat3("OBB Center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("OBB Orientation 0", &obb.orientation[0].x, 0.01f);
		ImGui::DragFloat3("OBB Orientation 1", &obb.orientation[1].x, 0.01f);
		ImGui::DragFloat3("OBB Orientation 2", &obb.orientation[2].x, 0.01f);
		ImGui::DragFloat3("OBB Half Size", &obb.halfSize.x, 0.01f);
		/*ImGui::DragFloat3("Sphere Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere Radius", &sphere.radius, 0.01f);*/
		
		ImGui::DragFloat3("Segment Center", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment Diff", &segment.diff.x, 0.01f);
		/*ImGui::DragFloat3("Plane Center", &planeCenter.x, 0.01f);
		ImGui::DragFloat("Plane Distance", &planeDistance, 0.01f);*/
		//ImGui::InputFloat3("Project", &project.x,"%.3f",ImGuiInputTextFlags_ReadOnly);
		ImGui::End();


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
