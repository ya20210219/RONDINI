
#include "fade.h"
#include "main.h"
#include "MyDirect3D.h"


typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
} FadeVertex;
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


static FADE_STATE	g_FadeState;
static SCENE_INDEX	g_FadeNextScene;

static float		g_FadeAlpha;
static D3DCOLOR		g_FadeColor;
static float		g_FadeFrame = 30.0f;


void Fade_Initialize(void)
{
	g_FadeState = FADE_STATE_NONE;
	g_FadeNextScene = SCENE_NONE;
	g_FadeAlpha = 0.0f;
	g_FadeColor = D3DCOLOR_RGBA(0, 0, 0, 255);
}

void Fade_Finalize(void)
{
}

void Fade_Update(void)
{
	//フェードアウトの場合
	if (g_FadeState == FADE_STATE_OUT)
	{

		if (g_FadeAlpha >= 1.0f)
		{
			g_FadeAlpha = 1.0f;
			g_FadeState = FADE_STATE_IN;
			Scene_Change(g_FadeNextScene);
		}

		g_FadeAlpha += 1.0f / g_FadeFrame;

	}
	else if (g_FadeState == FADE_STATE_IN)
	{

		if (g_FadeAlpha <= 0.0f)
		{
			g_FadeAlpha = 0.0f;
			g_FadeState = FADE_STATE_NONE;
		}

		g_FadeAlpha -= 1.0f / g_FadeFrame;
	}
}

void Fade_Draw(void)
{
	//フェード処理中ではない場合は終了する
	if (g_FadeState == FADE_STATE_NONE)
		return;

	D3DXCOLOR color(g_FadeColor);
	color.a = g_FadeAlpha;

	FadeVertex v[] = {
		{ D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH, 0.0f, 0.0f, 1.0f), color },
		{ D3DXVECTOR4(0.0f, SCREEN_HEIGHT, 0.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f), color },
	};

	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	pDevice->SetFVF(FVF_FADE_VERTEX);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

void Fade_SceneIn(SCENE_INDEX nextScene)
{
	g_FadeAlpha = 1.0f;
	g_FadeState = FADE_STATE_IN;
	Scene_Change(nextScene);
}

void Fade_SceneTransition(SCENE_INDEX nextScene)
{
	g_FadeNextScene = nextScene;

	g_FadeState = FADE_STATE_OUT;
}

FADE_STATE Fade_GetState(void)
{
	return g_FadeState;
}
