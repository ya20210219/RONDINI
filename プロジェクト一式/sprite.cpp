/*==============================================================================

   スプライト描画モジュール [sprite.cpp]
														 Author : Youhei Sato
														 Date   : 2018/06/17
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3dx9.h>
#include <math.h>
#include "MyDirect3d.h"
#include "texture.h"


/*------------------------------------------------------------------------------
   構造体宣言
------------------------------------------------------------------------------*/
// ２Dポリゴン頂点構造体
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position; // 頂点座標（座標変換済み頂点）
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
} Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ２Dポリゴン頂点フォーマット


/*------------------------------------------------------------------------------
   グローバル変数宣言
------------------------------------------------------------------------------*/
static D3DCOLOR g_Color = 0xffffffff;
// static D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);



/*------------------------------------------------------------------------------
   関数定義
------------------------------------------------------------------------------*/

// スプライトポリゴンの頂点カラー設定
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

// スプライト描画
// ※テクスチャ解像度と同じ大きさのスプライトを指定座標に描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int colortype)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	if (!pDevice) return;

	// テクスチャの幅、高さ取得
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	//基本
	if (colortype == 0)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	//赤
	if (colortype == 1)
	{
		g_Color = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//黄色
	if (colortype == 2)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 0, 255);
	}
	//緑
	if (colortype == 3)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}

	//薄暗い
	if (colortype == 4)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 120);
	}

	//青
	if (colortype == 5)
	{
		g_Color = D3DCOLOR_RGBA(0, 0, 255, 255);
	}

	//水色
	if (colortype == 6)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 255, 255);
	}

	// 頂点データ
	Vertex2D vertexes[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f) },
	};

	pDevice->SetFVF(FVF_VERTEX2D);                             // デバイスに頂点の構成要素を設定
	pDevice->SetTexture(0, Texture_GetTexture(texture_index)); // デバイスにテクスチャを設定

	 // プリミティブの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(Vertex2D));
}


// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, int colortype)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// UV座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	//基本
	if (colortype == 0)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	//赤
	if (colortype == 1)
	{
		g_Color = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//黄色
	if (colortype == 2)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 0, 255);
	}
	//緑
	if (colortype == 3)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}
	//薄暗い
	if (colortype == 4)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 120);
	}

	//青
	if (colortype == 5)
	{
		g_Color = D3DCOLOR_RGBA(0, 0, 255, 255);
	}

	//水色
	if (colortype == 6)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 255, 255);
	}

	Vertex2D vertexes[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[0], v[0]) },
		{ D3DXVECTOR4(dx + tw - 0.5f, dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[1], v[0]) },
		{ D3DXVECTOR4(dx - 0.5f, dy + th - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[0], v[1]) },
		{ D3DXVECTOR4(dx + tw - 0.5f, dy + th - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[1], v[1]) },
	};

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(Vertex2D));
}

// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に配置してスプライトの指定座標を中心に拡大、回転描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation, int colortype)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	// 座標変換
	float px[4], py[4];
	px[0] = -cx * sx * cosf(rotation) - -cy * sy * sinf(rotation);
	py[0] = -cx * sx * sinf(rotation) + -cy * sy * cosf(rotation);
	px[1] = (-cx + tw) * sx * cosf(rotation) - -cy * sy * sinf(rotation);
	py[1] = (-cx + tw) * sx * sinf(rotation) + -cy * sy * cosf(rotation);
	px[2] = -cx * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[2] = -cx * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);
	px[3] = (-cx + tw) * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[3] = (-cx + tw) * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);

	//float baseAngle = D3DX_PI / 4; // D3DXToRadian(45)
	//float size = 100.0f;
	//px[0] = size * cosf(rotation - baseAngle * 3);
	//py[0] = size * sinf(rotation - baseAngle * 3);
	//px[1] = size * cosf(rotation - baseAngle);
	//py[1] = size * sinf(rotation - baseAngle);
	//px[2] = size * cosf(rotation + baseAngle * 3);
	//py[2] = size * sinf(rotation + baseAngle * 3);
	//px[3] = size * cosf(rotation + baseAngle);
	//py[3] = size * sinf(rotation + baseAngle);

	//基本
	if (colortype == 0)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	//赤
	if (colortype == 1)
	{
		g_Color = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//黄色
	if (colortype == 2)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 0, 255);
	}
	//緑
	if (colortype == 3)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}
	//薄暗い
	if (colortype == 4)
	{
		g_Color = D3DCOLOR_RGBA(255, 255, 255, 120);
	}

	//青
	if (colortype == 5)
	{
		g_Color = D3DCOLOR_RGBA(0, 0, 255, 255);
	}

	//水色
	if (colortype == 6)
	{
		g_Color = D3DCOLOR_RGBA(0, 255, 255, 255);
	}

	Vertex2D vertexes[] = {
		{ D3DXVECTOR4(px[0] + dx - 0.5f, py[0] + dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[0], v[0]) },
		{ D3DXVECTOR4(px[1] + dx - 0.5f, py[1] + dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[1], v[0]) },
		{ D3DXVECTOR4(px[2] + dx - 0.5f, py[2] + dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[0], v[1]) },
		{ D3DXVECTOR4(px[3] + dx - 0.5f, py[3] + dy - 0.5f, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u[1], v[1]) },
	};

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(Vertex2D));
}