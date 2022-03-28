//=============================================================================
//
// ���b�V����̏��� [meshsky.cpp]
// Author :
//
//=============================================================================
#include "meshsky.h"
#include "MyDirect3D.h"
#include "input.h"
#include "camera.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME1		"data/TEXTURE/sky002.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C���� sky002
#define	TEXTURE_FILENAME2		"data/TEXTURE/sky004.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C���� sky002
#define	VALUE_MOVE_SKY			(5.0f)							// �ړ����x
#define	VALUE_ROTATE_SKY		(D3DX_PI * 0.001f)				// ��]���x

#define NUM_DIVIDE_H			(16)
#define NUM_DIVIDE_V			(8)
#define	NUM_VERTEX_CYLINDER		((NUM_DIVIDE_H + 1) * (NUM_DIVIDE_V + 1))						// ���_��
#define	NUM_POLYGON_CYLINDER	(NUM_DIVIDE_H * (NUM_DIVIDE_V * 2) + (NUM_DIVIDE_V - 1) * 4)	// �|���S����
#define	NUM_VERTEX_INDEX		(NUM_DIVIDE_H * (NUM_DIVIDE_V * 2) + NUM_DIVIDE_V * 2 + (NUM_DIVIDE_V - 1) * 2)
#define	SKY_RADIUS				(30000.0f)	// ��h�[���̔��a	�����l8000
#define	SKY_HEIGHT_RATE			(2.0f)		// ��h�[���̍����W��

#define	TEX_COUNT_LOOP			(1)			// �e�N�X�`���̌J��Ԃ���


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSky;			// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSky;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSky;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyTop;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyTop;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

D3DXMATRIX g_mtxWorldSky;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posSky;						// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 g_rotSky;						// �|���S���̉�]�p

bool g_bTransParent;
bool g_bReverse;

