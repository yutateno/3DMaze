#include "Stage.h"

Stage::Stage(int StageHandle)
{
	shapecount = 0.0f;
	shapeup = false;
	shapedown = false;

	// 頂点シェーダーを読み込む
	stagevshader = LoadVertexShader("stageshader\\Stage_PhongVS.vso");
	// ピクセルシェーダーを読み込む
	stagepshader = LoadPixelShader("stageshader\\Stage_PhongPS.pso");

	// ステージモデルの読み込み
	drawstagehandle = StageHandle;

	// ステージのモデルの座標を更新する
	MV1SetPosition(drawstagehandle, VGet(0, 0, 0));
}

Stage::~Stage()
{
	MV1DeleteModel(drawstagehandle);

	DeleteShader(stagevshader);
	DeleteShader(stagepshader);
	InitGraph();
	InitShader();
}

void Stage::Draw(int stagenum)
{
	// ステージ
	if (stagenum == 5)
	{
		MV1DrawModel(drawstagehandle);
	}
	else if (stagenum == 4)
	{
		MV1SetUseOrigShader(TRUE);
		// 使用する頂点シェーダーをセット
		SetUseVertexShader(stagevshader);
		// 使用するピクセルシェーダーをセット
		SetUsePixelShader(stagepshader);
		MV1SetMaterialDrawBlendMode(drawstagehandle, 0, DX_BLENDMODE_ADD);
		MV1DrawModel(drawstagehandle);
		MV1SetUseOrigShader(FALSE);
		// 設定した定数を解除
		ResetVSConstF(43, 8);
	}
	else
	{
		MV1SetUseOrigShader(TRUE);
		// 使用する頂点シェーダーをセット
		SetUseVertexShader(stagevshader);
		// 使用するピクセルシェーダーをセット
		SetUsePixelShader(stagepshader);
		MV1DrawModel(drawstagehandle);
		MV1SetUseOrigShader(FALSE);
		// 設定した定数を解除
		ResetVSConstF(43, 8);
	}
}

void Stage::ShapeStage(bool shapeup, bool shapedown, int stagenum)
{
	if (stagenum == 3 || stagenum == 4)			// 2.5
	{
		if (shapeup == true)
		{
			if (shapecount <= 1)
			{
				shapecount += 0.01f;
			}
		}
		if (shapedown == true)
		{
			if (shapecount >= 0)
			{
				shapecount -= 0.01f;
			}
		}

		MV1SetShapeRate(drawstagehandle, MV1SearchShape(drawstagehandle, "扉開き"), shapecount);
	}
	else if (stagenum == 2)		// 2
	{
		shapecount = 1.0f;

		MV1SetShapeRate(drawstagehandle, MV1SearchShape(drawstagehandle, "窓血祭り"), shapecount);
	}
}