//=============================================================================
//
// Mk3�A�r���e�B���� [Bomb.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BOMB			(2)							// �e�ő吔	�����l128

typedef struct
{
	bool			BombUse;
	int				BombCoolDownCnt;
	bool			BombCoolDownFlg;
	int				countW;
	int				countH;
	int				UseTimer;

}BOMBMGR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBomb(void);
void UninitBomb(void);
void UpdateBomb(void);
void DrawBomb(void);

int SetBomb(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* Bomb_GetSphere(int index);

BOMBMGR* GetBombMgr();

void Bomb_Destroy(int index);
void Bomb_LifeDown(int index);
bool Bomb_IsEnable(int index);
