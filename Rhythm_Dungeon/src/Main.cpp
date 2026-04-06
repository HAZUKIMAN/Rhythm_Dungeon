#include <DxLib.h>
#include <crtdbg.h>
#include "lib/frame/Frame.h"
#include "game/scene/SceneManager.h"
#include "lib/fade/Fade.h"
#include "Data.h"
#include "lib/Input/Input.h"
#include "game/effect/effekseer.h"


// プログラムは WinMain から始まります
int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// メモリリークチェック用
	ChangeWindowMode(TRUE);		// ウィンドウモードで起動
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	// 一番最初に１回だけやる処理
	SetDrawScreen(DX_SCREEN_BACK);
	SetTransColor(255, 0, 255);	// 透過色指定
	
	const int EFFCTNUM = 10;
	const int PARTICLENUM = 100000;

	//入力制御初期化
	Input::Init();
	//エフェクトの初期化
	CEffekseerCtrl::Init(EFFCTNUM, PARTICLENUM);

	// フレームレート初期化
	CFrameRate::Init();

	// オブジェクト作成
	CSceneManager scene;
	CFade::Alloc();


	Data* p = Data::GetInstance();
	//Data* d = new Data(*p);

	// ゲームメインループ
	while (ProcessMessage() != -1)
	{
		Sleep(1);	//システムに処理を返す

		// エスケープキーが押されたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
		// フレームを進めていいか判断
		if (!CFrameRate::CheckNext()) continue;

		ClearDrawScreen();		// 画像クリア

		// フレームレート計算
		CFrameRate::Calc();

		// ここからゲーム処理記述-----------------------------
		scene.Loop();
		scene.Draw();
		// ここまでゲーム処理記述-----------------------------

		// FPS表示
		CFrameRate::PrintFps();
		
		//エフェクト
		CEffekseerCtrl::UpdateAutoCamera();
		CEffekseerCtrl::Draw();

		//入力制御ステップ
		Input::Step();

		ScreenFlip();			// フリップ関数

	}

	// オブジェクト削除
	CFade::Release();

	//エフェクト破棄
	CEffekseerCtrl::Exit();

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
