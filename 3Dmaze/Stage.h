#pragma once
#include "DxLib.h"

class Stage
{
private:
	int drawstagehandle;	// ステージハンドル

	// 扉などの動き
	float shapecount;	// シェイプカウント
	bool shapeup;		// シェイプアップの開始
	bool shapedown;		// シェイプアップの終了開始

	int stagepshader;		// ピクセルシェーダを読み込む変数
	int stagevshader;		// 頂点シェーダを読み込む変数

public:
	Stage(int StageHandle);		// ステージハンドルを引数に持つコンストラクタ
	~Stage();					// デストラクタ

	void Draw(int stagenum);	// 描画

	void ShapeStage(bool shapeup, bool shapedown, int stagenum);	// シェイプの動きの関数
};