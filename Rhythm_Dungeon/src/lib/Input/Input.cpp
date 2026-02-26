#include"DxLib.h"
#include"input.h"
#include"math.h"

//現在フレームのキー情報
char Input::currentKeyBuf[KEY_BUF_LEN];
//現在のフレームのコントローラー情報
XINPUT_STATE Input::currentControllerBuf;
//前のフレームのキー情報
char Input::preKeyBuf[KEY_BUF_LEN];
//前のフレームのコントローラー情報
XINPUT_STATE Input::preControllerBuf;
//スティックの最大の値
float MAX_STICK(32768.0f);


//入力制御初期化
void Input::Init()
{
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		currentKeyBuf[index] = '\0';
		preKeyBuf[index] = '\0';
	}
	for (int index = 0; index < CONTROLLER_BUF_LEN; index++)
	{
		currentControllerBuf.Buttons[index] = '\0';
		preControllerBuf.Buttons[index] = '\0';
	}

}

//入力制御ステップ
//押され続けているか見るために
void Input::Step()
{

	//戻ってくる値の最大値が1にするために
	
	//前フレームのキー情報変数に記録しておく
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		preKeyBuf[index] = currentKeyBuf[index];
	}
	for (int index = 0; index < CONTROLLER_BUF_LEN; index++)
	{
		preControllerBuf.Buttons[index] = currentControllerBuf.Buttons[index];
	}

	//現在のキー情報を取得
	GetHitKeyStateAll(currentKeyBuf);

	//現在のコントローラー情報を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &currentControllerBuf);
}

/*==========================================================
						キーボード
==========================================================*/

//今押された：KEY_INPUT_～
bool Input::Key::Push(int key_code)
{
	//前フレで押されてない　かつ　現フレで押されている
	if (preKeyBuf[key_code] == 0 && currentKeyBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//押し続けられている：KEY_INPUT_～
bool Input::Key::Keep(int key_code)
{
	//前フレで押されている　かつ　現フレで押されている
	if (preKeyBuf[key_code] == 1 && currentKeyBuf[key_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

/*==========================================================
							コントローラー
==========================================================*/
//今押された：XINPUT_BUTTON_～
bool Input::Controller::Push(int button_code)
{
	//前フレで押されてない　かつ　現フレで押されている
	if (preControllerBuf.Buttons[button_code] == 0 && currentControllerBuf.Buttons[button_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//押し続けられている：XINPUT_BUTTON_～
bool Input::Controller::Keep(int button_code)
{
	//前フレで押されている　かつ　現フレで押されている
	if (preControllerBuf.Buttons[button_code] == 1 && currentControllerBuf.Buttons[button_code] == 1)
	{
		return true;
	}

	//押されてないので false
	return false;
}

//左スティック：倒し度の格納先
void Input::Controller::LStickIncline(float& stickX, float& stickY)
{
	stickX = 0.0f;
	stickY = 0.0f;

	//横方向
	//128＝倒されていない
	if (currentControllerBuf.ThumbLX == 128)
	{
		stickX = 0.0f;
	}
	//倒されている
	else
	{
		//-（←）
		if (currentControllerBuf.ThumbLX <= 128)
		{
			stickX = (float)currentControllerBuf.ThumbLX / MAX_STICK;
		}
		//+（→）
		else
		{
			stickX = (float)currentControllerBuf.ThumbLX / MAX_STICK;
		}
	}
	
	//縦方向
	//128＝倒されていない
	if (currentControllerBuf.ThumbLY == 128)
	{
		stickY = 0.0f;
	}
	//倒されている
	else
	{
		//-（↓）
		if (currentControllerBuf.ThumbLY <= 128)
		{
			stickY = (float)currentControllerBuf.ThumbLY / MAX_STICK;
		}
		//+（↑）
		else
		{
			stickY = (float)currentControllerBuf.ThumbLY / MAX_STICK;
		}
	}

	//ほぼ誤差なので消す
	if (fabs(stickX) <= 0.06f)
	{
		stickX = 0.0f;
	}
	if (fabs(stickY) <= 0.06f)
	{
		stickY = 0.0f;
	}
}

//右スティック：倒し度の格納先
void Input::Controller::RStickIncline(float& stickX, float& stickY)
{
	stickX = 0.0f;
	stickY = 0.0f;

	//横方向
	//128＝倒されていない
	if (currentControllerBuf.ThumbRX == 128)
	{
		stickX = 0.0f;
	}
	//倒されている
	else
	{
		//-（←）
		if (currentControllerBuf.ThumbRX <= 128)
		{
			stickX = (float)currentControllerBuf.ThumbRX / 32768.0f;
		}
		//+（→）
		else
		{
			stickX = (float)currentControllerBuf.ThumbRX / 32767.0f;
		}
	}

	//縦方向
	//128＝倒されていない
	if (currentControllerBuf.ThumbRY == 128)
	{
		stickY = 0.0f;
	}
	//倒されている
	else
	{
		//-（↓）
		if (currentControllerBuf.ThumbRY <= 128)
		{
			stickY = (float)currentControllerBuf.ThumbRY / 32768.0f;
		}
		//+（↑）
		else
		{
			stickY = (float)currentControllerBuf.ThumbRY / 32767.0f;
		}
	}

	//ほぼ誤差なので消す
	if (fabs(stickX) <= 0.0f)
	{
		stickX = 0.0f;
	}
	if (fabs(stickY) <= 0.0f)
	{
		stickY = 0.0f;
	}
}

//LT：倒し度の格納先
void Input::Controller::LeftTriggerIncline(float& trigger)
{
	trigger = 0.0f;
	trigger= (float)currentControllerBuf.LeftTrigger / 255.0f;
}

//LT：倒し度の格納先
void Input::Controller::RightTriggerIncline(float& trigger)
{
	trigger = 0.0f;
	trigger = (float)currentControllerBuf.RightTrigger / 255.0f;
}