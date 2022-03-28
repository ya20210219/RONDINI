//=============================================================================
//
// �o�b�N�t�@�C�A�A�j���[�V�������� [BackFire.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"

#define	MAX_BACKFIRE			(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBackFire(void);
void UninitBackFire(void);
void UpdateBackFire(void);
void DrawBackFire(void);

int SetBackFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
