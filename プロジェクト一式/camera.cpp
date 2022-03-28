//=============================================================================
//
// �J�������� [camera.cpp]
// Author :
//
//=============================================================================
#include "camera.h"
#include "MyDirect3D.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X				(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y				(100.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z				(-200.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X				(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y				(60.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z				(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE				(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z				(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(100000.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA		(2.0f)											// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA		(D3DX_PI * 0.01f)								// �J�����̉�]��
#define	VALUE_ADJUST_DISTANCE	(2.0f)											// ���_�ƒ����_�̋���������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA g_camera;		// �J�������

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, 50.0f, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fVecX,fVecZ;
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	/*if(Keyboard_IsPress(DIK_Z))
	{// ���_����u���v
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_C))
	{// ���_����u�E�v
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_Y))
	{// ���_�ړ��u��v
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if(Keyboard_IsPress(DIK_N))
	{// ���_�ړ��u���v
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}


	if(Keyboard_IsPress(DIK_Q))
	{// �����_����u���v
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_E))
	{// �����_����u�E�v
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	*/

	if(Keyboard_IsPress(DIK_T))
	{// �����_�ړ��u��v
		g_camera.posR.y += VALUE_MOVE_CAMERA;
	}
	if(Keyboard_IsPress(DIK_B))
	{// �����_�ړ��u���v
		g_camera.posR.y -= VALUE_MOVE_CAMERA;
	}

	if(Keyboard_IsPress(DIK_U))
	{// ���_�ƒ����_�̋����k�߂�
		g_camera.fDistance -= VALUE_ADJUST_DISTANCE;
		if(g_camera.fDistance < 100.0f)
		{
			g_camera.fDistance = 100.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_M))
	{// ���_�ƒ����_�̋����L�΂�
		g_camera.fDistance += VALUE_ADJUST_DISTANCE;
		if(g_camera.fDistance > 300.0f)
		{
			g_camera.fDistance = 300.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	/*if(Keyboard_IsPress(DIK_RETURN))
	{// ���Z�b�g
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fVecX,fVecZ;
		fVecX = g_camera.posV.x - g_camera.posR.x;
		fVecZ = g_camera.posV.z - g_camera.posR.z;
		g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
	}*/

	/*
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", g_camera.rot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** �����_���� ***\n");
	PrintDebugProc("��ړ� : T\n");
	PrintDebugProc("���ړ� : B\n");
	PrintDebugProc("\n");

	PrintDebugProc("���_�ƒ����_�̋������� : U / M\n");
	PrintDebugProc("��������             : C\n");

	PrintDebugProc("�J�����̎��_�E�����_���Z�b�g �F ENTER\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** �v���C���[���� ***\n");
	PrintDebugProc("�X�s�[�h�A�b�v : W\n");
	PrintDebugProc("����           : S\n");
	PrintDebugProc("��             : A\n");
	PrintDebugProc("�E             : D\n");
	PrintDebugProc("�e����         : P\n");
	PrintDebugProc("�W�����v       : SPACE\n");
	PrintDebugProc("\n");
	*/
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice(); 

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,			// �r���[���ʂ̎���p
								VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// �J�����̎��_
						&g_camera.posR,		// �J�����̒����_
						&g_camera.vecU);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}

