//=============================================================================
//
// バックファイアアニメーション処理 [BackFire.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"

#define	MAX_BACKFIRE			(10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBackFire(void);
void UninitBackFire(void);
void UpdateBackFire(void);
void DrawBackFire(void);

int SetBackFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
