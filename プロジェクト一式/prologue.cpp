#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "Score.h"
#include "sound.h"
#include "debugproc.h"

static int g_PrologueTimer;
static int g_ProNum;

void Prologue_Initialize(void)
{
	g_PrologueTimer = 0;
	g_ProNum = 0;
	PlaySound(SOUND_LABEL_PROLOGUEBGM);
}

void Prologue_Finalize(void)
{
	StopSound(SOUND_LABEL_PROLOGUEBGM);
}

void Prologue_Update(void)
{
	g_PrologueTimer++;

	if (g_PrologueTimer % 120 == 0)
	{
		g_ProNum++;
		if (g_ProNum > 8)
		{
			g_ProNum = 8;
		}
	}

	//�X�y�[�X�L�[�������ꂽ��V�[���J�ڂ���
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || g_PrologueTimer > 1200)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_TUTORIAL);
		}
	}
}

void Prologue_Draw(void)
{
	if (g_PrologueTimer < 120)
	{
		Sprite_Draw(TEXTURE_INDEX_BACKGROUND_PROLOGUE, 0, 0, 0);
	}
	if (g_PrologueTimer > 120)
	{
		Sprite_Draw(TEXTURE_INDEX_BACKGROUND_PROLOGUE, 0, 0, 4);
	}

	Sprite_Draw(TEXTURE_INDEX_PROLOGUE,									//�e�N�X�`���Ǘ��ԍ�
		500,															//�`����Wx
		160,															//�`����Wy
		0, 0,															//�e�N�X�`���؂�����Wx,y
		1000, (320 / 8) * g_ProNum,										//�e�N�X�`���؂��蕝x,y
		1000 / 2,														//���S���Wx
		320 / 2,														//���S���Wy
		1.0f,															//�g�嗦x����
		1.0f,															//�g�嗦y����
		0.0f,															//��]�p�x(���W�A��)
		0);																//�`�悷��F

	//PrintDebugProc("%d\n", g_PrologueTimer);
}
