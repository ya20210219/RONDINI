//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "meshfield.h"
#include "MyDirect3D.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_FILENAME	"data/TEXTURE/field001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FILENAME	"data/TEXTURE/teppan02.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9 g_pTextureField;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField;		// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9 g_pIdxBuffField;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

static D3DXMATRIX g_mtxWorldField;					// ���[���h�}�g���b�N�X
static D3DXVECTOR3 g_posField;						// �|���S���\���ʒu�̒��S���W
static D3DXVECTOR3 g_rotField;						// �|���S���̉�]�p

static DWORD g_nNumVertex;
static DWORD g_nNumVertexIndex;
static DWORD g_nNumPolygon;

static DWORD g_NumBlockX;
static DWORD g_NumBlockZ;
static float g_BlockSizeX;
static float g_BlockSizeZ;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int numBlockX, int numBlockZ, float sizeBlockX, float sizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	HRESULT hr;

	// �ʒu�E�����̏����ݒ�
	g_posField = pos;
	g_rotField = rot;

	g_NumBlockX = numBlockX;
	g_NumBlockZ = numBlockZ;
	g_BlockSizeX = sizeBlockX;
	g_BlockSizeZ = sizeBlockZ;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,	// �t�@�C���̖��O
								&g_pTextureField);	// �ǂݍ��ރ������[


	// ���_���̍쐬
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(g_pVtxBuffField)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}

	if(g_pIdxBuffField)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}

	if(g_pTextureField)
	{// �e�N�X�`���̊J��
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureField);

	// �|���S���̕`��
	// Type            : �v���~�e�B�u�̎��
	// BaseVertexIndex : �ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
	// MinIndex        : �ŏ��̒��_�C���f�b�N�X
	// NumVertices     : ���_�̐�
	// StartIndex      : �ǂݎ����J�n����ʒu
	// PrimitiveCount  : �����_�����O����v���~�e�B�u�̐�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertex, 0, g_nNumPolygon);

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//���i�l�p�j�̐�
	int block_x = g_NumBlockX;
	int block_y = g_NumBlockZ;

	//���_��
	g_nNumVertex = (block_x + 1) * (block_y + 1);
	//�C���f�b�N�X��
	g_nNumVertexIndex = (block_x + 1) * 2 * block_y + 2 * (block_y - 1);
	//�|���S����
	g_nNumPolygon = block_x * 2 * block_y + 4 * (block_y - 1);

	float size_x = g_BlockSizeX;
	float size_z = g_BlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pVtxBuffField,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndex,	// �C���f�b�N�X�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// �C���f�b�N�X�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,				// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pIdxBuffField,			// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		const float fTexSizeX = 1.0f;
		const float fTexSizeZ = 1.0f;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		{
			for (int y = 0; y < (block_y + 1); y++)
			{
				for (int x = 0; x < (block_x + 1); x++)
				{
					// ���_���W�̐ݒ�
					pVtx[y*(block_x + 1) + x].pos.x = (-size_x / 2) * block_x + size_x * x;
					pVtx[y*(block_x + 1) + x].pos.y = 0.0f;
					pVtx[y*(block_x + 1) + x].pos.z = ( size_z / 2) * block_y - size_z * y;

					// �@���̐ݒ�
					pVtx[y*(block_x + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

					// ���_�J���[�̐ݒ�
					pVtx[y*(block_x + 1) + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[y*(block_x + 1) + x].tex.x = (float)x;
					pVtx[y*(block_x + 1) + x].tex.y = (float)y;
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		int index = 0;

		//(block_y+1)�ɂ���Ɖ�肷���Ă��܂�
		for (int y = 0; y < block_y; y++)
		{
			//1�s���̏���
			for (int x = 0; x < (block_x + 1); x++)
			{
				pIdx[index] = ((block_x + 1) + x) + ((block_x + 1)*y);
				index++;
				pIdx[index] = (0 + x) + ((block_x + 1)*y);
				index++;
			}

			//�Ō�̍s�͂��̏������s��Ȃ�
			if (y < (block_y-1))
			{
				//�k�ރ|���S�����̃C���f�b�N�X�̐ݒ�
				pIdx[index] = block_x + ((block_x + 1)*y);
				index++;
				pIdx[index] = ((block_x + 1) * 2) + ((block_x + 1)*y);
				index++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pIdxBuffField->Unlock();
	}

	return S_OK;
}
