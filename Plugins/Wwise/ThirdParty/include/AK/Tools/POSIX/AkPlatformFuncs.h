/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2024 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include <AK/Tools/Common/AkAssert.h>
#include <AK/SoundEngine/Common/AkTypes.h>

#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/mman.h>
#ifndef AK_QNX
#include <sys/syscall.h>
#endif
#include <stdlib.h>

#if defined(AK_SUPPORT_THREADS)
#include <pthread.h>
#endif

#define AK_POSIX_NO_ERR 0
#define AK_POSIX

//-----------------------------------------------------------------------------
// Platform-specific thread properties definition.
//-----------------------------------------------------------------------------
struct AkThreadProperties
{
    int                 nPriority;		///< Thread priority
	size_t				uStackSize;		///< Thread stack size
	int					uSchedPolicy;	///< Thread scheduling policy
	AkUInt32			dwAffinityMask;	///< Affinity mask
};

//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
// g_fFreqRatio is used by time helpers to return time values in milliseconds.
// It is declared and updated by the sound engine.
namespace AK
{
    extern AkReal32 g_fFreqRatio;
}

//-----------------------------------------------------------------------------
// Defines for POSIX (Mac, iOS, Android)
//-----------------------------------------------------------------------------
#define AK_DECLARE_THREAD_ROUTINE( FuncName )   void* FuncName(void* lpParameter)
#define AK_THREAD_RETURN( _param_ )				return (_param_);
#define AK_THREAD_ROUTINE_PARAMETER             lpParameter
#define AK_GET_THREAD_ROUTINE_PARAMETER_PTR(type) reinterpret_cast<type*>( AK_THREAD_ROUTINE_PARAMETER )
#define AK_RETURN_THREAD_OK                     0x00000000
#define AK_RETURN_THREAD_ERROR                  0x00000001

#define AK_DEFAULT_STACK_SIZE                   (128*1024)

#if defined(AK_SUPPORT_THREADS)
#define AK_THREAD_DEFAULT_SCHED_POLICY			SCHED_FIFO

#define AK_THREAD_PRIORITY_NORMAL				(((sched_get_priority_max( SCHED_FIFO ) - sched_get_priority_min( SCHED_FIFO )) / 2) + sched_get_priority_min( SCHED_FIFO ))
#define AK_THREAD_PRIORITY_ABOVE_NORMAL			sched_get_priority_max( SCHED_FIFO )
#define AK_THREAD_PRIORITY_BELOW_NORMAL			sched_get_priority_min( SCHED_FIFO )
#else
#define AK_THREAD_DEFAULT_SCHED_POLICY			1

#define AK_THREAD_PRIORITY_NORMAL				50
#define AK_THREAD_PRIORITY_ABOVE_NORMAL			99
#define AK_THREAD_PRIORITY_BELOW_NORMAL			1
#endif


#define AK_THREAD_AFFINITY_DEFAULT				0xFFFF


// NULL objects
#define AK_NULL_THREAD                          0

#define AK_INFINITE                             (AK_UINT_MAX)

#define AkMax(x1, x2)	(((x1) > (x2))? (x1): (x2))
#define AkMin(x1, x2)	(((x1) < (x2))? (x1): (x2))
#define AkClamp(x, min, max)  ((x) < (min)) ? (min) : (((x) > (max) ? (max) : (x)))

#pragma GCC visibility push(hidden)

namespace AKPLATFORM
{
#define AkExitThread( _result ) return _result;

	// Simple automatic event API
    // ------------------------------------------------------------------

#ifndef AK_APPLE	
	/// Platform Independent Helper
	inline void AkClearEvent( AkEvent & out_event )
    {
		memset(&out_event,0,sizeof(AkEvent));
	}
	
	/// Platform Independent Helper
	inline AKRESULT AkCreateEvent( AkEvent & out_event )
    {
		int ret = sem_init(	
							&out_event,
							0,
							0 );
		
		return ( ret == AK_POSIX_NO_ERR  ) ? AK_Success : AK_Fail;
	}

	/// Platform Independent Helper
	inline void AkDestroyEvent( AkEvent & io_event )
	{
		AKVERIFY( sem_destroy( &io_event ) == AK_POSIX_NO_ERR);
		AkClearEvent(io_event); 
	}

	/// Platform Independent Helper
	inline void AkWaitForEvent( AkEvent & in_event )
	{
		AKVERIFY( sem_wait( &in_event ) == AK_POSIX_NO_ERR );
	}

	/// Platform Independent Helper
	inline void AkSignalEvent( AkEvent & in_event )
	{
		AKVERIFY( sem_post( &in_event ) == AK_POSIX_NO_ERR );
	}

	/// Platform Independent Helper
	AkForceInline void AkClearSemaphore(AkSemaphore& io_semaphore)
	{
		memset(&io_semaphore, 0, sizeof(AkSemaphore));
	}

		/// Platform Independent Helper
	inline AKRESULT AkCreateSemaphore( AkSemaphore& out_semaphore, AkUInt32 in_initialCount )
	{
		int ret = sem_init(	
							&out_semaphore,
							0,
							in_initialCount );
		
		return ( ret == AK_POSIX_NO_ERR  ) ? AK_Success : AK_Fail;
	}

	/// Platform Independent Helper
	inline void AkDestroySemaphore(AkSemaphore& io_semaphore)
	{
		AKVERIFY(sem_destroy(&io_semaphore) == AK_POSIX_NO_ERR);
		memset(&io_semaphore, 0, sizeof(AkSemaphore));
	}

	/// Platform Independent Helper - Semaphore wait, aka Operation P. Decrements value of semaphore, and, if the semaphore would be less than 0, waits for the semaphore to be released.
	inline void AkWaitForSemaphore(AkSemaphore& in_semaphore)
	{
		AKVERIFY(sem_wait(&in_semaphore) == AK_POSIX_NO_ERR);
	}

	/// Platform Independent Helper - Semaphore signal, aka Operation V. Increments value of semaphore by an arbitrary count.
	inline void AkReleaseSemaphore(AkSemaphore& in_semaphore, AkUInt32 in_count)
	{
		for (int i=0; i < in_count; i++)
		{
			AKVERIFY(sem_post(&in_semaphore) == AK_POSIX_NO_ERR);
		}
	}
#endif	

#if defined(AK_SUPPORT_THREADS)
    // Threads
    // ------------------------------------------------------------------

	/// Platform Independent Helper
	inline bool AkIsValidThread( AkThread * in_pThread )
	{
		return (*in_pThread != AK_NULL_THREAD);
	}

	/// Platform Independent Helper
	inline void AkClearThread( AkThread * in_pThread )
	{
		*in_pThread = AK_NULL_THREAD;
	}

	/// Platform Independent Helper
    inline void AkCloseThread( AkThread * in_pThread )
    {
        AKASSERT( in_pThread );
        AKASSERT( *in_pThread );
		
        AkClearThread( in_pThread );
    }

	/// Platform Independent Helper
	inline void AkGetDefaultThreadProperties( AkThreadProperties & out_threadProperties )
	{
		out_threadProperties.uStackSize		= AK_DEFAULT_STACK_SIZE;
		out_threadProperties.uSchedPolicy	= AK_THREAD_DEFAULT_SCHED_POLICY;
		out_threadProperties.nPriority		= AK_THREAD_PRIORITY_NORMAL;
		out_threadProperties.dwAffinityMask = AK_THREAD_AFFINITY_DEFAULT;	
	}

#if !defined(AK_ANDROID) && !defined(AK_LINUX_AOSP) && !defined(AK_HARMONY)
	/// Platform Independent Helper
	inline void AkCreateThread( 
		AkThreadRoutine pStartRoutine,					// Thread routine.
		void * pParams,									// Routine params.
		const AkThreadProperties & in_threadProperties,	// Properties. NULL for default.
		AkThread * out_pThread,							// Returned thread handle.
		const char * /*in_szThreadName*/ )				// Opt thread name.
    {
		AKASSERT( out_pThread != NULL );
		
		pthread_attr_t  attr;
		
		// Create the attr
		AKVERIFY(!pthread_attr_init(&attr));
		// Set the stack size
#ifndef AK_EMSCRIPTEN 
		// Apparently Emscripten doesn't like that we try to specify stack sizes.
		AKVERIFY(!pthread_attr_setstacksize(&attr,in_threadProperties.uStackSize));
#endif //AK_EMSCRIPTEN
		
		AKVERIFY(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));
		
