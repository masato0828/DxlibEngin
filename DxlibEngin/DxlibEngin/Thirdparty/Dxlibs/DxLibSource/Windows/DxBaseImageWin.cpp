//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�pBaseImage�v���O����
// 
//  	Ver 3.24e
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define DX_MAKE

#include "../DxCompileConfig.h"

// �C���N���[�h ---------------------------------------------------------------
#include "DxBaseImageWin.h"
#include "DxGuid.h"
#include "DxWinAPI.h"
#include "../DxFile.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "../DxBaseImage.h"
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

typedef struct tagBASEIMAGEMANAGE_WIN
{
	D_IWICImagingFactory *WICImagingFactory ;
} BASEIMAGEMANAGE_WIN ;

// �֐��錾 -------------------------------------------------------------------

static	int LoadWICProcess( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) ;						// WIC�ɂ��ǂݍ���

// �f�[�^��` -----------------------------------------------------------------

int ( *DefaultImageLoadFunc_PF[] )( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) =
{
	LoadWICProcess ,
	NULL
} ;

BASEIMAGEMANAGE_WIN BaseImageManageWin ;

// �v���O���� -----------------------------------------------------------------

// ���ˑ��������E�I���֐�

// ��{�C���[�W�Ǘ����̊��ˑ������̏�����
extern int InitializeBaseImageManage_PF( void )
{
	// IWICImagingFactory �̐���
	WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_WICIMAGINGFACTORY, NULL, CLSCTX_INPROC_SERVER, IID_IWICIMAGINGFACTORY, ( void ** )&BaseImageManageWin.WICImagingFactory ) ;

	// ���ˑ��̓ǂݍ��݊֐��͌�Ɏ��s����悤�ɂ���
	BASEIM.PlatformLoadFunctionAfterFlag = TRUE ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�Ǘ����̊��ˑ������̌�n��
extern int TerminateBaseImageManage_PF( void )
{
	// IWICImagingFactory �̉��
	if( BaseImageManageWin.WICImagingFactory != NULL )
	{
		BaseImageManageWin.WICImagingFactory->Release() ;
		BaseImageManageWin.WICImagingFactory = NULL ;
	}

	// �I��
	return 0 ;
}

