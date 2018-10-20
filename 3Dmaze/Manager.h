#pragma once
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"

class Manager
{
private:
	// 実体
	eScene preScene;	// 直前のシーン
	eScene nowScene;	// 今のシーン

	Scene* scene;	// シーンの本体
	Load* load;

	void Manage();	// シーン変更処理

	int stagenum;	// ステージ番号

	int playmodel;		// プレイヤーモデル番号
	bool cameratype;	// カメラモードの番号

public:
	Manager();	// コンストラクタ
	~Manager();	// デストラクタ

	void Update();	// 更新
};