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

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkAssert.h>

//-----------------------------------------------------------------------------
// CAkLock class
//-----------------------------------------------------------------------------
class CAkLock
{
public:
    /// Constructor
	CAkLock()
    {
#if defined(AK_SUPPORT_THREADS)
		pthread_mutexattr_t	mutex_attr;
		AKVERIFY(!pthread_mutexattr_init( &mutex_attr ));
		AKVERIFY(!pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_RECURSIVE ));
		AKVERIFY(!pthread_mutex_init( &m_mutex, &mutex_attr));
		AKVERIFY(!pthread_mutexattr_destroy( &mutex_attr ));
#endif
    }

	/// Destructor
	~CAkLock()
    {
#if defined(AK_SUPPORT_THREADS)
		AKVERIFY(!pthread_mutex_destroy( &m_mutex ));
#endif
    }

    /// Lock 
    inline AKRESULT Lock( void )
	{
#if defined(AK_SUPPORT_THREADS)
		if( !pthread_mutex_lock(&m_mutex) )
		{
			return AK_Success;
		}
		return AK_Fail;
#else
		return AK_Success;
#endif
	}

	/// Unlock
    inline AKRESULT Unlock( void )
	{
#if defined(AK_SUPPORT_THREADS)
		if( !pthread_mutex_unlock(&m_mutex) )
		{
			return AK_Success;
		}
		return AK_Fail;
#else
		return AK_Success;
#endif
	}

	inline bool TryLock()
	{
#if defined(AK_SUPPORT_THREADS)
		return pthread_mutex_trylock(&m_mutex);
#else
		return true;
#endif
	}

private:
#if defined(AK_SUPPORT_THREADS)
    pthread_mutex_t m_mutex;
#endif
};