		// Try to set the thread policy
		int sched_policy = in_threadProperties.uSchedPolicy;
		if( pthread_attr_setschedpolicy( &attr, sched_policy )  )
		{
			AKASSERT( !"AKCreateThread invalid sched policy, will automatically set it to FIFO scheduling" );
			sched_policy = AK_THREAD_DEFAULT_SCHED_POLICY;
			AKVERIFY( !pthread_attr_setschedpolicy( &attr, sched_policy ));
		}

		// Get the priority for the policy
		int minPriority, maxPriority;
		minPriority = sched_get_priority_min(sched_policy);
		maxPriority = sched_get_priority_max(sched_policy);
		
		// Set the thread priority if valid
		AKASSERT( in_threadProperties.nPriority >= minPriority && in_threadProperties.nPriority <= maxPriority );
		if(  in_threadProperties.nPriority >= minPriority && in_threadProperties.nPriority <= maxPriority )
		{
			sched_param schedParam;
			AKVERIFY( !pthread_attr_getschedparam(&attr, &schedParam));
			schedParam.sched_priority = in_threadProperties.nPriority;
			AKVERIFY( !pthread_attr_setschedparam( &attr, &schedParam ));
		}
#ifdef AK_APPLE
		int inherit;
		pthread_attr_getinheritsched(&attr, &inherit );
		pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED );
#endif
		// Create the tread
		int     threadError = pthread_create( out_pThread, &attr, pStartRoutine, pParams);
		AKASSERT( threadError == 0 );
		AKVERIFY(!pthread_attr_destroy(&attr));
		
		if( threadError != 0 )
		{
			AkClearThread( out_pThread );
			return;
		}

		// ::CreateThread() return NULL if it fails.
        if ( !*out_pThread )
        {
			AkClearThread( out_pThread );
            return;
        }		
    }
#endif

	/// Platform Independent Helper
    inline void AkWaitForSingleThread( AkThread * in_pThread )
    {
        AKASSERT( in_pThread );
        AKASSERT( *in_pThread );
		AKVERIFY(!pthread_join( *in_pThread, NULL ));
    }

	/// Returns the calling thread's ID.
	inline AkThreadID CurrentThread()
	{
		return pthread_self();
	}

#else // AK_SUPPORT_THREADS

	inline bool AkIsValidThread( AkThread * in_pThread )
	{
		return false;
	}

	inline void AkClearThread( AkThread * in_pThread )
	{
	}

	inline void AkCloseThread( AkThread * in_pThread )
	{
	}

	/// Platform Independent Helper
	inline void AkGetDefaultThreadProperties( AkThreadProperties & out_threadProperties )
	{
		out_threadProperties.uStackSize		= AK_DEFAULT_STACK_SIZE;
		out_threadProperties.uSchedPolicy	= AK_THREAD_DEFAULT_SCHED_POLICY;
		out_threadProperties.nPriority		= AK_THREAD_PRIORITY_NORMAL;
		out_threadProperties.dwAffinityMask = AK_THREAD_AFFINITY_DEFAULT;	
	}

	inline void AkCreateThread( 
		AkThreadRoutine pStartRoutine,					// Thread routine.
		void * pParams,									// Routine params.
		const AkThreadProperties & in_threadProperties,	// Properties. NULL for default.
		AkThread * out_pThread,							// Returned thread handle.
		const char * /*in_szThreadName*/ )				// Opt thread name.
	{
	}

	/// Platform Independent Helper
	inline void AkWaitForSingleThread( AkThread * in_pThread )
	{
	}

	/// Returns the calling thread's ID.
	inline AkThreadID CurrentThread()
	{
		return 1;
	}

