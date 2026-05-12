#include <Novice.h>
#define _USE_MATH_DEFINES 
#include <math.h>

const char kWindowTitle[] = "GC1A_11_ヨ_カンリン_タイトル";


static const int kColumnWidth = 60;
static const int kRowHeight = 20;


struct Vector3 {
	float x;
	float y;
	float z;
};

void PrintVector3(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x+ kColumnWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x+ kColumnWidth*2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x+ kColumnWidth*3, y, "%s",label);
}

struct Matrix4x4 {
	float m[4][4];
};

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

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1.0f;
	}
	return result;
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


void MatrixScreenPrint(int x, int y, const Matrix4x4& v, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; col++) {
			Novice::ScreenPrintf(x + col * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", v.m[row][col]);
		}
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 scale = { 1.2f, 0.79f, -2.1f };
	Vector3 rotation ={ 0.4f,1.43f, -0.8f };
	Vector3 translation = { 2.7f, -4.15f, 1.57f };

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

	
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotation, translation);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrint(0, 0 , worldMatrix,"worldMatrix");
		

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
