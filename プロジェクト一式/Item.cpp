//=============================================================================
//
// �A�C�e���Ǘ� [Item.cpp]
// Author : 
//
//=============================================================================

#include "MyDirect3D.h"
#include "shadow.h"
#include "scene.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "Item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ITEM1			"data/MODEL/Life001.x"	
#define	VALUE_ITEM_ROTATE	(D3DX_PI * 0.008f)		// ��]��

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


static ITEM					g_Item[MAX_ITEM];	// �A�C�e��

static int					g_ItemEnableCnt = MAX_ITEM;	//���݂��Ă���G�̃J�E���g
static bool					g_ItemAllDestroy = false;			//�S�Ńt���O
static int					g_BirthCnt;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	PLAYER *pPlayer = GetPlayer();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_ITEM1,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
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

	for (int i = 0; i < MAX_ITEM; i++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Item[i].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_Item[i].rot = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Item[i].bUse = false;

		g_Item[i].type = 1;

		g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
		g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
		g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;

		g_Item[i].col_Sphere.r = 10.0f;

		g_Item[i].idxItemShadow = CreateShadow(g_Item[i].pos, 30.0f, 30.0f);
	}

	g_ItemAllDestroy = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_Item[i].bUse = false;
	}

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
void UpdateItem(void)
{
	PLAYER *pPlayer = GetPlayer();
	CAMERA *cam = GetCamera();

	g_ItemEnableCnt = MAX_ITEM;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		
		if (!g_Item[i].bUse)
		{
			g_ItemEnableCnt--;
			continue;
		}

		g_Item[i].rot.y += VALUE_ITEM_ROTATE;

		if (g_Item[i].rot.y < 0)
		{
			g_Item[i].rot.y += D3DX_PI * 2.0f;
		}

		if (g_Item[i].rot.y > D3DX_PI * 2)
		{
			g_Item[i].rot.y -= D3DX_PI * 2.0f;
		}

		g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
		g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
		g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
		g_Item[i].col_Sphere.r = 10.0f;

		SetPositionShadow(g_Item[i].idxItemShadow, D3DXVECTOR3(g_Item[i].pos.x, 0.0f, g_Item[i].pos.z));
	}

	if (g_ItemEnableCnt == 0)
	{
		g_ItemAllDestroy = true;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Item[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[i].rot.y, g_Item[i].rot.x, g_Item[i].rot.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Item[i].mtxWorld);

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

void SetItem(D3DXVECTOR3 pos)
{	
	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		g_BirthCnt = rand() % 100 + 1;

		if (g_BirthCnt < 40)
		{ 
			for (int i = 0; i < MAX_ITEM; i++)
			{
				if (!g_Item[i].bUse)
				{
					g_Item[i].pos = pos;
					g_Item[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					g_Item[i].bUse = true;

					g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
					g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
					g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
					g_Item[i].col_Sphere.r = 10.0f;

					break;
				}
			}
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME)
	{
		g_BirthCnt = rand() % 100 + 1;

		if (g_BirthCnt < 10)
		{
			for (int i = 0; i < MAX_ITEM; i++)
			{
				if (!g_Item[i].bUse)
				{
					g_Item[i].pos = pos;
					g_Item[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					g_Item[i].bUse = true;

					g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
					g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
					g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
					g_Item[i].col_Sphere.r = 10.0f;

					break;
				}
			}
		}
	}
	
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM* GetItem(int index)
{
	return &(g_Item[index]);
}

ITEM* GetAllItem()
{
	return g_Item;
}

SPHERE* Item_GetSphere(int index)
{
	return (&g_Item[index].col_Sphere);
}

void Item_Destroy(int index)
{
	g_Item[index].bUse = false;
	ReleaseShadow(g_Item[index].idxItemShadow);
}

bool Item_IsEnable(int index)
{
	return g_Item[index].bUse;
}

bool Item_AllDestroy()
{
	return g_ItemAllDestroy;
}