//=============================================================================
//
// �v���C���[�Z���N�g�V�[���Ǘ� [Select.cpp]
// Author : 
//
//=============================================================================
#include "input.h"
#include "Select.h"
#include "main.h"
#include "MyDirect3D.h"
#include "debugproc.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "sound.h"
#include "sprite.h"
#include "fade.h"
#include <time.h>
#include "light.h"
#include "meshfield.h"
#include "meshsky.h"
#include "player.h"
#include "UI.h"
#include "scene.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER1		"data/MODEL/Rondini_MK1(ver2).x"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER2		"data/MODEL/Rondini_MK2(ver2).x"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER3		"data/MODEL/Rondini_MK3(ver5).x"	// �ǂݍ��ރ��f����

#define	VALUE_MOVE		(2.0f)					// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)		// ��]��

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

static SELECT				g_Select;			// �v���C���[
static float				g_Rotate;
static int					g_Pattern;
static D3DXVECTOR3			g_StandardPos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSelect(void)
{
	// �����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �e�̏�����
	InitShadow();

	// UI�̏�����
	InitUI();


	// ��̏���������
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);


	// �n�ʂ̏���������
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);

	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER1,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pBuffMat[0],			// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
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
	g_Select.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_Select.rot = D3DXVECTOR3(0.0f, D3DX_PI / 4	, 0.0f);
	g_Select.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Select.life = 0;
	g_Select.spd = 0;

	//�e�̏�����
	g_Select.idxShadow = CreateShadow(g_Select.pos, 30.0f, 30.0f);


	g_Pattern = 0;
	g_Rotate = 0.0f;


	g_Select.pos.x = g_StandardPos.x + (cosf(g_Rotate));  //��]���鎞�̈ʒu
	g_Select.pos.z = g_StandardPos.z + (sinf(g_Rotate));  //��]���鎞�̈ʒu
	

	CAMERA *cam = GetCamera();

	cam->posR.x = g_Select.pos.x;
	cam->posR.z = g_Select.pos.z;
	cam->posV.x = cam->posR.x - sinf(g_Select.rot.y - (D3DX_PI / 3)) * cam->fDistance;
	cam->posV.z = cam->posR.z - cosf(g_Select.rot.y - (D3DX_PI / 3)) * cam->fDistance;

	PlaySound(SOUND_LABEL_SELECTBGM);



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSelect(void)
{
	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// ��̏I������
	UninitMeshSky();

	// �n�ʂ̏I������
	UninitMeshField();

	// �e�̏I������
	UninitShadow();

	// UI�̏I������
	UninitShadow();

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

	StopSound(SOUND_LABEL_SELECTBGM);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSelect(void)
{

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �󏈗��̍X�V����
	UpdateMeshSky();

	// �n�ʂ̍X�V����
	UpdateMeshField();

	// UI�̍X�V����
	UpdateUI();

	CAMERA *cam = GetCamera();
	   	 
	//�؂�ւ�
	if (Keyboard_IsTrigger(DIK_A))
	{
		g_Pattern -= 1;
		if (g_Pattern < 0)
		{
			g_Pattern = 2;
		}

	}
	if (Keyboard_IsTrigger(DIK_D))
	{
		g_Pattern += 1;
		if (g_Pattern > 2)
		{
			g_Pattern = 0;
		}
	}

	switch (g_Pattern)
	{
	case 0:
		g_Select.life = 40;
		g_Select.spd = 30;
		g_Select.atk = 30;
		break;
	case 1:
		g_Select.life = 30;
		g_Select.spd = 40;
		g_Select.atk = 30;
		break;
	case 2:
		g_Select.life = 50;
		g_Select.spd = 20;
		g_Select.atk = 40;
		break;
	default:
		break;
	}

	g_Select.rot.y += VALUE_ROTATE;
	   	   	 
	// ���W�̍X�V
	//g_Select.rot.y = g_Select.rot.y + cam->rot.y;	

	// �e�̍��W���X�V����
	D3DXVECTOR3 shadowPos = g_Select.pos;
	shadowPos.y = 0.0f;
	SetPositionShadow(g_Select.idxShadow, D3DXVECTOR3(g_Select.pos.x, 0.0f, g_Select.pos.z));
	SetScaleShadow(g_Select.idxShadow, D3DXVECTOR3(50.0f - (g_Select.pos.y / 2), 0.0f, 50.0f - (g_Select.pos.y / 2)));

	//�o�b�N�X�y�[�X�������ꂽ��V�[���J�ڂ���	
	if (Keyboard_IsTrigger(DIK_BACKSPACE))
	{
		//�N���A�J�E���g����Ńt�F�[�h����V�[�������߂�
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			if (GetClearCnt() == 0)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME);
			}
			if (GetClearCnt() == 1)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME2);
			}
			if (GetClearCnt() == 2)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME3);
			}
			if (GetClearCnt() == 3)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME4);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSelect(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �󏈗��̕`�揈��
	DrawMeshSky();

	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// UI�̕`�揈��
	DrawUI();

	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Select.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Select.scl.x, g_Select.scl.y, g_Select.scl.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Select.rot.y, g_Select.rot.x, g_Select.rot.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Select.pos.x, g_Select.pos.y, g_Select.pos.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Select.mtxWorld);


	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMat[g_Pattern]->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Pattern]; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�(�e�N�X�`���̗L�郂�f����ǂݍ��ނƂ��͒���)
		pDevice->SetTexture(0, g_pTexture);

		// �`��
		g_pMesh[g_Pattern]->DrawSubset(nCntMat);
	}
	
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

int SelectPlayer(void)
{
	return g_Pattern;
}

void SetSelectPlayer(int index)
{
	g_Pattern = index;
}

SELECT* GetSelect()
{
	return &g_Select;
}