//=============================================================================
//
// �G�l�~�[�Ǘ� [Item.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_ITEM		(MAX_ENEMY)				// �G�l�~�[�̐�

struct ITEM
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					type;

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					idxItemShadow;

	bool				bUse;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

ITEM* GetItem(int index);
ITEM* GetAllItem();
SPHERE* Item_GetSphere(int index);
void Item_Destroy(int index);
bool Item_IsEnable(int index);
void SetItem(D3DXVECTOR3 pos);
bool Item_AllDestroy();
