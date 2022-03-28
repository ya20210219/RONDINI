//=============================================================================
//
// �v���C���[�Ǘ� [player.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define MAX_PLAYER		(3)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			speed;				// �ړ��X�s�[�h
	int					life;				//���C�t

	D3DXVECTOR3         BulletBirthPos;		//�e�̐����ʒu

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X


	float				PlayerSpd;
	int					idxShadow;			// �e�̎��ʔԍ�
	bool				bUse;
	int					HpColor;
	int					Pattern;

	SPHERE				col_Sphere;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);
SPHERE* Player_GetSphere(void);
void Player_Destroy(void);
void Player_Damage(int damage);
void Player_AddLife(int life);
bool Player_IsEnable(void);
