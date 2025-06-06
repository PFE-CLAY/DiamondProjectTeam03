/*******************************************************************************
The content of this file includes portions of the proprietary AUDIOKINETIC Wwise
Technology released in source code form as part of the game integration package.
The content of this file may not be used without valid licenses to the
AUDIOKINETIC Wwise Technology.
Note that the use of the game engine is subject to the Unreal(R) Engine End User
License Agreement at https://www.unrealengine.com/en-US/eula/unreal
 
License Usage
 
Licensees holding valid licenses to the AUDIOKINETIC Wwise Technology may use
this file in accordance with the end user license agreement provided with the
software or, alternatively, in accordance with the terms contained
in a written agreement between you and Audiokinetic Inc.
Copyright (c) 2024 Audiokinetic Inc.
*******************************************************************************/

#pragma once
#include "HAL/PreprocessorHelpers.h"

#if defined(AK_WWISE_SOUNDENGINE_2022_1)
#include "Wwise/WwiseLowLevelIOHook_2022_1.h"
#elif defined(AK_WWISE_SOUNDENGINE_2023_1)
#include "Wwise/WwiseLowLevelIOHook_2023_1.h"
#elif defined(AK_WWISE_SOUNDENGINE_2024_1)
#include "Wwise/WwiseLowLevelIOHook_2024_1.h"
#elif defined(AK_WWISE_SOUNDENGINE_2025_1)
#include "Wwise/WwiseLowLevelIOHook_2025_1.h"
#else
#error "This file should be updated so SN-DBS can support this version"
#endif
