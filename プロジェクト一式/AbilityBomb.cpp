//=============================================================================
//
// �e���� [Bomb.cpp]
// Author :
//
//=============================================================================
#include "AbilityBomb.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "Lockon.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/AbilityBomb.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BOMB_WIDTH		(50.0f)							// �e�̕�
#define	BOMB_HEIGHT		(50.0f)							// �e�̍���
#define	VALUE_MOVE_BOMB	(0.08f)							// �ړ����x
#define BOMB_LIFE			(300)

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
	int				Life;		// ���C�t
	int				idxShadow;	// �eID
	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

	bool			BombUse;
	int				BombCoolDownCnt;
	bool			BombCoolDownFlg;
	int				countW;
	int				countH;

	SPHERE			col_Sphere;

} BOMB;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h);
void SetVertexBomb(int nIdxBullet, float fWidth, float fHeight);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBomb = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBomb = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static BOMB					g_Bomb[MAX_BOMB];		// �e���[�N
static BOMBMGR				g_BombMgr;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexBomb(pDevice, 0, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BULLET,			// �t�@�C���̖��O
		&g_pTextureBomb);		// �ǂݍ��ރ������[

// �o���b�g�z��̏�����
	for (int i = 0; i < MAX_BOMB; i++)
	{
		g_Bomb[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bomb[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bomb[i].timer = 0;
		g_Bomb[i].Life = 0;
		g_Bomb[i].bUse = false;
	}

	g_BombMgr.BombUse = false;
	g_BombMgr.BombCoolDownCnt = BOMB_LIFE;
	g_BombMgr.BombCoolDownFlg = false;
	g_BombMgr.countW = 0;
	g_BombMgr.countH = 0;
	g_BombMgr.UseTimer = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBomb(void)
{
	if (g_pTextureBomb != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBomb->Release();
		g_pTextureBomb = NULL;
	}

	if (g_pVtxBuffBomb != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBomb->Release();
		g_pVtxBuffBomb = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBomb(void)
{
	LOCKON* pLock = GetLockOn();
	PLAYER* pPlayer = GetPlayer();

	if (g_BombMgr.BombUse == true)
	{		
		g_BombMgr.UseTimer++;
		if (g_BombMgr.UseTimer > BOMB_LIFE)
		{
			g_BombMgr.BombCoolDownCnt = 0;
			g_BombMgr.BombUse = false;
			g_BombMgr.BombCoolDownFlg = true;
		}
		
		//�����̒e�����݂��Ȃ���΃N�[���_�E���J�n
		if (g_Bomb[0].bUse == false && g_Bomb[1].bUse == false)
		{
			g_BombMgr.BombCoolDownCnt = 0;
			g_BombMgr.BombUse = false;
			g_BombMgr.BombCoolDownFlg = true;
		}

		//�e�̔z���S�����ׂ�
		for (int i = 0; i < MAX_BOMB; i++)
		{
			if (g_Bomb[i].Life < 1)
			{
				g_Bomb[i].bUse = false;
			}

			//�L�����t���O��ON�̕��̂ݏ������s��
			if (g_Bomb[i].bUse)
			{
				g_Bomb[i].countW++;
				if (g_Bomb[i].countW > 5)
				{
					g_Bomb[i].countH++;
					if (g_Bomb[i].countH > 2)
					{
						g_Bomb[i].countH = 0;
					}
					g_Bomb[i].countW = 0;
				}
				LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
				MakeVertexBomb(pDevice, g_Bomb[i].countW, g_Bomb[i].countH);

				// �ړ�
				g_Bomb[i].pos.x += g_Bomb[i].dir.x;
				g_Bomb[i].pos.z += g_Bomb[i].dir.z;

				g_Bomb[i].col_Sphere.pos.x = g_Bomb[i].pos.x;
				g_Bomb[i].col_Sphere.pos.y = g_Bomb[i].pos.y;
				g_Bomb[i].col_Sphere.pos.z = g_Bomb[i].pos.z;

				g_Bomb[i].col_Sphere.r = BOMB_WIDTH / 2;
				//g_Bomb[i].col_Sphere.r = 10000.0f;

				// �����t���[���̃^�C�}�[�����炷
				g_Bomb[i].timer--;

				// �����t���[����0�ȉ��ɂȂ������������
				if (g_Bomb[i].timer <= 0 || g_BombMgr.BombCoolDownFlg == true)
				{
					// �e�̔j��
					ReleaseShadow(g_Bomb[i].idxShadow);
					g_Bomb[i].bUse = false;
				}
				else
				{
					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_Bomb[i].idxShadow, D3DXVECTOR3(g_Bomb[i].pos.x, 0.1f, g_Bomb[i].pos.z));
				}
			}
		}	
	}	

	if (g_BombMgr.BombCoolDownFlg == true)
	{
		g_BombMgr.UseTimer = 0;
		g_BombMgr.BombCoolDownCnt++;
		if (g_BombMgr.BombCoolDownCnt > BOMB_LIFE)
		{
			g_BombMgr.BombCoolDownFlg = false;
			g_BombMgr.BombCoolDownCnt = BOMB_LIFE;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (g_Bomb[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Bomb[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_Bomb[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_Bomb[i].mtxWorld._41 = 0.0f;
			g_Bomb[i].mtxWorld._42 = 0.0f;
			g_Bomb[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_Bomb[i].mtxWorld._11 = mtxView._11;
			g_Bomb[i].mtxWorld._12 = mtxView._21;
			g_Bomb[i].mtxWorld._13 = mtxView._31;
			g_Bomb[i].mtxWorld._21 = mtxView._12;
			g_Bomb[i].mtxWorld._22 = mtxView._22;
			g_Bomb[i].mtxWorld._23 = mtxView._32;
			g_Bomb[i].mtxWorld._31 = mtxView._13;
			g_Bomb[i].mtxWorld._32 = mtxView._23;
			g_Bomb[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_Bomb[i].scale.x,
				g_Bomb[i].scale.y,
				g_Bomb[i].scale.z);
			D3DXMatrixMultiply(&g_Bomb[i].mtxWorld, &g_Bomb[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Bomb[i].pos.x,
				g_Bomb[i].pos.y,
				g_Bomb[i].pos.z);
			D3DXMatrixMultiply(&g_Bomb[i].mtxWorld, &g_Bomb[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Bomb[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffBomb, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBomb);

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
HRESULT MakeVertexBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffBomb,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BOMB_WIDTH / 2, BOMB_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BOMB_WIDTH / 2, BOMB_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BOMB_WIDTH / 2, -BOMB_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BOMB_WIDTH / 2, -BOMB_HEIGHT / 2, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * h));
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * h));
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * (h + 1)));
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * (h + 1)));

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBomb->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBomb(int nIdxBomb, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBomb->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetBomb(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int nIdxBomb = -1;

	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (!g_Bomb[i].bUse && g_BombMgr.UseTimer < 1)
		{
			g_Bomb[i].pos = pos;
			g_Bomb[i].pos.y = BOMB_HEIGHT / 6;
			g_Bomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bomb[i].dir = dir;
			g_Bomb[i].Life = 5;
			g_Bomb[i].timer = BOMB_LIFE;
			g_Bomb[i].bUse = true;

			PlaySound(SOUND_LABEL_SE_BOMB01);

			// �e�̐ݒ�
			g_Bomb[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// �e�̐ݒ�

			nIdxBomb = i;

			break;
		}
	}
	return nIdxBomb;
}

BOMB* GetBomb()
{
	return g_Bomb;
}

BOMBMGR* GetBombMgr()
{
	return &g_BombMgr;
}

SPHERE* Bomb_GetSphere(int index)
{
	return (&g_Bomb[index].col_Sphere);
}

void Bomb_Destroy(int index)
{
	g_Bomb[index].bUse = false;
	ReleaseShadow(g_Bomb[index].idxShadow);
}

void Bomb_LifeDown(int index)
{
	g_Bomb[index].Life--;
}

bool Bomb_IsEnable(int index)
{
	return g_Bomb[index].bUse;
}
