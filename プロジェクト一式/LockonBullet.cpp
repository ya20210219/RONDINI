//=============================================================================
//
// ���b�N�I���e���� [Lockonbullet.cpp]
// Author :	Y.Ariyoshi
//
//=============================================================================

#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "player.h"
#include "main.h"
#include "collision.h"
#include "enemy.h"
#include "radian.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/LockonBullet.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	LOCKBULLET_WIDTH		(10.0f)							// �e�̕�
#define	LOCKBULLET_HEIGHT		(10.0f)							// �e�̍���
#define	LOCKBULLET_SPEED		(8.0f)							// �ړ����x

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLockonBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexLockonBullet(int nIdxLockonBullet, float fWidth, float fHeight);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureLockonBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLockonBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static LOCKONBULLET				g_LockonBullet[MAX_LOCKON];		// �e���[�N
static float					g_T_rot;
static bool						g_LockFire;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitLockonBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexLockonBullet(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BULLET,			// �t�@�C���̖��O
		&g_pTextureLockonBullet);		// �ǂݍ��ރ������[

	// �o���b�g�z��̏�����
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockonBullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_LockonBullet[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].timer = 200;
		g_LockonBullet[i].bUse = false;
		g_LockonBullet[i].LockonUse = false;
	}

	g_LockFire = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLockonBullet(void)
{
	if (g_pTextureLockonBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureLockonBullet->Release();
		g_pTextureLockonBullet = NULL;
	}

	if (g_pVtxBuffLockonBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffLockonBullet->Release();
		g_pVtxBuffLockonBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLockonBullet(void)
{
	LOCKON* pLock = GetLockOn();
	PLAYER* pPlayer = GetPlayer();

	if (LockOnRange_IsEnable() && Keyboard_IsRelease(DIK_LSHIFT))
	{
		g_LockFire = true;
	}

	if (Keyboard_IsTrigger(DIK_LSHIFT))
	{
		g_LockFire = false;
	}

	//�e�̔z���S�����ׂ�
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//�L�����t���O��ON�̕��̂ݏ������s��
		if (g_LockonBullet[i].bUse /*&& (pLock + i)->bUse*/ && GetLockonRange()->LockonFreeFlg == false)
		{
			g_LockonBullet[i].TargetRotY = (atan2f(((pLock + i)->posL.z - g_LockonBullet[i].pos.z), ((pLock + i)->posL.x - g_LockonBullet[i].pos.x)) - (D3DX_PI / 2));

			g_T_rot = (float)LerpRadian(g_LockonBullet[i].rot.y, g_LockonBullet[i].TargetRotY, 0.75f);

			g_LockonBullet[i].rot.y = g_T_rot;
				 
			g_LockonBullet[i].dir.x = sinf(D3DX_PI * 0.0f - g_LockonBullet[i].rot.y) * LOCKBULLET_SPEED;
			g_LockonBullet[i].dir.z = cosf(D3DX_PI * 0.0f - g_LockonBullet[i].rot.y) * LOCKBULLET_SPEED;

			g_LockonBullet[i].col_Sphere.r = LOCKBULLET_WIDTH;
				
			// �ړ�
			g_LockonBullet[i].pos.x += g_LockonBullet[i].dir.x;
			g_LockonBullet[i].pos.z += g_LockonBullet[i].dir.z;

			g_LockonBullet[i].col_Sphere.pos.x = g_LockonBullet[i].pos.x;
			g_LockonBullet[i].col_Sphere.pos.y = g_LockonBullet[i].pos.y;
			g_LockonBullet[i].col_Sphere.pos.z = g_LockonBullet[i].pos.z;

			// �����t���[���̃^�C�}�[�����炷
			g_LockonBullet[i].timer--;

			// �����t���[����0�ȉ��ɂȂ������������
			if (g_LockonBullet[i].timer <= 0)
			{
				// �e�̔j��
				ReleaseShadow(g_LockonBullet[i].idxShadow);

				g_LockonBullet[i].bUse = false;
			}
			else
			{
				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_LockonBullet[i].idxShadow, D3DXVECTOR3(g_LockonBullet[i].pos.x, 0.1f, g_LockonBullet[i].pos.z));
			}
		}
	}

	//�e�̔z���S�����ׂ�
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (g_LockonBullet[i].bUse && GetLockonRange()->LockonFreeFlg == true)
		{
			//g_LockonBullet[i].rot.y = pPlayer->rot.y;

			if (g_LockonBullet[i].bUse)
			{
				//�ړ�
				g_LockonBullet[i].pos.x += g_LockonBullet[i].dir.x;
				g_LockonBullet[i].pos.z += g_LockonBullet[i].dir.z;

				g_LockonBullet[i].col_Sphere.pos.x = g_LockonBullet[i].pos.x;
				g_LockonBullet[i].col_Sphere.pos.y = g_LockonBullet[i].pos.y;
				g_LockonBullet[i].col_Sphere.pos.z = g_LockonBullet[i].pos.z;

				//�����t���[���̃^�C�}�[�����炷
				g_LockonBullet[i].timer--;

				//�����t���[����0�ȉ��ɂȂ������������
				if (g_LockonBullet[i].timer <= 0)
				{
					//�e�̔j��
					ReleaseShadow(g_LockonBullet[i].idxShadow);

					g_LockonBullet[i].bUse = false;
				}
				else
				{
					//�e�̈ʒu�ݒ�
					SetPositionShadow(g_LockonBullet[i].idxShadow, D3DXVECTOR3(g_LockonBullet[i].pos.x, 0.1f, g_LockonBullet[i].pos.z));
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLockonBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (g_LockonBullet[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_LockonBullet[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_LockonBullet[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_LockonBullet[i].mtxWorld._41 = 0.0f;
			g_LockonBullet[i].mtxWorld._42 = 0.0f;
			g_LockonBullet[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_LockonBullet[i].mtxWorld._11 = mtxView._11;
			g_LockonBullet[i].mtxWorld._12 = mtxView._21;
			g_LockonBullet[i].mtxWorld._13 = mtxView._31;
			g_LockonBullet[i].mtxWorld._21 = mtxView._12;
			g_LockonBullet[i].mtxWorld._22 = mtxView._22;
			g_LockonBullet[i].mtxWorld._23 = mtxView._32;
			g_LockonBullet[i].mtxWorld._31 = mtxView._13;
			g_LockonBullet[i].mtxWorld._32 = mtxView._23;
			g_LockonBullet[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_LockonBullet[i].scale.x,
				g_LockonBullet[i].scale.y,
				g_LockonBullet[i].scale.z);
			D3DXMatrixMultiply(&g_LockonBullet[i].mtxWorld, &g_LockonBullet[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_LockonBullet[i].pos.x,
				g_LockonBullet[i].pos.y,
				g_LockonBullet[i].pos.z);
			D3DXMatrixMultiply(&g_LockonBullet[i].mtxWorld, &g_LockonBullet[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_LockonBullet[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffLockonBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureLockonBullet);

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
HRESULT MakeVertexLockonBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffLockonBullet,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLockonBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-LOCKBULLET_WIDTH / 2, LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LOCKBULLET_WIDTH / 2, LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-LOCKBULLET_WIDTH / 2, -LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(LOCKBULLET_WIDTH / 2, -LOCKBULLET_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffLockonBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexLockonBullet(int nIdxLockonBullet, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLockonBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffLockonBullet->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetLockonBullet(D3DXVECTOR3 pos, float y)
{
	int nIdxLockonBullet = -1;

	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (!g_LockonBullet[i].bUse && GetLockonRange()->LockonCoolDownFlg == false)
		{
			g_LockonBullet[i].pos = pos;
			g_LockonBullet[i].pos.y = LOCKBULLET_HEIGHT / 2;
			g_LockonBullet[i].rot = D3DXVECTOR3(0.0f, y, 0.0f);
			g_LockonBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_LockonBullet[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (GetLockonRange()->LockonFreeFlg == true)
			{
				g_LockonBullet[i].dir.x = -sinf(g_LockonBullet[i].rot.y - 0.1f + (0.1f * i)) * LOCKBULLET_SPEED;
				g_LockonBullet[i].dir.y = 0.0f;
				g_LockonBullet[i].dir.z = -cosf(g_LockonBullet[i].rot.y - 0.1f + (0.1f * i)) * LOCKBULLET_SPEED;
			}
			g_LockonBullet[i].timer = 200;
			g_LockonBullet[i].bUse = true;

			// �e�̐ݒ�
			g_LockonBullet[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// �e�̐ݒ�

			nIdxLockonBullet = i;

			PlaySound(SOUND_LABEL_SE_LOCKONBULLET);

			break;
		}
	}

	return nIdxLockonBullet;
}

LOCKONBULLET* GetLockonBullet()
{
	return g_LockonBullet;
}

SPHERE* LockonBullet_GetSphere(int index)
{
	return (&g_LockonBullet[index].col_Sphere);
}

void LockonBullet_Destroy(int index)
{
	LOCKON* pLock = GetLockOn();

	g_LockonBullet[index].bUse = false;
	g_LockonBullet[index].LockonUse = false;

	(pLock + index)->bUse = false;

	ReleaseShadow(g_LockonBullet[index].idxShadow);
}

bool LockonBullet_IsEnable(int index)
{
	return g_LockonBullet[index].bUse;
}
