//=============================================================================
//
// �e���� [bullet.cpp]
// Author :
//
//=============================================================================
#include "bullet.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "Lockon.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/PlayerBullet.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_WIDTH		(5.0f)							// �e�̕�
#define	BULLET_HEIGHT		(5.0f)							// �e�̍���
#define	VALUE_MOVE_BULLET	(0.08f)							// �ړ����x
#define BULLET_LIFE			(200)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ��]
	D3DXVECTOR3		scale;		// �X�P�[��
	D3DXVECTOR3		dir;		// �ړ���
	int				timer;		// �����t���[��
	int				idxShadow;	// �eID
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O
	bool			LockonUse;

	SPHERE			col_Sphere;

} BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int nIdxBullet, float fWidth, float fHeight);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static BULLET					g_Bullet[MAX_BULLET];		// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_BULLET,			// �t�@�C���̖��O
								&g_pTextureBullet);		// �ǂݍ��ރ������[

	// �o���b�g�z��̏�����
	for(int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].timer = 0;
		g_Bullet[i].bUse = false;
		g_Bullet[i].LockonUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if(g_pTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if(g_pVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	LOCKON* pLock = GetLockOn();
	PLAYER* pPlayer = GetPlayer();

	//�e�̔z���S�����ׂ�
	for(int i = 0; i < MAX_BULLET; i++)
	{
		//�L�����t���O��ON�̕��̂ݏ������s��
		if (g_Bullet[i].bUse)
		{
			// �ړ�
			g_Bullet[i].pos.x += g_Bullet[i].dir.x;
			g_Bullet[i].pos.z += g_Bullet[i].dir.z;

			g_Bullet[i].col_Sphere.pos.x = g_Bullet[i].pos.x;
			g_Bullet[i].col_Sphere.pos.y = g_Bullet[i].pos.y;
			g_Bullet[i].col_Sphere.pos.z = g_Bullet[i].pos.z;

			g_Bullet[i].col_Sphere.r = 5.0f;

			// �����t���[���̃^�C�}�[�����炷
			g_Bullet[i].timer--;

			// �����t���[����0�ȉ��ɂȂ������������
			if(g_Bullet[i].timer <= 0)
			{
				// �e�̔j��
				ReleaseShadow(g_Bullet[i].idxShadow);

				g_Bullet[i].bUse = false;
			}
			else
			{
				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_Bullet[i].idxShadow, D3DXVECTOR3(g_Bullet[i].pos.x, 0.1f, g_Bullet[i].pos.z));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for(int i = 0; i < MAX_BULLET; i++)
	{
		if(g_Bullet[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Bullet[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_Bullet[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_Bullet[i].mtxWorld._41 = 0.0f;
			g_Bullet[i].mtxWorld._42 = 0.0f;
			g_Bullet[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_Bullet[i].mtxWorld._11 = mtxView._11;
			g_Bullet[i].mtxWorld._12 = mtxView._21;
			g_Bullet[i].mtxWorld._13 = mtxView._31;
			g_Bullet[i].mtxWorld._21 = mtxView._12;
			g_Bullet[i].mtxWorld._22 = mtxView._22;
			g_Bullet[i].mtxWorld._23 = mtxView._32;
			g_Bullet[i].mtxWorld._31 = mtxView._13;
			g_Bullet[i].mtxWorld._32 = mtxView._23;
			g_Bullet[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_Bullet[i].scale.x,
				g_Bullet[i].scale.y,
				g_Bullet[i].scale.z);
			D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].pos.x,
				g_Bullet[i].pos.y,
				g_Bullet[i].pos.z);
			D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

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
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pVtxBuffBullet,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))					// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_WIDTH / 2, BULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_WIDTH / 2, BULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_WIDTH / 2, -BULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_WIDTH / 2, -BULLET_HEIGHT / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(int nIdxBullet, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int nIdxBullet = -1;

	for(int i = 0; i < MAX_BULLET; i++)
	{
		if(!g_Bullet[i].bUse)
		{
			g_Bullet[i].pos = pos;
			g_Bullet[i].pos.y = BULLET_HEIGHT/2;
			g_Bullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet[i].dir = dir;
			g_Bullet[i].timer = BULLET_LIFE;
			g_Bullet[i].bUse = true;

			// �e�̐ݒ�
			g_Bullet[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// �e�̐ݒ�

			nIdxBullet = i;

			break;
		}
	}

	return nIdxBullet;
}

BULLET* GetBullet()
{
	return g_Bullet;
}

SPHERE* Bullet_GetSphere(int index)
{
	return (&g_Bullet[index].col_Sphere);
}

void Bullet_Destroy(int index)
{
	g_Bullet[index].bUse = false;
	g_Bullet[index].LockonUse = false;
	ReleaseShadow(g_Bullet[index].idxShadow);
}

bool Bullet_IsEnable(int index)
{
	return g_Bullet[index].bUse;
}
