#pragma once
#include "DxLib.h"

class Load
{
private:
	int gr;			// 背景画像
	int gr_bax;		// 背景画像
	int baxcount;	// 赤い量
	int max;		// 非同期処理の個数
	int scene;
	int playmodel;
	int stagenum;

	bool loadend;

	int DStageHandle;	// 描画用ステージハンドル
	int CStageHandle;	// 判定用ステージハンドル
	int ModelHandle;	// プレイヤーモデルハンドル
	int bgm;			// BGMハンドル

	int TitleStageHandle;		// ステージハンドル
	int TitleModelHandle_one;	// 一つ目のモデルハンドル
	int TitleModelHandle_two;	// 二つ目のモデルハンドル
	int Titlebgm;				// BGMハンドル

	int ShadowModel;	// 影キャラのモデルハンドル

public:
	Load(int scene, int stagenum, int playmodel);	// ステージ番号とプレイモデルを引数に取ったコンストラクタ
	~Load();							// デストラクタ

	void Update();	// 更新

	// ゲッター
	int GetDStageH();		// 描画用ステージハンドル
	int GetCStageH();		// 判定用ステージハンドル
	int GetModelH();		// 操作モデルハンドル
	int GetBGM();			// BGM
	// タイトル
	int GetTitleStageH();		// ステージハンドル
	int GetTitleModelH_one();	// 一つ目のモデルハンドル
	int GetTitleModelH_two();	// 二つ目のモデルハンドル
	int GetTitlebgm();			// BGMハンドル
	// おまけ
	int GetShadowM();	//影キャラのモデルハンドル

	bool GetLoadEnd();
};