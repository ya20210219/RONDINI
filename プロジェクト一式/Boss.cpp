//=============================================================================
//
// �G�l�~�[�Ǘ� [Boss.cpp]
// Author : 
//
//=============================================================================
#include "Boss.h"
#include "MyDirect3D.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "BossBullet.h"
#include "radian.h"
#include "scene.h"
#include "Score.h"
#include "BossBomb.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_Boss1			"data/MODEL/Rondini_BossBattleShip_type05.x"
#define	MODEL_Boss2			"data/MODEL/Rondini_BossBattleShip_type06.x"
#define	MODEL_LastBoss		"data/MODEL/Rondini_LastBossBattleShip_Ver02.x"
//#define	MODEL_Boss		"data/MODEL/Rondini_Boss_type03.x"
#define	VALUE_Boss_ROTATE	(D3DX_PI * 0.008f)		// ��]��
#define BOSS_LIFE			(360)	//300	
#define BOSS_R				(120.0f)
#define BOSS_TYPE_NUM       (3)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPD3DXMESH			g_pMesh[BOSS_TYPE_NUM] = { NULL };		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			g_pBuffMat[BOSS_TYPE_NUM] = { NULL };	// �}�e���A�����ւ̃|�C���^
static DWORD				g_nNumMat[BOSS_TYPE_NUM] = { 0 };		// �}�e���A�����̐�
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// �e�N�X�`��


static BOSS					g_Boss[MAX_BOSS];	// �G�l�~�[

