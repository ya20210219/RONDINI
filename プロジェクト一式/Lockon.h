//=============================================================================
//
// ���b�N�I���@�\���� [Target.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define	MAX_LOCKON			(3)							// ���b�N�I���ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		posL;		// �ʒu
	D3DXVECTOR3		rotL;		// ��]
	D3DXVECTOR3		scaleL;		// �X�P�[��
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O
	int				TargetNum;	//�Ώۂ̃i���o�[
} LOCKON;

typedef struct
{
	D3DXVECTOR3		posR;		// �ʒu
	D3DXVECTOR3		rotR;		// ��]
	D3DXVECTOR3		scaleR;		// �X�P�[��
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

	int				LockonCoolDownCnt;
	bool			LockonCoolDownFlg;
	int				UnLockonCnt;
	bool			LockonFreeFlg;

	SPHERE				col_Sphere;

} LOCKONRANGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLockOn(void);
void UninitLockOn(void);
void UpdateLockOn(void);
void DrawLockOn(void);

LOCKON* GetLockOn();
LOCKONRANGE* GetLockonRange();

void SetLockOn(int index, D3DXVECTOR3 pos, bool enable);
SPHERE* LockOnRange_GetSphere();

bool SetLockOnEnable(bool enable);
bool LockOnRange_IsEnable();
void LockOn_Destroy(int index);
bool LockOn_IsEnable(int index);
