#include "Stage.h"

Stage::Stage(int StageHandle)
{
	shapecount = 0.0f;
	shapeup = false;
	shapedown = false;

	// ���_�V�F�[�_�[��ǂݍ���
	stagevshader = LoadVertexShader("stageshader\\Stage_PhongVS.vso");
	// �s�N�Z���V�F�[�_�[��ǂݍ���
	stagepshader = LoadPixelShader("stageshader\\Stage_PhongPS.pso");

	// �X�e�[�W���f���̓ǂݍ���
	drawstagehandle = StageHandle;

	// �X�e�[�W�̃��f���̍��W���X�V����
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
	// �X�e�[�W
	if (stagenum == 5)
	{
		MV1DrawModel(drawstagehandle);
	}
	else if (stagenum == 4)
	{
		MV1SetUseOrigShader(TRUE);
		// �g�p���钸�_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(stagevshader);
		// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(stagepshader);
		MV1SetMaterialDrawBlendMode(drawstagehandle, 0, DX_BLENDMODE_ADD);
		MV1DrawModel(drawstagehandle);
		MV1SetUseOrigShader(FALSE);
		// �ݒ肵���萔������
		ResetVSConstF(43, 8);
	}
	else
	{
		MV1SetUseOrigShader(TRUE);
		// �g�p���钸�_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(stagevshader);
		// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(stagepshader);
		MV1DrawModel(drawstagehandle);
		MV1SetUseOrigShader(FALSE);
		// �ݒ肵���萔������
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

		MV1SetShapeRate(drawstagehandle, MV1SearchShape(drawstagehandle, "���J��"), shapecount);
	}
	else if (stagenum == 2)		// 2
	{
		shapecount = 1.0f;

		MV1SetShapeRate(drawstagehandle, MV1SearchShape(drawstagehandle, "�����Ղ�"), shapecount);
	}
}