#endif // AK_SUPPORT_THREADS

	/// Platform Independent Helper
    inline void AkSleep( AkUInt32 in_ulMilliseconds )
    {
		// usleep in micro second
		usleep( in_ulMilliseconds * 1000 );
    }

	/// Platform Independent Helper
    inline void UpdatePerformanceFrequency()
	{
        AkInt64 iFreq;
        PerformanceFrequency( &iFreq );
        AK::g_fFreqRatio = (AkReal32)((AkReal64)iFreq / 1000);
	}

	/// Returns a time range in milliseconds, using the sound engine's updated count->milliseconds ratio.
    inline AkReal32 Elapsed( const AkInt64 & in_iNow, const AkInt64 & in_iStart )
    {
        return ( in_iNow - in_iStart ) / AK::g_fFreqRatio;
    }

	/// String conversion helper
	inline AkInt32 AkWideCharToChar( const wchar_t*	in_pszUnicodeString,
									 AkUInt32	in_uiOutBufferSize,
									 char*	io_pszAnsiString )
	{
		AKASSERT( io_pszAnsiString != NULL );
		io_pszAnsiString[0] = 0;

		AkInt32 ConvRet = (AkInt32) wcstombs(io_pszAnsiString,  in_pszUnicodeString, in_uiOutBufferSize);

		return ConvRet;
	}

#ifdef AK_SUPPORT_WCHAR	
	/// String conversion helper
	inline AkInt32 AkCharToWideChar( const char*	in_pszAnsiString,
									 AkUInt32		in_uiOutBufferSize,
									 wchar_t*			io_pvUnicodeStringBuffer )
	{
		AKASSERT( io_pvUnicodeStringBuffer != NULL );
		io_pvUnicodeStringBuffer[0] = 0;

		AkInt32 ConvRet = (AkInt32) mbstowcs((wchar_t *)io_pvUnicodeStringBuffer, in_pszAnsiString, in_uiOutBufferSize );

		return ConvRet;
	}

	inline AkInt32 AkUtf8ToWideChar( const char*	in_pszUtf8String,
									 AkUInt32		in_uiOutBufferSize,
									 void*			io_pvUnicodeStringBuffer )
	{
		return AkCharToWideChar( in_pszUtf8String, in_uiOutBufferSize, (wchar_t*)io_pvUnicodeStringBuffer );
	}
