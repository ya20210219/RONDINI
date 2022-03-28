//=============================================================================
//
// �e���� [BossBomb.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BOSS_BOMB			(1)							// �e�ő吔	�����l128

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ��]
	D3DXVECTOR3		scale;		// �X�P�[��
	D3DXVECTOR3		dir;		// �ړ���
	int				countW;
	int				countH;
	float			PlayerY;
	int				timer;		// �����t���[��
	int				idxShadow;	// �eID
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

	SPHERE			col_Sphere;

} ENEMYBOMB;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossBomb(void);
void UninitBossBomb(void);
void UpdateBossBomb(void);
void DrawBossBomb(void);

int SetBossBomb(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
SPHERE* BossBomb_GetSphere(int index);

void BossBomb_Destroy(int index);
bool BossBomb_IsEnable(int index);
