//=============================================================================
//
// �{�X�Ǘ� [Boss.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "Boss.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_BOSS		(1)				// �G�l�~�[�̐�

struct BOSS
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3         spd;
	int					Life;
	int					MaxLife;
	int					HpColor;
	float				g_Dist;
	int					type;

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3			base_pos;			// �x�[�X�ƂȂ�ʒu

	int					idxBossShadow;

	bool				bUse;
	D3DXVECTOR3         BossBulletBirthPos;		//�e�̐����ʒu
	int					BossBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

BOSS* GetBoss();
BOSS* GetAllBoss();
SPHERE* Boss_GetSphere(int index);
void Boss_Destroy(int index);
void Boss_Damage(int index, int damage);
bool Boss_IsEnable(int index);
bool Boss_AllDestroy();
