//=============================================================================
//
// �e���� [shadow.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseShadow(int nIdxShadow);

void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetScaleShadow(int nIdxShadow, D3DXVECTOR3 scl);
