/*==============================================================================

   �e�N�X�`���Ǘ����W���[�� [texture.h]
                                                         Author : Youhei Sato
                                                         Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include <d3d9.h>


/*------------------------------------------------------------------------------
   �e�N�X�`���񋓌^
------------------------------------------------------------------------------*/
// �e�N�X�`���Ǘ��ԍ�
typedef enum
{
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_TUTORIAL,
	TEXTURE_INDEX_RESULT,
	TEXTURE_INDEX_FIELD,
	TEXTURE_INDEX_BULLET,
	TEXTURE_INDEX_SHADOW,
	TEXTURE_INDEX_BACKFIRE,
	TEXTURE_INDEX_HPBAR,
	TEXTURE_INDEX_RADAR,
	TEXTURE_INDEX_TEPPAN01,
	TEXTURE_INDEX_RONDINI_TEXT,
	TEXTURE_INDEX_RONDINI_STATUS_TEXT,
	TEXTURE_INDEX_RONDINI_SKILL_TEXT,
	TEXTURE_INDEX_RONDINI_MISSION_TEXT,
	TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,
	TEXTURE_INDEX_RONDINI_NEXT_TEXT,
	TEXTURE_INDEX_NUMBER,
	TEXTURE_INDEX_RONDINI_STAGE_TEXT,
	TEXTURE_INDEX_RONDINI_COMMU_TEXT,
	TEXTURE_INDEX_CLEAR_TEXT,
	TEXTURE_INDEX_ENDING,
	TEXTURE_INDEX_EPILOGUE,
	TEXTURE_INDEX_CREDIT,
	TEXTURE_INDEX_PROLOGUE,
	TEXTURE_INDEX_BACKGROUND_PROLOGUE,
	TEXTURE_INDEX_AD,
	TEXTURE_INDEX_PRESSENTER,
	TEXTURE_INDEX_BOSSHP,
	TEXTURE_INDEX_PLAYERGAUGE,
	TEXTURE_INDEX_SCORE_TEXT,

    TEXTURE_INDEX_MAX
}TextureIndex;


/*------------------------------------------------------------------------------
   �֐��̃v���g�^�C�v�錾
------------------------------------------------------------------------------*/

// �e�N�X�`���̓ǂݍ���
//
// �߂�l:�ǂݍ��߂Ȃ�������
//
int Texture_Load(void);

// �e�N�X�`���̉��
void Texture_Release(void);

// �e�N�X�`���C���^�[�t�F�[�X�̎擾
//
// ���� ... index �e�N�X�`���Ǘ��ԍ�
//
// �߂�l:�e�N�X�`���C���^�[�t�F�[�X
//        ���������݂��Ȃ�index���w�肵���ꍇ�ANULL
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index);

// �e�N�X�`���𑜓x���̎擾
//
// ���� ... index �e�N�X�`���Ǘ��ԍ�
//
int Texture_GetWidth(TextureIndex index);

// �e�N�X�`���𑜓x�����̎擾
//
// ���� ... index �e�N�X�`���Ǘ��ԍ�
//
int Texture_GetHeight(TextureIndex index);
