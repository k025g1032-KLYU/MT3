#include <Novice.h>
#define _USE_MATH_DEFINES 
#include <math.h>

const char kWindowTitle[] = "GC1A_11_ヨ_カンリン_タイトル";

struct Vector3 {
	float x;
	float y;
	float z;
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
	result.m[1][1] = cosf(radian);
	result.m[1][2] = sinf(radian);
	result.m[2][1] = -sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotationYMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = cosf(radian);
	result.m[0][2] = -sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotationZMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = cosf(radian);
	result.m[0][1] = sinf(radian);
	result.m[1][0] = -sinf(radian);
	result.m[1][1] = cosf(radian);
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
	result.m[2][2] = -2.0f / (farZ - nearZ);
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -(farZ + nearZ) / (farZ - nearZ);
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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{2.8f, 0.4f, -1.3f };


	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f, 0.0f, 5.0f };
	Vector3 cameraPosition{ 0.0f, 0.0f, -10.0f };
	Vector3 cameraRotation{ 0.0f, 0.0f, 0.0f };

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

		///
		/// ↓更新処理ここから
		///
		
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
			cameraPosition.z += 0.1f;
		}
		if (keys[DIK_DOWN])
		{
			cameraPosition.z -= 0.1f;
		}
		if (keys[DIK_K])
		{
			cameraRotation.y += 0.01f;
		}
		if (keys[DIK_L])
		{
			cameraRotation.y -= 0.01f;
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
		

		rotate.y += 0.03f;
		
		
		Vector3 cross = Cross(v1, v2);

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotation, cameraPosition);
		
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f , float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		Vector3 screenVertics[3];
		for (int i = 0; i < 3; i++) {
			Vector3 ndcvertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertics[i] = Transform(ndcvertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(
			int(screenVertics[0].x), int(screenVertics[0].y),
			int(screenVertics[1].x), int(screenVertics[1].y),
			int(screenVertics[2].x), int(screenVertics[2].y),
			RED, kFillModeSolid);

		Novice::DrawTriangle(
			int(kLocalVertices[0].x), int(kLocalVertices[0].y),
			int(kLocalVertices[1].x), int(kLocalVertices[1].y),
			int(kLocalVertices[2].x), int(kLocalVertices[2].y),
			BLACK, kFillModeWireFrame);
		
		
		PrintVector3(0, 0, cross, "cross");

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
