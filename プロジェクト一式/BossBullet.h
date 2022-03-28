//=============================================================================
//
// 弾処理 [BossBullet.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BOSS_BULLET			(600)							// 弾最大数	初期値128

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossBullet(void);
void UninitBossBullet(void);
void UpdateBossBullet(void);
void DrawBossBullet(void);

int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* BossBullet_GetSphere(int index);

void BossBullet_Destroy(int index);
bool BossBullet_IsEnable(int index);
