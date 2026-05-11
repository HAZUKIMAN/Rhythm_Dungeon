#pragma once

//	ゲームの様々な場所で使いそうな定義を集める


//unsigned int を UnInt として扱う
typedef unsigned int UnInt;

// ゲームの遷移
enum tagScene {
	SCENEID_TITLE,		// タイトル画面
	SCENEID_GAME,		// ゲーム画面
	SCENEID_GAMEOVER,	// タイトル画面
	SCENEID_CLEAR,		// タイトル画面

	SCENEID_NUM,		// シーン数
};

//ウィンドウの幅・高さ
static constexpr int WINDOW_WIDTH = 1600;
static constexpr int WINDOW_HEIGHT = 900;

//ウィンドウの中心
static constexpr int WINDOW_WIDTH_HALF = WINDOW_WIDTH / 2;
static constexpr int WINDOW_HEIGHT_HALF = WINDOW_HEIGHT / 2;

static constexpr int FRAME_RATE = 60;								// 1秒辺りのフレーム数
static constexpr int FRAME_RATE_MILLI_SECOND = 1000 / FRAME_RATE;	// 何ミリ秒で1フレームか

//現在の位置情報
static constexpr int STATE_POSITION = 1400;

// 増加していく数値
static constexpr int COUNTUP_NUM = 10;

// スコアの桁数
static constexpr int SCORE_DIGIT = 5;
// スコアの画像サイズ
static constexpr int SCORE_SIZE_X = 16;
static constexpr int SCORE_SIZE_Y = 32;
// スコアのY座標
static constexpr int SCORE_HIGH = 310;
// コンボ数のX座標
static constexpr int COMBO_X = 1200;
// コンボ数のY座標
static constexpr int COMBO_Y = WINDOW_HEIGHT_HALF - 90;
// コンボ数の画像サイズ
static constexpr int COMBO_SIZE_X = 12;
static constexpr int COMBO_SIZE_Y = 24;

//マップエディター関連
constexpr int MAP_W = 50;
constexpr int MAP_H = 50;
constexpr float WALL_HIGHT = 5.0f;
constexpr float TILE_SIZE = 5.0f;

//色(代入する値が定数でないためconstを使用)
static const UnInt WHITE = GetColor(255, 255, 255);			//白
static const UnInt RED	 = GetColor(255,   0,   0);			//赤
static const UnInt BLUE  = GetColor(  0,   0, 255);			//青
static const UnInt GREEN = GetColor(  0, 255,   0);			//緑
static const UnInt PINKU = GetColor(255, 100, 1000);		//ピンク