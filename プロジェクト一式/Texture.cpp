/*==============================================================================

   �e�N�X�`���Ǘ����W���[�� [texture.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3dx9.h>
#include "debugproc.h"
#include "MyDirect3d.h"
#include "texture.h"


/*------------------------------------------------------------------------------
   �\���̐錾
------------------------------------------------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C�����ő啶�����i�p�X�ANULL�����܂ށj

// �e�N�X�`���t�@�C���Ǘ��\����
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX]; // �e�N�X�`���t�@�C����
	int width;  // �e�N�X�`���𑜓x����
	int height; // �e�N�X�`���𑜓x�c��
} TextureFile;


/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/
// �ǂݍ��݃e�N�X�`�����
static const TextureFile g_TextureFiles[] = {
	{ "data/TEXTURE/Rondini.Title001_Ver01.png",			1280,  720 },
	{ "data/TEXTURE/tutorial.png",		1280,  720 },
	{ "data/TEXTURE/result.png",		1280,  720 },
	{ "data/TEXTURE/field000.jpg",		 160,  160 },
	{ "data/TEXTURE/bullet.png",		  80,   80 },
	{ "data/TEXTURE/shadow000.jpg",		  80,   80 },
	{ "data/TEXTURE/backfire.png",		 960,  384 },
	{ "data/TEXTURE/HPbar.png",			  10,   10 },
	{ "data/TEXTURE/Radar_main.png",	 250,  250 },
	{ "data/TEXTURE/teppan02.png",		 160,  160 },
	{ "data/TEXTURE/rondini_text.png",	 820,  650 },
	{ "data/TEXTURE/rondini_status_text.png",	 500,  300 },
	{ "data/TEXTURE/Rondini_Skill_Text.png",	 820,  650 },
	{ "data/TEXTURE/Rondini_MissionName.png",	 700,  200 },
	{ "data/TEXTURE/Tutorial_tex1.png",	 780,  1200 },
	{ "data/TEXTURE/NextWord.png",	 600,  100 },
	{ "data/TEXTURE/Number.png",	 256,  100 },
	{ "data/TEXTURE/StageText.png",	 820,  870 },
	{ "data/TEXTURE/commuText.png",	 820,  870 },
	{ "data/TEXTURE/MissionClearText.png",	 500,  50 },
	{ "data/TEXTURE/ending1.png",		1280,  720 },
	{ "data/TEXTURE/epilogue.png",		850,  200 },
	{ "data/TEXTURE/credit.png",		1800,  2500 },
	{ "data/TEXTURE/prologue.png",		1000,  320 },
	{ "data/TEXTURE/Prologue_Back.png",		1280,  720 },
	{ "data/TEXTURE/AD.png",		330,  180 },
	{ "data/TEXTURE/Enter.png",		320,  50 },
	{ "data/TEXTURE/BossHP.png",	 700,  50 },
	{ "data/TEXTURE/PlayerGauge.png",	 800,  200 },
	{ "data/TEXTURE/ScoreText.png",		 150,  50 },
};

// �ǂݍ��݃e�N�X�`����
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);
// static const int TEXTURE_FILE_COUNT = ARRAYSIZE(g_TextureFiles); // required Windows.h

// �ǂݍ��݃e�N�X�`�����ƃe�N�X�`���Ǘ��ԍ��񋓐��ɍ����������ꍇ�R���p�C���G���[�Ƃ���
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");


/*------------------------------------------------------------------------------
   �O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {}; // �e�N�X�`���C���^�[�t�F�[�X�Ǘ��z��


/*------------------------------------------------------------------------------
   �֐���`
------------------------------------------------------------------------------*/

// �e�N�X�`���̓ǂݍ���
int Texture_Load(void)
{   
    LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	if( !pDevice ) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( FAILED(D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTextures[i])) ) {
            // DebugPrintf("�e�N�X�`���̓ǂݍ��݂Ɏ��s ... %s\n", g_TextureFiles[i].filename);
			failed_count++;
		}
	}

	return failed_count;
}

// �e�N�X�`���̉��
void Texture_Release(void)
{
	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( g_pTextures[i] ) {
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

// �e�N�X�`���C���^�[�t�F�[�X�̎擾
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_pTextures[index];
}

// �e�N�X�`���𑜓x���̎擾
int Texture_GetWidth(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].width;
}

// �e�N�X�`���𑜓x�����̎擾
int Texture_GetHeight(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].height;
}
