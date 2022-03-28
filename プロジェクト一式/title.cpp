#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"
#include <d3d9.h>


void Title_Initialize(void)
{

}

void Title_Finalize(void)
{
}

void Title_Update(void)
{
	//スペースキーが押されたらシーン遷移する	Fade_GetState() == FADE_STATE_NONE
	if( Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE))
	{
		PlaySound(SOUND_LABEL_SE_LOCKONBULLET);
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_PROLOGUE);
		}
	}
}

void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f, 0);
	Sprite_Draw(TEXTURE_INDEX_PRESSENTER, SCREEN_WIDTH - 320, SCREEN_HEIGHT - 50, 0);
}
