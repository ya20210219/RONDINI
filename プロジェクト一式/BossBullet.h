//=============================================================================
//
// �e���� [BossBullet.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BOSS_BULLET			(600)							// �e�ő吔	�����l128

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossBullet(void);
void UninitBossBullet(void);
void UpdateBossBullet(void);
void DrawBossBullet(void);

int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* BossBullet_GetSphere(int index);

void BossBullet_Destroy(int index);
bool BossBullet_IsEnable(int index);
