#include "DxLib.h"
#include"Vector3.h"

//関数宣言
//球の描画
//DxLib = >int DrawSphere3D(VECTOR	ConterPos, float	r, int	DivNum, unsigned	int	DifColor, unsigned	int	Spccolor, unsigned	int	FillFlag);
int DrawSphere3D(const	Vector3& ConterPos, const float	r, const int	DivNum,
	const unsigned	int	DifColor, const unsigned	int	Spccolor, const unsigned	int	FillFlag)
{
	VECTOR	centerPos = { ConterPos.x,ConterPos.y, ConterPos.z };//構造体　初期化リスト

	return	DrawSphere3D(centerPos, r, DivNum, DifColor, Spccolor, FillFlag);//DxLib関数呼び出し
}

//円錐（cone）の描画
int	DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const	int	DivNum, const	unsigned	int	DifColor, const	unsigned	int	SpcColor, const	int	FillFlag)
{
	VECTOR	topPos = { TopPos.x,TopPos.y, TopPos.z };
	VECTOR	bottomPos{ BottomPos.x,BottomPos.y, BottomPos.z, };

	return	DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

//線分の描画
//int	DrawLine3D(VECTOR	Pos1, VECTOR	Pos2, unsigned int color);
int	DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, unsigned int color)
{
	VECTOR	p1 = { Pos1.x,Pos1.y,Pos1.z };
	VECTOR	p2 = { Pos2.x,Pos2.y,Pos2.z };

	return	DrawLine3D(p1, p2, color);
}

//カメラの位置と姿勢
int	SetCameraPositionAndTargetAndUpVec(
	const	Vector3& cameraPosition, //位置
	const	Vector3& cameraTarget,	//注視点
	const	Vector3& cameraUp		//上向き
)
{
	VECTOR	position = { cameraPosition.x,cameraPosition.y, cameraPosition.z };
	VECTOR	target = { cameraTarget.x,cameraTarget.y, cameraTarget.z };
	VECTOR	up = { cameraUp.x,cameraUp.y, cameraUp.z };

	return	SetCameraPositionAndTargetAndUpVec(position, target, up);
}

void DrawAxis3D(const float length)	//	x,y,z軸の描画
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	//	x軸
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	//	y軸
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	//	z軸
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	const int WindowWidth = 1024;
	const int WindowHeight = 576;
	ChangeWindowMode(true);//ウィンドウモードになる
	SetGraphMode(WindowWidth, WindowHeight, 32);//画面モードのセット
	SetBackgroundColor(0, 0, 64);//背景色RGB
	if (DxLib_Init() == -1) { return -1; }// DXlibの初期化
	SetDrawScreen(DX_SCREEN_BACK);// (ダブルバッファ)描画先グラフィック領域は裏面を指定

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	//カメラ初期化
	Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//クリップ面      近　　　遠
	SetCameraNearFar(1.0f, 1000.0f);//カメラの有効範囲の設定
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);//画面の中心をカメラの中心に合わせる
	SetCameraPositionAndTargetAndUpVec(
		Vector3(0.0f, 0.0f, -120.0f),	//	カメラの位置
		Vector3(0.0f, 0.0f, 0.0f),		//	カメラの注視点
		Vector3(0.0f, 1.0f, 0.0f)		//	カメラの上の向き
	);

	// 時間計測に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	// 補間で使うデータ
	// start→endを5[s]で完了させる
	Vector3 start(-100.0f, 0, 0);	//	スタート地点
	Vector3 end(+100.0f, 0, 0);		//	エンド地点
	float maxTime = 5.0f;			//	全体時間[s]
	float timeRate;					//	何%時間が進んだか(率)

	// 玉の位置
	Vector3 position;

	// 実行前にカウント値を取得
	startCount = GetNowHiPerformanceCount();	//	long long int型 64bit int

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 更新----------------------------

		// [R]キーで、リセット
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}

		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float>(elapsedCount) / 1'000'000.0f;

		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		// 描画----------------------------
		ClearDrawScreen();
		DrawAxis3D(500.0f);

		// 玉の描画------------------------
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), true);

		//
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position(%5.1f,%5.1f,%5.1f)",
			position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f[s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R]:Restart");

		//フリップする
		ScreenFlip();
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
