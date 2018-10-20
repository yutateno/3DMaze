#pragma once
#include "DxLib.h"

// キーボードの入力状態
class KeyData
{
private:
	static int key[256];	// キーの入力状態格納用変数
	static char tmpKey[256];	// 現在のキーの入力状態を格納する

public:
	KeyData();
	~KeyData();

	static void UpDate();	// キー入力の状態更新
	static int Get(int KeyCode);	// キー入力状態取得
	static bool CheckEnd();	// 強制終了
};


// ゲームパッドの入力状態
class PadData {
	static int button[4][16];  // ゲームパッドの入力状態格納用変数
	static XINPUT_STATE input[4];	// ゲームパッドのナンバー
	static __int8 padNum;	// 繋がってるゲームパッドの数

public:
	PadData();
	~PadData();

	static void SetPadNum();
	static void UpDate();	// ゲームパッドの入力の状態更新
	static int Get(int code, int padNum);	// ゲームパッドの入力状態取得
	static bool CheckEnd();	// 強制終了
};