#endif
	/// Safe unicode string copy.
	inline void SafeStrCpy( wchar_t * in_pDest, const wchar_t* in_pSrc, size_t in_uDestMaxNumChars )
	{
		size_t iSizeCopy = AkMin( in_uDestMaxNumChars - 1, wcslen( in_pSrc ) + 1 );
		wcsncpy( in_pDest, in_pSrc, iSizeCopy );
		in_pDest[iSizeCopy] = '\0';
	}

	/// Safe ansi string copy.
	inline void SafeStrCpy( char * in_pDest, const char* in_pSrc, size_t in_uDestMaxNumChars )
	{
		size_t iSizeCopy = AkMin( in_uDestMaxNumChars - 1, strlen( in_pSrc ) + 1 );
		strncpy( in_pDest, in_pSrc, iSizeCopy );
		in_pDest[iSizeCopy] = '\0';
	}

	/// Safe unicode string concatenation.
	inline void SafeStrCat( wchar_t * in_pDest, const wchar_t* in_pSrc, size_t in_uDestMaxNumChars )
	{
		size_t iAvailableSize = in_uDestMaxNumChars - wcslen( in_pDest ) - 1;
		wcsncat( in_pDest, in_pSrc, AkMin( iAvailableSize, wcslen( in_pSrc ) ) );
	}

	/// Safe ansi string concatenation.
	inline void SafeStrCat( char * in_pDest, const char* in_pSrc, size_t in_uDestMaxNumChars )
	{
		size_t iAvailableSize = in_uDestMaxNumChars - strlen( in_pDest ) - 1;
		strncat( in_pDest, in_pSrc, AkMin( iAvailableSize, strlen( in_pSrc ) ) );
	}

	inline int SafeStrFormat(wchar_t * in_pDest, size_t in_uDestMaxNumChars, const wchar_t* in_pszFmt, ...)
	{
		va_list args;
		va_start(args, in_pszFmt);
		int r = vswprintf(in_pDest, in_uDestMaxNumChars, in_pszFmt, args);
		va_end(args);
		return r;
	}

	inline int SafeStrFormat(char * in_pDest, size_t in_uDestMaxNumChars, const char* in_pszFmt, ...)
	{
		va_list args;
		va_start(args, in_pszFmt);
		int r = vsnprintf(in_pDest, in_uDestMaxNumChars, in_pszFmt, args);
		va_end(args);
		return r;
	}
	
	/// Get the length, in characters, of a NULL-terminated AkUtf16 string
	/// \return The length, in characters, of the specified string (excluding terminating NULL)
	inline size_t AkUtf16StrLen( const AkUtf16* in_pStr )
	{
		size_t len = 0;
		while( *in_pStr != 0 )
		{
			in_pStr++;
			len++;
		}
		return len;
	}

#if !defined(AK_ANDROID) && !defined(AK_LINUX_AOSP) && !defined(AK_HARMONY)
	#ifndef AK_OPTIMIZED	
		/// Output a debug message on the console (Unicode string)
		inline void OutputDebugMsg( const wchar_t* in_pszMsg )
		{
			fputws( in_pszMsg, stderr );
		}

		/// Output a debug message on the console (Ansi string)
		inline void OutputDebugMsg( const char* in_pszMsg )
		{
			fputs( in_pszMsg, stderr );
		}

		/// Output a debug message on the console (Unicode string)
		template <int MaxSize = 0> // Unused
		inline void OutputDebugMsgV( const wchar_t* in_pszFmt, ... )
		{
			va_list args;
			va_start(args, in_pszFmt);
			vfwprintf(stderr, in_pszFmt, args);
			va_end(args);
		}

		/// Output a debug message on the console (Ansi string)
		template <int MaxSize = 0> // Unused
		inline void OutputDebugMsgV( const char* in_pszFmt, ... )
		{
			va_list args;
			va_start(args, in_pszFmt);
			vfprintf(stderr, in_pszFmt, args);
			va_end(args);
		}
	#else
		inline void OutputDebugMsg( const wchar_t* ){}
		inline void OutputDebugMsg( const char* ){}

		template <int MaxSize = 0> // Unused
		inline void OutputDebugMsgV( const wchar_t* in_pszFmt, ... ){}

		template <int MaxSize = 0> // Unused
		inline void OutputDebugMsgV( const char* in_pszFmt, ... ){}
	#endif
