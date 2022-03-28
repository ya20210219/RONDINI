#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "Score.h"

static int g_ResultTimer;

void Result_Initialize(void)
{
	g_ResultTimer = 0;
}

void Result_Finalize(void)
{
}

void Result_Update(void)
{
	UpdateScore();
	g_ResultTimer++;

	//スペースキーが押されたらシーン遷移する
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || g_ResultTimer > 360)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_TITLE);
		}
	}
}

void Result_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f, 0);
	Sprite_Draw(TEXTURE_INDEX_SCORE_TEXT, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 3, 0);
	DrawScore();
}
