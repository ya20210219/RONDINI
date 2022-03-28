//=============================================================================
//
// Mk2�A�r���e�B���� [AbilityThunder.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define	MAX_THUNDERANIME			(10)

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		scale;		// �X�P�[��
	int				timer;		// �����t���[��
	int				countW;		//�J�E���g
	int				countH;		//�J�E���g
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

	bool			PlusThunder;
	int				PlusThunderUseCnt;
	int				PlusThunderCnt;
	bool            PlusThunderCoolDownFlg;

	SPHERE          col_Sphere;

} AbilityThunder;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitAbilityThunder(void);
void UninitAbilityThunder(void);
void UpdateAbilityThunder(void);
void DrawAbilityThunder(void);

AbilityThunder* GetAbilityThunder();
SPHERE* GetAbilityThunderSphere();
void AbilityThunder_Destroy();
bool AbilityThunder_IsEnable();