#endif

	/// Converts a wchar_t string to an AkOSChar string.
	/// \remark On some platforms the AkOSChar string simply points to the same string,
	/// on others a new buffer is allocated on the stack using AkAlloca. This means
	/// you must make sure that:
	/// - The source string stays valid and unmodified for as long as you need the
	///   AkOSChar string (for cases where they point to the same string)
	/// - The AkOSChar string is used within this scope only -- for example, do NOT
	///   return that string from a function (for cases where it is allocated on the stack)
	#define CONVERT_WIDE_TO_OSCHAR( _astring_, _oscharstring_ ) \
		_oscharstring_ = (AkOSChar*)AkAlloca( (1 + wcslen( _astring_ )) * sizeof(AkOSChar)); \
		AKPLATFORM::AkWideCharToChar( _astring_, (AkUInt32)(1 + wcslen(_astring_ )), (AkOSChar*)( _oscharstring_ ) )
	
	/// Converts a char string to an AkOSChar string.
	/// \remark On some platforms the AkOSChar string simply points to the same string,
	/// on others a new buffer is allocated on the stack using AkAlloca. This means
	/// you must make sure that:
	/// - The source string stays valid and unmodified for as long as you need the
	///   AkOSChar string (for cases where they point to the same string)
	/// - The AkOSChar string is used within this scope only -- for example, do NOT
	///   return that string from a function (for cases where it is allocated on the stack)
	#define CONVERT_CHAR_TO_OSCHAR( _astring_, _oscharstring_ ) ( _oscharstring_ ) = (AkOSChar*)( _astring_ )

	/// Converts a AkOSChar string into wide char string.
	/// \remark On some platforms the AkOSChar string simply points to the same string,
	/// on others a new buffer is allocated on the stack using AkAlloca. This means
	/// you must make sure that:
	/// - The source string stays valid and unmodified for as long as you need the
	///   AkOSChar string (for cases where they point to the same string)
	/// - The AkOSChar string is used within this scope only -- for example, do NOT
	///   return that string from a function (for cases where it is allocated on the stack)
	#define CONVERT_OSCHAR_TO_WIDE( _osstring_, _wstring_ ) \
		_wstring_ = (wchar_t*)AkAlloca((1+strlen(_osstring_)) * sizeof(wchar_t)); \
		AKPLATFORM::AkCharToWideChar( _osstring_, (AkUInt32)(1 + strlen(_osstring_ )), _wstring_ )

	/// Converts a AkOSChar string into char string.
	/// \remark On some platforms the AkOSChar string simply points to the same string,
	/// on others a new buffer is allocated on the stack using AkAlloca. This means
	/// you must make sure that:
	/// - The source string stays valid and unmodified for as long as you need the
	///   AkOSChar string (for cases where they point to the same string)
	/// - The AkOSChar string is used within this scope only -- for example, do NOT
	///   return that string from a function (for cases where it is allocated on the stack)
	#define CONVERT_OSCHAR_TO_CHAR( _osstring_, _astring_ ) _astring_ = (char*)_osstring_
	
	inline size_t OsStrLen( const AkOSChar* in_pszString )
	{
		return ( strlen( in_pszString ) );
	}

	/// AkOSChar version of snprintf().
	#define AK_OSPRINTF snprintf

	inline int OsStrCmp( const AkOSChar* in_pszString1, const AkOSChar* in_pszString2 )
	{
		return ( strcmp( in_pszString1,  in_pszString2 ) );
	}

	/// Compare two NULL-terminated AkOSChar strings up to the specified count of characters.
	/// \return
	/// - \< 0 if in_pszString1 \< in_pszString2
	/// -    0 if the two strings are identical
	/// - \> 0 if in_pszString1 \> in_pszString2
	/// \remark The comparison is case-sensitive
	inline int OsStrNCmp( const AkOSChar* in_pszString1, const AkOSChar* in_pszString2, size_t in_MaxCountSize )
	{
		return ( strncmp(in_pszString1, in_pszString2, in_MaxCountSize) );
	}

	/// Detects whether the string represents an absolute path to a file
	inline bool IsAbsolutePath(const AkOSChar* in_pszPath, size_t in_pathLen)
	{
		return in_pathLen >= 1 && in_pszPath[0] == '/';
	}

	// Use with AkOSChar.
#ifndef AK_PATH_SEPARATOR
	#define AK_PATH_SEPARATOR "/"
#endif

#ifndef AK_LIBRARY_PREFIX
	#define AK_LIBRARY_PREFIX	"lib"
#endif

#ifndef AK_DYNAMIC_LIBRARY_EXTENSION
	#define AK_DYNAMIC_LIBRARY_EXTENSION	".so"
#endif

	#define AK_FILEHANDLE_TO_UINTPTR(_h) ((AkUIntPtr)_h)
	#define AK_SET_FILEHANDLE_TO_UINTPTR(_h,_u) _h = (AkFileHandle)_u
}

#pragma GCC visibility pop

