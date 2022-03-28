//=============================================================================
//
// 弾処理 [bullet.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BULLET			(200)							// 弾最大数	初期値128

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* Bullet_GetSphere(int index);

void Bullet_Destroy(int index);
bool Bullet_IsEnable(int index);
