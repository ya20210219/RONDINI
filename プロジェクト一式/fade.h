#pragma once

#include <d3d9.h>
#include "scene.h"


enum FADE_STATE
{
	FADE_STATE_NONE,	//�t�F�[�h���������ĂȂ�
	FADE_STATE_IN,		//�t�F�[�h�C��������
	FADE_STATE_OUT		//�t�F�[�h�A�E�g������
};

void Fade_Initialize(void);
void Fade_Finalize(void);
void Fade_Update(void);
void Fade_Draw(void);

void Fade_SceneIn(SCENE_INDEX nextScene);
void Fade_SceneTransition(SCENE_INDEX nextScene);
FADE_STATE Fade_GetState(void);

void Fade_SetColor(BYTE r, BYTE g, BYTE b);
