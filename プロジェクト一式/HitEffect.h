//=============================================================================
//
// �o�b�N�t�@�C�A�A�j���[�V�������� [BackFire.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "bullet.h"

#define	MAX_HITEFFECT			(MAX_BULLET)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHitEffect(void);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);

void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

