#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "Score.h"
#include "sound.h"

static int  g_EndingTime;
static int  g_EndingMoveTime;
static int  g_EpiNum;
static int  g_CreditPosY;
static bool g_EndScoreFlg;

void Ending_Initialize(void)
{
	g_EndingTime = 0;
	g_EndingMoveTime = 0;
	g_EpiNum = 0;
	g_EndingMoveTime;g_EndScoreFlg = false;
	g_EpiNum;g_CreditPosY = SCREEN_HEIGHT;
	g_CreditPosY;PlaySound(SOUND_LABEL_ENDINGBGM);

}

void Ending_Finalize(void)
{
	SetScore(0);
	StopSound(SOUND_LABEL_ENDINGBGM);
}

void Ending_Update(void)
{
	UpdateScore();

	if (g_EndingTime != -1)
	{
		g_EndingTime++;
	}
	
	if (g_EndingTime % 180 == 0)
	{
		g_EpiNum++;
		if (g_EpiNum > 5)
		{
			g_EpiNum = 5;
		}
	}
	if (g_EndingTime > 1250)
	{
		g_EndingTime = -1;
	}

	if (g_EndingTime == -1)
	{
		g_CreditPosY--;
		if (g_CreditPosY < -1250)
		{
			g_EndScoreFlg = true;
		}
		
	}

	//�X�y�[�X�L�[�������ꂽ��V�[���J�ڂ���
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || g_CreditPosY < -1800)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_TITLE);
			ResetClearCnt();
		}
	}
}

void Ending_Draw(void)
{	
	Sprite_Draw(TEXTURE_INDEX_ENDING, 0.0f, 0.0f, 0);

	if (g_EndingTime < 1200 && g_EndingTime != -1)
	{
		Sprite_Draw(TEXTURE_INDEX_EPILOGUE,									//�e�N�X�`���Ǘ��ԍ�
			SCREEN_WIDTH / 2,												//�`����Wx
			SCREEN_HEIGHT / 2,												//�`����Wy
			0, 0,															//�e�N�X�`���؂�����Wx,y
			850, (200 / 5) * g_EpiNum,										//�e�N�X�`���؂��蕝x,y
			850 / 2,														//���S���Wx
			200 / 2,														//���S���Wy
			1.0f,															//�g�嗦x����
			1.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F
	}

	if (g_EndingTime == -1)
	{
		Sprite_Draw(TEXTURE_INDEX_CREDIT,									//�e�N�X�`���Ǘ��ԍ�
			SCREEN_WIDTH / 4,												//�`����Wx
			g_CreditPosY,													//�`����Wy
			0, 0,															//�e�N�X�`���؂�����Wx,y
			1800, 2500,														//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F
	}

	if (g_EndScoreFlg == true)
	{
		DrawScore();
		Sprite_Draw(TEXTURE_INDEX_SCORE_TEXT, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2, 0);
	}
}

bool GetEndScoreFlg()
{
	return g_EndScoreFlg;
}
