//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // メインBGM
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_BGM001,			// ボスBGM
	SOUND_LABEL_SE_DAMAGE,		// 爆発音
	SOUND_LABEL_SE_BOMB01,		//アビリティボム用
	SOUND_LABEL_SE_THUNDER,		//アビリティサンダー用
	SOUND_LABEL_BGM002,			//チュートリアル用
	SOUND_LABEL_SE_LOCKON,		//アビリティサンダー用
	SOUND_LABEL_SE_CLEAR,		//アビリティサンダー用
	SOUND_LABEL_SE_LOCKONBULLET,//アビリティサンダー用
	SOUND_LABEL_SELECTBGM,		//セレクトシーン用
	SOUND_LABEL_ENDINGBGM,		//エンディング用
	SOUND_LABEL_PROLOGUEBGM,		//プロローグ用

	SOUND_LABEL_MAX,


} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

