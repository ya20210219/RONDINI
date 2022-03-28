//=============================================================================
//
// �G�̒e���� [BossBomb.cpp]
// Author : 
//
//=============================================================================
#include "BossBomb.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "radian.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ENEMY_TEXTURE_BULLET_BULLET		"data/TEXTURE/BossBomb2.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ENEMY_BOSS_BULLET_WIDTH			(75.0f)							// �e�̕�
#define	ENEMY_BOSS_BULLET_HEIGHT		(75.0f)							// �e�̍���
#define	ENEMY_VALUE_MOVE_BOSS_BULLET	(3.0f)							// �ړ����x

#define ENEMY_BOSS_BULLET_LIFE			(500)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBossBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h);
void SetVertexBossBomb(int nIdxBossBomb, float fWidth, float fHeight);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBossBomb = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBossBomb = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static ENEMYBOMB				g_BossBomb[MAX_BOSS_BOMB];		// �e���[�N
static float					g_TargetY = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexBossBomb(pDevice, 0, 0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		ENEMY_TEXTURE_BULLET_BULLET,			// �t�@�C���̖��O
		&g_pTextureBossBomb);		// �ǂݍ��ރ������[

// �o���b�g�z��̏�����
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		g_BossBomb[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BossBomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BossBomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BossBomb[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BossBomb[i].timer = 0;
		g_BossBomb[i].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossBomb(void)
{
	if (g_pTextureBossBomb != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBossBomb->Release();
		g_pTextureBossBomb = NULL;
	}

	if (g_pVtxBuffBossBomb != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBossBomb->Release();
		g_pVtxBuffBossBomb = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossBomb(void)
{
	PLAYER* pPlayer = GetPlayer();

	//�e�̔z���S�����ׂ�
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		//�L�����t���O��ON�̕��̂ݏ������s��
		if (g_BossBomb[i].bUse)
		{
			g_BossBomb[i].countW++;
			if (g_BossBomb[i].countW > 5)
			{
				g_BossBomb[i].countH++;
				if (g_BossBomb[i].countH > 2)
				{
					g_BossBomb[i].countH = 0;
				}
				g_BossBomb[i].countW = 0;
			}
			LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
			MakeVertexBossBomb(pDevice, g_BossBomb[i].countW, g_BossBomb[i].countH);


			if (g_BossBomb[i].rot.y < 0)
			{
				g_BossBomb[i].rot.y += D3DX_PI * 2.0f;
			}

			if (g_BossBomb[i].rot.y > D3DX_PI * 2)
			{
				g_BossBomb[i].rot.y -= D3DX_PI * 2.0f;
			}

			g_BossBomb[i].PlayerY = (-atan2f((pPlayer->pos.z - g_BossBomb[i].pos.z), (pPlayer->pos.x - g_BossBomb[i].pos.x)) - (D3DX_PI / 2));

			if (g_BossBomb[i].PlayerY < 0)
			{
				g_BossBomb[i].PlayerY += D3DX_PI * 2.0f;
			}

			if (g_BossBomb[i].PlayerY > D3DX_PI * 2.0f)
			{
				g_BossBomb[i].PlayerY -= D3DX_PI * 2.0f;
			}

			g_TargetY = (float)LerpRadian(g_BossBomb[i].rot.y, g_BossBomb[i].PlayerY, 0.01f);
			
			g_BossBomb[i].rot.y = g_TargetY;

			g_BossBomb[i].dir.x = sinf(D3DX_PI * 0.0f - g_BossBomb[i].rot.y) * ENEMY_VALUE_MOVE_BOSS_BULLET;
			g_BossBomb[i].dir.z = cosf(D3DX_PI * 0.0f - g_BossBomb[i].rot.y) * ENEMY_VALUE_MOVE_BOSS_BULLET;

			// �ړ�
			g_BossBomb[i].pos.x += g_BossBomb[i].dir.x;
			g_BossBomb[i].pos.z -= g_BossBomb[i].dir.z;

			g_BossBomb[i].col_Sphere.pos.x = g_BossBomb[i].pos.x;
			g_BossBomb[i].col_Sphere.pos.y = g_BossBomb[i].pos.y;
			g_BossBomb[i].col_Sphere.pos.z = g_BossBomb[i].pos.z;

			g_BossBomb[i].col_Sphere.r = 60.0f;

			// �����t���[���̃^�C�}�[�����炷
			g_BossBomb[i].timer--;

			// �����t���[����0�ȉ��ɂȂ������������
			if (g_BossBomb[i].timer <= 0)
			{
				// �e�̔j��
				ReleaseShadow(g_BossBomb[i].idxShadow);

				g_BossBomb[i].bUse = false;
			}
			else
			{
				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_BossBomb[i].idxShadow, D3DXVECTOR3(g_BossBomb[i].pos.x, 0.1f, g_BossBomb[i].pos.z));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		if (g_BossBomb[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_BossBomb[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_BossBomb[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_BossBomb[i].mtxWorld._41 = 0.0f;
			g_BossBomb[i].mtxWorld._42 = 0.0f;
			g_BossBomb[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_BossBomb[i].mtxWorld._11 = mtxView._11;
			g_BossBomb[i].mtxWorld._12 = mtxView._21;
			g_BossBomb[i].mtxWorld._13 = mtxView._31;
			g_BossBomb[i].mtxWorld._21 = mtxView._12;
			g_BossBomb[i].mtxWorld._22 = mtxView._22;
			g_BossBomb[i].mtxWorld._23 = mtxView._32;
			g_BossBomb[i].mtxWorld._31 = mtxView._13;
			g_BossBomb[i].mtxWorld._32 = mtxView._23;
			g_BossBomb[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_BossBomb[i].scale.x,
				g_BossBomb[i].scale.y,
				g_BossBomb[i].scale.z);
			D3DXMatrixMultiply(&g_BossBomb[i].mtxWorld, &g_BossBomb[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_BossBomb[i].pos.x,
				g_BossBomb[i].pos.y,
				g_BossBomb[i].pos.z);
			D3DXMatrixMultiply(&g_BossBomb[i].mtxWorld, &g_BossBomb[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_BossBomb[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffBossBomb, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBossBomb);

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
HRESULT MakeVertexBossBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffBossBomb,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBossBomb->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-ENEMY_BOSS_BULLET_WIDTH / 2, ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMY_BOSS_BULLET_WIDTH / 2, ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMY_BOSS_BULLET_WIDTH / 2, -ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMY_BOSS_BULLET_WIDTH / 2, -ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffBossBomb->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBossBomb(int nIdxBossBomb, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBossBomb->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBossBomb->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetBossBomb(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nIdxBossBomb = -1;

	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		if (!g_BossBomb[i].bUse)
		{
			g_BossBomb[i].pos = pos;
			g_BossBomb[i].pos.y = ENEMY_BOSS_BULLET_HEIGHT / 2;
			g_BossBomb[i].rot = rot;
			g_BossBomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_BossBomb[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BossBomb[i].timer = ENEMY_BOSS_BULLET_LIFE;
			g_BossBomb[i].bUse = true;

			PlaySound(SOUND_LABEL_SE_BOMB01);

			nIdxBossBomb = i;

			break;
		}
	}

	return nIdxBossBomb;
}

ENEMYBOMB* GetBossBomb()
{
	return g_BossBomb;
}

SPHERE* BossBomb_GetSphere(int index)
{
	return (&g_BossBomb[index].col_Sphere);
}

void BossBomb_Destroy(int index)
{
	g_BossBomb[index].timer = 0;
	g_BossBomb[index].bUse = false;
	ReleaseShadow(g_BossBomb[index].idxShadow);
}

bool BossBomb_IsEnable(int index)
{
	return g_BossBomb[index].bUse;
}
