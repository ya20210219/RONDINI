//=============================================================================
//
// �e���� [shadow.cpp]
// Author :
//
//=============================================================================
#include "shadow.h"
#include "MyDirect3D.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SHADOW_SIZE_X		(1.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(1.0f)							// �e�̍���

#define	MAX_SHADOW			(128)							// �e�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scl;		// �X�P�[��
	bool bUse;				// �g�p���Ă��邩�ǂ���
} SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

SHADOW					g_aShadow[MAX_SHADOW];		// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexShadow(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SHADOW,			// �t�@�C���̖��O
								&g_pTextureShadow);		// �ǂݍ��ރ������[

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].scl = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
		g_aShadow[nCntShadow].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	if(g_pTextureShadow != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	if(g_pVtxBuffShadow != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);		// ���� = �]����(DEST) - �]����(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z��r�Ȃ�
	DWORD zfunc;
	pDevice->GetRenderState(D3DRS_ZFUNC, &zfunc);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for(int i = 0; i < MAX_SHADOW; i++)
	{
		if(g_aShadow[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aShadow[i].scl.x, g_aShadow[i].scl.y, g_aShadow[i].scl.z);
			D3DXMatrixMultiply(&g_aShadow[i].mtxWorld, &g_aShadow[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aShadow[i].pos.x, g_aShadow[i].pos.y, g_aShadow[i].pos.z);
			D3DXMatrixMultiply(&g_aShadow[i].mtxWorld, &g_aShadow[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[i].mtxWorld);

			// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, zfunc);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,			// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pVtxBuffShadow,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		pVtx[1].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		pVtx[2].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
		pVtx[3].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}


//=============================================================================
// �e�̍쐬
//=============================================================================
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	// �e�̔z���S�����ׂ�
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// �g���Ă��Ȃ��ԍ���T��
		if(!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].scl = D3DXVECTOR3(fSizeX, 1.0f, fSizeZ);
			g_aShadow[nCntShadow].bUse = true;

			nIdxShadow = nCntShadow;
			break;
		}
	}

	return nIdxShadow;
}

//=============================================================================
// �e�̔j��
//=============================================================================
void ReleaseShadow(int nIdxShadow)
{
	if(nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos = pos;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetScaleShadow(int nIdxShadow, D3DXVECTOR3 scl)
{
	g_aShadow[nIdxShadow].scl = scl;
}