static D3DXVECTOR3			g_Target;
static int					g_BossEnableCnt = MAX_BOSS;			//���݂��Ă���G�̃J�E���g
static bool					g_BossAllDestroy = false;			//�S�Ńt���O
static float				g_MoverotY;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	PLAYER *pPlayer = GetPlayer();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_Boss2,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[0],							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat[0],								// D3DXMATERIAL�\���̂̐�
		&g_pMesh[0])))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_Boss1,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[1],							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat[1],								// D3DXMATERIAL�\���̂̐�
		&g_pMesh[1])))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_LastBoss,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[2],							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMat[2],								// D3DXMATERIAL�\���̂̐�
		&g_pMesh[2])))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}



	D3DXMATERIAL *pMat;

	for (int i = 0; i < BOSS_TYPE_NUM; i++)
	{
		// �}�e���A�����ɑ΂���|�C���^���擾
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

	if (GetSceneNumber() != SCENE_INDEX_TUTORIAL)
	{
		for (int i = 0; i < MAX_BOSS; i++)
		{
			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Boss[i].pos = D3DXVECTOR3(0.0f, 50.0f, 3000.0f);
			g_Boss[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Boss[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Boss[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Boss[i].NowRotY = 0.0f;
			g_Boss[i].TargetRotY = 0.0f;

			// �x�[�X�ʒu�ɕۑ�����
			g_Boss[i].base_pos = g_Boss[i].pos;

			g_Boss[i].bUse = true;

			g_Boss[i].BossBulletTimer = 0;
			if (GetSceneNumber() == SCENE_INDEX_GAME)
			{
				g_Boss[i].type = 0;
				g_Boss[i].Life = 3;	//BOSS_LIFE
				g_Boss[i].Life = BOSS_LIFE;	//360
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			if (GetSceneNumber() == SCENE_INDEX_GAME2)
			{
				g_Boss[i].type = 1;
				g_Boss[i].Life = 520;
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			if (GetSceneNumber() == SCENE_INDEX_GAME4)
			{
				g_Boss[i].type = 2;
				g_Boss[i].Life = 720;
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			g_Boss[i].col_Sphere.pos.x = g_Boss[i].pos.x;
			g_Boss[i].col_Sphere.pos.y = g_Boss[i].pos.y;
			g_Boss[i].col_Sphere.pos.z = g_Boss[i].pos.z;

			g_Boss[i].col_Sphere.r = BOSS_R;

			g_Boss[i].idxBossShadow = CreateShadow(g_Boss[i].pos, 60.0f, 60.0f);
		}
	}	

	g_BossAllDestroy = false;

	g_MoverotY = 0.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	if (g_pTexture != NULL)
	{// �e�N�X�`���̊J��
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	for (int i = 0; i < BOSS_TYPE_NUM; i++)
	{
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
void UpdateBoss(void)
{
	PLAYER *pPlayer = GetPlayer();
	CAMERA *cam = GetCamera();

	g_BossEnableCnt = MAX_BOSS;

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (!g_Boss[i].bUse)
		{
			g_BossEnableCnt--;
			continue;
		}

		if (g_Boss[i].Life < 0)
		{
			Boss_Destroy(i);
		}

		g_Boss[i].g_Dist = GetFloatDist(Boss_GetSphere(i), Player_GetSphere());

		g_Target = pPlayer->pos;

		if (g_Boss[i].rot.y < 0)
		{
			g_Boss[i].rot.y += D3DX_PI * 2.0f;
		}

		if (g_Boss[i].rot.y > D3DX_PI * 2)
		{
			g_Boss[i].rot.y -= D3DX_PI * 2.0f;
		}

		g_Boss[i].NowRotY = g_Boss[i].rot.y;

		g_Boss[i].TargetRotY = (-atan2f((g_Target.z - g_Boss[i].pos.z), (g_Target.x - g_Boss[i].pos.x)) - (D3DX_PI / 2));

		g_MoverotY = (float)LerpRadian(g_Boss[i].NowRotY, g_Boss[i].TargetRotY, 0.01f);	//0.2��8

		g_Boss[i].rot.y = g_MoverotY;

		if (GetSceneNumber() != SCENE_INDEX_GAME4)
		{
			g_Boss[i].spd.x = sinf(D3DX_PI * 0.0f - g_Boss[i].rot.y) * 0.8f;
			g_Boss[i].spd.z = cosf(D3DX_PI * 0.0f - g_Boss[i].rot.y) * 0.8f;
		}

		g_Boss[i].pos.x += g_Boss[i].spd.x;
		g_Boss[i].pos.z -= g_Boss[i].spd.z;

		g_Boss[i].BossBulletTimer++;

		//��1�b��1��U��	5��U��
		if (g_Boss[i].type == 0)
		{
			if (g_Boss[i].BossBulletTimer == 60)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletTimer = 0;
			}
		}

		//��1�b��1��U��	3��ƍ��E�΂�2��
		if (g_Boss[i].type == 1)
		{
			if (g_Boss[i].BossBulletTimer == 60)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletTimer = 0;
			}
		}
		
		//��0.5�b��1��U��	�O��ƍ��E�΂�
		if (g_Boss[i].type == 2)
		{
			if (g_Boss[i].BossBulletTimer % 30 == 1)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 2.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 2.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));
				
			}

			if (g_Boss[i].BossBulletTimer > 180)
			{
				SetBossBomb(D3DXVECTOR3(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z - BOSS_R), g_Boss[i].rot);
				g_Boss[i].BossBulletTimer = 0;
			}
		}

		g_Boss[i].col_Sphere.pos.x = g_Boss[i].pos.x;
		g_Boss[i].col_Sphere.pos.y = g_Boss[i].pos.y;
		g_Boss[i].col_Sphere.pos.z = g_Boss[i].pos.z;
		g_Boss[i].col_Sphere.r = BOSS_R;

		SetPositionShadow(g_Boss[i].idxBossShadow, D3DXVECTOR3(g_Boss[i].pos.x, 0.0f, g_Boss[i].pos.z));

		if (g_Boss[i].Life > (g_Boss[i].MaxLife / 10 * 5))
		{
			g_Boss[i].HpColor = 3;
		}

		if (g_Boss[i].Life < (g_Boss[i].MaxLife / 10 * 5))
		{
			g_Boss[i].HpColor = 2;
		}

		if (g_Boss[i].Life < (g_Boss[i].MaxLife / 10 * 2))
		{
			g_Boss[i].HpColor = 1;
		}
	}
	   	 
	if (g_BossEnableCnt == 0)
	{
		g_BossAllDestroy = true;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (!g_Boss[i].bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Boss[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss[i].rot.y, g_Boss[i].rot.x, g_Boss[i].rot.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss[i].mtxWorld);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMat[g_Boss[i].type]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Boss[i].type]; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�(�e�N�X�`���̗L�郂�f����ǂݍ��ނƂ��͒���)
			pDevice->SetTexture(0, g_pTexture);

			// �`��
			g_pMesh[g_Boss[i].type]->DrawSubset(nCntMat);
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
BOSS* GetBoss()
{
	return &(g_Boss[0]);
}

BOSS* GetAllBoss()
{
	return g_Boss;
}

SPHERE* Boss_GetSphere(int index)
{
	return (&g_Boss[index].col_Sphere);
}

void Boss_Destroy(int index)
{
	PlaySound(SOUND_LABEL_SE_EXPLOSION);
	AddScore(1000);
	g_Boss[index].bUse = false;
	ReleaseShadow(g_Boss[index].idxBossShadow);
}

void Boss_Damage(int index, int damage)
{
	g_Boss[index].Life -= damage;
	if (g_Boss[index].Life > 1)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}
}

bool Boss_IsEnable(int index)
{
	return g_Boss[index].bUse;
}

bool Boss_AllDestroy()
{
	return g_BossAllDestroy;
}