// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�n���h���Ǘ��v���O����
// 
// 				Ver 3.24e
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxHandle.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxASyncLoad.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxLog.h"

#ifndef DX_NON_ASYNCLOAD
#include "DxGraphics.h"
#include "DxSoftImage.h"
#include "DxSound.h"
#include "DxMask.h"
#include "DxNetwork.h"
#include "DxModel.h"
#include "DxFile.h"
#include "DxFont.h"
#endif // DX_NON_ASYNCLOAD

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

HANDLEMANAGE HandleManageArray[ DX_HANDLETYPE_MAX ] ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �v���O���� --------------------------------------------------------------------


// �n���h�����ʊ֌W

// �n���h���Ǘ���������������
// ( InitializeFlag �ɂ� FALSE �������Ă���K�v������ )
extern int InitializeHandleManage(
	int HandleType,
	int OneSize,
	int MaxNum,
	int ( *InitializeFunction )( HANDLEINFO *HandleInfo ),
	int ( *TerminateFunction )( HANDLEINFO *HandleInfo ),
	int ( *DumpInfoFunction )( HANDLEINFO *HandleInfo ),
	const wchar_t *Name
)
{
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// ���ɏ���������Ă����牽�����Ȃ�
	if( HandleManage->InitializeFlag )
		return -1 ;

	// NextID�ȊO�[��������
	int NextID = HandleManage->NextID ;
	_MEMSET( HandleManage, 0, sizeof( *HandleManage ) ) ;
	HandleManage->NextID = NextID ;

	// �p�����[�^�Z�b�g
	HandleManage->HandleTypeMask = HandleType << DX_HANDLETYPE_ADDRESS ;
	HandleManage->OneSize = OneSize ;
	HandleManage->MaxNum = MaxNum ;
	HandleManage->InitializeFunction = InitializeFunction ;
	HandleManage->TerminateFunction = TerminateFunction ;
	HandleManage->DumpInfoFunction = DumpInfoFunction ;
	HandleManage->Name = Name ;
	ConvString( ( const char * )HandleManage->Name, -1, WCHAR_T_CHARCODEFORMAT, HandleManage->NameUTF16LE, sizeof( HandleManage->NameUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;

	// �n���h���̃f�[�^�|�C���^�A�h���X���i�[���郁�����A�h���X�z��̊m��
	HandleManage->Handle = ( HANDLEINFO ** )DXCALLOC( sizeof( HANDLEINFO * ) * MaxNum ) ;
	if( HandleManage->Handle == NULL )
		return -1 ;

	// �n���h�����X�g�̏�����
	InitializeHandleList( &HandleManage->ListFirst, &HandleManage->ListLast ) ;

	// �폜���N�G�X�g�̃n���h�����X�g�̏�����
	InitializeHandleList( &HandleManage->DeleteRequestListFirst, &HandleManage->DeleteRequestListLast ) ;

	// �N���e�B�J���Z�N�V�����̏�����
	CriticalSection_Initialize( &HandleManage->CriticalSection ) ;

	// �������t���O�𗧂Ă�
	HandleManage->InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// �n���h���Ǘ����̌�n�����s��
extern int TerminateHandleManage( int HandleType )
{
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// ���Ɍ�n������Ă����牽�����Ȃ�
	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// ���ׂẴn���h�����폜
	AllHandleSub( HandleType ) ;

	// �n���h���|�C���^�i�[�p�̃������A�h���X�z������
	if( HandleManage->Handle != NULL )
	{
		DXFREE( HandleManage->Handle ) ;
		HandleManage->Handle = NULL ;
	}

	// �N���e�B�J���Z�N�V�����̍폜
	CriticalSection_Delete( &HandleManage->CriticalSection ) ;

	// NextID�ȊO�[��������
	int NextID = HandleManage->NextID ;
	_MEMSET( HandleManage, 0, sizeof( *HandleManage ) ) ;
	HandleManage->NextID = NextID ;

	// �������t���O��|��
	HandleManage->InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}

// �n���h����ǉ�����
extern int AddHandle( int HandleType, int ASyncThread, int Handle )
{
	int NextIndex ;
	int NewHandle ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �ǉ��ł��Ȃ��ꍇ�͏I��
	if( HandleManage->Num == HandleManage->MaxNum )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		DXST_LOGFILEFMT_ADDUTF16LE(( "\x25\x00\x73\x00\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x70\x65\x4c\x30\x50\x96\x4c\x75\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x66\x30\xb0\x65\x5f\x30\x6a\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x92\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"%s�n���h���̐������E��( %d )�ɒB���Ă��ĐV���ȃn���h�����쐬�ł��܂���" @*/, HandleManage->NameUTF16LE, HandleManage->MaxNum )) ;
		return -1 ;
	}

	// �󂫔z��ԍ��̌���
	if( Handle != -1 && HandleManage->Handle[ Handle & DX_HANDLEINDEX_MASK ] == NULL )
	{
		NextIndex = Handle & DX_HANDLEINDEX_MASK ;
		ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
	}
	else
	{
		if( HandleManage->Num == 0 )
		{
			NextIndex = 0 ;
			ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
		}
		else
		{
			if( HandleManage->AreaMax + 1 < HandleManage->MaxNum )
			{
				NextIndex = HandleManage->AreaMax + 1 ;
				ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
			}
			else
			if( HandleManage->AreaMin - 1 > 0 )
			{
				NextIndex = HandleManage->AreaMin - 1 ;
				ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
			}
			else
			{
				ppHandleInfo = HandleManage->Handle ;
				for( NextIndex = 0 ; *ppHandleInfo != NULL ; NextIndex ++, ppHandleInfo ++ ){}
			}
		}
	}

	// �f�[�^�̈���m�ۂ���
	*ppHandleInfo = ( HANDLEINFO * )DXCALLOC( ( size_t )HandleManage->OneSize ) ;
	if( *ppHandleInfo == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		DXST_LOGFILEFMT_ADDUTF16LE(( "\x25\x00\x73\x00\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"%s�n���h���̃f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���" @*/, HandleManage->NameUTF16LE )) ;
		return -1 ;
	}

	// �G���[�`�F�b�N�p�h�c�̐ݒ�
	if( Handle != -1 )
	{
		(*ppHandleInfo)->ID = ( Handle & DX_HANDLECHECK_MASK ) >> DX_HANDLECHECK_ADDRESS ;
	}
	else
	{
		HandleManage->NextID ++ ;
		if( HandleManage->NextID >= ( DX_HANDLECHECK_MASK >> DX_HANDLECHECK_ADDRESS ) )
			HandleManage->NextID = 0 ;

		(*ppHandleInfo)->ID = HandleManage->NextID ;
	}

	// �������̊m�ۃT�C�Y��ۑ�
	( *ppHandleInfo )->AllocSize = HandleManage->OneSize ;

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��݂�����������n���h�����폜����t���O��������
	( *ppHandleInfo )->ASyncLoadFinishDeleteRequestFlag = FALSE ;

	// �񓯊��ǂݍ��݂�����������Ă΂��R�[���o�b�N�֐��̏���������
	( *ppHandleInfo )->ASyncLoadFinishCallback = NULL ;
	( *ppHandleInfo )->ASyncLoadFinishCallbackData = NULL ;

	// �񓯊��X���b�h����Ă΂ꂽ�ꍇ�͔񓯊��ǂݍ��݃J�E���g���P�ɂ���
	if( ASyncThread )
	{
		( *ppHandleInfo )->ASyncLoadCount  = 1 ;
		( *ppHandleInfo )->ASyncDataNumber = -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h���l��ۑ�
	NewHandle = (*ppHandleInfo)->Handle = NextIndex | HandleManage->HandleTypeMask | ( (*ppHandleInfo)->ID << DX_HANDLECHECK_ADDRESS ) ;

	// �n���h���̐��𑝂₷
	HandleManage->Num ++ ;

	// �g�p����Ă���n���h�������݂���͈͂��X�V����
	if( HandleManage->Num == 1 )
	{
		HandleManage->AreaMax = NextIndex ;
		HandleManage->AreaMin = NextIndex ;
	}
	else
	{
		if( HandleManage->AreaMax < NextIndex ) HandleManage->AreaMax = NextIndex ;
		if( HandleManage->AreaMin > NextIndex ) HandleManage->AreaMin = NextIndex ;
	}

	// ���X�g�֗v�f��ǉ�
	AddHandleList( &HandleManage->ListFirst, &(*ppHandleInfo)->List, (*ppHandleInfo)->Handle, *ppHandleInfo ) ;

	// �������֐����Ă�
	if( HandleManage->InitializeFunction )
	{
		HandleManage->InitializeFunction( *ppHandleInfo ) ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �n���h����Ԃ�
	return NewHandle ;
}

// SubHandle �̎������֐�
static int SubHandleBase( HANDLEMANAGE *HandleManage, HANDLEINFO *HandleInfo )
{
	int Index ;
	int HandleType = ( int )( ( ( DWORD )HandleInfo->Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	Index = HandleInfo->Handle & DX_HANDLEINDEX_MASK ;

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��ݒ��ł���ꍇ�ł܂������������Ă��Ȃ������珈�����L�����Z������
	if( HandleInfo->ASyncLoadCount != 0 )
	{
		if( HandleInfo->ASyncDataNumber < 0 ||
			DeleteASyncLoadData( HandleInfo->ASyncDataNumber, TRUE ) < -1 )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

			// �폜�Ɏ��s������ǂݍ��ݏ������I���܂ő҂�
			while( HandleInfo->ASyncLoadCount != 0 )
			{
				ProcessASyncLoadRequestMainThread() ;
				Thread_Sleep( 0 );
			}

			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;
		}
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h���^�C�v�X�̌�n������
	if( HandleManage->TerminateFunction )
	{
		// �߂�l���P�̏ꍇ�͍폜�L�����Z��
		if( HandleManage->TerminateFunction( HandleInfo ) == 1 )
			goto END ;
	}

	// �폜�t���O��-1��������
	if( HandleInfo->DeleteFlag != NULL )
	{
		*HandleInfo->DeleteFlag = -1 ;
	}

	// ���X�g����v�f���O��
	SubHandleList( &HandleInfo->List ) ;

	// �폜���N�G�X�g����Ă�����폜���N�G�X�g���X�g����O��
	if( HandleInfo->DeleteRequestFlag )
	{
		SubHandleList( &HandleInfo->DeleteRequestList ) ;

		// �폜���N�G�X�g����Ă������� 1 ���炷
		HandleManage->DeleteRequestHandleNum -- ;
	}

	// �f�[�^�̈���������
	DXFREE( HandleInfo ) ;

	// �e�[�u���� NULL ���Z�b�g����
	HandleManage->Handle[ Index ] = NULL ;

	// �n���h���̑��������炷
	HandleManage->Num -- ;

	// �L���ȃn���h�������݂���͈͂̍X�V
	if( HandleManage->Num == 0 )
	{
		HandleManage->AreaMax = 0 ;
		HandleManage->AreaMin = 0 ;
	}
	else
	{
		if( Index == HandleManage->AreaMax )
		{
			while( HandleManage->Handle[ HandleManage->AreaMax ] == NULL )
				HandleManage->AreaMax -- ; 
		}
		else
		if( Index == HandleManage->AreaMin )
		{
			while( HandleManage->Handle[ HandleManage->AreaMin ] == NULL )
				HandleManage->AreaMin ++ ; 
		}
	}

END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}

// �n���h�����폜����
extern int SubHandle( int Handle, int ASyncLoadFlag, int ASyncThread )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// ���ɍ폜���N�G�X�g���X�g�ɓo�^����Ă���ꍇ�̓G���[
	if( HandleInfo->DeleteRequestFlag )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread || ASyncLoadFlag )
	{
		// �񓯊����s��]��ʃX���b�h����̌Ăяo���̏ꍇ�͍폜���N�G�X�g���X�g�ɒǉ�
		AddHandleList( &HandleManage->DeleteRequestListFirst, &HandleInfo->DeleteRequestList, HandleInfo->Handle, HandleInfo ) ;

		// �폜���N�G�X�g���X�g�ɓo�^����Ă���t���O�𗧂Ă�
		HandleInfo->DeleteRequestFlag = TRUE ;

		// �폜���N�G�X�g�̃n���h�������C���N�������g
		HandleManage->DeleteRequestHandleNum ++ ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		// �񓯊����s��]��ʃX���b�h����̌Ăяo���ł͂Ȃ��ꍇ�͑����ɍ폜
		if( SubHandleBase( HandleManage, HandleInfo ) < 0 )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

			return -1 ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}

// �n���h���̏����i�[���郁�����̈�̃T�C�Y��ύX����A�񓯊��ǂݍ��ݒ��łȂ����Ƃ��O��
extern int ReallocHandle( int Handle, size_t NewSize )
{
	HANDLEINFO *HandleInfo ;
	int Index ;
	void *NewBuffer ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return -1 ;

	Index = Handle & DX_HANDLEINDEX_MASK ;

	// �m�ۃ������̃T�C�Y���ω����Ȃ��ꍇ�͉��������I��
	if( ( size_t )HandleInfo->AllocSize == NewSize )
		return 0 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �������̍Ċm��
	NewBuffer = ( HANDLEINFO * )DXREALLOC( HandleInfo, ( size_t )NewSize ) ;

	// �������̍Ċm�ې����`�F�b�N
	if( NewBuffer == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �m�ۃ������A�h���X���ω������ꍇ�̓��X�g�̂Ȃ�����
	if( NewBuffer != ( void * )HandleInfo )
	{
		// �V�����������A�h���X���Z�b�g
		HandleInfo = HandleManage->Handle[ Index ] = ( HANDLEINFO * )NewBuffer ;

		// ���X�g�̌q������
		NewMemoryHandleList( &HandleInfo->List, HandleInfo ) ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}

// �n���h���̏����擾����
extern HANDLEINFO *GetHandleInfo( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return NULL ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return NULL ;

	// �n���h���̏���Ԃ�
	return HandleInfo ;
}

// �n���h���Ǘ����ɓo�^����Ă��邷�ׂẴn���h�����폜
extern int AllHandleSub( int HandleType, int (*DeleteCancelCheckFunction)( HANDLEINFO *HandleInfo ) )
{
	int i ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

//	// �N���e�B�J���Z�N�V�����̎擾
//	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;
//
//#ifndef DX_NON_ASYNCLOAD
//	// �񓯊��ǂݍ��݈˗�����������폜���Ă���
//	ppHandleInfo = &HandleManage->Handle[ HandleManage->AreaMin ] ;
//	for( i = HandleManage->AreaMin ; i <= HandleManage->AreaMax ; i ++, ppHandleInfo ++ )
//	{
//		if( *ppHandleInfo )
//		{
//			if( DeleteCancelCheckFunction == NULL ||
//				DeleteCancelCheckFunction( *ppHandleInfo ) == FALSE )
//			{
//				if( (*ppHandleInfo)->ASyncLoadCount != 0 )
//				{
//					if( DeleteASyncLoadData( (*ppHandleInfo)->ASyncDataNumber, TRUE ) >= -1 )
//					{
//						(*ppHandleInfo)->ASyncLoadCount = 0 ;
//					}
//				}
//			}
//		}
//	}
//#endif // DX_NON_ASYNCLOAD
//
//	// �N���e�B�J���Z�N�V�����̉��
//	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// ���ׂẴn���h���f�[�^���폜����
	{
		int AreaMin ;
		int AreaMax ;
		int Handle ;
		int DeleteFlag ;
		int LoopFlag ;

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

		do
		{
			LoopFlag = FALSE ;

			AreaMin = HandleManage->AreaMin ;
			AreaMax = HandleManage->AreaMax ;

			ppHandleInfo = &HandleManage->Handle[ AreaMin ] ;
			for( i = AreaMin ; i <= AreaMax ; i ++, ppHandleInfo ++ )
			{
				if( *ppHandleInfo == NULL )
				{
					continue ;
				}

				if( DeleteCancelCheckFunction != NULL )
				{
					DeleteFlag = DeleteCancelCheckFunction( *ppHandleInfo ) == FALSE ? TRUE : FALSE ;
				}
				else
				{
					DeleteFlag = TRUE ;
				}

				if( DeleteFlag )
				{
					Handle = (*ppHandleInfo)->Handle ;

					// �N���e�B�J���Z�N�V�����̉��
					CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

					SubHandle( Handle, FALSE, FALSE ) ;

					// �N���e�B�J���Z�N�V�����̎擾
					CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

					LoopFlag = TRUE ;
					break ;
				}
			}

		}while( LoopFlag == TRUE ) ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;
	}

	// �I��
	return 0 ;
}









#ifndef DX_NON_ASYNCLOAD

// �񓯊��ǂݍ��݊֌W

// �n���h���̔񓯊��ǂݍ��݂��������Ă��邩�ǂ������擾����( TRUE:�܂��������Ă��Ȃ�  FALSE:�������Ă���  -1:�G���[ )
extern int NS_CheckHandleASyncLoad( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadCount != 0 ? TRUE : FALSE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return Result ;
}

// �n���h���̔񓯊��ǂݍ��ݏ����̖߂�l���擾����( �񓯊��ǂݍ��ݒ��̏ꍇ�͈�O�̔񓯊��ǂݍ��ݏ����̖߂�l���Ԃ��Ă��܂� )
extern int NS_GetHandleASyncLoadResult( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadResult ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return Result ;
}

// �n���h���̔񓯊��ǂݍ��ݏ���������������n���h�����폜����t���O�𗧂Ă�
extern int NS_SetASyncLoadFinishDeleteFlag(	int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// ���ɔ񓯊��ǂݍ��݂��������Ă����炱�̏�Ńn���h�����폜����
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		SubHandle( Handle, FALSE, FALSE ) ;
	}
	else
	{
		HandleInfo->ASyncLoadFinishDeleteRequestFlag = TRUE ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return 0 ;
}

// �n���h���̔񓯊��ǂݍ��ݏ���������������Ă΂��֐����Z�b�g����
extern int NS_SetASyncLoadFinishCallback( int Handle, void ( *Callback )( int Handle, void *Data ), void *Data )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// ���ɔ񓯊��ǂݍ��݂��������Ă����炱�̏�ŃR�[���o�b�N�֐����Ă�
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		Callback( Handle, Data ) ;
	}
	else
	{
		HandleInfo->ASyncLoadFinishCallback = ( volatile void (*)( int, void * ) )Callback ;
		HandleInfo->ASyncLoadFinishCallbackData = Data ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return 0 ;
}

// �w��̃n���h���̔񓯊��ǂݍ��ݏ������I������܂ő҂�
extern int NS_WaitHandleASyncLoad( int Handle )
{
	return WaitASyncLoad( Handle ) ;
}

// �S�Ă̔񓯊��ǂݍ��݃f�[�^���ǂݍ��ݏI���܂ő҂�
extern int NS_WaitHandleASyncLoadAll( void )
{
	while( NS_GetASyncLoadNum() > 0 )
	{
		if( NS_ProcessMessage() < 0 )
		{
			break ;
		}
		Thread_Sleep( 1 ) ;
	}

	return 0 ;
}

// �n���h���̔񓯊��ǂݍ��ݒ��J�E���g���C���N�������g����
extern int IncASyncLoadCount( int Handle, int ASyncDataNumber )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	HandleInfo->ASyncLoadCount ++ ;
	HandleInfo->ASyncDataNumber = ASyncDataNumber ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}

// �n���h���̔񓯊��ǂݍ��ݒ��J�E���g���f�N�������g����
extern int DecASyncLoadCount( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	HandleInfo->ASyncLoadCount -- ;

	// �J�E���g�� 0 �������珈���𕪊�
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		// �ǂݍ��݂��I�������ĂԃR�[���o�b�N�֐����ݒ肳��Ă�����Ă�
		if( HandleInfo->ASyncLoadFinishCallback != NULL )
		{
			HandleInfo->ASyncLoadFinishCallback( Handle, HandleInfo->ASyncLoadFinishCallbackData ) ;
		}

		// �ǂݍ��݂��I�������폜����t���O�������Ă�����n���h�����폜����
		if( HandleInfo->ASyncLoadFinishDeleteRequestFlag )
		{
			SubHandle( Handle, FALSE, TRUE ) ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}

// �n���h���̔񓯊��ǂݍ��݊�����ɍ폜���邩�ǂ����̃t���O���擾����
extern int GetASyncLoadFinishDeleteFlag( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadFinishDeleteRequestFlag ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return Result ;
}

// �n���h�����񓯊��ǂݍ��ݒ��������ꍇ�A�񓯊��ǂݍ��݂���������܂ő҂�
extern int WaitASyncLoad( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// ���C���X���b�h�ł͂Ȃ�������G���[
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// �񓯊��ǂݍ��ݒ��ł���ꍇ�͏�������������܂ő҂�
	if( HandleInfo->ASyncLoadCount != 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		// �����I���҂�
		while( HandleInfo->ASyncLoadCount != 0 )
		{
			ProcessASyncLoadRequestMainThread() ;
			Thread_Sleep( 1 );
		}
	}
	else
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �폜���N�G�X�g�����Ă���n���h�����폜����
extern int DeleteRequestHandleDelete( int AllDelete )
{
	HANDLEINFO *HandleInfo ;
	int StartTime = NS_GetNowCount( FALSE ) ;
	int i ;
	int DeleteRequestTotal ;

	// ���C���X���b�h�ł͂Ȃ�������G���[
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}

START:

	DeleteRequestTotal = 0 ;

	for( i = 0 ; i < DX_HANDLETYPE_MAX ; i ++ )
	{
		// �n���h���Ǘ��f�[�^������������Ă��Ȃ���Ή������Ȃ�
		if( HandleManageArray[ i ].InitializeFlag == FALSE )
		{
			continue ;
		}

		// �폜���N�G�X�g��������Ή������Ȃ�
		if( HandleManageArray[ i ].DeleteRequestHandleNum == 0 )
		{
			continue ;
		}

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &HandleManageArray[ i ].CriticalSection ) ;

		// �폜���N�G�X�g��������΃��[�v���甲����
		while( HandleManageArray[ i ].DeleteRequestHandleNum > 0 )
		{
			// �񓯊��ǂݍ��݂��������Ă���n���h����T��
			HandleInfo = ( HANDLEINFO * )HandleManageArray[ i ].DeleteRequestListLast.Prev->Data ;
			while( HandleInfo != NULL && HandleInfo->ASyncLoadCount != 0 )
			{
				HandleInfo = ( HANDLEINFO * )HandleInfo->DeleteRequestList.Next->Data ;
			}

			// �񓯊��ǂݍ��݂��������Ă���n���h�������������烋�[�v�𔲂���
			if( HandleInfo == NULL )
			{
				break ;
			}

			// �n���h�����폜
			if( HandleInfo != NULL )
			{
				SubHandleBase( &HandleManageArray[ i ], HandleInfo ) ;
			}

			// 2ms�o�߂��Ă����烋�[�v�𔲂���
			if( NS_GetNowCount( FALSE ) - StartTime >= 2 )
			{
				break ;
			}
		}

		// �폜���N�G�X�g�̎c�萔�����Z
		DeleteRequestTotal += HandleManageArray[ i ].DeleteRequestHandleNum ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ i ].CriticalSection ) ;
	}

	// �S�č폜����w��ŁA�܂��폜�ł��Ă��Ȃ��n���h��������ꍇ�͊֐��̐擪�ɖ߂�
	if( AllDelete && DeleteRequestTotal > 0 )
	{
		// ���C���X���b�h����������񓯊��ǂݍ��݂̏������s��
		ProcessASyncLoadRequestMainThread() ;

		goto START ;
	}

	// ����I��
	return 0 ;
}

#endif // DX_NON_ASYNCLOAD









// �n���h�����X�g

// ���X�g�̏�����
extern int InitializeHandleList( HANDLELIST *First, HANDLELIST *Last )
{
	First->Handle = -1 ;
	First->Data   = NULL ;
	First->Prev   = NULL ;
	First->Next   = Last ;
	Last->Handle  = -1 ;
	Last->Data    = NULL ;
	Last->Prev    = First ;
	Last->Next    = NULL ;

	// �I��
	return 0 ;
}

// ���X�g�֗v�f��ǉ�
extern int AddHandleList( HANDLELIST *First, HANDLELIST *List, int Handle, void *Data )
{
	List->Handle      = Handle ;
	List->Data        = Data ;
	List->Prev        = First ;
	List->Next        = First->Next ;
	First->Next->Prev = List ;
	First->Next       = List ;

	// �I��
	return 0 ;
}

// ���X�g����v�f���O��
extern int SubHandleList( HANDLELIST *List )
{
	List->Prev->Next = List->Next ;
	List->Next->Prev = List->Prev ;
	List->Prev = NULL ;
	List->Next = NULL ;

	return 0 ;
}

// ���X�g�����݂��郁�������ύX���ꂽ�ꍇ�Ƀ��X�g�̑O����X�V����
extern int NewMemoryHandleList( HANDLELIST *List, void *Data )
{
	List->Data = Data ;

	List->Prev->Next = List ;
	List->Next->Prev = List ;

	// �I��
	return 0 ;
}









// �V���v�����X�g

// ���X�g�̏�����
extern int InitializeSimpleList( SIMPLELIST *First, SIMPLELIST *Last )
{
	First->Data = NULL ;
	First->Prev = NULL ;
	First->Next = Last ;
	Last->Data  = NULL ;
	Last->Prev  = First ;
	Last->Next  = NULL ;

	// �I��
	return 0 ;
}

// ���X�g�֗v�f��ǉ�
extern int AddSimpleList( SIMPLELIST *First, SIMPLELIST *List, void *Data )
{
	List->Data        = Data ;
	List->Prev        = First ;
	List->Next        = First->Next ;
	First->Next->Prev = List ;
	First->Next       = List ;

	// �I��
	return 0 ;
}

// ���X�g����v�f���O��
extern int SubSimpleList( SIMPLELIST *List )
{
	List->Prev->Next = List->Next ;
	List->Next->Prev = List->Prev ;

	return 0 ;
}

// ���X�g�����݂��郁�������ύX���ꂽ�ꍇ�Ƀ��X�g�̑O����X�V����
extern int NewMemorySimpleList( SIMPLELIST *List, void *Data )
{
	List->Data = Data ;

	List->Prev->Next = List ;
	List->Next->Prev = List ;

	// �I��
	return 0 ;
}







// �n���h�����폜���ꂽ�Ƃ��Ɂ|�P���ݒ肳���ϐ���o�^����
extern int NS_SetDeleteHandleFlag( int Handle, int *DeleteFlag )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;

	// �G���[����
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return -1 ;

	HandleInfo->DeleteFlag = DeleteFlag  ;

	// �I��
	return 0 ;
}

// �w��̃^�C�v�̃n���h���̐����擾����
extern int NS_GetHandleNum(	int HandleType /* DX_HANDLETYPE_GRAPH�� */ )
{
	// �^�C�v�`�F�b�N
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	// �n���h���̐���Ԃ�
	return HandleManageArray[ HandleType ].Num ;
}

// �w��̃^�C�v�̃n���h�����ő�Ŋ�쐬�ł��邩���擾����
extern int NS_GetMaxHandleNum( int HandleType /* DX_HANDLETYPE_GRAPH�� */ )
{
	// �^�C�v�`�F�b�N
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	// �n���h���̍ő吔��Ԃ�
	return HandleManageArray[ HandleType ].MaxNum ;
}

// �w��̃^�C�v�̑S�n���h���̏������O�ɏo�͂���
extern int NS_DumpHandleInfo( int HandleType /* DX_HANDLETYPE_GRAPH�� */ )
{
	int i ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// �^�C�v�`�F�b�N
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	ppHandleInfo = &HandleManage->Handle[ HandleManage->AreaMin ] ;
	for( i = HandleManage->AreaMin ; i <= HandleManage->AreaMax ; i ++, ppHandleInfo ++ )
	{
		if( *ppHandleInfo == NULL )
		{
			continue ;
		}

		if( HandleManage->DumpInfoFunction != NULL )
		{
			HandleManage->DumpInfoFunction( *ppHandleInfo ) ;
		}
	}
	DXST_LOGFILEFMT_ADDW(( L"Total Count : %d", HandleManage->Num )) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// �I��
	return 0 ;
}







#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE