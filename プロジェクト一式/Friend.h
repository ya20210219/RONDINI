//=============================================================================
//
// �F�R�Ǘ� [Friend.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_FRIEND		(4)				// �G�l�~�[�̐�

struct FRIEND
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	float				rotP;
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3         spd;
	float				g_Dist;
	int					life;
	float				FriendSpd;

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3			base_pos;			// �x�[�X�ƂȂ�ʒu

	D3DXVECTOR3			Target;
	int					TargetNum;

	int					idxFriendShadow;

	bool				bUse;
	bool				LockEnable;
	D3DXVECTOR3         FriendBulletBirthPos;		//�e�̐����ʒu
	int					FriendBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFriend(void);
void UninitFriend(void);
void UpdateFriend(void);
void DrawFriend(void);

FRIEND* GetFriend(int index);
FRIEND* GetAllFriend();
SPHERE* Friend_GetSphere(int index);
void Friend_Destroy(int index);
bool Friend_IsEnable(int index);
bool FriendLockOn_IsEnable(int index);

void SetFriendLockOn_IsEnable(int index, bool enable);
void Friend_Damage(int index);
void Friend_SelectDamage(int index, int damage);
bool Friend_AllDestroy();

