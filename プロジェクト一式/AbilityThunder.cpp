//=============================================================================
//
// �o�b�N�t�@�C�A���� [AbilityThunder.cpp]
// Author : 
//
//=============================================================================
#include "AbilityThunder.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_AbilityThunder	"data/TEXTURE/AbilityThunder.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	AbilityThunder_WIDTH		(100.0f)							// �o�b�N�t�@�C�A�̕�
#define	AbilityThunder_HEIGHT		(100.0f)							// �o�b�N�t�@�C�A�̍���

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexAbilityThunder(LPDIRECT3DDEVICE9 pDevice, int w, int h);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureAbilityThunder = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAbilityThunder = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static AbilityThunder					g_AbilityThunder;		// �o�b�N�t�@�C�A���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAbilityThunder(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// ���_���̍쐬
	MakeVertexAbilityThunder(pDevice, 0, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_AbilityThunder,			// �t�@�C���̖��O
		&g_pTextureAbilityThunder);		// �ǂݍ��ރ������[

	// �o�b�N�t�@�C�A�̏�����

	PLAYER* pPlayer = GetPlayer();

	g_AbilityThunder.pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	g_AbilityThunder.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_AbilityThunder.timer = 0;
	g_AbilityThunder.countW = 0;
	g_AbilityThunder.countH = 0;
	g_AbilityThunder.bUse = true;

	g_AbilityThunder.PlusThunderCnt = 300;
	g_AbilityThunder.PlusThunderUseCnt = 0;
	g_AbilityThunder.PlusThunder = false;
	g_AbilityThunder.PlusThunderCoolDownFlg = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAbilityThunder(void)
{
	if (g_pTextureAbilityThunder != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureAbilityThunder->Release();
		g_pTextureAbilityThunder = NULL;
	}

	if (g_pVtxBuffAbilityThunder != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffAbilityThunder->Release();
		g_pVtxBuffAbilityThunder = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateAbilityThunder(void)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->Pattern != 1)
	{
		g_AbilityThunder.bUse = false;
	}

	if (g_AbilityThunder.bUse == true && g_AbilityThunder.PlusThunder == true)
	{
		if (g_AbilityThunder.timer <= 60)
		{
			if (g_AbilityThunder.timer % 2 == 0)
			{
				g_AbilityThunder.countW++;
				if (g_AbilityThunder.countW > 5)
				{
					g_AbilityThunder.countH++;
					if (g_AbilityThunder.countH > 2)
					{
						g_AbilityThunder.countH = 0;
					}
					g_AbilityThunder.countW = 0;
				}
				LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
				MakeVertexAbilityThunder(pDevice, g_AbilityThunder.countW, g_AbilityThunder.countH);
			}
			if (g_AbilityThunder.timer == 60)
			{
				g_AbilityThunder.timer = 0;
			}
		}

		pPlayer->PlayerSpd = 3.0f;

		g_AbilityThunder.pos.x = pPlayer->pos.x;
		g_AbilityThunder.pos.y = pPlayer->pos.y;
		g_AbilityThunder.pos.z = pPlayer->pos.z;

		g_AbilityThunder.col_Sphere.pos.x = pPlayer->pos.x;
		g_AbilityThunder.col_Sphere.pos.y = pPlayer->pos.y;
		g_AbilityThunder.col_Sphere.pos.z = pPlayer->pos.z;

		g_AbilityThunder.col_Sphere.r = 50.0f;

		g_AbilityThunder.PlusThunderUseCnt++;

		if (g_AbilityThunder.PlusThunderUseCnt == 1)
		{
			PlaySound(SOUND_LABEL_SE_THUNDER);
		}

		if (g_AbilityThunder.PlusThunderUseCnt >= 180)
		{
			g_AbilityThunder.PlusThunder = false;
			g_AbilityThunder.PlusThunderCoolDownFlg = true;
			g_AbilityThunder.PlusThunderUseCnt = 0;
			g_AbilityThunder.PlusThunderCnt = 0;
			StopSound(SOUND_LABEL_SE_THUNDER);
		}
	}

	if (g_AbilityThunder.PlusThunderCoolDownFlg == true)
	{
		pPlayer->PlayerSpd = 1.8f;

		g_AbilityThunder.PlusThunderCnt++;
		if (g_AbilityThunder.PlusThunderCnt > 300)
		{
			g_AbilityThunder.PlusThunderCoolDownFlg = false;
			g_AbilityThunder.PlusThunderCnt = 300;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAbilityThunder(void)
{
	if (g_AbilityThunder.PlusThunder == true)
	{
		LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
		D3DXMATRIX mtxScale, mtxTranslate;

		// ���C�e�B���O�𖳌���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���e�X�g��L����
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (g_AbilityThunder.bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_AbilityThunder.mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_AbilityThunder.mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_AbilityThunder.mtxWorld._41 = 0.0f;
			g_AbilityThunder.mtxWorld._42 = 0.0f;
			g_AbilityThunder.mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_AbilityThunder.mtxWorld._11 = mtxView._11;
			g_AbilityThunder.mtxWorld._12 = mtxView._21;
			g_AbilityThunder.mtxWorld._13 = mtxView._31;
			g_AbilityThunder.mtxWorld._21 = mtxView._12;
			g_AbilityThunder.mtxWorld._22 = mtxView._22;
			g_AbilityThunder.mtxWorld._23 = mtxView._32;
			g_AbilityThunder.mtxWorld._31 = mtxView._13;
			g_AbilityThunder.mtxWorld._32 = mtxView._23;
			g_AbilityThunder.mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_AbilityThunder.scale.x,
				g_AbilityThunder.scale.y,
				g_AbilityThunder.scale.z);
			D3DXMatrixMultiply(&g_AbilityThunder.mtxWorld, &g_AbilityThunder.mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_AbilityThunder.pos.x,
				g_AbilityThunder.pos.y,
				g_AbilityThunder.pos.z);
			D3DXMatrixMultiply(&g_AbilityThunder.mtxWorld, &g_AbilityThunder.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_AbilityThunder.mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffAbilityThunder, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureAbilityThunder);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		// ���C�e�B���O��L����
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// ���e�X�g�𖳌���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexAbilityThunder(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffAbilityThunder,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffAbilityThunder->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-AbilityThunder_WIDTH / 2, AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(AbilityThunder_WIDTH / 2, AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-AbilityThunder_WIDTH / 2, -AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(AbilityThunder_WIDTH / 2, -AbilityThunder_HEIGHT / 2, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * h));
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * h));
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * (h + 1)));
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * (h + 1)));

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffAbilityThunder->Unlock();
	}

	return S_OK;
}

AbilityThunder* GetAbilityThunder()
{
	return &g_AbilityThunder;
}

SPHERE* GetAbilityThunderSphere()
{
	return &g_AbilityThunder.col_Sphere;
}

void AbilityThunder_Destroy()
{
	g_AbilityThunder.bUse = false;
}

bool AbilityThunder_IsEnable()
{
	return g_AbilityThunder.PlusThunder;
}
