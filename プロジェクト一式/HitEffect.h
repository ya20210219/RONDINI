//=============================================================================
//
// バックファイアアニメーション処理 [BackFire.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "bullet.h"

#define	MAX_HITEFFECT			(MAX_BULLET)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHitEffect(void);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);

void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

