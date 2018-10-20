#pragma once
#include "DxLib.h"
#include "input.h"

// シーン
enum eScene
{
	STARTLOAD, TITLE, LOAD, GAME
};

class Scene
{
protected:
	static bool endflag;	// 終了フラッグ

	static eScene scene;	// 現在のシーン

	int playmodel;		// プレイヤーモデルの番号
	bool cameratype;	// カメラモードの番号

	// フェードアウトフェードイン用
	bool start;		// 開始フラッグ
	bool end;		// 終了フラッグ
	int count;		// フェードインフェードアウト用カウント変数

public:
	Scene();					// コンストラクタ
	virtual ~Scene() {};		// デストラクタ

	virtual void Update() = 0;	// 更新

	static bool GetEndFlag();	// 終了ゲッター
	static eScene GetScene();	// 今のシーンゲッター

	// セッター
	void SetScene(eScene scene);
	// ゲッター
	int GetPlayModel();		// プレイモデル番号
	bool GetCameraType();	// カメラタイプ
};