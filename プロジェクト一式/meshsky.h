//=============================================================================
//
// ���b�V����̏��� [meshsky.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						float fRadius, int nNumBlockX, int nNumBlockZ, bool bReverse = false);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);
