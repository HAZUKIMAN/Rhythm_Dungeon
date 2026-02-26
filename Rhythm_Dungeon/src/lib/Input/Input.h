#pragma once

#define KEY_BUF_LEN			256
#define CONTROLLER_BUF_LEN	16

class Input
{
private:
	//現在フレームのキー情報
	static char currentKeyBuf[KEY_BUF_LEN];
	//現在のフレームのコントローラー情報
	static XINPUT_STATE currentControllerBuf;

	//前のフレームのキー情報
	static char preKeyBuf[KEY_BUF_LEN];
	//前のフレームのコントローラー情報
	static XINPUT_STATE preControllerBuf;
	

public:
	//入力制御初期化
	static void Init();

	//入力制御ステップ
	//押され続けているか見るための
	static void Step();

	/*==========================================================
							キーボード
	==========================================================*/
	struct Key
	{
		//今押された：KEY_INPUT_～
		static bool Push(int key_code);

		//押し続けられている：KEY_INPUT_～
		static bool Keep(int key_code);

	};
	/*==========================================================
							コントローラー
	==========================================================*/
	struct Controller
	{
		//今押された：XINPUT_BUTTON_～
		static bool Push(int button_code);
		//押し続けられている：XINPUT_BUTTON_～
		static bool Keep(int button_code);

		//左スティック：倒し度の格納先
		static void LStickIncline(float &stickX,float &stickY);
		//右スティック：倒し度の格納先
		static void RStickIncline(float& stickX, float& stickY);

		//LT：倒し度の格納先
		static void LeftTriggerIncline(float& trigger);
		//LT：倒し度の格納先
		static void RightTriggerIncline(float& trigger);
	};
};