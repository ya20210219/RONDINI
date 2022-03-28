//=============================================================================
//
// �v���C���[�Ǘ� [player.cpp]
// Author :
//
//=============================================================================
#include "input.h"
#include "player.h"
#include "MyDirect3D.h"
#include "debugproc.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "sound.h"
#include "sprite.h"
#include "Select.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "AbilityThunder.h"
#include "AbilityBomb.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER1		"data/MODEL/Rondini_MK1(ver2).x"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER2		"data/MODEL/Rondini_MK2(ver2).x"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER3		"data/MODEL/Rondini_MK3(ver5).x"	// �ǂݍ��ރ��f����

#define	VALUE_MOVE		(2.0f)					// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)		// ��]��
#define JUMP_FORCE		(5.0f)
#define GRAVITY			(0.5f)
#define MAX_LIFE		(50)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPD3DXMESH			g_pMesh[MAX_PLAYER] = { NULL };		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			g_pBuffMat[MAX_PLAYER] = { NULL };	// �}�e���A�����ւ̃|�C���^
static DWORD				g_nNumMat[MAX_PLAYER] = { 0 };		// �}�e���A�����̐�
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// �e�N�X�`��

static PLAYER				g_Player;			// �v���C���[
static int					g_BulletCnt;
static int					g_FullLife;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(MODEL_PLAYER1,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
								D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
								pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
								NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								&g_pBuffMat[0],				// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
								&g_nNumMat[0],				// D3DXMATERIAL�\���̂̐�
								&g_pMesh[0])))				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER2,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[1],			// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat[1],				// D3DXMATERIAL�\���̂̐�
		&g_pMesh[1])))				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER3,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[2],			// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat[2],				// D3DXMATERIAL�\���̂̐�
		&g_pMesh[2])))				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	D3DXMATERIAL *pMat;
	// �}�e���A�����ɑ΂���|�C���^���擾
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMat[i]->GetBufferPointer();

		// �}�e���A���Ƀe�N�X�`�����o�^����Ă���Γǂݍ���
		if (pMat[i].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
				pMat[i].pTextureFilename,			// �t�@�C���̖��O
				&g_pTexture);						// �ǂݍ��ރ������[
		}
	}

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Player.life = MAX_LIFE;
	g_Player.bUse = true;

	g_Player.Pattern = SelectPlayer();
	
	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		g_Player.Pattern = 0;
	}

	if (g_Player.Pattern == 0)
	{
		g_Player.PlayerSpd = 1.5f;
	}
	if (g_Player.Pattern == 1)
	{
		g_Player.PlayerSpd = 1.8f;
		g_Player.life = g_Player.life / 5 * 4;	//40
	}
	if (g_Player.Pattern == 2)
	{
		g_Player.PlayerSpd = 1.2f;
		g_Player.life = g_Player.life / 5 * 6;	//60
	}

	g_FullLife = g_Player.life;

	g_Player.PlayerSpd = 1.5f;
	g_BulletCnt = 0;

	//�e�̏�����
	g_Player.idxShadow = CreateShadow(g_Player.pos, 30.0f, 30.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	g_Player.bUse = false;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_pTexture != NULL)
		{// �e�N�X�`���̊J��
			g_pTexture->Release();
			g_pTexture = NULL;
		}

		if (g_pMesh != NULL)
		{// ���b�V���̊J��
			g_pMesh[i]->Release();
			g_pMesh[i] = NULL;
		}

		if (g_pBuffMat != NULL)
		{// �}�e���A���̊J��
			g_pBuffMat[i]->Release();
			g_pBuffMat[i] = NULL;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	LOCKONRANGE* pLockOnRange = GetLockonRange();
	LOCKON* pLock = GetLockOn();
	LOCKONBULLET* pLBullet = GetLockonBullet();
	AbilityThunder * pAT = GetAbilityThunder();
	BOMBMGR* pAB = GetBombMgr();

	if (g_Player.life < 1)
	{
		g_Player.bUse = false;
	}

	if (g_Player.bUse == true)
	{
		g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * g_Player.PlayerSpd;
		g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * g_Player.PlayerSpd;
	
		if (Keyboard_IsPress(DIK_A))
		{
			if (Keyboard_IsPress(DIK_W))
			{
				g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);	//g_PlayerSpd / 3 * 4
				g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);

				/*
				g_Player.pos.x += g_Player.speed.x;
				g_Player.pos.z -= g_Player.speed.z;
				*/
			}

			g_Player.rot.y -= VALUE_ROTATE;
			g_Player.rot.z -= VALUE_ROTATE;
			if (g_Player.rot.y < -D3DX_PI)
			{
				g_Player.rot.y += D3DX_PI * 2.0f;
			}

			if (g_Player.rot.z < -D3DX_PI / 4)
			{
				g_Player.rot.z = -D3DX_PI / 4;
			}
		}

		else if (Keyboard_IsPress(DIK_D))
		{
			if (Keyboard_IsPress(DIK_W))
			{
				g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
				g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
			}
				
			g_Player.rot.y += VALUE_ROTATE;
			g_Player.rot.z += VALUE_ROTATE;

			if (g_Player.rot.y > D3DX_PI)
			{
				g_Player.rot.y -= D3DX_PI * 2.0f;
			}
			if (g_Player.rot.z > D3DX_PI / 4)
			{
				g_Player.rot.z = D3DX_PI / 4;
			}
		}

		if (Keyboard_IsPress(DIK_W))
		{
			g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
			g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
		}

		if (Keyboard_IsPress(DIK_S))
		{
			g_Player.speed.x *= 0.75f;
			g_Player.speed.z *= 0.75f;
		}

		if (g_Player.Pattern == 0)
		{
			if (Keyboard_IsPress(DIK_LSHIFT) && GetLockonRange()->LockonCoolDownFlg == false)
			{
				pLockOnRange->bUse = true;
			}

			if (Keyboard_IsRelease(DIK_LSHIFT))
			{
				pLockOnRange->bUse = false;
				//���b�N�I�����Ă���ꍇ
				for (int i = 0; i < MAX_LOCKON; i++)
				{
					if ((pLock + i)->bUse == true)
					{
						SetLockonBullet(D3DXVECTOR3(g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f,
							0.5f,
							g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f), g_Player.rot.y);
						//PlaySound(SOUND_LABEL_SE_LOCKONBULLET);
					}
				}

				//���b�N�I�����Ă��Ȃ��ꍇ
				if (pLockOnRange->LockonFreeFlg)
				{
					for (int i = 0; i < MAX_LOCKON; i++)
					{
						SetLockonBullet(D3DXVECTOR3(g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f,
							0.5f,
							g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f), g_Player.rot.y);
					}
				}
			}			
		}

		//�@�̃p�^�[��1�̏ꍇ
		if (g_Player.Pattern == 1)
		{
			if (Keyboard_IsPress(DIK_LSHIFT) && pAT->PlusThunderCoolDownFlg == false)
			{
				//�A�r���e�B����
				pAT->PlusThunder = true;
			}
		}

		if (g_Player.Pattern == 2)
		{

			//�{��2������
			if (Keyboard_IsPress(DIK_LSHIFT) && pAB->BombCoolDownFlg == false)
			{
				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y - 1.0f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y - 1.0f) * 18.0f;

				SetBomb(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y - 0.1f) * 4.0f, 0.0f, -cosf(g_Player.rot.y - 0.1f) * 4.0f));

				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y + 2.0f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y + 2.0f) * 18.0f;

				SetBomb(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y + 0.1f) * 4.0f, 0.0f, -cosf(g_Player.rot.y + 0.1f) * 4.0f));
				pAB->BombUse = true;
			}
		}
	   
		if (g_Player.rot.z > 0 && !Keyboard_IsPress(DIK_D))
		{
			g_Player.rot.z -= (VALUE_ROTATE / 2);
		}
		if (g_Player.rot.z < 0 && !Keyboard_IsPress(DIK_A))
		{
			g_Player.rot.z += (VALUE_ROTATE / 2);
		}


		// ���W�̍X�V
		g_Player.rot.y = g_Player.rot.y + cam->rot.y;
		g_Player.pos.x += g_Player.speed.x;
		g_Player.pos.z -= g_Player.speed.z;

		g_Player.col_Sphere.pos.x = g_Player.pos.x;
		g_Player.col_Sphere.pos.y = g_Player.pos.y;
		g_Player.col_Sphere.pos.z = g_Player.pos.z;

		g_Player.col_Sphere.r = 25.0f;

		g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y) * 18.0f;
		g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y) * 18.0f;
		
		if (g_Player.Pattern == 2)
		{
			g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f;
			g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f;
		}


		// �J�����̒����_�Ǝ��_����l���ɒǏ]������
		cam->posR.x = g_Player.pos.x;
		cam->posR.z = g_Player.pos.z;
		cam->posV.x = cam->posR.x - sinf(g_Player.rot.y + D3DX_PI) * cam->fDistance;
		cam->posV.z = cam->posR.z - cosf(g_Player.rot.y + D3DX_PI) * cam->fDistance;

		if (Keyboard_IsPress(DIK_C))
		{
			cam->posV.x = cam->posR.x - sinf(g_Player.rot.y) * cam->fDistance;
			cam->posV.z = cam->posR.z - cosf(g_Player.rot.y) * cam->fDistance;
		}

		if (Keyboard_IsPress(DIK_RSHIFT))
		{
			g_BulletCnt++;
			if (g_BulletCnt > 9)
			{
				g_BulletCnt = 0;
			}
		}
		if (!Keyboard_IsPress(DIK_RSHIFT))
		{
			g_BulletCnt = 0;			
		}

		if (g_BulletCnt % 10 == 9 || Keyboard_IsTrigger(DIK_RSHIFT))
		{
			PlaySound(SOUND_LABEL_SE_SHOT);
			SetBullet(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f));
			if (g_Player.Pattern == 2)
			{
				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y + 0.5f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y + 0.5f) * 18.0f;

				SetBullet(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f));
			}
		}

		// �e�̍��W���X�V����
		D3DXVECTOR3 shadowPos = g_Player.pos;
		shadowPos.y = 0.0f;
		SetPositionShadow(g_Player.idxShadow, D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z));
		SetScaleShadow(g_Player.idxShadow, D3DXVECTOR3(50.0f - (g_Player.pos.y / 2), 0.0f, 50.0f - (g_Player.pos.y / 2)));
	
		if (g_Player.life > (g_FullLife / 10 * 5))
		{
			g_Player.HpColor = 3;
		}

		if (g_Player.life < (g_FullLife / 10 * 5))
		{
			g_Player.HpColor = 2;
		}

		if (g_Player.life < (g_FullLife / 10 * 2))
		{
			g_Player.HpColor = 1;
		}

		//=======�f�o�b�O�p=======================
		/*
		if (Keyboard_IsTrigger(DIK_1))
		{
			g_Player.Pattern = 0;
		}
		if (Keyboard_IsTrigger(DIK_2))
		{
			g_Player.Pattern = 1;
		}
		if (Keyboard_IsTrigger(DIK_3))
		{
			g_Player.Pattern = 2;
		}
		*/
		//========================================
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	if (g_Player.bUse == true)
	{

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player.mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);


		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMat[g_Player.Pattern]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Player.Pattern]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�(�e�N�X�`���̗L�郂�f����ǂݍ��ނƂ��͒���)
			pDevice->SetTexture(0, g_pTexture);

			// �`��
			g_pMesh[g_Player.Pattern]->DrawSubset(nCntMat);
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	/*
	//==============HP�o�[===========================================================================
	Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
		SCREEN_WIDTH - 300.0f,											//�`����Wx
		10.0f,															//�`����Wy
		0, 0,															//�e�N�X�`���؂�����Wx,y
		10 * g_Player.life, 10,											//�e�N�X�`���؂��蕝x,y
		10.0f * 0.5f,													//���S���Wx
		10.0f * 0.5f,													//���S���Wy
		3.0f,															//�g�嗦x����
		3.0f,															//�g�嗦y����
		0.0f,															//��]�p�x(���W�A��)
		g_Player.HpColor);												//�`�悷��F
		*/
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
PLAYER* GetPlayer()
{
	return &(g_Player);
}

SPHERE* Player_GetSphere(void)
{
	return (&g_Player.col_Sphere);
}
void Player_Destroy(void)
{
	g_Player.bUse = false;
	ReleaseShadow(g_Player.idxShadow);
}
void Player_Damage(int damage)
{
	g_Player.life -= damage;
}

void Player_AddLife(int life)
{
	g_Player.life += life;
	if (g_Player.life > g_FullLife)
	{
		g_Player.life = g_FullLife;
	}
}

bool Player_IsEnable(void)
{
	return g_Player.bUse;
}