int g_nNumBlockHSky, g_nNumBlockVSky;		// �u���b�N��
int g_nNumVertexSky;						// �����_��	
int g_nNumVertexIndexSky;					// ���C���f�b�N�X��
int g_nNumPolygonSky;						// ���|���S����
float g_fRadius;							// ���a

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius,
									int nNumBlockH, int nNumBlockV, bool bReverse)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	g_bTransParent = false;

	g_bReverse = bReverse;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	g_posSky = pos;

	g_rotSky = rot;

	g_fRadius = fRadius;

	if (GetSceneNumber() != SCENE_INDEX_GAME4)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME1,		// �t�@�C���̖��O
			&g_pTextureSky);		// �ǂݍ��ރ������[
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME2,		// �t�@�C���̖��O
			&g_pTextureSky);		// �ǂݍ��ރ������[
	}


	// �u���b�N���̐ݒ�
	g_nNumBlockHSky = nNumBlockH;
	g_nNumBlockVSky = nNumBlockV;

	// ���_���̐ݒ�
	g_nNumVertexSky = (nNumBlockH + 1) * (nNumBlockV + 1);

	// �|���S�����̐ݒ�
	g_nNumPolygonSky = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);

	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexSky = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX_CYLINDER,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pVtxBuffSky,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (NUM_DIVIDE_H + 1),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pVtxBuffSkyTop,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NUM_VERTEX_INDEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,				// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pIdxBuffSky,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (NUM_DIVIDE_H + 2),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,				// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pIdxBuffSkyTop,			// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / NUM_DIVIDE_H;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (NUM_DIVIDE_V + 1);
		const float WIDTH = 1.0f / NUM_DIVIDE_H;
		const float HEIGHT = 1.0f / NUM_DIVIDE_V;
		float fLengthXZ;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffSky->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntV = 0; nCntV < (NUM_DIVIDE_V + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * g_fRadius;

			for(int nCntH = 0; nCntH < (NUM_DIVIDE_H + 1); nCntH++, pVtx++)
			{
				// ���_���W�̐ݒ�
				pVtx->pos.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				if(g_bReverse)
				{
					pVtx->pos.y = -sinf(ANGLE_V * nCntV) * g_fRadius * SKY_HEIGHT_RATE;
				}
				else
				{
					pVtx->pos.y = sinf(ANGLE_V * nCntV) * g_fRadius * SKY_HEIGHT_RATE;
				}
				pVtx->pos.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// �@���̐ݒ�
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				if(g_bReverse)
				{
					pVtx->col = D3DXCOLOR(0.0625f, 0.0625f, 0.375f, 0.5f);
					pVtx->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}
				else
				{
					pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					//pVtx->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}

				// �e�N�X�`�����W�̐ݒ�
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if(nCntV == NUM_DIVIDE_V)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffSky->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffSkyTop->Lock( 0, 0, (void**)&pVtx, 0 );

		// ���_���W�̐ݒ�
		pVtx->pos.x = 0.0f;
		if(g_bReverse)
		{
			pVtx->pos.y = -sinf(ANGLE_V * (NUM_DIVIDE_V + 1)) * g_fRadius * SKY_HEIGHT_RATE;
		}
		else
		{
			pVtx->pos.y = sinf(ANGLE_V * (NUM_DIVIDE_V + 1)) * g_fRadius * SKY_HEIGHT_RATE;
		}
		pVtx->pos.z = 0.0f;

		// �@���̐ݒ�
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		if(g_bReverse)
		{
			pVtx->col = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
		}
		else
		{
			pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius;
		for(int nCntH = 0; nCntH < NUM_DIVIDE_H; nCntH++, pVtx++)
		{
			// ���_���W�̐ݒ�
			pVtx->pos.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			if(g_bReverse)
			{
				pVtx->pos.y = -sinf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius * SKY_HEIGHT_RATE;
			}
			else
			{
				pVtx->pos.y = sinf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius * SKY_HEIGHT_RATE;
			}
			pVtx->pos.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// �@���̐ݒ�
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			if(g_bReverse)
			{
				pVtx->col = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
			}
			else
			{
				pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffSkyTop->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pIdxBuffSky->Lock( 0, 0, (void**)&pIdx, 0 );

		for(int nCntV = 0; nCntV < NUM_DIVIDE_V; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (NUM_DIVIDE_H + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < NUM_DIVIDE_H + 1; nCntH++)
			{
				*pIdx = nCntV * (NUM_DIVIDE_H + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (NUM_DIVIDE_H + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (NUM_DIVIDE_V - 1))
			{
				*pIdx = (nCntV + 1) * (NUM_DIVIDE_H + 1);
				pIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pIdxBuffSky->Unlock();

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pIdxBuffSkyTop->Lock( 0, 0, (void**)&pIdx, 0 );

		for(int nCntH = 0; nCntH < NUM_DIVIDE_H + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pIdxBuffSkyTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshSky(void)
{
	if(g_pVtxBuffSky)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffSky->Release();
		g_pVtxBuffSky = NULL;
	}

	if(g_pIdxBuffSky)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pIdxBuffSky->Release();
		g_pIdxBuffSky = NULL;
	}

	if(g_pVtxBuffSkyTop)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffSkyTop->Release();
		g_pVtxBuffSkyTop = NULL;
	}

	if(g_pIdxBuffSkyTop)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pIdxBuffSkyTop->Release();
		g_pIdxBuffSkyTop = NULL;
	}

	if(g_pTextureSky)
	{// �e�N�X�`���̊J��
		g_pTextureSky->Release();
		g_pTextureSky = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshSky(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for (int i = 0; i < 2; i++)
	{
		// ���C�e�B���O�𖳌���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldSky);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotSky.x, g_rotSky.y + ( D3DX_PI * i), g_rotSky.z);	//yxz�̏��ŕ���ł���
		D3DXMatrixMultiply(&g_mtxWorldSky, &g_mtxWorldSky, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_posSky.x, g_posSky.y, g_posSky.z);
		D3DXMatrixMultiply(&g_mtxWorldSky, &g_mtxWorldSky, &mtxTranslate);

		if (g_bTransParent)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// Z�o�b�t�@���g�p���Ȃ�
		}
		else
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// Z�o�b�t�@���g�p����
		}

		if (g_bReverse)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// �\�ʂ��J�����O
		}
		else
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSky);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffSky, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffSky);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSky);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_VERTEX_CYLINDER, 0, NUM_POLYGON_CYLINDER);


		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffSkyTop, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffSkyTop);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (NUM_DIVIDE_H + 2), 0, NUM_DIVIDE_H);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O

		// ���C�e�B���O��L����
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}
}

