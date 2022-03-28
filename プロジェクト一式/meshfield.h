//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int numBlockX, int numBlockZ, float sizeBlockX, float sizeBlockZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

