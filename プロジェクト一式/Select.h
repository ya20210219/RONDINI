//=============================================================================
//
// �v���C���[�Z���N�g�V�[���Ǘ� [Select.cpp]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct SELECT
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					life;
	int					spd;
	int					atk;

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					idxShadow;			// �e�̎��ʔԍ�
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
int SelectPlayer(void);
void SetSelectPlayer(int index);
SELECT* GetSelect();

