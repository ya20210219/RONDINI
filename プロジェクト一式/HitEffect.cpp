//=============================================================================
//
// �q�b�g�G�t�F�N�g���� [HitEffect.cpp]
// Author :  
//
//=============================================================================
#include "HitEffect.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_HitEffect	"data/TEXTURE/HitEffect.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	HitEffect_WIDTH		(30.0f)							// �o�b�N�t�@�C�A�̕�
#define	HitEffect_HEIGHT	(30.0f)							// �o�b�N�t�@�C�A�̍���
#define HITEffect_Timer		(24)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		scale;		// �X�P�[��
	int				timer;		// �����t���[��
	int				countW;		//�J�E���g
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

} HitEffect;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice, int w);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureHitEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffHitEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static HitEffect					g_HitEffect[MAX_HITEFFECT];		// �o�b�N�t�@�C�A���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// ���_���̍쐬
	MakeVertexHitEffect(pDevice, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_HitEffect,			// �t�@�C���̖��O
		&g_pTextureHitEffect);		// �ǂݍ��ރ������[

	// �q�b�g�G�t�F�N�g�̏�����

	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		g_HitEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitEffect[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_HitEffect[i].timer = 0;
		g_HitEffect[i].countW = 0;
		g_HitEffect[i].bUse = false;

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHitEffect(void)
{
	if (g_pTextureHitEffect != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureHitEffect->Release();
		g_pTextureHitEffect = NULL;
	}

	if (g_pVtxBuffHitEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffHitEffect->Release();
		g_pVtxBuffHitEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHitEffect(void)
{
	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (g_HitEffect[i].bUse)
		{
			g_HitEffect[i].timer++;

			if (g_HitEffect[i].timer <= HITEffect_Timer)
			{
				if (g_HitEffect[i].timer % 3 == 0)
				{
					g_HitEffect[i].countW++;
					if (g_HitEffect[i].countW > 8)
					{
						g_HitEffect[i].countW = 0;
					}
					LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
					MakeVertexHitEffect(pDevice, g_HitEffect[i].countW);
				}
				if (g_HitEffect[i].timer == HITEffect_Timer)
				{
					g_HitEffect[i].timer = 0;
					g_HitEffect[i].bUse = false;
				}
			}
		}
	}	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (g_HitEffect[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_HitEffect[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_HitEffect[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_HitEffect[i].mtxWorld._41 = 0.0f;
			g_HitEffect[i].mtxWorld._42 = 0.0f;
			g_HitEffect[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_HitEffect[i].mtxWorld._11 = mtxView._11;
			g_HitEffect[i].mtxWorld._12 = mtxView._21;
			g_HitEffect[i].mtxWorld._13 = mtxView._31;
			g_HitEffect[i].mtxWorld._21 = mtxView._12;
			g_HitEffect[i].mtxWorld._22 = mtxView._22;
			g_HitEffect[i].mtxWorld._23 = mtxView._32;
			g_HitEffect[i].mtxWorld._31 = mtxView._13;
			g_HitEffect[i].mtxWorld._32 = mtxView._23;
			g_HitEffect[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_HitEffect[i].scale.x,
				g_HitEffect[i].scale.y,
				g_HitEffect[i].scale.z);
			D3DXMatrixMultiply(&g_HitEffect[i].mtxWorld, &g_HitEffect[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_HitEffect[i].pos.x,
				g_HitEffect[i].pos.y,
				g_HitEffect[i].pos.z);
			D3DXMatrixMultiply(&g_HitEffect[i].mtxWorld, &g_HitEffect[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_HitEffect[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffHitEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureHitEffect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice, int w)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffHitEffect,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-HitEffect_WIDTH / 2, HitEffect_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(HitEffect_WIDTH / 2, HitEffect_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-HitEffect_WIDTH / 2, -HitEffect_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(HitEffect_WIDTH / 2, -HitEffect_HEIGHT / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * w), 0.0f );
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * (w + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * w), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.125f * (w + 1)), 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffHitEffect->Unlock();
	}

	return S_OK;
}

void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (!g_HitEffect[i].bUse)
		{
			g_HitEffect[i].pos = pos;
			g_HitEffect[i].scale = scale;
			g_HitEffect[i].timer = 0;
			g_HitEffect[i].bUse = true;

			break;
		}
	}
}

/*
HitEffect* GetHitEffect()
{
	return &g_HitEffect[i];
}

void HitEffect_Destroy()
{
	g_HitEffect[i].bUse = false;
}

bool HitEffect_IsEnable()
{
	return g_HitEffect[i].bUse;
}
*/
