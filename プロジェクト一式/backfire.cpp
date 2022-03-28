//=============================================================================
//
// �o�b�N�t�@�C�A���� [BackFire.cpp]
// Author :  
//
//=============================================================================
#include "BackFire.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BackFire	"data/TEXTURE/BackFire.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BackFire_WIDTH		(10.0f)							// �o�b�N�t�@�C�A�̕�
#define	BackFire_HEIGHT		(10.0f)							// �o�b�N�t�@�C�A�̍���

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
	int				countH;		//�J�E���g
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

} BackFire;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBackFire(LPDIRECT3DDEVICE9 pDevice, int w, int h);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBackFire = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBackFire = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static BackFire					g_BackFire;		// �o�b�N�t�@�C�A���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBackFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// ���_���̍쐬
	MakeVertexBackFire(pDevice, 0, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BackFire,			// �t�@�C���̖��O
		&g_pTextureBackFire);		// �ǂݍ��ރ������[

	// �o�b�N�t�@�C�A�̏�����

	PLAYER* pPlayer = GetPlayer();

	g_BackFire.pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	g_BackFire.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_BackFire.timer = 0;
	g_BackFire.countW = 0;
	g_BackFire.countH = 0;
	g_BackFire.bUse = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBackFire(void)
{
	if (g_pTextureBackFire != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBackFire->Release();
		g_pTextureBackFire = NULL;
	}

	if (g_pVtxBuffBackFire != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBackFire->Release();
		g_pVtxBuffBackFire = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBackFire(void)
{
	g_BackFire.scale = D3DXVECTOR3(0.9f, 0.9f, 0.9f);

	if (g_BackFire.timer <= 60)
	{
		if (g_BackFire.timer % 2 == 0)
		{
			g_BackFire.countW++;
			if(g_BackFire.countW > 5)
			{
				g_BackFire.countH++;
				if (g_BackFire.countH > 2)
				{
					g_BackFire.countH = 0;
				}
				g_BackFire.countW = 0;
			}
			LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
			MakeVertexBackFire(pDevice, g_BackFire.countW, g_BackFire.countH);
		}
		if (g_BackFire.timer == 60)
		{
			g_BackFire.timer = 0;
		}
	}

	PLAYER* pPlayer = GetPlayer();

	g_BackFire.pos.x = pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI) * 7.5f;
	g_BackFire.pos.y = pPlayer->pos.y;
	g_BackFire.pos.z = pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI) * 7.5f;

	if (Keyboard_IsPress(DIK_W))
	{
		g_BackFire.scale = D3DXVECTOR3(1.1f, 1.1f, 1.1f);
	}
	if (Keyboard_IsPress(DIK_S))
	{
		g_BackFire.scale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);
	}
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBackFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (g_BackFire.bUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_BackFire.mtxWorld);

		D3DXMATRIX mtxView;

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
		// �t�s������߂�
		D3DXMatrixInverse(&g_BackFire.mtxWorld, NULL, &mtxView);

		// ���s�ړ��̕����͖�������
		g_BackFire.mtxWorld._41 = 0.0f;
		g_BackFire.mtxWorld._42 = 0.0f;
		g_BackFire.mtxWorld._43 = 0.0f;
#else
		//�]�u�s��
		g_BackFire.mtxWorld._11 = mtxView._11;
		g_BackFire.mtxWorld._12 = mtxView._21;
		g_BackFire.mtxWorld._13 = mtxView._31;
		g_BackFire.mtxWorld._21 = mtxView._12;
		g_BackFire.mtxWorld._22 = mtxView._22;
		g_BackFire.mtxWorld._23 = mtxView._32;
		g_BackFire.mtxWorld._31 = mtxView._13;
		g_BackFire.mtxWorld._32 = mtxView._23;
		g_BackFire.mtxWorld._33 = mtxView._33;
#endif

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, g_BackFire.scale.x,
			g_BackFire.scale.y,
			g_BackFire.scale.z);
		D3DXMatrixMultiply(&g_BackFire.mtxWorld, &g_BackFire.mtxWorld, &mtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_BackFire.pos.x,
			g_BackFire.pos.y,
			g_BackFire.pos.z);
		D3DXMatrixMultiply(&g_BackFire.mtxWorld, &g_BackFire.mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_BackFire.mtxWorld);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffBackFire, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBackFire);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBackFire(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffBackFire,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBackFire->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BackFire_WIDTH / 2, BackFire_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BackFire_WIDTH / 2, BackFire_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BackFire_WIDTH / 2, -BackFire_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BackFire_WIDTH / 2, -BackFire_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffBackFire->Unlock();
	}

	return S_OK;
}

BackFire* GetBackFire()
{
	return &g_BackFire;
}

void BackFire_Destroy()
{
	g_BackFire.bUse = false;
}

bool BackFire_IsEnable()
{
	return g_BackFire.bUse;
}
