//=============================================================================
//
// ���b�N�I���e���� [bullet.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"


typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ��]
	D3DXVECTOR3		scale;		// �X�P�[��
	D3DXVECTOR3		dir;		// �ړ���
	int				timer;		// �����t���[��
	int				idxShadow;	// �eID
	float			TargetRotY;
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O
	bool			LockonUse;

	SPHERE			col_Sphere;

} LOCKONBULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLockonBullet(void);
void UninitLockonBullet(void);
void UpdateLockonBullet(void);
void DrawLockonBullet(void);

LOCKONBULLET* GetLockonBullet();
int SetLockonBullet(D3DXVECTOR3 pos, float y);
SPHERE* LockonBullet_GetSphere(int index);

void LockonBullet_Destroy(int index);
bool LockonBullet_IsEnable(int index);
