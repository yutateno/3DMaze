#pragma once
#include "Scene.h"
#include "Character.h"
#include "Camera.h"
#include <math.h>

class Title : public Scene
{
private:
	// 画像
	int gr_icon;			// アイコンの画像
	// 最初
	int gr_start;			// スタートコマンド
	int gr_end;				// 終了コマンド
	// 2番目
	int gr_player1;			// 最初のキャラの説明（シキミ）
	int gr_player2;			// 二番目のキャラの説明（アカネ）
	int gr_chara_ok;		// キャラ選択のOK
	int gr_chara_no;		// キャラ選択のNO
	// 3番目
	int gr_camera1;			// カメラモード１
	int gr_camera2;			// カメラモード２
	int gr_cameraimage;		// カメラのイメージ図
	// 4番目
	int gr_last;			// 最後にいいかどうかの背景画像
	int gr_ok;				// 最後のOK
	int gr_no;				// 最後のNO
	// 最後
	int gr_red;				// フェードアウトの時の画像
	// おまけ
	int gr_bonus;			// おまけのスタートコマンド
	int gr_returnreturn;	// おまけから戻るコマンド
	int gr_black;

	// 音
	int se_select;			// 選択音
	int se_kettei;			// 決定音
	int se_return;			// 戻る音

	// カーソル位置
	int iconY;	// カーソルのY座標
	int iconX;	// カーソルのX座標

	int menumode;	//0:開始 1:キャラモデル 2:説明文 3:カメラ 4:最終

	bool bonus;		// おまけサイドに移るかどうか

	// 位置座標
	VECTOR camera;		// カメラ
	VECTOR view;		// カメラの注視
	VECTOR character;	// キャラクター１
	VECTOR character2;	// キャラクター２
	VECTOR light;		// 光源

	// 実体
	Character* objectchara1;	// キャラクター１
	Character* objectchara2;	// キャラクター２
	Character* bonuschara;		// ボーナスキャラクター
	Camera* bonuscamera;		// ボーナス時のカメラ

	int TitleStageHandle;		// ステージハンドル
	int TitleModelHandle_one;	// 一つ目のモデルハンドル
	int TitleModelHandle_two;	// 二つ目のモデルハンドル
	int Titlebgm;				// BGMハンドル
	int ShadowModel;	// 影キャラのモデルハンドル

	int stagepshader;		// ピクセルシェーダ
	int stagevshader;		// 頂点シェーダ

	void Process();		// プロセス
	void Draw();		// 描画

public:
	Title(int TitleStageHandle, int TitleModelHandle_one, int TitleModelHandle_two, int Titlebgm, int ShadowModel);		// コンストラクタ
	~Title();																											// デストラクタ

	void Update();		// 更新
};