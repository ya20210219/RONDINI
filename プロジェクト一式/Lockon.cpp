//=============================================================================
//
// ���b�N�I������ [Lockon.cpp]
// Author :
//
//=============================================================================
#include "Lockon.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "UI.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_LOCKON		"data/TEXTURE/TargetMarker.png"	// �ǂݍ��ރe�N�X�`���t�@�C����	
#define	TEXTURE_LOCKON		"data/TEXTURE/LockUI.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	LockOn_WIDTH		(30.0f)							// ���b�N�I���̕�
#define	LockOn_HEIGHT		(30.0f)							// ���b�N�I���̍���
#define LOCKONRANGE_R		(300.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLockOn(LPDIRECT3DDEVICE9 pDevice);
void SetVertexLockOn(int nIdxLockOn, float fWidth, float fHeight);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureLockOn = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLockOn = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static LOCKON					g_LockOn[MAX_LOCKON];		// ���b�N�I�����[�N

static LOCKONRANGE				g_LockOnRange;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitLockOn(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// ���_���̍쐬
	MakeVertexLockOn(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_LOCKON,			// �t�@�C���̖��O
		&g_pTextureLockOn);		// �ǂݍ��ރ������[

	// ���b�N�I���z��̏�����
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockOn[i].posL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockOn[i].rotL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockOn[i].scaleL = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_LockOn[i].TargetNum = 0;
		g_LockOn[i].bUse = false;
	}

	g_LockOnRange.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOnRange.rotR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOnRange.scaleR = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_LockOnRange.col_Sphere.pos = g_LockOnRange.posR;
	g_LockOnRange.col_Sphere.r = LOCKONRANGE_R;
	g_LockOnRange.bUse = false;

	g_LockOnRange.LockonCoolDownCnt = 200;
	g_LockOnRange.LockonCoolDownFlg = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLockOn(void)
{
	if (g_pTextureLockOn != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureLockOn->Release();
		g_pTextureLockOn = NULL;
	}

	if (g_pVtxBuffLockOn != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffLockOn->Release();
		g_pVtxBuffLockOn = NULL;
	}

	g_LockOnRange.bUse = false;
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockOn[i].bUse = false;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLockOn(void)
{
	PLAYER* pPlayer = GetPlayer();
	ENEMY* pEnemy = GetAllEnemy();
	   
	g_LockOnRange.UnLockonCnt = 0;

	//���b�N�I���̔z���S�����ׂ�
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//�L�����t���O��ON�̕��̂ݏ������s��
		if (g_LockOn[i].bUse)
		{
			g_LockOnRange.LockonFreeFlg = false;
			g_LockOn[i].posL = (pEnemy + g_LockOn[i].TargetNum)->pos;
		
			if ((pEnemy + g_LockOn[i].TargetNum)->bUse == false)
			{
				g_LockOn[i].bUse = false;
			}
		}
		if (!g_LockOn[i].bUse)
		{
			g_LockOnRange.UnLockonCnt++;
		}
	}

	if (g_LockOnRange.UnLockonCnt == MAX_LOCKON)
	{
		g_LockOnRange.LockonFreeFlg = true;
	}

	if (Keyboard_IsRelease(DIK_LSHIFT) && g_LockOnRange.LockonCoolDownCnt == 200)
	{
		g_LockOnRange.LockonCoolDownFlg = true;
		g_LockOnRange.LockonCoolDownCnt = 0;
	}

	if (g_LockOnRange.LockonCoolDownFlg == true)
	{
		g_LockOnRange.LockonCoolDownCnt++;
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (!(pEnemy + i)->bUse || !(pEnemy + i)->LockEnable)
			{
				continue;
			}

			(pEnemy + i)->LockEnable = false;
		}

		if (g_LockOnRange.LockonCoolDownCnt > 200)
		{
			g_LockOnRange.LockonCoolDownCnt = 200;
			g_LockOnRange.LockonCoolDownFlg = false;
		}
	}

	g_LockOnRange.posR.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * LOCKONRANGE_R;
	g_LockOnRange.posR.y = pPlayer->pos.y;
	g_LockOnRange.posR.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * LOCKONRANGE_R;

	g_LockOnRange.col_Sphere.pos = g_LockOnRange.posR;
	g_LockOnRange.col_Sphere.r = LOCKONRANGE_R;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLockOn(void)
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
		if (g_LockOn[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_LockOn[i].mtxWorld);

			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// �t�s������߂�
			D3DXMatrixInverse(&g_LockOn[i].mtxWorld, NULL, &mtxView);

			// ���s�ړ��̕����͖�������
			g_LockOn[i].mtxWorld._41 = 0.0f;
			g_LockOn[i].mtxWorld._42 = 0.0f;
			g_LockOn[i].mtxWorld._43 = 0.0f;
#else
			//�]�u�s��
			g_LockOn[i].mtxWorld._11 = mtxView._11;
			g_LockOn[i].mtxWorld._12 = mtxView._21;
			g_LockOn[i].mtxWorld._13 = mtxView._31;
			g_LockOn[i].mtxWorld._21 = mtxView._12;
			g_LockOn[i].mtxWorld._22 = mtxView._22;
			g_LockOn[i].mtxWorld._23 = mtxView._32;
			g_LockOn[i].mtxWorld._31 = mtxView._13;
			g_LockOn[i].mtxWorld._32 = mtxView._23;
			g_LockOn[i].mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_LockOn[i].scaleL.x,
				g_LockOn[i].scaleL.y,
				g_LockOn[i].scaleL.z);
			D3DXMatrixMultiply(&g_LockOn[i].mtxWorld, &g_LockOn[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_LockOn[i].posL.x,
				g_LockOn[i].posL.y,
				g_LockOn[i].posL.z);
			D3DXMatrixMultiply(&g_LockOn[i].mtxWorld, &g_LockOn[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_LockOn[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffLockOn, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureLockOn);	

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
HRESULT MakeVertexLockOn(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,			// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffLockOn,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLockOn->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-LockOn_WIDTH / 2, LockOn_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LockOn_WIDTH / 2, LockOn_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-LockOn_WIDTH / 2, -LockOn_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(LockOn_WIDTH / 2, -LockOn_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffLockOn->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexLockOn(int nIdxLockOn, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffLockOn->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffLockOn->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void SetLockOn(int index, D3DXVECTOR3 pos, bool enable)
{
	g_LockOn[index].posL = pos;
	g_LockOn[index].rotL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOn[index].scaleL = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_LockOn[index].bUse = enable;
}

LOCKON* GetLockOn()
{
	return g_LockOn;
}

LOCKONRANGE* GetLockonRange()
{
	return &g_LockOnRange;
}

void LockOn_Destroy(int index)
{
	g_LockOn[index].bUse = false;
}

bool LockOn_IsEnable(int index)
{
	return g_LockOn[index].bUse;
}

bool LockOnRange_IsEnable()
{
	return g_LockOnRange.bUse;
}

SPHERE* LockOnRange_GetSphere()
{
	return (&g_LockOnRange.col_Sphere);
}