//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // ���C��BGM
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_BGM001,			// �{�XBGM
	SOUND_LABEL_SE_DAMAGE,		// ������
	SOUND_LABEL_SE_BOMB01,		//�A�r���e�B�{���p
	SOUND_LABEL_SE_THUNDER,		//�A�r���e�B�T���_�[�p
	SOUND_LABEL_BGM002,			//�`���[�g���A���p
	SOUND_LABEL_SE_LOCKON,		//�A�r���e�B�T���_�[�p
	SOUND_LABEL_SE_CLEAR,		//�A�r���e�B�T���_�[�p
	SOUND_LABEL_SE_LOCKONBULLET,//�A�r���e�B�T���_�[�p
	SOUND_LABEL_SELECTBGM,		//�Z���N�g�V�[���p
	SOUND_LABEL_ENDINGBGM,		//�G���f�B���O�p
	SOUND_LABEL_PROLOGUEBGM,		//�v�����[�O�p

	SOUND_LABEL_MAX,


} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