// WIC�ɂ��ǂݍ���
static int LoadWICProcess( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	HRESULT hr ;
	D_IWICStream *WICStream ;
	D_IWICBitmapDecoder *WICBitmapDecoder = NULL ;
	D_IWICBitmapFrameDecode *WICBitmapFrameDecode = NULL ;
	D_IWICFormatConverter *WICFormatConverter = NULL ;
	D_WICPixelFormatGUID WICPixelFormatGUID ;
	BYTE *SrcP = NULL ;
	size_t FileBytes ;
	int UseConvert = FALSE ;

	// IWICImagingFactory ���Ȃ���Ή��������I��
	if( BaseImageManageWin.WICImagingFactory == NULL )
	{
		return -1 ;
	}

	// �������ɓǂݍ��܂ꂽ�f�[�^�ł͂Ȃ��ꍇ�͏������Ȃ�
	if( GetMemStreamDataShredStruct()->Read != Src->ReadShred.Read )
	{
		return -1 ;
	}

	// �������̓ǂݍ��܂ꂽ�f�[�^�̃A�h���X���擾
	SrcP = *( ( BYTE ** )Src->DataPoint ) ;
	FileBytes = *( ( size_t * )( ( BYTE ** )Src->DataPoint + 1 ) ) ;

	// IWICStream �̍쐬�ƃZ�b�g�A�b�v
	hr = BaseImageManageWin.WICImagingFactory->CreateStream( &WICStream ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}
	hr = WICStream->InitializeFromMemory( SrcP, ( DWORD )FileBytes ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

    // �f�R�[�_�[�̍쐬
	hr = BaseImageManageWin.WICImagingFactory->CreateDecoderFromStream( WICStream, NULL, D_WICDecodeMetadataCacheOnDemand, &WICBitmapDecoder ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// �t���[���̎擾
	hr = WICBitmapDecoder->GetFrame( 0, &WICBitmapFrameDecode );
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// �T�C�Y�̎擾
	hr = WICBitmapFrameDecode->GetSize( ( UINT * )&BaseImage->Width, ( UINT * )&BaseImage->Height ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// �s�N�Z���t�H�[�}�b�g�̎擾
	hr = WICBitmapFrameDecode->GetPixelFormat( &WICPixelFormatGUID ) ;
	if( FAILED( hr ) )
	{
		goto ERR ;
	}

	// �s�N�Z���t�H�[�}�b�g�̔���
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppBGRA, sizeof( GUID ) ) == 0 )
	{
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppRGBA, sizeof( GUID ) ) == 0 )
	{
		NS_CreateABGR8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppBGR, sizeof( GUID ) ) == 0 )
	{
		NS_CreateXRGB8ColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT32bppRGB, sizeof( GUID ) ) == 0 )
	{
		NS_CreateColorData( &BaseImage->ColorData, 32, 0x0000ff, 0x00ff00, 0xff0000, 0, 0, 0, FALSE ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT24bppBGR, sizeof( GUID ) ) == 0 )
	{
		NS_CreateFullColorData( &BaseImage->ColorData ) ;
	}
	else
	if( _MEMCMP( &WICPixelFormatGUID, &GUID_WICPIXELFORMAT24bppRGB, sizeof( GUID ) ) == 0 )
	{
		NS_CreateBGR8ColorData( &BaseImage->ColorData ) ;
	}
	else
	{
		UseConvert = TRUE ;

		// ��Ή��̃t�H�[�}�b�g�̏ꍇ�̓R���o�[�g
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
	}

	// �摜��ۑ����郁�����̈�̊m��
	BaseImage->Pitch = BaseImage->ColorData.PixelByte * BaseImage->Width ;
	BaseImage->GraphData = DXALLOC( BaseImage->Pitch * BaseImage->Height ) ;
	if( BaseImage->GraphData == NULL )
	{
		goto ERR ;
	}
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ; 

	// �R���o�[�g���邩�ǂ����ŏ����𕪊�
	if( UseConvert )
	{
		BOOL CanConvertResult = FALSE ;

		// ARGB8 �t�H�[�}�b�g�ɃR���o�[�g����
		NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;

		// �R���o�[�^�[�̍쐬
		hr = BaseImageManageWin.WICImagingFactory->CreateFormatConverter( &WICFormatConverter ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}

		// �R���o�[�g�\���`�F�b�N
		hr = WICFormatConverter->CanConvert( WICPixelFormatGUID, GUID_WICPIXELFORMAT32bppBGRA, &CanConvertResult ) ;
		if( FAILED( hr ) || CanConvertResult == FALSE )
		{
			goto ERR ;
		}

		// �R���o�[�^�[�̏�����
		hr = WICFormatConverter->Initialize( WICBitmapFrameDecode, GUID_WICPIXELFORMAT32bppBGRA, D_WICBitmapDitherTypeErrorDiffusion, NULL, 0, D_WICBitmapPaletteTypeMedianCut ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}

		// �s�N�Z�������擾
		hr = WICFormatConverter->CopyPixels( NULL, BaseImage->Pitch, BaseImage->Pitch * BaseImage->Height, ( BYTE * )BaseImage->GraphData ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}
	}
	else
	{
		// �s�N�Z�������擾
		hr = WICBitmapFrameDecode->CopyPixels( NULL, BaseImage->Pitch, BaseImage->Pitch * BaseImage->Height, ( BYTE * )BaseImage->GraphData ) ;
		if( FAILED( hr ) )
		{
			goto ERR ;
		}
	}

	if( WICFormatConverter != NULL )
	{
		WICFormatConverter->Release() ;
		WICFormatConverter = NULL ;
	}

	if( WICBitmapFrameDecode != NULL )
	{
		WICBitmapFrameDecode->Release() ;
		WICBitmapFrameDecode = NULL ;
	}

	if( WICBitmapDecoder != NULL )
	{
		WICBitmapDecoder->Release() ;
		WICBitmapDecoder = NULL ;
	}

	if( WICStream != NULL )
	{
		WICStream->Release() ;
		WICStream = NULL ;
	}

	// ����I��
	return 0 ;

ERR :

	if( BaseImage->GraphData != NULL )
	{
		DXFREE( BaseImage->GraphData ) ;
		BaseImage->GraphData = NULL ;
	}

	if( WICFormatConverter != NULL )
	{
		WICFormatConverter->Release() ;
		WICFormatConverter = NULL ;
	}

	if( WICBitmapFrameDecode != NULL )
	{
		WICBitmapFrameDecode->Release() ;
		WICBitmapFrameDecode = NULL ;
	}

	if( WICBitmapDecoder != NULL )
	{
		WICBitmapDecoder->Release() ;
		WICBitmapDecoder = NULL ;
	}

	if( WICStream != NULL )
	{
		WICStream->Release() ;
		WICStream = NULL ;
	}

	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

