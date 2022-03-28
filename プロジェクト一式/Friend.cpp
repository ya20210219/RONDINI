//=============================================================================
//
// �F�R�Ǘ� [Friend.cpp]
// Author : 
//
//=============================================================================
#include "Friend.h"
#include "MyDirect3D.h"
#include "scene.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "EnemyBullet.h"
#include "radian.h"
#include "bullet.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_FRIEND		"data/MODEL/Rondini_friend_type03.x"	
#define	VALUE_ENEMY_ROTATE	(D3DX_PI * 0.008f)		// ��]��
#define MAX_FRIEND_LIFE     (30)
#define FRIEND_R			(20.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPD3DXMESH			g_pMesh = NULL;		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			g_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
static DWORD				g_nNumMat = 0;		// �}�e���A�����̐�
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// �e�N�X�`��


static FRIEND				g_Friend[MAX_FRIEND];	// �G�l�~�[

static int					g_FriendSearchCnt;
static float				g_F_E_Dist;
static int					g_FriendEnableCnt = MAX_FRIEND;		//���݂��Ă���G�̃J�E���g
static bool					g_FriendAllDestroy = false;			//�S�Ńt���O
static float				g_rotY1;
static int					g_ReTargetCnt;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFriend(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_FRIEND,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat,							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat,								// D3DXMATERIAL�\���̂̐�
		&g_pMesh)))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}


	D3DXMATERIAL *pMat;
	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMat->GetBufferPointer();

	// �}�e���A���Ƀe�N�X�`�����o�^����Ă���Γǂݍ���
	if (pMat[0].pTextureFilename != NULL)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			pMat[0].pTextureFilename,			// �t�@�C���̖��O
			&g_pTexture);						// �ǂݍ��ރ������[
	}

	float interval = 75.0f;

	g_F_E_Dist = 1000.0f;

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		for (int i = 0; i < MAX_FRIEND; i++)
		{
			if (GetSceneNumber() != SCENE_INDEX_GAME3)
			{
				// �ʒu�E��]�E�X�P�[���̏����ݒ�
				g_Friend[i].pos = D3DXVECTOR3(-(interval / 2)*(MAX_FRIEND - 1) + (interval*i), 10.0f, -500.0f);
				g_Friend[i].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
				g_Friend[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				g_Friend[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			if (GetSceneNumber() == SCENE_INDEX_GAME3)
			{
				// �ʒu�E��]�E�X�P�[���̏����ݒ�
				g_Friend[i].pos = D3DXVECTOR3(-(interval / 2)*(MAX_FRIEND - 1) + (interval*i), 10.0f, 2000.0f);
				g_Friend[i].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
				g_Friend[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				g_Friend[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}


			//g_Friend[i].Target = pPlayer->pos;

			//g_Friend[i].rot.y = (-atan2f((g_Target.z - g_Friend[i].pos.z), (g_Target.x - g_Friend[i].pos.x)) - (D3DX_PI / 2));

			g_Friend[i].NowRotY = 0.0f;
			g_Friend[i].TargetRotY = 0.0f;

			// �x�[�X�ʒu�ɕۑ�����
			g_Friend[i].base_pos = g_Friend[i].pos;

			g_Friend[i].bUse = true;
			g_Friend[i].LockEnable = false;

			g_Friend[i].FriendBulletTimer = 0;
			g_Friend[i].life = MAX_FRIEND_LIFE;
			g_Friend[i].FriendSpd = 1.0f;

			g_Friend[i].col_Sphere.pos.x = g_Friend[i].pos.x;
			g_Friend[i].col_Sphere.pos.y = g_Friend[i].pos.y;
			g_Friend[i].col_Sphere.pos.z = g_Friend[i].pos.z;

			g_Friend[i].col_Sphere.r = FRIEND_R;

			g_Friend[i].idxFriendShadow = CreateShadow(g_Friend[i].pos, 30.0f, 30.0f);
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		for (int i = 0; i < MAX_FRIEND; i++)
		{
			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Friend[i].pos = D3DXVECTOR3(-(interval / 2)*(MAX_FRIEND - 1) + (interval*i), 10.0f, -500.0f);
			g_Friend[i].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			g_Friend[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Friend[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//g_Friend[i].Target = pPlayer->pos;

			//g_Friend[i].rot.y = (-atan2f((g_Target.z - g_Friend[i].pos.z), (g_Target.x - g_Friend[i].pos.x)) - (D3DX_PI / 2));

			g_Friend[i].NowRotY = 0.0f;
			g_Friend[i].TargetRotY = 0.0f;

			// �x�[�X�ʒu�ɕۑ�����
			g_Friend[i].base_pos = g_Friend[i].pos;

			g_Friend[i].bUse = true;
			g_Friend[i].LockEnable = false;

			g_Friend[i].FriendBulletTimer = 0;
			g_Friend[i].life = MAX_FRIEND_LIFE;
			g_Friend[i].FriendSpd = 1.0f;

			g_Friend[i].col_Sphere.pos.x = g_Friend[i].pos.x;
			g_Friend[i].col_Sphere.pos.y = g_Friend[i].pos.y;
			g_Friend[i].col_Sphere.pos.z = g_Friend[i].pos.z;

			g_Friend[i].col_Sphere.r = FRIEND_R;

			g_Friend[i].idxFriendShadow = CreateShadow(g_Friend[i].pos, 30.0f, 30.0f);
		}
	}
	
	g_FriendAllDestroy = false;

	g_rotY1 = 0.0f;
	g_ReTargetCnt = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFriend(void)
{
	if (g_pTexture != NULL)
	{// �e�N�X�`���̊J��
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	if (g_pMesh != NULL)
	{// ���b�V���̊J��
		g_pMesh->Release();
		g_pMesh = NULL;
	}

	if (g_pBuffMat != NULL)
	{// �}�e���A���̊J��
		g_pBuffMat->Release();
		g_pBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFriend(void)
{
	PLAYER *pPlayer = GetPlayer();
	CAMERA *cam = GetCamera();
	ENEMY* pEnemy = GetAllEnemy();

	g_F_E_Dist = 1000.0f;

	g_FriendEnableCnt = MAX_FRIEND;

	g_ReTargetCnt++;
	if (g_ReTargetCnt >= 721)
	{
		g_ReTargetCnt = 0;
	}

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		for (int i = 0; i < MAX_FRIEND; i++)
		{
			if (g_Friend[i].life < 1 && g_Friend[i].bUse == true)
			{
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				g_Friend[i].bUse = false;
			}
			if (!g_Friend[i].bUse)
			{
				g_FriendEnableCnt--;
				continue;
			}

			//�W�I(�G��)��������(�W�I�����ʂ����^�[�Q�b�g�̎��Ԃ�������)
			if ((pEnemy + g_Friend[i].TargetNum)->bUse == false || g_ReTargetCnt == 720)
			{
				for (int j = 0; j < MAX_ENEMY; j++)
				{
					if ((pEnemy + j)->bUse == false)
					{
						continue;
					}
					float dist = GetFloatDist(Enemy_GetSphere(j), Friend_GetSphere(i));
					//�������ŒZ�̓G��������
					if (g_F_E_Dist > dist)
					{
						g_F_E_Dist = dist;
						g_Friend[i].TargetNum = j;
					}
				}
			}

			g_Friend[i].g_Dist = GetFloatDist(Friend_GetSphere(i), Player_GetSphere());

			if (Enemy_AllDestroy() == false)
			{
				g_Friend[i].Target = (pEnemy + g_Friend[i].TargetNum)->pos;
			}

			if (Enemy_AllDestroy() == true)
			{
				if (i <= 1)
				{
					g_Friend[i].Target.x = pPlayer->pos.x - sinf(pPlayer->rot.y - (D3DX_PI * 2 / 3)) * (30.0f + (30.0f * i));
					g_Friend[i].Target.z = pPlayer->pos.z - cosf(pPlayer->rot.y - (D3DX_PI * 2 / 3)) * (30.0f + (30.0f * i));
				}
				if (i >= 2)
				{
					g_Friend[i].Target.x = pPlayer->pos.x - sinf(pPlayer->rot.y + (D3DX_PI * 2 / 3)) * (30.0f + (30.0f * (i - 2)));
					g_Friend[i].Target.z = pPlayer->pos.z - cosf(pPlayer->rot.y + (D3DX_PI * 2 / 3)) * (30.0f + (30.0f * (i - 2)));
				}

				g_Friend[i].FriendSpd = pPlayer->PlayerSpd;
			}


			if (g_Friend[i].rot.y < 0)
			{
				g_Friend[i].rot.y += D3DX_PI * 2.0f;
			}

			if (g_Friend[i].rot.y > D3DX_PI * 2)
			{
				g_Friend[i].rot.y -= D3DX_PI * 2.0f;
			}

			g_Friend[i].NowRotY = g_Friend[i].rot.y;

			g_Friend[i].TargetRotY = (-atan2f((g_Friend[i].Target.z - g_Friend[i].pos.z), (g_Friend[i].Target.x - g_Friend[i].pos.x)) - (D3DX_PI / 2));

			if (g_Friend[i].TargetRotY < 0)
			{
				g_Friend[i].TargetRotY += D3DX_PI * 2.0f;
			}

			if (g_Friend[i].TargetRotY > D3DX_PI * 2.0f)
			{
				g_Friend[i].TargetRotY -= D3DX_PI * 2.0f;
			}

			g_rotY1 = (float)LerpRadian(g_Friend[i].NowRotY, g_Friend[i].TargetRotY, 0.02f);

			g_Friend[i].rot.y = g_rotY1;

			g_Friend[i].rotP = (-atan2f((pPlayer->pos.z - g_Friend[i].pos.z), (pPlayer->pos.x - g_Friend[i].pos.x)) - (D3DX_PI / 2));

			g_Friend[i].spd.x = sinf(D3DX_PI * 0.0f - g_Friend[i].rot.y) * g_Friend[i].FriendSpd;
			g_Friend[i].spd.z = cosf(D3DX_PI * 0.0f - g_Friend[i].rot.y) * g_Friend[i].FriendSpd;

			//g_Friend[i].spd = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);

			g_Friend[i].pos.x += g_Friend[i].spd.x;
			g_Friend[i].pos.z -= g_Friend[i].spd.z;

			g_Friend[i].FriendBulletTimer++;

			if (g_Friend[i].FriendBulletTimer == 180)
			{
				g_Friend[i].FriendBulletBirthPos.x = g_Friend[i].pos.x - sinf(g_Friend[i].rot.y) * 18.0f;
				g_Friend[i].FriendBulletBirthPos.z = g_Friend[i].pos.z - cosf(g_Friend[i].rot.y) * 18.0f;

				SetBullet(g_Friend[i].FriendBulletBirthPos, D3DXVECTOR3(-sinf(g_Friend[i].rot.y) * 4.0f, 0.0f, -cosf(g_Friend[i].rot.y) * 4.0f));
				g_Friend[i].FriendBulletTimer = 0;
			}

			g_Friend[i].col_Sphere.pos.x = g_Friend[i].pos.x;
			g_Friend[i].col_Sphere.pos.y = g_Friend[i].pos.y;
			g_Friend[i].col_Sphere.pos.z = g_Friend[i].pos.z;
			g_Friend[i].col_Sphere.r = FRIEND_R;

			SetPositionShadow(g_Friend[i].idxFriendShadow, D3DXVECTOR3(g_Friend[i].pos.x, 0.0f, g_Friend[i].pos.z));
		}
	}	

	if (g_FriendEnableCnt == 0)
	{
		g_FriendAllDestroy = true;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFriend(void)
{
	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		D3DXMATERIAL *pMat;
		D3DMATERIAL9 matDef;

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int i = 0; i < MAX_FRIEND; i++)
		{
			if (!g_Friend[i].bUse)
			{
				continue;
			}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Friend[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_Friend[i].scl.x, g_Friend[i].scl.y, g_Friend[i].scl.z);
			D3DXMatrixMultiply(&g_Friend[i].mtxWorld, &g_Friend[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Friend[i].rot.y, g_Friend[i].rot.x, g_Friend[i].rot.z);
			D3DXMatrixMultiply(&g_Friend[i].mtxWorld, &g_Friend[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Friend[i].pos.x, g_Friend[i].pos.y, g_Friend[i].pos.z);
			D3DXMatrixMultiply(&g_Friend[i].mtxWorld, &g_Friend[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Friend[i].mtxWorld);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�(�e�N�X�`���̗L�郂�f����ǂݍ��ނƂ��͒���)
				pDevice->SetTexture(0, g_pTexture);

				// �`��
				g_pMesh->DrawSubset(nCntMat);
			}
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
FRIEND* GetFriend(int index)
{
	return &(g_Friend[index]);
}

FRIEND* GetAllFriend()
{
	return g_Friend;
}

SPHERE* Friend_GetSphere(int index)
{
	return (&g_Friend[index].col_Sphere);
}

void Friend_Destroy(int index)
{
	PlaySound(SOUND_LABEL_SE_EXPLOSION);
	g_Friend[index].bUse = false;
	g_Friend[index].LockEnable = false;
	ReleaseShadow(g_Friend[index].idxFriendShadow);
}

bool Friend_IsEnable(int index)
{
	return g_Friend[index].bUse;
}

bool FriendLockOn_IsEnable(int index)
{
	return g_Friend[index].LockEnable;
}

void SetFriendLockOn_IsEnable(int index, bool enable)
{
	g_Friend[index].LockEnable = enable;
}

void Friend_Damage(int index)
{
	g_Friend[index].life--;
}

void Friend_SelectDamage(int index, int damage)
{
	g_Friend[index].life -= damage;
}

bool Friend_AllDestroy()
{
	return g_FriendAllDestroy;
}