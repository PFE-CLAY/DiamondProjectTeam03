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

#include "Wwise/API/WwiseSoundEngineAPI.h"

class WWISESOUNDENGINE_API FWwiseSoundEngineAPI_2022_1 : public IWwiseSoundEngineAPI
{
public:
	UE_NONCOPYABLE(FWwiseSoundEngineAPI_2022_1);
	FWwiseSoundEngineAPI_2022_1();

	///////////////////////////////////////////////////////////////////////
	/// @name Initialization
	//@{

	/// Query whether or not the sound engine has been successfully initialized.
	/// \warning This function is not thread-safe. It should not be called at the same time as \c SoundEngine::Init() or \c SoundEngine::Term().
	/// \return \c True if the sound engine has been initialized, \c False otherwise.
	/// \sa
	/// - \ref soundengine_integration_init_advanced
	/// - <tt>AK::SoundEngine::Init()</tt>
	/// - <tt>AK::SoundEngine::Term()</tt>
	virtual bool IsInitialized() override;

	/// Initialize the sound engine.
	/// \warning This function is not thread-safe.
	/// \remark The initial settings should be initialized using <tt>AK::SoundEngine::GetDefaultInitSettings()</tt>
	///			and <tt>AK::SoundEngine::GetDefaultPlatformInitSettings()</tt> to fill the structures with their 
	///			default settings. This is not mandatory, but it helps avoid backward compatibility problems.
	///		
	/// \return 
	/// - \c AK_Success if the initialization was successful
	/// - \c AK_MemManagerNotInitialized if the memory manager is not available or not properly initialized
	/// - \c AK_StreamMgrNotInitialized if the stream manager is not available or not properly initialized
	/// - \c AK_SSEInstructionsNotSupported if the machine does not support SSE instruction (only on the PC)
	/// - \c AK_InsufficientMemory if there is not enough memory available to initialize the sound engine properly
	/// - \c AK_InvalidParameter if some parameters are invalid
	/// - \c AK_AlreadyInitialized if the sound engine is already initialized, or if the provided settings result in insufficient 
	/// - \c AK_Fail for unknown errors, check with AK Support.
	/// resources for the initialization.
	/// \sa
	/// - \ref soundengine_integration_init_advanced
	/// - \ref workingwithsdks_initialization
	/// - <tt>AK::SoundEngine::Term()</tt>
	/// - <tt>AK::SoundEngine::GetDefaultInitSettings()</tt>
	/// - <tt>AK::SoundEngine::GetDefaultPlatformInitSettings()</tt>
	virtual AKRESULT Init(
		AkInitSettings* in_pSettings,   		///< Initialization settings (can be NULL, to use the default values)
		AkPlatformInitSettings* in_pPlatformSettings  	///< Platform-specific settings (can be NULL, to use the default values)
		) override;

	/// Gets the default values of the platform-independent initialization settings.
	/// \warning This function is not thread-safe.
	/// \sa
	/// - \ref soundengine_integration_init_advanced
	/// - <tt>AK::SoundEngine::Init()</tt>
	/// - <tt>AK::SoundEngine::GetDefaultPlatformInitSettings()</tt>
	virtual void GetDefaultInitSettings(
		AkInitSettings& out_settings   			///< Returned default platform-independent sound engine settings
		) override;

	/// Gets the default values of the platform-specific initialization settings.
	///
	/// Windows Specific:
	///		HWND is the handle of the window associated with the audio. 
	///		Each game must specify the HWND of the application for device detection purposes. 
	///		The value returned by GetDefaultPlatformInitSettings is the foreground HWND at
	///		the moment of the initialization of the sound engine and might not be the correct one for your game.
	///		Each game must provide the correct HWND to use.
	///
	/// \warning This function is not thread-safe.
	/// \sa 
	/// - \ref soundengine_integration_init_advanced
	/// - <tt>AK::SoundEngine::Init()</tt>
	/// - <tt>AK::SoundEngine::GetDefaultInitSettings()</tt>
	virtual void GetDefaultPlatformInitSettings(
		AkPlatformInitSettings& out_platformSettings  	///< Returned default platform-specific sound engine settings
		) override;

	/// Terminates the sound engine.
	/// If some sounds are still playing or events are still being processed when this function is 
	///	called, they will be stopped.
	/// \warning This function is not thread-safe.
	/// \warning Before calling Term, you must ensure that no other thread is accessing the sound engine.
	/// \sa 
	/// - \ref soundengine_integration_init_advanced
	/// - <tt>AK::SoundEngine::Init()</tt>
	virtual void Term() override;

	/// Gets the configured audio settings.
	/// Call this function to get the configured audio settings.
	/// 
	/// \warning This function is not thread-safe.
	/// \warning Call this function only after the sound engine has been properly initialized.
	/// \return
	/// - \c AK_NotInitialized if <tt>AK::SoundEngine::Init()</tt> was not called
	/// - \c AK_Success otherwise.		
	virtual AKRESULT GetAudioSettings(
		AkAudioSettings& out_audioSettings  	///< Returned audio settings
		) override;

	/// Gets the output speaker configuration of the specified output.
	/// Call this function to get the speaker configuration of the output (which may not correspond
	/// to the physical output format of the platform, in the case of downmixing provided by the platform itself). 
	/// You may initialize the sound engine with a user-specified configuration, but the resulting 
	/// configuration is determined by the sound engine, based on the platform, output type and
	/// platform settings (for e.g. system menu or control panel option).
	/// If the speaker configuration of the output is object-based, the speaker configuration of the
	/// main mix is returned. To query more information on object-based output devices, see AK::SoundEngine::GetOutputDeviceConfiguration.
	///
	/// It is recommended to call GetSpeakerConfiguration anytime after receiving a callback from RegisterAudioDeviceStatusCallback to know if the channel configuration has changed.
	/// 
	/// \warning Call this function only after the sound engine has been properly initialized. 
	/// If you are initializing the sound engine with AkInitSettings::bUseLEngineThread to false, it is required to call RenderAudio() at least once before calling this function to complete the sound engine initialization. 
	/// The Init.bnk must be loaded prior to this call.
	/// \return The output configuration. An empty AkChannelConfig not AkChannelConfig::IsValid() if device does not exist or if the Init.bnk was not loaded yet.		
	/// \sa 
	/// - AkSpeakerConfig.h
	/// - AkOutputSettings
	/// - <tt>AK::SoundEngine::GetOutputDeviceConfiguration()</tt>
	virtual AkChannelConfig GetSpeakerConfiguration(
		AkOutputDeviceID	in_idOutput = 0				///< Output ID to set the bus on.  As returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output	
		) override;

	/// Gets the configuration of the specified output device.
	/// Call this function to get the channel configuration of the output device as well as its 3D audio capabilities.
	/// If the configuration of the output device is object-based (io_channelConfig.eConfigType == AK_ChannelConfigType_Objects),
	/// io_capabilities can be inspected to determine the channel configuration of the main mix (Ak3DAudioSinkCapabilities::channelConfig),
	/// whether or not the output device uses a passthrough mix (Ak3DAudioSinkCapabilities::bPassthrough) and the maximum number of objects
	/// that can play simultaneously on this output device (Ak3DAudioSinkCapabilities::uMax3DAudioObjects). Note that if
	/// Ak3DAudioSinkCapabilities::bMultiChannelObjects is false, multi-channel objects will be split into multiple mono objects
	/// before being sent to the output device.
	///
	/// \warning Call this function only after the sound engine has been properly initialized. If you are initializing the sound engine with AkInitSettings::bUseLEngineThread to false, it is required to call RenderAudio() at least once before calling this function to complete the sound engine initialization.
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_IDNotFound is the output was not found in the system.
	/// - \c AK_NotInitialized if the sound engine is not initialized
	/// \sa
	/// - AkSpeakerConfig.h
	/// - AkOutputSettings
	/// - <tt>AK::SoundEngine::GetSpeakerConfiguration()</tt>
	virtual AKRESULT GetOutputDeviceConfiguration(
		AkOutputDeviceID in_idOutput,
		AkChannelConfig& io_channelConfig,
		Ak3DAudioSinkCapabilities& io_capabilities
		) override;

	/// Gets the panning rule of the specified output.
	/// \warning Call this function only after the sound engine has been properly initialized.
	/// Returns the supported configuration in out_ePanningRule: 
	/// - AkPanningRule_Speakers
	/// - AkPanningRule_Headphone
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_IDNotFound is the output was not found in the system.
	/// - \c AK_NotInitialized if the sound engine is not initialized
	/// \sa 
	/// - AkSpeakerConfig.h
	virtual AKRESULT GetPanningRule(
		AkPanningRule& out_ePanningRule,			///< Returned panning rule (AkPanningRule_Speakers or AkPanningRule_Headphone) for given output.
		AkOutputDeviceID	in_idOutput = 0				///< Output ID to set the bus on.  As returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output	
		) override;

	/// Sets the panning rule of the specified output.
	/// This may be changed anytime once the sound engine is initialized.
	/// \warning This function posts a message through the sound engine's internal message queue, whereas GetPanningRule() queries the current panning rule directly.
	/// \aknote 
	///	The specified panning rule will only impact the sound if the processing format is downmixing to Stereo in the mixing process. It
	/// will not impact the output if the audio stays in 5.1 until the end, for example.
	/// \endaknote
	virtual AKRESULT SetPanningRule(
		AkPanningRule		in_ePanningRule,			///< Panning rule.
		AkOutputDeviceID	in_idOutput = 0				///< Output ID to set the bus on.  As returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output	
		) override;

	/// Gets speaker angles of the specified device. Speaker angles are used for 3D positioning of sounds over standard configurations.
	/// Note that the current version of Wwise only supports positioning on the plane.
	/// The speaker angles are expressed as an array of loudspeaker pairs, in degrees, relative to azimuth ]0,180].
	/// Supported loudspeaker setups are always symmetric; the center speaker is always in the middle and thus not specified by angles.
	/// Angles must be set in ascending order. 
	/// You may call this function with io_pfSpeakerAngles set to NULL to get the expected number of angle values in io_uNumAngles, 
	/// in order to allocate your array correctly. You may also obtain this number by calling
	/// AK::GetNumberOfAnglesForConfig( AK_SPEAKER_SETUP_DEFAULT_PLANE ).
	/// If io_pfSpeakerAngles is not NULL, the array is filled with up to io_uNumAngles.
	/// Typical usage:
	/// - AkUInt32 uNumAngles;
	/// - GetSpeakerAngles( NULL, uNumAngles, AkOutput_Main ) = 0;
	/// - AkReal32 * pfSpeakerAngles = AkAlloca( uNumAngles * sizeof(AkReal32) ) = 0;
	/// - GetSpeakerAngles( pfSpeakerAngles, uNumAngles, AkOutput_Main ) = 0;
	/// \aknote 
	///	On most platforms, the angle set on the plane consists of 3 angles, to account for 7.1. 
	/// - When panning to stereo (speaker mode, see <tt>AK::SoundEngine::SetPanningRule()</tt>), only angle[0] is used, and 3D sounds in the back of the listener are mirrored to the front. 
	/// - When panning to 5.1, the front speakers use angle[0], and the surround speakers use (angle[2] - angle[1]) / 2.
	/// \endaknote
	/// \warning Call this function only after the sound engine has been properly initialized.
	/// \return AK_Success if device exists.
	/// \sa SetSpeakerAngles()
	virtual AKRESULT GetSpeakerAngles(
		AkReal32* io_pfSpeakerAngles,			///< Returned array of loudspeaker pair angles, in degrees relative to azimuth [0,180]. Pass NULL to get the required size of the array.
		AkUInt32& io_uNumAngles,				///< Returned number of angles in io_pfSpeakerAngles, which is the minimum between the value that you pass in, and the number of angles corresponding to AK::GetNumberOfAnglesForConfig( AK_SPEAKER_SETUP_DEFAULT_PLANE ), or just the latter if io_pfSpeakerAngles is NULL.
		AkReal32& out_fHeightAngle,			///< Elevation of the height layer, in degrees relative to the plane [-90,90].
		AkOutputDeviceID	in_idOutput = 0				///< Output ID to set the bus on.  As returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output			
		) override;

	/// Sets speaker angles of the specified device. Speaker angles are used for 3D positioning of sounds over standard configurations.
	/// Note that the current version of Wwise only supports positioning on the plane.
	/// The speaker angles are expressed as an array of loudspeaker pairs, in degrees, relative to azimuth ]0,180].
	/// Supported loudspeaker setups are always symmetric; the center speaker is always in the middle and thus not specified by angles.
	/// Angles must be set in ascending order. 
	/// Note:
	/// - This function requires the minimum speaker angle between any pair of speakers to be at least 5 degrees.
	/// - When setting angles for a 5.1 speaker layout, we recommend that you select an angle for the SL and SR channels, then subtract 15 degrees for in_pfSpeakerAngles[1] and add 15 degrees for in_pfSpeakerAngles[2] to set the arc appropriately.
	///
	/// Typical usage: 
	/// - Initialize the sound engine and/or add secondary output(s).
	/// - Get number of speaker angles and their value into an array using GetSpeakerAngles().
	/// - Modify the angles and call SetSpeakerAngles().
	/// This function posts a message to the audio thread through the command queue, so it is thread safe. However the result may not be immediately read with GetSpeakerAngles().
	/// \warning This function only applies to configurations (or subset of these configurations) that are standard and whose speakers are on the plane (2D).
	/// \return 
	/// - \c AK_Success if successful.
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	/// - \c AK_InsufficientMemory if there wasn't enough memory in the message queue
	/// - \c AK_InvalidParameter one of the parameter is invalid, check the debug log.
	/// \sa GetSpeakerAngles()
	virtual AKRESULT SetSpeakerAngles(
		const AkReal32* in_pfSpeakerAngles,			///< Array of loudspeaker pair angles, in degrees relative to azimuth [0,180].
		AkUInt32			in_uNumAngles,				///< Number of elements in in_pfSpeakerAngles. It must correspond to AK::GetNumberOfAnglesForConfig( AK_SPEAKER_SETUP_DEFAULT_PLANE ) (the value returned by GetSpeakerAngles()).
		AkReal32 			in_fHeightAngle,			///< Elevation of the height layer, in degrees relative to the plane  [-90,90].
		AkOutputDeviceID	in_idOutput = 0				///< Output ID to set the bus on.  As returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output			
		) override;

	/// Allows the game to set the volume threshold to be used by the sound engine to determine if a voice must go virtual.
	/// This may be changed anytime once the sound engine was initialized.
	/// If this function is not called, the used value will be the value specified in the platform specific project settings.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if the threshold was not between 0 and -96.3 dB.
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	virtual AKRESULT SetVolumeThreshold(
		AkReal32 in_fVolumeThresholdDB ///< Volume Threshold, must be a value between 0 and -96.3 dB
		) override;

	/// Allows the game to set the maximum number of non virtual voices to be played simultaneously.
	/// This may be changed anytime once the sound engine was initialized.
	/// If this function is not called, the used value will be the value specified in the platform specific project settings.
	/// \return 
	/// - \c AK_InvalidParameter if the threshold was not between 1 and MaxUInt16.
	/// - \c AK_Success if successful
	virtual AKRESULT SetMaxNumVoicesLimit(
		AkUInt16 in_maxNumberVoices ///< Maximum number of non-virtual voices.
		) override;
				
	/// Allows the game to set new values for the maximum active workers for the job manager
	/// This may be changed anytime once the sound engine was initialized.
	/// This function may only be used if a worker function has been set via \ref AkJobMgrSettings.
	/// \return 
	/// - \c AK_InvalidParameter in_jobType is not a valid jobtype, or in_uNewMaxActiveWorkers is not greater than 0
	/// - \c AK_NotInitialized JobMgr system was not already initialized with a function to request for workers
	/// - \c AK_Success if successful
	virtual AKRESULT SetJobMgrMaxActiveWorkers( 
		AkJobType in_jobType,            ///< JobType to set new uMaxActiveWorkers for
		AkUInt32 in_uNewMaxActiveWorkers ///< New maximum number of active workers will request
		) override;
				
	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Rendering Audio
	//@{

	/// Processes all commands in the sound engine's command queue.
	/// This method has to be called periodically (usually once per game frame).
	/// \sa 
	/// - \ref concept_events
	/// - \ref soundengine_events
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// \return Always returns AK_Success
	virtual AKRESULT RenderAudio(
		bool in_bAllowSyncRender = true				///< When AkInitSettings::bUseLEngineThread is false, RenderAudio may generate an audio buffer -- unless in_bAllowSyncRender is set to false. Use in_bAllowSyncRender=false when calling RenderAudio from a Sound Engine callback.
		) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Component Registration
	//@{

	/// Query interface to global plug-in context used for plug-in registration/initialization.
	/// \return Global plug-in context.
	virtual AK::IAkGlobalPluginContext* GetGlobalPluginContext() override;

	/// Registers a plug-in with the sound engine and sets the callback functions to create the 
	/// plug-in and its parameter node.  
	/// \aknote 
	///	This function is deprecated. Registration is now automatic if you link plug-ins statically. If plug-ins are dynamic libraries (such as DLLs or SOs), use \c RegisterPluginDLL.
	/// \endaknote
	/// \sa
	/// - \ref register_effects
	/// - \ref plugin_xml
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if invalid parameters were provided
	/// - \c AK_InsufficientMemory if there isn't enough memory to register the plug-in
	/// \remarks
	/// Codecs and plug-ins must be registered before loading banks that use them.\n
	/// Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	virtual AKRESULT RegisterPlugin(
		AkPluginType in_eType,								///< Plug-in type (for example, source or effect)
		AkUInt32 in_ulCompanyID,							///< Company identifier (as declared in the plug-in description XML file)
		AkUInt32 in_ulPluginID,								///< Plug-in identifier (as declared in the plug-in description XML file)
		AkCreatePluginCallback in_pCreateFunc,				///< Pointer to the plug-in's creation function
		AkCreateParamCallback in_pCreateParamFunc,			///< Pointer to the plug-in's parameter node creation function
		AkGetDeviceListCallback in_pGetDeviceList = NULL	///< Optional pointer to the plug-in's device enumeration function. Specify for a sink plug-in to support \ref AK::SoundEngine::GetDeviceList.
		) override;

	/// Loads a plug-in dynamic library and registers it with the sound engine.  
	/// With dynamic linking, all plugins are automatically registered.
	/// The plug-in DLL must be in the OS-specific library path or in the same location as the executable. If not, set AkInitSettings.szPluginDLLPath.
	/// \return 
	/// - \c AK_Success if successful.  
	/// - \c AK_FileNotFound if the DLL is not found in the OS path or if it has extraneous dependencies not found.  
	/// - \c AK_InsufficientMemory if the system ran out of resources while loading the dynamic library
	/// - \c AK_NotCompatible if the file was found but is not binary-compatible with the system's expected executable format
	/// - \c AK_InvalidFile if the symbol g_pAKPluginList is not exported by the dynamic library
	/// - \c AK_Fail if an unexpected system error was encountered
	virtual AKRESULT RegisterPluginDLL(
		const AkOSChar* in_DllName,					///< Name of the DLL to load, without "lib" prefix or extension.  
		const AkOSChar* in_DllPath = NULL			///< Optional path to the DLL. Will override szPluginDLLPath that was set in AkInitSettings.
		) override;
	
	/// Query whether plug-in is registered with the sound engine.
	/// \return true when plug-in is registered, false otherwise.
	virtual bool IsPluginRegistered(
		AkPluginType in_eType,								///< Plug-in type (for example, source or effect)
		AkUInt32 in_ulCompanyID,							///< Company identifier (as declared in the plug-in description XML file)
		AkUInt32 in_ulPluginID								///< Plug-in identifier (as declared in the plug-in description XML file)
		) override;

	/// Registers a codec type with the sound engine and set the callback functions to create the 
	/// codec's file source and bank source nodes.
	/// \aknote 
	///	This function is deprecated. Registration is now automatic if you link plugins statically. If plugins are dynamic libraries (such as DLLs or SOs), use RegisterPluginDLL.
	/// \endaknote		
	/// \sa 
	/// - \ref register_effects
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if invalid parameters were provided
	/// - \c AK_InsufficientMemory if there isn't enough memory to register the plug-in
	/// \remarks
	/// Codecs and plug-ins must be registered before loading banks that use them.\n
	/// Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the Event will fail.
	virtual AKRESULT RegisterCodec(
		AkUInt32 in_ulCompanyID,						///< Company identifier (as declared in the plug-in description XML file)
		AkUInt32 in_ulCodecID,							///< Codec identifier (as declared in the plug-in description XML file)
		AkCreateFileSourceCallback in_pFileCreateFunc,	///< Pointer to the codec's file source node creation function
		AkCreateBankSourceCallback in_pBankCreateFunc	///< Pointer to the codec's bank source node creation function
		) override;

	/// Registers a global callback function. This function will be called from the audio rendering thread, at the
	/// location specified by in_eLocation. This function will also be called from the thread calling 			
	/// AK::SoundEngine::Term with in_eLocation set to AkGlobalCallbackLocation_Term.
	/// For example, in order to be called at every audio rendering pass, and once during teardown for releasing resources, you would call 
	/// RegisterGlobalCallback(myCallback, AkGlobalCallbackLocation_BeginRender  | AkGlobalCallbackLocation_Term, myCookie, AkPluginTypeNone, 0, 0) = 0;  
	/// \remarks
	/// A Plugin Type, Company ID and Plugin ID can be provided to this function to enable timing in the performance monitor.
	/// If the callback is being timed, it will contribute to the Total Plug-in CPU measurement, and also appear in the Plug-ins tab of the Advanced Profiler by plug-in type and ID. 
	/// It is illegal to call this function while already inside of a global callback.
	/// This function might stall for several milliseconds before returning.
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if parameters are out of range (check debug console or Wwise Profiler)
	/// \sa 
	/// - <tt>AK::SoundEngine::UnregisterGlobalCallback()</tt>
	/// - AkGlobalCallbackFunc
	/// - AkGlobalCallbackLocation
	virtual AKRESULT RegisterGlobalCallback(
		AkGlobalCallbackFunc in_pCallback,				///< Function to register as a global callback.
		AkUInt32 in_eLocation = AkGlobalCallbackLocation_BeginRender, ///< Callback location defined in AkGlobalCallbackLocation. Bitwise OR multiple locations if needed.
		void* in_pCookie = NULL,						///< User cookie.
		AkPluginType in_eType = AkPluginTypeNone,		///< Plug-in type (for example, source or effect). AkPluginTypeNone for no timing.
		AkUInt32 in_ulCompanyID = 0,					///< Company identifier (as declared in the plug-in description XML file). 0 for no timing. 
		AkUInt32 in_ulPluginID = 0						///< Plug-in identifier (as declared in the plug-in description XML file). 0 for no timing. 
		) override;

	/// Unregisters a global callback function, previously registered using RegisterGlobalCallback.
	/// \remarks
	/// It is legal to call this function while already inside of a global callback, If it is unregistering itself and not
	/// another callback.
	/// This function might stall for several milliseconds before returning.
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if parameters are out of range (check debug console or Wwise Profiler)
	/// \sa 
	/// - <tt>AK::SoundEngine::RegisterGlobalCallback()</tt>
	/// - AkGlobalCallbackFunc
	/// - AkGlobalCallbackLocation
	virtual AKRESULT UnregisterGlobalCallback(
		AkGlobalCallbackFunc in_pCallback,				///< Function to unregister as a global callback.
		AkUInt32 in_eLocation = AkGlobalCallbackLocation_BeginRender ///< Must match in_eLocation as passed to RegisterGlobalCallback for this callback.
		) override;

	/// Registers a resource monitor callback function that gets all of the resource usage data contained in the 
	/// AkResourceMonitorDataSummary structure. This includes general information about the system, such as CPU usage, 
	/// active Voices, and Events. This function will be called from the audio rendering thread at the end of each frame.
	/// \remarks
	/// If the callback is being timed, it will contribute to the Total Plug-in CPU measurement, and also appear in the Plug-ins tab of the Advanced Profiler by plug-in type and ID. 
	/// It is illegal to call this function while already inside of a resource callback.
	/// This function might stall for several milliseconds before returning.
	/// This function will return AK_Fail in Release
	/// \sa 
	/// - <tt>AK::SoundEngine::UnregisterResourceMonitorCallback()</tt>
	/// - AkResourceMonitorCallbackFunc
	virtual AKRESULT RegisterResourceMonitorCallback(
		AkResourceMonitorCallbackFunc in_pCallback				///< Function to register as a resource monitor callback.
		) override;

	/// Unregisters a resource monitor callback function, previously registered using RegisterResourceMonitorCallback.
	/// \remarks
	/// It is legal to call this function while already inside of a resource monitor callback, If it is unregistering itself and not
	/// another callback.
	/// This function might stall for several milliseconds before returning.
	/// \sa 
	/// - <tt>AK::SoundEngine::RegisterResourceMonitorCallback()</tt>
	/// - AkResourceMonitorCallbackFunc
	virtual AKRESULT UnregisterResourceMonitorCallback(
		AkResourceMonitorCallbackFunc in_pCallback				///< Function to unregister as a resource monitor callback.
		) override;

	/// Registers a callback for the Audio Device status changes.  
	/// The callback will be called from the audio thread
	/// Can be called prior to AK::SoundEngine::Init
	/// \sa AK::SoundEngine::AddOutput
	virtual AKRESULT RegisterAudioDeviceStatusCallback(
		AK::AkDeviceStatusCallbackFunc in_pCallback				///< Function to register as a status callback.						
		) override;

	/// Unregisters the callback for the Audio Device status changes, registered by RegisterAudioDeviceStatusCallback				
	virtual AKRESULT UnregisterAudioDeviceStatusCallback() override;
	//@}

#ifdef AK_SUPPORT_WCHAR
	////////////////////////////////////////////////////////////////////////
	/// @name Getting ID from strings
	//@{

	/// Universal converter from Unicode string to ID for the sound engine.
	/// This function will hash the name based on a algorithm ( provided at : /AK/Tools/Common/AkFNVHash.h )
	/// Note:
	///		This function does return a AkUInt32, which is totally compatible with:
	///		AkUniqueID, AkStateGroupID, AkStateID, AkSwitchGroupID, AkSwitchStateID, AkRtpcID, and so on...
	/// \sa
	/// - <tt>AK::SoundEngine::PostEvent</tt>
	/// - <tt>AK::SoundEngine::SetRTPCValue</tt>
	/// - <tt>AK::SoundEngine::SetSwitch</tt>
	/// - <tt>AK::SoundEngine::SetState</tt>
	/// - <tt>AK::SoundEngine::PostTrigger</tt>
	/// - <tt>AK::SoundEngine::SetGameObjectAuxSendValues</tt>
	/// - <tt>AK::SoundEngine::LoadBank</tt>
	/// - <tt>AK::SoundEngine::UnloadBank</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent</tt>
	/// - <tt>AK::SoundEngine::PrepareGameSyncs</tt>
	virtual AkUInt32 GetIDFromString(const wchar_t* in_pszString) override;
#endif //AK_SUPPORT_WCHAR

	/// Universal converter from string to ID for the sound engine.
	/// This function will hash the name based on a algorithm ( provided at : /AK/Tools/Common/AkFNVHash.h )
	/// Note:
	///		This function does return a AkUInt32, which is totally compatible with:
	///		AkUniqueID, AkStateGroupID, AkStateID, AkSwitchGroupID, AkSwitchStateID, AkRtpcID, and so on...
	/// \sa
	/// - <tt>AK::SoundEngine::PostEvent</tt>
	/// - <tt>AK::SoundEngine::SetRTPCValue</tt>
	/// - <tt>AK::SoundEngine::SetSwitch</tt>
	/// - <tt>AK::SoundEngine::SetState</tt>
	/// - <tt>AK::SoundEngine::PostTrigger</tt>
	/// - <tt>AK::SoundEngine::SetGameObjectAuxSendValues</tt>
	/// - <tt>AK::SoundEngine::LoadBank</tt>
	/// - <tt>AK::SoundEngine::UnloadBank</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent</tt>
	/// - <tt>AK::SoundEngine::PrepareGameSyncs</tt>
	virtual AkUInt32 GetIDFromString(const char* in_pszString) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Event Management
	//@{

	/// Asynchronously posts an Event to the sound engine (by event ID).
	/// The callback function can be used to be noticed when markers are reached or when the event is finished.
	/// An array of wave file sources can be provided to resolve External Sources triggered by the event. 
	/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
	/// \remarks
	/// If used, the array of external sources should contain the information for each external source triggered by the
	/// event. When triggering an event with multiple external sources, you need to differentiate each source 
	/// by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
	/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project 
	/// (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.
	/// \endaknote		
	/// \sa 
	/// - \ref concept_events
	/// - \ref integrating_external_sources
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	virtual AkPlayingID PostEvent(
		AkUniqueID in_eventID,							///< Unique ID of the event
		AkGameObjectID in_gameObjectID,					///< Associated game object ID
		AkUInt32 in_uFlags = 0,							///< Bitmask: see \ref AkCallbackType
		AkCallbackFunc in_pfnCallback = NULL,			///< Callback function
		void* in_pCookie = NULL,						///< Callback cookie that will be sent to the callback function along with additional information
		AkUInt32 in_cExternals = 0,						///< Optional count of external source structures
		AkExternalSourceInfo* in_pExternalSources = NULL,///< Optional array of external source resolution information
		AkPlayingID	in_PlayingID = AK_INVALID_PLAYING_ID///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Posts an Event to the sound engine (by Event name), using callbacks.
	/// The callback function can be used to be noticed when markers are reached or when the event is finished.
	/// An array of wave file sources can be provided to resolve External Sources triggered by the event. 
	/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
	/// \remarks
	/// If used, the array of external sources should contain the information for each external source triggered by the
	/// event. When triggering an event with multiple external sources, you need to differentiate each source 
	/// by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
	/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project 
	/// (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.
	/// \endaknote		
	/// \sa 
	/// - \ref concept_events
	/// - \ref integrating_external_sources
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	virtual AkPlayingID PostEvent(
		const wchar_t* in_pszEventName,					///< Name of the event
		AkGameObjectID in_gameObjectID,					///< Associated game object ID
		AkUInt32 in_uFlags = 0,							///< Bitmask: see \ref AkCallbackType
		AkCallbackFunc in_pfnCallback = NULL,			///< Callback function
		void* in_pCookie = NULL,						///< Callback cookie that will be sent to the callback function along with additional information.
		AkUInt32 in_cExternals = 0,						///< Optional count of external source structures
		AkExternalSourceInfo* in_pExternalSources = NULL,///< Optional array of external source resolution information
		AkPlayingID	in_PlayingID = AK_INVALID_PLAYING_ID///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Posts an Event to the sound engine (by Event name), using callbacks.
	/// The callback function can be used to be noticed when markers are reached or when the event is finished.
	/// An array of Wave file sources can be provided to resolve External Sources triggered by the event. P
	/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
	/// \remarks
	/// If used, the array of external sources should contain the information for each external source triggered by the
	/// event. When triggering an Event with multiple external sources, you need to differentiate each source 
	/// by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
	/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project 
	/// (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.
	/// \endaknote
	/// \sa 
	/// - \ref concept_events
	/// - \ref integrating_external_sources
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	virtual AkPlayingID PostEvent(
		const char* in_pszEventName,					///< Name of the event
		AkGameObjectID in_gameObjectID,					///< Associated game object ID
		AkUInt32 in_uFlags = 0,							///< Bitmask: see \ref AkCallbackType
		AkCallbackFunc in_pfnCallback = NULL,			///< Callback function
		void* in_pCookie = NULL,						///< Callback cookie that will be sent to the callback function along with additional information.
		AkUInt32 in_cExternals = 0,						///< Optional count of external source structures
		AkExternalSourceInfo* in_pExternalSources = NULL,///< Optional array of external source resolution information
		AkPlayingID	in_PlayingID = AK_INVALID_PLAYING_ID///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
		) override;

	/// Executes an action on all nodes that are referenced in the specified event in an action of type play.
	/// \return
	/// - \c AK_Success if the action was successfully queued.
	/// - \c AK_IDNotFound if the Event was not found (not loaded or there is a typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::AkActionOnEventType</tt>
	virtual AKRESULT ExecuteActionOnEvent(
		AkUniqueID in_eventID,												///< Unique ID of the event
		AK::SoundEngine::AkActionOnEventType in_ActionType,									///< Action to execute on all the elements that were played using the specified event.
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,			///< Associated game object ID
		AkTimeMs in_uTransitionDuration = 0,								///< Fade duration
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the transition
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID					///< Associated PlayingID
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Executes an action on all nodes that are referenced in the specified event in an action of type play.
	/// \return
	/// - \c AK_Success if the action was successfully queued.
	/// - \c AK_IDNotFound if the Event was not found (not loaded or there is a typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::AkActionOnEventType</tt>
	virtual AKRESULT ExecuteActionOnEvent(
		const wchar_t* in_pszEventName,										///< Name of the event
		AK::SoundEngine::AkActionOnEventType in_ActionType,									///< Action to execute on all the elements that were played using the specified event.
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,			///< Associated game object ID
		AkTimeMs in_uTransitionDuration = 0,								///< Fade duration
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the transition
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID					///< Associated PlayingID
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Executes an Action on all nodes that are referenced in the specified Event in an Action of type Play.
	/// \return
	/// - \c AK_Success if the action was successfully queued.
	/// - \c AK_IDNotFound if the Event was not found (not loaded or there is a typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::AkActionOnEventType</tt>
	virtual AKRESULT ExecuteActionOnEvent(
		const char* in_pszEventName,										///< Name of the event
		AK::SoundEngine::AkActionOnEventType in_ActionType,									///< Action to execute on all the elements that were played using the specified event.
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,			///< Associated game object ID
		AkTimeMs in_uTransitionDuration = 0,								///< Fade duration
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the transition
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID					///< Associated PlayingID
		) override;


	/// Executes a number of MIDI Events on all nodes that are referenced in the specified Event in an Action of type Play.
	/// The time at which a MIDI Event is posted is determined by in_bAbsoluteOffsets. If false, each MIDI event will be
	/// posted in AkMIDIPost::uOffset samples from the start of the current frame. If true, each MIDI event will be posted
	/// at the absolute time AkMIDIPost::uOffset samples.
	/// To obtain the current absolute time, see AK::SoundEngine::GetSampleTick.
	/// The duration of a sample can be determined from the sound engine's audio settings, via a call to AK::SoundEngine::GetAudioSettings.
	/// If a playing ID is specified then that playing ID must be active. Otherwise a new playing ID will be assigned.
	/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
	/// \sa
	/// - <tt>AK::SoundEngine::GetAudioSettings</tt>
	/// - <tt>AK::SoundEngine::GetSampleTick</tt>
	/// - <tt>AK::SoundEngine::StopMIDIOnEvent</tt>
	/// - \ref soundengine_midi_event_playing_id
	virtual AkPlayingID PostMIDIOnEvent(
		AkUniqueID in_eventID,												///< Unique ID of the Event
		AkGameObjectID in_gameObjectID,										///< Associated game object ID
		AkMIDIPost* in_pPosts,												///< MIDI Events to post
		AkUInt16 in_uNumPosts,												///< Number of MIDI Events to post
		bool in_bAbsoluteOffsets = false,									///< Set to true when AkMIDIPost::uOffset are absolute, false when relative to current frame
		AkUInt32 in_uFlags = 0,												///< Bitmask: see \ref AkCallbackType
		AkCallbackFunc in_pfnCallback = NULL,								///< Callback function
		void* in_pCookie = NULL,											///< Callback cookie that will be sent to the callback function along with additional information
		AkPlayingID in_playingID = AK_INVALID_PLAYING_ID					///< Target playing ID
		) override;

	/// Stops MIDI notes on all nodes that are referenced in the specified event in an action of type play,
	/// with the specified Game Object. Invalid parameters are interpreted as wildcards. For example, calling
	/// this function with in_eventID set to AK_INVALID_UNIQUE_ID will stop all MIDI notes for Game Object
	/// in_gameObjectID.
	/// \return 
	/// - \c AK_Success if the stop command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PostMIDIOnEvent</tt>
	/// - \ref soundengine_midi_event_playing_id
	virtual AKRESULT StopMIDIOnEvent(
		AkUniqueID in_eventID = AK_INVALID_UNIQUE_ID,						///< Unique ID of the Event
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,			///< Associated game object ID
		AkPlayingID in_playingID = AK_INVALID_PLAYING_ID					///< Target playing ID
		) override;


	/// Starts streaming the first part of all streamed files referenced by an Event into a cache buffer. Caching streams are serviced when no other streams require the 
	/// available bandwidth. The files will remain cached until UnpinEventInStreamCache is called, or a higher priority pinned file needs the space and the limit set by 
	/// uMaxCachePinnedBytes is exceeded.  
	/// \remarks The amount of data from the start of the file that will be pinned to cache is determined by the prefetch size. The prefetch size is set via the authoring tool and stored in the sound banks.  
	/// \remarks It is possible to override the prefetch size stored in the sound bank via the low level IO. For more information see <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> and AkFileSystemFlags.
	/// \remarks If this function is called additional times with the same event, then the priority of the caching streams are updated. Note however that priority is passed down to the stream manager 
	///	on a file-by-file basis, and if another event is pinned to cache that references the same file but with a different priority, then the first priority will be updated with the most recent value.
	/// \remarks If the event references files that are chosen based on a State Group (via a switch container), all files in all states will be cached. Those in the current active state
	/// will get cached with active priority, while all other files will get cached with inactive priority.
	/// \remarks in_uInactivePriority is only relevant for events that reference switch containers that are assigned to State Groups. This parameter is ignored for all other events, including events that only reference
	/// switch containers that are assigned to Switch Groups. Files that are chosen based on a Switch Group have a different switch value per game object, and are all effectively considered active by the pin-to-cache system.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen</tt>
	/// - AkFileSystemFlags
	virtual AKRESULT PinEventInStreamCache(
		AkUniqueID in_eventID,											///< Unique ID of the event
		AkPriority in_uActivePriority,									///< Priority of active stream caching I/O
		AkPriority in_uInactivePriority 								///< Priority of inactive stream caching I/O
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Starts streaming the first part of all streamed files referenced by an event into a cache buffer. Caching streams are serviced when no other streams require the 
	/// available bandwidth. The files will remain cached until UnpinEventInStreamCache is called, or a higher priority pinned file needs the space and the limit set by 
	/// uMaxCachePinnedBytes is exceeded.  
	/// \remarks The amount of data from the start of the file that will be pinned to cache is determined by the prefetch size. The prefetch size is set via the authoring tool and stored in the sound banks.  
	/// \remarks It is possible to override the prefetch size stored in the sound bank via the low level IO. For more information see <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> and AkFileSystemFlags.
	/// \remarks If this function is called additional times with the same event, then the priority of the caching streams are updated. Note however that priority is passed down to the stream manager 
	///	on a file-by-file basis, and if another event is pinned to cache that references the same file but with a different priority, then the first priority will be updated with the most recent value.
	/// \remarks If the event references files that are chosen based on a State Group (via a Switch Container), all files in all states will be cached. Those in the current active state
	/// will get cached with active priority, while all other files will get cached with inactive priority.
	/// \remarks in_uInactivePriority is only relevant for events that reference switch containers that are assigned to State Groups. This parameter is ignored for all other events, including events that only reference
	/// switch containers that are assigned to Switch Groups. Files that are chosen based on a Switch Group have a different switch value per game object, and are all effectively considered active by the pin-to-cache system.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen</tt>
	/// - AkFileSystemFlags
	virtual AKRESULT PinEventInStreamCache(
		const wchar_t* in_pszEventName,									///< Name of the event
		AkPriority in_uActivePriority,									///< Priority of active stream caching I/O
		AkPriority in_uInactivePriority 								///< Priority of inactive stream caching I/O
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Starts streaming the first part of all streamed files referenced by an event into a cache buffer. Caching streams are serviced when no other streams require the 
	/// available bandwidth. The files will remain cached until UnpinEventInStreamCache is called, or a higher priority pinned file needs the space and the limit set by 
	/// uMaxCachePinnedBytes is exceeded.  
	/// \remarks The amount of data from the start of the file that will be pinned to cache is determined by the prefetch size. The prefetch size is set via the authoring tool and stored in the sound banks.  
	/// \remarks It is possible to override the prefetch size stored in the sound bank via the low level IO. For more information see <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> and AkFileSystemFlags.
	/// \remarks If this function is called additional times with the same event, then the priority of the caching streams are updated. Note however that priority is passed down to the stream manager 
	/// on a file-by-file basis, and if another event is pinned to cache that references the same file but with a different priority, then the first priority will be updated with the most recent value.
	/// \remarks If the event references files that are chosen based on a State Group (via a switch container), all files in all states will be cached. Those in the current active state
	/// will get cached with active priority, while all other files will get cached with inactive priority.
	/// \remarks in_uInactivePriority is only relevant for events that reference switch containers that are assigned to State Groups. This parameter is ignored for all other events, including events that only reference
	/// switch containers that are assigned to Switch Groups. Files that are chosen based on a Switch Group have a different switch value per game object, and are all effectively considered active by the pin-to-cache system.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen</tt>
	/// - AkFileSystemFlags
	virtual AKRESULT PinEventInStreamCache(
		const char* in_pszEventName,									///< Name of the event
		AkPriority in_uActivePriority,									///< Priority of active stream caching I/O
		AkPriority in_uInactivePriority 								///< Priority of inactive stream caching I/O
		) override;

	/// Releases the set of files that were previously requested to be pinned into cache via <tt>AK::SoundEngine::PinEventInStreamCache()</tt>. The file may still remain in stream cache
	/// after <tt>AK::SoundEngine::UnpinEventInStreamCache()</tt> is called, until the memory is reused by the streaming memory manager in accordance with to its cache management algorithm.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	virtual AKRESULT UnpinEventInStreamCache(
		AkUniqueID in_eventID											///< Unique ID of the event
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Releases the set of files that were previously requested to be pinned into cache via <tt>AK::SoundEngine::PinEventInStreamCache()</tt>. The file may still remain in stream cache
	/// after <tt>AK::SoundEngine::UnpinEventInStreamCache()</tt> is called, until the memory is reused by the streaming memory manager in accordance with to its cache management algorithm.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	virtual AKRESULT UnpinEventInStreamCache(
		const wchar_t* in_pszEventName									///< Name of the event
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Releases the set of files that were previously requested to be pinned into cache via <tt>AK::SoundEngine::PinEventInStreamCache()</tt>. The file may still remain in stream cache
	/// after <tt>AK::SoundEngine::UnpinEventInStreamCache()</tt> is called, until the memory is reused by the streaming memory manager in accordance with to its cache management algorithm.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::GetBufferStatusForPinnedEvent</tt>
	virtual AKRESULT UnpinEventInStreamCache(
		const char* in_pszEventName										///< Name of the event
		) override;

	/// Returns information about an Event that was requested to be pinned into cache via <tt>AK::SoundEngine::PinEventInStreamCache()</tt>.
	/// Retrieves the smallest buffer fill-percentage for each file referenced by the event, and whether 
	/// the cache-pinned memory limit is preventing any of the files from filling up their buffer.
	/// \remarks To set the limit for the maximum number of bytes that can be pinned to cache, see \c AkDeviceSettings 
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - AkDeviceSettings
	virtual AKRESULT GetBufferStatusForPinnedEvent(
		AkUniqueID in_eventID,											///< Unique ID of the event
		AkReal32& out_fPercentBuffered,									///< Fill-percentage (out of 100) of requested buffer size for least buffered file in the event.
		bool& out_bCachePinnedMemoryFull								///< True if at least one file can not complete buffering because the cache-pinned memory limit would be exceeded.
		) override;

	/// Returns information about an Event that was requested to be pinned into cache via \c <tt>AK::SoundEngine::PinEventInStreamCache()</tt>.
	/// Retrieves the smallest buffer fill-percentage for each file referenced by the event, and whether 
	/// the cache-pinned memory limit is preventing any of the files from filling up their buffer.
	/// \remarks To set the limit for the maximum number of bytes that can be pinned to cache, see AkDeviceSettings 
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - AkDeviceSettings
	virtual AKRESULT GetBufferStatusForPinnedEvent(
		const char* in_pszEventName,									///< Name of the event
		AkReal32& out_fPercentBuffered,									///< Fill-percentage (out of 100) of requested buffer size for least buffered file in the event.
		bool& out_bCachePinnedMemoryFull								///< True if at least one file can not complete buffering because the cache-pinned memory limit would be exceeded.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Returns information about an Event that was requested to be pinned into cache via \c <tt>AK::SoundEngine::PinEventInStreamCache()</tt>.
	/// Retrieves the smallest buffer fill-percentage for each file referenced by the event, and whether 
	/// the cache-pinned memory limit is preventing any of the files from filling up their buffer.
	/// \remarks To set the limit for the maximum number of bytes that can be pinned to cache, see AkDeviceSettings 
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::PinEventInStreamCache</tt>
	/// - <tt>AK::SoundEngine::UnpinEventInStreamCache</tt>
	/// - AkDeviceSettings
	virtual AKRESULT GetBufferStatusForPinnedEvent(
		const wchar_t* in_pszEventName,									///< Name of the event
		AkReal32& out_fPercentBuffered,									///< Fill-percentage (out of 100) of requested buffer size for least buffered file in the event.
		bool& out_bCachePinnedMemoryFull								///< True if at least one file can not complete buffering because the cache-pinned memory limit would be exceeded.
		) override;
#endif

	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- Sounds/segments are stopped if in_iPosition is greater than their duration.
	///		- in_iPosition is clamped internally to the beginning of the sound/segment.
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, in_iPosition is relative to the Entry Cue, in milliseconds. Use a negative
	///			value to seek within the Pre-Entry.
	///		- Music segments cannot be looped. You may want to listen to the AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during that time
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time.  
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	///
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		AkUniqueID in_eventID,										///< Unique ID of the event
		AkGameObjectID in_gameObjectID,								///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkTimeMs in_iPosition,										///< Desired position where playback should restart, in milliseconds
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see note above)
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- With Music Segments, in_iPosition is relative to the Entry Cue, in milliseconds. Use a negative
	///			value to seek within the Pre-Entry.
	///		- Sounds/segments are stopped if in_iPosition is greater than their duration.
	///		- in_iPosition is clamped internally to the beginning of the sound/segment.
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, in_iPosition is relative to the Entry Cue, in milliseconds. Use a negative
	///			value to seek within the Pre-Entry.
	///		- Music segments cannot be looped. You may want to listen to the AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during that time
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time. 
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		const wchar_t* in_pszEventName,								///< Name of the event
		AkGameObjectID in_gameObjectID,								///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkTimeMs in_iPosition,										///< Desired position where playback should restart, in milliseconds
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see note above)
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- With Music Segments, in_iPosition is relative to the Entry Cue, in milliseconds. Use a negative
	///			value to seek within the Pre-Entry.
	///		- Sounds/segments are stopped if in_iPosition is greater than their duration.
	///		- in_iPosition is clamped internally to the beginning of the sound/segment.
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, in_iPosition is relative to the Entry Cue, in milliseconds. Use a negative
	///			value to seek within the Pre-Entry.
	///		- Music segments cannot be looped. You may want to listen to the AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during that time
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time. 
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	///
	/// \return 
	/// - \c AK_Success if command was queued
	/// - \c AK_IDNotFound if the Event ID is unknown (not loaded or typo in the ID)
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		const char* in_pszEventName,								///< Name of the event
		AkGameObjectID in_gameObjectID,								///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkTimeMs in_iPosition,										///< Desired position where playback should restart, in milliseconds
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see note above)	
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
		) override;

	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	/// Seek position is specified as a percentage of the sound's total duration, and takes looping into account. 
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- in_iPosition is clamped internally to the beginning of the sound/segment.
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, \c in_fPercent is relative to the Entry Cue, and the segment's duration is the 
	///			duration between its entry and exit cues. Consequently, you cannot seek within the pre-entry or
	///			post-exit of a segment using this method. Use absolute values instead.
	///		- Music Segments cannot be looped. You may want to listen to the \c AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during the time that period
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time. 
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	///
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		AkUniqueID in_eventID,										///< Unique ID of the event
		AkGameObjectID in_gameObjectID,							///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkReal32 in_fPercent,										///< Desired position where playback should restart, expressed in a percentage of the file's total duration, between 0 and 1.f (see note above about infinite looping sounds)
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see note above)	
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	/// Seek position is specified as a percentage of the sound's total duration, and takes looping into account. 
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, \c in_fPercent is relative to the Entry Cue, and the segment's duration is the 
	///			duration between its entry and exit cues. Consequently, you cannot seek within the pre-entry or
	///			post-exit of a segment using this method. Use absolute values instead.
	///		- Music Segments cannot be looped. You may want to listen to the \c AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during the time that period
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time. 
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	///
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		const wchar_t* in_pszEventName,								///< Name of the event
		AkGameObjectID in_gameObjectID,							///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkReal32 in_fPercent,										///< Desired position where playback should restart, expressed in a percentage of the file's total duration, between 0 and 1.f (see note above about infinite looping sounds)
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see note above)	
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Seeks inside all playing objects that are referenced in Play Actions of the specified Event.
	/// Seek position is specified as a percentage of the sound's total duration, and takes looping into account. 
	///
	/// Notes:
	///		- This works with all objects of the actor-mixer hierarchy, and also with Music Segments and Music Switch Containers. 
	///		- There is a restriction with sounds that play within a continuous sequence. Seeking is ignored 
	///			if one of their ancestors is a continuous (random or sequence) container with crossfade or 
	///			trigger rate transitions. Seeking is also ignored with sample-accurate transitions, unless
	///			the sound that is currently playing is the first sound of the sequence.
	///		- Seeking is also ignored with voices that can go virtual with "From Beginning" behavior. 
	///		- If the option "Seek to nearest marker" is used, the seeking position snaps to the nearest marker.
	///			With objects of the actor-mixer hierarchy, markers are embedded in wave files by an external wave editor.
	///			Note that looping regions ("sampler loop") are not considered as markers. Also, the "add file name marker" of the 
	///			conversion settings dialog adds a marker at the beginning of the file, which is considered when seeking
	///			to nearest marker. In the case of objects of the interactive music hierarchy, user (wave) markers are ignored:
	///			seeking occurs to the nearest segment cue (authored in the segment editor), including the Entry Cue, but excluding the Exit Cue.
	///		- This method posts a command in the sound engine queue, thus seeking will not occur before 
	///			the audio thread consumes it (after a call to RenderAudio()). 
	///
	///	Notes specific to Music Segments:
	///		- With Music Segments, in_fPercent is relative to the Entry Cue, and the segment's duration is the 
	///			duration between its entry and exit cues. Consequently, you cannot seek within the pre-entry or
	///			post-exit of a segment using this method. Use absolute values instead.
	///		- Music segments cannot be looped. You may want to listen to the AK_EndOfEvent notification 
	///			in order to restart them if required.
	///		- In order to restart at the correct location, with all their tracks synchronized, Music Segments 
	///			take the "look-ahead time" property of their streamed tracks into account for seeking. 
	///			Consequently, the resulting position after a call to SeekOnEvent() might be earlier than the 
	///			value that was passed to the method. Use <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> to query 
	///			the exact position of a segment. Also, the segment will be silent during the time that period
	///			(so that it restarts precisely at the position that you specified). <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt> 
	///			also informs you about the remaining look-ahead time. 
	///
	/// Notes specific to Music Switch Containers:
	///		- Seeking triggers a music transition towards the current (or target) segment.
	///			This transition is subject to the container's transition rule that matches the current and defined in the container,
	///			so the moment when seeking occurs depends on the rule's "Exit At" property. On the other hand, the starting position 
	///			in the target segment depends on both the desired seeking position and the rule's "Sync To" property.
	///		- If the specified time is greater than the destination segment's length, the modulo is taken.
	///
	/// \sa
	/// - <tt>AK::SoundEngine::RenderAudio()</tt>
	/// - <tt>AK::SoundEngine::PostEvent()</tt>
	/// - <tt>AK::SoundEngine::GetSourcePlayPosition()</tt>
	/// - <tt>AK::MusicEngine::GetPlayingSegmentInfo()</tt>
	virtual AKRESULT SeekOnEvent(
		const char* in_pszEventName,								///< Name of the event
		AkGameObjectID in_gameObjectID,								///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		AkReal32 in_fPercent,										///< Desired position where playback should restart, expressed in a percentage of the file's total duration, between 0 and 1.f (see note above about infinite looping sounds)
		bool in_bSeekToNearestMarker = false,						///< If true, the final seeking position will be made equal to the nearest marker (see notes above).	
		AkPlayingID in_PlayingID = AK_INVALID_PLAYING_ID			///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
		) override;

	/// Cancels all Event callbacks associated with a specific callback cookie.\n
	/// \sa 
	/// - \c <tt>AK::SoundEngine::PostEvent()</tt>
	virtual void CancelEventCallbackCookie(
		void* in_pCookie 							///< Callback cookie to be cancelled
		) override;

	/// Cancels all Event callbacks associated with a specific game object.\n
	/// \sa 
	/// - \c <tt>AK::SoundEngine::PostEvent()</tt>
	virtual void CancelEventCallbackGameObject(
		AkGameObjectID in_gameObjectID 				///< ID of the game object to be cancelled
		) override;

	/// Cancels all Event callbacks for a specific playing ID.
	/// \sa 
	/// - \c <tt>AK::SoundEngine::PostEvent()</tt>
	virtual void CancelEventCallback(
		AkPlayingID in_playingID 					///< Playing ID of the event that must not use callbacks
		) override;

	/// Gets the current position of the source associated with this playing ID, obtained from PostEvent(). If more than one source is playing,
	/// the first to play is returned.
	/// Notes:
	/// - You need to pass AK_EnableGetSourcePlayPosition to PostEvent() in order to use this function, otherwise
	/// 	it returns AK_Fail, even if the playing ID is valid.
	/// - The source's position is updated at every audio frame, and the time at which this occurs is stored. 
	///		When you call this function from your thread, you therefore query the position that was updated in the previous audio frame.
	///		If in_bExtrapolate is true (default), the returned position is extrapolated using the elapsed time since last 
	///		sound engine update and the source's playback rate.
	/// \return 
	/// - \c AK_Success if successful.
	/// - \c AK_InvalidParameter if the provided pointer is not valid.
	/// - \c AK_PlayingIDNotFound if the playing ID is invalid (not playing yet, or finished playing).
	/// \sa 
	/// - \ref soundengine_query_pos
	/// - \ref concept_events
	virtual AKRESULT GetSourcePlayPosition(
		AkPlayingID		in_PlayingID,				///< Playing ID returned by <tt>AK::SoundEngine::PostEvent()</tt>
		AkTimeMs* out_puPosition,				///< Position of the source (in ms) associated with the specified playing ID
		bool			in_bExtrapolate = true		///< Position is extrapolated based on time elapsed since last sound engine update.
		) override;

	/// Gets the current position of the sources associated with this playing ID, obtained from PostEvent().
	/// Notes:
	/// - You need to pass AK_EnableGetSourcePlayPosition to PostEvent() in order to use this function, otherwise
	/// 	it returns AK_Fail, even if the playing ID is valid.
	/// - The source's position is updated at every audio frame, and the time at which this occurs is stored. 
	///		When you call this function from your thread, you therefore query the position that was updated in the previous audio frame.
	///		If in_bExtrapolate is true (default), the returned position is extrapolated using the elapsed time since last 
	///		sound engine update and the source's playback rate.
	/// - If 0 is passed in for the number of entries (*in_pcPositions == 0) then only the number of positions will be returned and the
	///     position array (out_puPositions) will not be updated.
	/// - The io_pcPositions pointer must be non-NULL.
	///     out_puPositions may be NULL if *io_pcPositions == 0, otherwise it must be non-NULL.
	/// \return 
	/// - \c AK_Success if successful.
	/// - \c AK_InvalidParameter if the provided pointer is not valid.
	/// - \c AK_PlayingIDNotFound if the playing ID is invalid (not playing yet, or finished playing).
	/// \sa 
	/// - \ref soundengine_query_pos
	/// - \ref concept_events
	virtual AKRESULT GetSourcePlayPositions(
		AkPlayingID		in_PlayingID,				///< Playing ID returned by <tt>AK::SoundEngine::PostEvent()</tt>
		AkSourcePosition* out_puPositions,			///< Audio Node IDs and positions of sources associated with the specified playing ID
		AkUInt32* io_pcPositions,				///< Number of entries in out_puPositions. Needs to be set to the size of the array: it is adjusted to the actual number of returned entries
		bool			in_bExtrapolate = true		///< Position is extrapolated based on time elapsed since last sound engine update
		) override;

	/// Gets the stream buffering of the sources associated with this playing ID, obtained from PostEvent().
	/// Notes:
	/// - You need to pass AK_EnableGetSourceStreamBuffering to PostEvent() in order to use this function, otherwise
	/// 	it returns AK_Fail, even if the playing ID is valid.
	/// - The sources stream buffering is updated at every audio frame. If there are multiple sources associated with this playing ID,
	///		the value returned corresponds to the least buffered source. 
	/// - The returned buffering status out_bIsBuffering will be true If any of the sources associated with the playing ID are actively being buffered.
	///		It will be false if all of them have reached the end of file, or have reached a state where they are buffered enough and streaming is temporarily idle.
	/// - Purely in-memory sources are excluded from this database. If all sources are in-memory, GetSourceStreamBuffering() will return AK_Fail.
	/// - The returned buffering amount and state is not completely accurate with some hardware-accelerated codecs. In such cases, the amount of stream buffering is generally underestimated.
	///		On the other hand, it is not guaranteed that the source will be ready to produce data at the next audio frame even if out_bIsBuffering has turned to false.
	/// \return 
	/// - \c AK_Success if successful.
	/// - \c AK_PlayingIDNotFound if the source data associated with this playing ID is not found, for example if PostEvent() was not called with AK_EnableGetSourceStreamBuffering, or if the header was not parsed.
	/// \sa 
	/// - \ref concept_events
	virtual AKRESULT GetSourceStreamBuffering(
		AkPlayingID		in_PlayingID,				///< Playing ID returned by <tt>AK::SoundEngine::PostEvent()</tt>
		AkTimeMs& out_buffering,				///< Returned amount of buffering (in ms) of the source (or one of the sources) associated with that playing ID
		bool& out_bIsBuffering			///< Returned buffering status of the source(s) associated with that playing ID
		) override;

	/// Stops the current content playing associated to the specified game object ID.
	/// If no game object is specified, all sounds will be stopped.
	virtual void StopAll(
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT ///< (Optional)Specify a game object to stop only playback associated to the provided game object ID.
		) override;

	/// Stop the current content playing associated to the specified playing ID.
	/// \aknote 
	///	This function is deprecated. Please use ExecuteActionOnPlayingID() in its place.
	/// \endaknote
	/// \sa
	///  - <tt>AK::SoundEngine::ExecuteActionOnPlayingID()</tt>
	virtual void StopPlayingID(
		AkPlayingID in_playingID,											///< Playing ID to be stopped.
		AkTimeMs in_uTransitionDuration = 0,								///< Fade duration
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear	///< Curve type to be used for the transition
		) override;

	/// Executes an Action on the content associated to the specified playing ID.
	/// \sa
	/// - <tt>AK::SoundEngine::AkActionOnEventType</tt>
	virtual void ExecuteActionOnPlayingID(
		AK::SoundEngine::AkActionOnEventType in_ActionType,					///< Action to execute on the specified playing ID.
		AkPlayingID in_playingID,											///< Playing ID on which to execute the action.
		AkTimeMs in_uTransitionDuration = 0,								///< Fade duration
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear	///< Curve type to be used for the transition
		) override;

	/// Sets the random seed value. Can be used to synchronize randomness
	/// across instances of the Sound Engine.
	/// \remark This seeds the number generator used for all container randomization 
	///         and the plug-in RNG; since it acts globally, this should be called right 
	///         before any PostEvent call where randomness synchronization is required,
	///         and cannot guarantee similar results for continuous containers.
	/// \sa
	/// - <tt>AK::IAkPluginServiceRNG</tt>
	virtual void SetRandomSeed(
		AkUInt32 in_uSeed													///< Random seed.
		) override;

	/// Mutes/Unmutes the busses tagged as background music.  
	/// This is automatically called for platforms that have user-music support.
	/// This function is provided to give the same behavior on platforms that don't have user-music support.
	virtual void MuteBackgroundMusic(
		bool in_bMute ///< Sets true to mute, false to unmute.
		) override;
	//@}

	/// Gets the state of the Background Music busses. This state is either set directly 
	/// with \c AK::SoundEngine::MuteBackgroundMusic or by the OS, if it has User Music services.
	/// \return true if the background music busses are muted, false if not.
	virtual bool GetBackgroundMusicMute() override;
	//@}


	/// Sends custom game data to a plug-in that resides on a bus (insert Effect or mixer plug-in).
	/// Data will be copied and stored into a separate list.
	/// Previous entry is deleted when a new one is sent.
	/// Sets the data pointer to NULL to clear item from the list.
	/// \aknote The plug-in type and ID is passed and matched with plugins set on the desired bus. 
	/// This means that you cannot send different data to various instances of the plug-in on a same bus.\endaknote
	/// \return AK_Success if data was sent successfully.
	virtual AKRESULT SendPluginCustomGameData(
		AkUniqueID in_busID,			///< Bus ID
		AkGameObjectID in_busObjectID,	///< Bus Object ID. Pass AK_INVALID_GAME_OBJECT to send custom data with global scope. Game object scope supersedes global scope, as with RTPCs. 
		AkPluginType in_eType,			///< Plug-in type (for example, source or effect)
		AkUInt32 in_uCompanyID,		///< Company identifier (as declared in the plug-in description XML file)
		AkUInt32 in_uPluginID,			///< Plug-in identifier (as declared in the plug-in description XML file)
		const void* in_pData,			///< The data blob
		AkUInt32 in_uSizeInBytes		///< Size of data
		) override;
	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Game Objects
	//@{

	/// Registers a game object.
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if the specified AkGameObjectID is invalid. Range 0xFFFFFFFFFFFFFFE0 (-32) to 0xFFFFFFFFFFFFFFFF (-1) are invalid inclusively.
	/// \remark Registering a game object twice does nothing. Unregistering it once unregisters it no 
	///			matter how many times it has been registered.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnregisterGameObj()</tt>
	/// - <tt>AK::SoundEngine::UnregisterAllGameObj()</tt>
	/// - \ref concept_gameobjects
	virtual AKRESULT RegisterGameObj(
		AkGameObjectID in_gameObjectID								///< ID of the game object to be registered. Valid range is [0 to 0xFFFFFFFFFFFFFFDF].
		) override;

	/// Registers a game object.
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if the specified AkGameObjectID is invalid. Range 0xFFFFFFFFFFFFFFE0 (-32) to 0xFFFFFFFFFFFFFFFF (-1) are invalid inclusively.
	/// \remark Registering a game object twice does nothing. Unregistering it once unregisters it no 
	///			matter how many times it has been registered.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnregisterGameObj()</tt>
	/// - <tt>AK::SoundEngine::UnregisterAllGameObj()</tt>
	/// - \ref concept_gameobjects
	virtual AKRESULT RegisterGameObj(
		AkGameObjectID in_gameObjectID,							///< ID of the game object to be registered. Valid range is [0 to 0xFFFFFFFFFFFFFFDF].
		const char* in_pszObjName								///< Name of the game object (for monitoring purpose)
		) override;

	/// Unregisters a game object.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if the specified AkGameObjectID is invalid. Range 0xFFFFFFFFFFFFFFE0 (-32) to 0xFFFFFFFFFFFFFFFF (-1) are invalid inclusively.
	/// \remark Registering a game object twice does nothing. Unregistering it once unregisters it no 
	///			matter how many times it has been registered. Unregistering a game object while it is 
	///			in use is allowed, but the control over the parameters of this game object is lost.
	///			For example, say a sound associated with this game object is a 3D moving sound. This sound will 
	///			stop moving when the game object is unregistered, and there will be no way to regain control over the game object.
	/// \sa 
	/// - <tt>AK::SoundEngine::RegisterGameObj()</tt>
	/// - <tt>AK::SoundEngine::UnregisterAllGameObj()</tt>
	/// - \ref concept_gameobjects
	virtual AKRESULT UnregisterGameObj(
		AkGameObjectID in_gameObjectID				///< ID of the game object to be unregistered. Valid range is [0 to 0xFFFFFFFFFFFFFFDF]. Use 
	       											/// AK_INVALID_GAME_OBJECT to unregister all game objects.
		) override;

	/// Unregister all game objects, or all game objects with a particular matching set of property flags.
	/// This function to can be used to unregister all game objects.  
	/// \return
	/// - \c AK_Success if successful
	/// \remark Registering a game object twice does nothing. Unregistering it once unregisters it no 
	///			matter how many times it has been registered. Unregistering a game object while it is 
	///			in use is allowed, but the control over the parameters of this game object is lost.
	///			For example, if a sound associated with this game object is a 3D moving sound, it will 
	///			stop moving once the game object is unregistered, and there will be no way to recover 
	///			the control over this game object.
	/// \sa 
	/// - <tt>AK::SoundEngine::RegisterGameObj()</tt>
	/// - <tt>AK::SoundEngine::UnregisterGameObj()</tt>
	/// - \ref concept_gameobjects
	virtual AKRESULT UnregisterAllGameObj(
		) override;

	/// Sets the position of a game object.
	/// \warning The object's orientation vector (in_Position.Orientation) must be normalized.
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidParameter if parameters are not valid, for example: 
	/// + in_Position makes an invalid transform 
	/// + in_eFlags is not one of the valid enum values 
	/// + the game object ID is in the reserved ID range.
	/// \sa 
	/// - \ref soundengine_3dpositions
	virtual AKRESULT SetPosition(
		AkGameObjectID in_GameObjectID,		///< Game Object identifier
		const AkSoundPosition& in_Position,///< Position to set; in_Position.Orientation must be normalized.
		AkSetPositionFlags in_eFlags = AkSetPositionFlags_Default ///< Optional flags to independently set the position of the emitter or listener component.
		) override;

	/// Sets multiple positions to a single game object.
	/// Setting multiple positions on a single game object is a way to simulate multiple emission sources while using the resources of only one voice.
	/// This can be used to simulate wall openings, area sounds, or multiple objects emitting the same sound in the same area.
	/// \aknote
	/// - Calling <tt>AK::SoundEngine::SetMultiplePositions()</tt> with only one position is the same as calling <tt>AK::SoundEngine::SetPosition()</tt> 
	/// - If a sound has diffraction enabled, it is treated as <tt>MultiPositionType_MultiDirections</tt>. <tt>MultiPositionType_MultiSources</tt> is not supported in this case.
	/// \endaknote
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_CommandTooLarge if the number of positions is too large for the command queue. Reduce the number of positions.
	/// - \c AK_InvalidParameter if parameters are not valid, for example: 
	/// + in_Position makes an invalid transform 
	/// + in_eFlags is not one of the valid enum values 
	/// + the game object ID is in the reserved ID range.
	/// \sa 
	/// - \ref soundengine_3dpositions
	/// - \ref soundengine_3dpositions_multiplepos
	/// - \ref AK::SoundEngine::MultiPositionType
	virtual AKRESULT SetMultiplePositions(
		AkGameObjectID in_GameObjectID,						///< Game Object identifier.
		const AkSoundPosition* in_pPositions,				///< Array of positions to apply.
		AkUInt16 in_NumPositions,							///< Number of positions specified in the provided array.
		AK::SoundEngine::MultiPositionType in_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiDirections, ///< \ref AK::SoundEngine::MultiPositionType
		AkSetPositionFlags in_eFlags = AkSetPositionFlags_Default ///< Optional flags to independently set the position of the emitter or listener component.
		) override;

	/// Sets multiple positions to a single game object, with flexible assignment of input channels.
	/// Setting multiple positions on a single game object is a way to simulate multiple emission sources while using the resources of only one voice.
	/// This can be used to simulate wall openings, area sounds, or multiple objects emitting the same sound in the same area.
	/// \aknote Calling <tt>AK::SoundEngine::SetMultiplePositions()</tt> with only one position is the same as calling <tt>AK::SoundEngine::SetPosition()</tt> \endaknote
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_CommandTooLarge if the number of positions is too large for the command queue. Reduce the number of positions.
	/// - \c AK_InvalidParameter if parameters are not valid.
	/// \sa 
	/// - \ref soundengine_3dpositions
	/// - \ref soundengine_3dpositions_multiplepos
	/// - \ref AK::SoundEngine::MultiPositionType
	virtual AKRESULT SetMultiplePositions(
		AkGameObjectID in_GameObjectID,						///< Game Object identifier.
		const AkChannelEmitter* in_pPositions,			///< Array of positions to apply, each using its own channel mask.
		AkUInt16 in_NumPositions,							///< Number of positions specified in the provided array.
		AK::SoundEngine::MultiPositionType in_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiDirections, ///< \ref AK::SoundEngine::MultiPositionType
		AkSetPositionFlags in_eFlags = AkSetPositionFlags_Default ///< Optional flags to independently set the position of the emitter or listener component.
		) override;

	/// Sets the scaling factor of a Game Object.
	/// Modify the attenuation computations on this Game Object to simulate sounds with a larger or smaller area of effect.
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidParameter if the scaling factor specified was 0 or negative.
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	virtual AKRESULT SetScalingFactor(
		AkGameObjectID in_GameObjectID,				///< Game object identifier
		AkReal32 in_fAttenuationScalingFactor		///< Scaling Factor, 1 means 100%, 0.5 means 50%, 2 means 200%, and so on.
		) override;

	/// Use the position of a separate game object for distance calculations for a specified listener.
	/// When <tt>AK::SoundEngine::SetDistanceProbe()</tt> is called, Wwise calculates distance attenuation and filtering 
	/// based on the distance between the distance probe Game Object (\c in_distanceProbeGameObjectID) and the emitter Game Object's position.
	/// In third-person perspective applications, the distance probe Game Object may be set to the player character's position, 
	/// and the listener Game Object's position to that of the camera. In this scenario, attenuation is based on 
	/// the distance between the character and the sound, whereas panning, spatialization, and spread and focus calculations are base on the camera.
	/// Both Game Objects, \c in_listenerGameObjectID and \c in_distanceProbeGameObjectID must have been previously registered using <tt>AK::SoundEngine::RegisterGameObj</tt>.
	/// This function is optional. if <tt>AK::SoundEngine::SetDistanceProbe()</tt> is never called, distance calculations are based on the listener Game Object position.
	/// To clear the distance probe, and revert to using the listener position for distance calculations, pass \c AK_INVALID_GAME_OBJECT to \c in_distanceProbeGameObjectID.
	/// \aknote If the distance probe Game Object is assigned multiple positions, then the first position is used for distance calculations by the listener. \endaknote
	/// \return 
	/// - \c AK_Success when successful
	/// \sa 
	/// - <tt>AK::SoundEngine::SetPosition()</tt>
	virtual AKRESULT SetDistanceProbe(
		AkGameObjectID in_listenerGameObjectID,		///< Game object identifier for the listener. Must have been previously registered via RegisterGameObj.
		AkGameObjectID in_distanceProbeGameObjectID	///< Game object identifier for the distance probe, or \c AK_INVALID_GAME_OBJECT to reset distance probe.  If valid, must have been previously registered via RegisterGameObj.
		) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Bank Management
	//@{

	/// Unload all currently loaded banks.
	/// It also internally calls ClearPreparedEvents() since at least one bank must have been loaded to allow preparing events.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized or if there is not enough memory to handle the command
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \ref soundengine_banks
	virtual AKRESULT ClearBanks() override;

	/// Sets the I/O settings of the bank load and prepare event processes.
	/// The sound engine uses default values unless explicitly set by calling this method.
	/// \warning This function must be called before loading banks.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console
	/// \sa 
	/// - \ref soundengine_banks
	/// - \ref streamingdevicemanager
	virtual AKRESULT SetBankLoadIOSettings(
		AkReal32            in_fThroughput,         ///< Average throughput of bank data streaming (bytes/ms) (the default value is AK_DEFAULT_BANK_THROUGHPUT)
		AkPriority          in_priority             ///< Priority of bank streaming (the default value is AK_DEFAULT_PRIORITY)
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Load a bank synchronously (by Unicode string).\n
	/// The bank name and type are passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console
	/// - \c AK_InvalidBankType if the bank type parameter is out of range.
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure, check the debug console)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The sound engine internally calls GetIDFromString(in_pszString) to return the correct bank ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. The path should be resolved in 
	/// your implementation of the Stream Manager, or in the Low-Level I/O module if you use the default Stream Manager's implementation.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref streamingdevicemanager
	/// - \ref streamingmanager_lowlevel
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		const wchar_t* in_pszString,					///< Name of the bank to load
		AkBankID& out_bankID,						///< Returned bank ID
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Loads a bank synchronously.\n
	/// The bank name and type are passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console
	/// - \c AK_InvalidBankType if the bank type parameter is out of range.
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The sound engine internally calls GetIDFromString(in_pszString) to return the correct bank ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. The path should be resolved in 
	/// your implementation of the Stream Manager, or in the Low-Level I/O module if you use the default Stream Manager's implementation.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref streamingdevicemanager
	/// - \ref streamingmanager_lowlevel
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		const char* in_pszString,					///< Name of the bank to load
		AkBankID& out_bankID,						///< Returned bank ID
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;

	/// Loads a bank synchronously (by ID).\n
	/// \aknote Requires that the "Use SoundBank names" option be unchecked in the Wwise Project Settings. \endaknote
	/// The bank ID is passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error. The bank is either shorter than expected or its data corrupted.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console or Wwise Profiler
	/// - \c AK_InvalidBankType if the bank type parameter is out of range.
	/// - \c AK_FileNotFound if the bank file was not found on disk.
	/// - \c AK_FilePermissionError if the file permissions are wrong for the file
	/// - \c AK_Fail: Load or unload failed for any other reason. , check the debug console or Wwise Profiler
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		AkBankID			in_bankID,						///< Bank ID of the bank to load
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;

	/// Loads a bank synchronously (from in-memory data, in-place, user bank only).\n
	///
	/// IMPORTANT: Banks loaded from memory with in-place data MUST be unloaded using the UnloadBank function
	/// providing the same memory pointer. Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryView when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine.
	/// In-memory loading is in-place: *** the memory must be valid until the bank is unloaded. ***
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is stored in the first few bytes of the bank file. You may use this 
	/// ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console 
	/// - \c AK_InvalidBankType if the bank is not a user-defined bank.
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The memory must be aligned on platform-specific AK_BANK_PLATFORM_DATA_ALIGNMENT bytes (see AkTypes.h).
	/// - (XboxOne only): If the bank may contain XMA in memory data, the memory must be allocated using the Device memory allocator.
	/// - Avoid using this function for banks containing a lot of events or structure data: this data will be unpacked into the sound engine heap,
	///   making the supplied bank memory redundant. For event/structure-only banks, prefer LoadBankMemoryCopy().
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryView(
        const void *		in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
        AkBankID &          out_bankID				///< Returned bank ID
        ) override;

	/// Loads a bank synchronously (from in-memory data, in-place, any bank type).\n
	///
	/// IMPORTANT: Banks loaded from memory with in-place data MUST be unloaded using the UnloadBank function
	/// providing the same memory pointer. Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryView when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine.
	/// In-memory loading is in-place: *** the memory must be valid until the bank is unloaded. ***
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is stored in the first few bytes of the bank file. You may use this 
	/// ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console 
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The memory must be aligned on platform-specific AK_BANK_PLATFORM_DATA_ALIGNMENT bytes (see AkTypes.h).
	/// - (XboxOne only): If the bank may contain XMA in memory data, the memory must be allocated using the Device memory allocator.
	/// - Avoid using this function for banks containing a lot of events or structure data: this data will be unpacked into the sound engine heap,
	///   making the supplied bank memory redundant. For event/structure-only banks, prefer LoadBankMemoryCopy().
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryView(
		const void* in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankID& out_bankID,				///< Returned bank ID
		AkBankType& out_bankType			///< Returned bank type
		) override;

	/// Loads a bank synchronously (from in-memory data, out-of-place, user bank only).\n
	///
	/// NOTE: Banks loaded from in-memory with out-of-place data must be unloaded using the standard UnloadBank function
	/// (with no memory pointer). Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryCopy when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine, the media section of the bank will be copied into newly 
	/// allocated memory.  
	/// In-memory loading is out-of-place: the buffer can be release as soon as the function returns. The advantage of using this
	/// over the in-place version is that there is no duplication of bank structures.
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is stored in the first few bytes of the bank file. You may use this 
	/// ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console
	/// - \c AK_InvalidBankType if the bank is not a user-defined bank.
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryCopy(
		const void *		in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after return)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankID &          out_bankID				///< Returned bank ID
		) override;

	/// Loads a bank synchronously (from in-memory data, out-of-place, any bank type).\n
	///
	/// NOTE: Banks loaded from in-memory with out-of-place data must be unloaded using the standard UnloadBank function
	/// (with no memory pointer). Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryCopy when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine, the media section of the bank will be copied into newly 
	/// allocated memory.  
	/// In-memory loading is out-of-place: the buffer can be release as soon as the function returns. The advantage of using this
	/// over the in-place version is that there is no duplication of bank structures.
	/// A bank load request will be posted, and consumed by the Bank Manager thread.
	/// The function returns when the request has been completely processed.
	/// \return 
	/// The bank ID, which is stored in the first few bytes of the bank file. You may use this 
	/// ID with UnloadBank().
	/// - \c AK_Success: Load or unload successful.
	/// - \c AK_BankAlreadyLoaded: This bank is already loaded, nothing done.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_NotInitialized if the sound engine was not correctly initialized
	/// - \c AK_InvalidParameter if some parameters are invalid, check the debug console
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryCopy(
		const void* in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after return)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankID& out_bankID,				///< Returned bank ID
		AkBankType& out_bankType			///< Returned bank type
		) override;

	/// Synchronously decodes Vorbis-encoded and Opus-encoded (Software version) media in a SoundBank. The file should already be read in memory before the decode operation. The out_pDecodedBankPtr can then be used with variants of LoadBank that load from in-memory data.
	/// \n
	/// CPU usage, RAM size, storage size and Internet bandwidth must be accounted for when developing a game, especially when it is aimed at mobile platforms. The DecodeBank function makes it possible to decode media at load time instead of decoding them every time they are played.
	virtual AKRESULT DecodeBank(
		const void* in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to decode (pointer is not stored in sound engine, memory can be released after return)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to decode
		AkMemPoolId			in_uPoolForDecodedBank,	///< Memory pool to allocate decoded bank into. Specify AK_INVALID_POOL_ID and out_pDecodedBankPtr=NULL to obtain decoded bank size without performing the decode operation. Pass AK_INVALID_POOL_ID and out_pDecodedBankPtr!=NULL to decode bank into specified pointer.
		void*& out_pDecodedBankPtr,	///< Decoded bank memory location.
		AkUInt32& out_uDecodedBankSize	///< Decoded bank memory size.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Loads a bank asynchronously (by Unicode string).\n
	/// The bank name is passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// AK_Success if the scheduling was successful, AK_Fail otherwise.
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The sound engine internally calls GetIDFromString(in_pszString) to return the correct bank ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. The path should be resolved in 
	/// your implementation of the Stream Manager (<tt>AK::IAkStreamMgr::CreateStd()</tt>), or in the Low-Level I/O module 
	/// (<tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt>) if you use the default Stream Manager's implementation.
	/// - The cookie (in_pCookie) is passed to the Low-Level I/O module for your convenience, in <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt> 
	// as AkFileSystemFlags::pCustomParam.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref streamingdevicemanager
	/// - \ref streamingmanager_lowlevel
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		const wchar_t* in_pszString,					///< Name/path of the bank to load
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void* in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function, and also to  <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> as AkFileSystemFlags::pCustomParam)
		AkBankID& out_bankID,						///< Returned bank ID
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Loads a bank asynchronously.\n
	/// The bank name is passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_InvalidBankType if in_bankType was invalid
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the Event will fail.
	/// - The sound engine internally calls GetIDFromString(in_pszString) to return the correct bank ID.
	/// Therefore, \c in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. The path should be resolved in 
	/// your implementation of the Stream Manager (<tt>AK::IAkStreamMgr::CreateStd()</tt>), or in the Low-Level I/O module 
	/// (<tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt>) if you use the default Stream Manager's implementation.
	/// - The cookie (in_pCookie) is passed to the Low-Level I/O module for your convenience, in <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt> 
	// as <tt>AkFileSystemFlags::pCustomParam</tt>.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref streamingdevicemanager
	/// - \ref streamingmanager_lowlevel
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		const char* in_pszString,					///< Name/path of the bank to load
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void* in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function, and also to  <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> as AkFileSystemFlags::pCustomParam)
		AkBankID& out_bankID,						///< Returned bank ID
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;

	/// Loads a bank asynchronously (by ID).\n
	/// \aknote Requires that the "Use SoundBank names" option be unchecked in the Wwise Project Settings. \endaknote
	/// The bank ID is passed to the Stream Manager.
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_InvalidBankType if in_bankType was invalid
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with \c UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The file path should be resolved in your implementation of the Stream Manager, or in the Low-Level I/O module if 
	/// you use the default Stream Manager's implementation. The ID overload of <tt>AK::IAkStreamMgr::CreateStd()</tt> and <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt> are called.
	/// - The cookie (in_pCookie) is passed to the Low-Level I/O module for your convenience, in <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt> 
	// as AkFileSystemFlags::pCustomParam.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBank(
		AkBankID			in_bankID,						///< Bank ID of the bank to load
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void* in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function, and also to  <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> as AkFileSystemFlags::pCustomParam)
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to load
		) override;

	/// Loads a bank asynchronously (from in-memory data, in-place, user bank only).\n
	///
	/// IMPORTANT: Banks loaded from memory with in-place data MUST be unloaded using the UnloadBank function
	/// providing the same memory pointer. Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryView when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine.
	/// In-memory loading is in-place: *** the memory must be valid until the bank is unloaded. ***
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_InvalidBankType if the bank is not a user-defined bank.
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The memory must be aligned on platform-specific AK_BANK_PLATFORM_DATA_ALIGNMENT bytes (see AkTypes.h).
	/// - (XboxOne only): If the bank may contain XMA in memory data, the memory must be allocated using the Device memory allocator.
	/// - Avoid using this function for banks containing a lot of events or structure data: this data will be unpacked into the sound engine heap,
	///   making the supplied bank memory redundant. For event/structure-only banks, prefer LoadBankMemoryCopy().
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryView(
		const void* in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankCallbackFunc  in_pfnBankCallback,	    ///< Callback function
		void* in_pCookie,				///< Callback cookie
		AkBankID &          out_bankID				///< Returned bank ID
	    ) override;

	/// Loads a bank asynchronously (from in-memory data, in-place, any bank type).\n
	///
	/// IMPORTANT: Banks loaded from memory with in-place data MUST be unloaded using the UnloadBank function
	/// providing the same memory pointer. Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryView when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine.
	/// In-memory loading is in-place: *** the memory must be valid until the bank is unloaded. ***
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// - The memory must be aligned on platform-specific AK_BANK_PLATFORM_DATA_ALIGNMENT bytes (see AkTypes.h).
	/// - (XboxOne only): If the bank may contain XMA in memory data, the memory must be allocated using the Device memory allocator.
	/// - Avoid using this function for banks containing a lot of events or structure data: this data will be unpacked into the sound engine heap,
	///   making the supplied bank memory redundant. For event/structure-only banks, prefer LoadBankMemoryCopy().
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryView(
		const void *		in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankCallbackFunc  in_pfnBankCallback,	    ///< Callback function
		void *              in_pCookie,				///< Callback cookie
		AkBankID &          out_bankID,				///< Returned bank ID
		AkBankType &		out_bankType			///< Returned bank type
        ) override;

	/// Loads a bank asynchronously (from in-memory data, out-of-place, user bank only).\n
	///
	/// NOTE: Banks loaded from in-memory with out-of-place data must be unloaded using the standard UnloadBank function
	/// (with no memory pointer). Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryCopy when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine, the media section of the bank will be copied into newly allocated 
	/// memory.
	/// In-memory loading is out-of-place: the buffer can be released after the callback function is called. The advantage of using this
	/// over the in-place version is that there is no duplication of bank structures.
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_InvalidBankType if the bank is not a user-defined bank.
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryCopy(
		const void* in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after callback)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankCallbackFunc  in_pfnBankCallback,	    ///< Callback function
		void* in_pCookie,				///< Callback cookie
		AkBankID& out_bankID				///< Returned bank ID
		) override;

	/// Loads a bank asynchronously (from in-memory data, out-of-place, any bank type).\n
	///
	/// NOTE: Banks loaded from in-memory with out-of-place data must be unloaded using the standard UnloadBank function
	/// (with no memory pointer). Make sure you are using the correct UnloadBank(...) overload
	///
	/// Use LoadBankMemoryCopy when you want to manage I/O on your side. Load the bank file
	/// in a buffer and pass its address to the sound engine, the media section of the bank will be copied into newly allocated 
	/// memory.
	/// In-memory loading is out-of-place: the buffer can be released after the callback function is called. The advantage of using this
	/// over the in-place version is that there is no duplication of bank structures.
	/// A bank load request will be posted to the Bank Manager consumer thread.
	/// The function returns immediately.
	/// \return 
	/// - \c AK_Success if the scheduling was successful,
	/// - \c AK_InvalidBankType if in_bankType was invalid
	/// - \c AK_DataAlignmentError if the data pointer is not aligned properly 
	/// Use a callback to be notified when completed, and get the status of the request.
	/// The bank ID, which is obtained by hashing the bank name (see GetIDFromString()). 
	/// You may use this ID with UnloadBank().
	/// \remarks
	/// - The initialization bank must be loaded first.
	/// - All SoundBanks subsequently loaded must come from the same Wwise project as the
	///   initialization bank. If you need to load SoundBanks from a different project, you
	///   must first unload ALL banks, including the initialization bank, then load the
	///   initialization bank from the other project, and finally load banks from that project.
	/// - Codecs and plug-ins must be registered before loading banks that use them.
	/// - Loading a bank referencing an unregistered plug-in or codec will result in a load bank success,
	/// but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in 
	/// will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, 
	/// posting the event will fail.
	/// \sa 
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref integrating_elements_plugins
	/// - \ref sdk_bank_training
	virtual AKRESULT LoadBankMemoryCopy(
		const void *		in_pInMemoryBankPtr,	///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after callback)
		AkUInt32			in_uInMemoryBankSize,	///< Size of the in-memory bank to load
		AkBankCallbackFunc  in_pfnBankCallback,	    ///< Callback function
		void *              in_pCookie,				///< Callback cookie
		AkBankID &          out_bankID,				///< Returned bank ID
		AkBankType &		out_bankType			///< Returned bank type
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Unloads a bank synchronously (by Unicode string).\n
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// \return AK_Success if successful, AK_Fail otherwise. AK_Success is returned when the bank was not loaded.
	/// \remarks
	/// - The sound engine internally calls GetIDFromString(in_pszString) to retrieve the bank ID, 
	/// then it calls the synchronous version of UnloadBank() by ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. 
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        const wchar_t*      in_pszString,					///< Name of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Unloads a bank synchronously.\n
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// \return AK_Success if successful, AK_Fail otherwise. AK_Success is returned when the bank was not loaded.
	/// \remarks
	/// - The sound engine internally calls GetIDFromString(in_pszString) to retrieve the bank ID, 
	/// then it calls the synchronous version of UnloadBank() by ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. 
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        const char*         in_pszString,					///< Name of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;

	/// Unloads a bank synchronously (by ID and memory pointer).\n
	/// \return AK_Success if successful, AK_Fail otherwise. AK_Success is returned when the bank was not loaded.
	/// \remarks
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        AkBankID            in_bankID,						///< ID of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Unloads a bank asynchronously (by Unicode string).\n
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// \return AK_Success if scheduling successful (use a callback to be notified when completed)
	/// \remarks
	/// The sound engine internally calls GetIDFromString(in_pszString) to retrieve the bank ID, 
	/// then it calls the synchronous version of UnloadBank() by ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. 
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        const wchar_t*      in_pszString,					///< Name of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void *              in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function)
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Unloads a bank asynchronously.\n
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// \return AK_Success if scheduling successful (use a callback to be notified when completed)
	/// \remarks
	/// The sound engine internally calls GetIDFromString(in_pszString) to retrieve the bank ID, 
	/// then it calls the synchronous version of UnloadBank() by ID.
	/// Therefore, in_pszString should be the real name of the SoundBank (with or without the BNK extension - it is trimmed internally),
	/// not the name of the file (if you changed it), nor the full path of the file. 
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        const char*         in_pszString,				    ///< Name of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void *              in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function)
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;

	/// Unloads a bank asynchronously (by ID and memory pointer).\n
	/// Refer to \ref soundengine_banks_general for a discussion on using strings and IDs.
	/// \return AK_Success if scheduling successful (use a callback to be notified when completed)
	/// \remarks
	/// - In order to force the memory deallocation of the bank, sounds that use media from this bank will be stopped. 
	/// This means that streamed sounds or generated sounds will not be stopped.
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	virtual AKRESULT UnloadBank(
        AkBankID            in_bankID,						///< ID of the bank to unload
		const void *		in_pInMemoryBankPtr,			///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
		AkBankCallbackFunc  in_pfnBankCallback,				///< Callback function
		void *              in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function)
		AkBankType			in_bankType = AkBankType_User	///< Type of the bank to unload
		) override;

	/// Cancels all Event callbacks associated with a specific callback cookie specified while loading Banks of preparing Events.\n
	/// \sa 
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::UnloadBank()</tt>
	/// - <tt>AK::SoundEngine::ClearBanks()</tt>
	/// - AkBankCallbackFunc
	virtual void CancelBankCallbackCookie(
		void* in_pCookie 							///< Callback cookie to be canceled
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// This function will load the Events, structural content, and optionally, the media content from the SoundBank. If the flag AkBankContent_All is specified, PrepareBank() will load the media content from 
	/// the bank; but, as opposed to LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using PrepareBank(), alone or in combination with PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType</tt>
	/// \remarks
	/// PrepareBank(), when called with the flag AkBankContent_StructureOnly, requires additional calls to PrepareEvent() to load the media for each event. PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///	for example, a game may have a tool mode that uses PrepareEvent() to load loose files on-demand and, also, a game mode that uses LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		const wchar_t* in_pszString,				///< Name of the bank to Prepare/Unprepare.
		AK::SoundEngine::AkBankContent	in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType						in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// This function will load the Events, structural content, and optionally, the media content from the SoundBank. If the flag AkBankContent_All is specified, PrepareBank() will load the media content from 
	/// the bank; but, as opposed to LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using PrepareBank(), alone or in combination with PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType</tt>
	/// \remarks
	/// \c PrepareBank(), when called with the flag \c AkBankContent_StructureOnly, requires additional calls to \c PrepareEvent() to load the media for each event. \c PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///		for example, a game may have a tool mode that uses PrepareEvent() to load loose files on-demand and, also, a game mode that uses \c LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		const char* in_pszString,				///< Name of the bank to Prepare/Unprepare.
		AK::SoundEngine::AkBankContent	in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType						in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;

	/// \n\aknote Requires that the "Use SoundBank names" option be unchecked in the Wwise Project Settings. \endaknote
	/// This function will load the events, structural content, and optionally, the media content from the SoundBank. If the flag AkBankContent_All is specified, PrepareBank() will load the media content from 
	/// the bank, but as opposed to LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using PrepareBank(), alone or in combination with PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType</tt>
	/// \remarks
	/// \c PrepareBank(), when called with the flag AkBankContent_StructureOnly, requires additional calls to PrepareEvent() to load the media for each event. PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///		for example, a game may have a tool mode that uses PrepareEvent() to load loose files on-demand and, also, a game mode that uses LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType	in_PreparationType,				///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkBankID							in_bankID,						///< ID of the bank to Prepare/Unprepare.
		AK::SoundEngine::AkBankContent		in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType							in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// This function will load the Events, structural content, and optionally, the media content from the SoundBank. If the flag AkBankContent_All is specified, PrepareBank() will load the media content from 
	/// the bank, but as opposed to LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using PrepareBank(), alone or in combination with PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType</tt>
	/// \remarks
	/// PrepareBank(), when called with the flag AkBankContent_StructureOnly, requires additional calls to PrepareEvent() to load the media for each event. PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///		for example, a game may have a tool mode that uses PrepareEvent() to load loose files on-demand and, also, a game mode that uses LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		const wchar_t* in_pszString,								///< Name of the bank to Prepare/Unprepare.
		AkBankCallbackFunc	in_pfnBankCallback,							///< Callback function
		void* in_pCookie,									///< Callback cookie (reserved to user, passed to the callback function)
		AK::SoundEngine::AkBankContent	in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType						in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// This function will load the events, structural content, and optionally, the media content from the SoundBank. If the flag \c AkBankContent_All is specified, \c PrepareBank() will load the media content from 
	/// the bank, but as opposed to \c LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using \c PrepareBank(), alone or in combination with PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType()</tt>
	/// \remarks
	/// PrepareBank(), when called with the flag AkBankContent_StructureOnly, requires additional calls to PrepareEvent() to load the media for each event. PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///		for example, a game may have a tool mode that uses PrepareEvent() to load loose files on-demand and, also, a game mode that uses LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		const char* in_pszString,								///< Name of the bank to Prepare/Unprepare.
		AkBankCallbackFunc	in_pfnBankCallback,							///< Callback function
		void* in_pCookie,									///< Callback cookie (reserved to user, passed to the callback function)
		AK::SoundEngine::AkBankContent	in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType						in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;

	/// \n\aknote Requires that the "Use SoundBank names" option be unchecked in the Wwise Project Settings. \endaknote
	/// This function will load the events, structural content, and optionally, the media content from the SoundBank. If the flag AkBankContent_All is specified, \c PrepareBank() will load the media content from 
	/// the bank, but as opposed to \c LoadBank(), the media will be loaded one media item at a time instead of in one contiguous memory block. Using \c PrepareBank(), alone or in combination with \c PrepareEvent(), 
	/// will prevent in-memory duplication of media at the cost of some memory fragmentation.
	/// Calling this function specifying the flag AkBankContent_StructureOnly will load only the structural part (including events) of this bank, 
	/// allowing using PrepareEvent() to load media on demand.  
	/// \sa 
	/// - \ref soundengine_banks_preparingbanks
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - <tt>AK::SoundEngine::PreparationType()</tt>
	/// \remarks
	/// \c PrepareBank(), when called with the flag AkBankContent_StructureOnly, requires additional calls to PrepareEvent() to load the media for each event. \c PrepareEvent(), however, is unable to 
	///		access media content contained within SoundBanks and requires that the media be available as loose files in the file system. This flag may be useful to implement multiple loading configurations;
	///		for example, a game may have a tool mode that uses \c PrepareEvent() to load loose files on-demand and, also, a game mode that uses \c LoadBank() to load the bank in entirety.
	virtual AKRESULT PrepareBank(
		AK::SoundEngine::PreparationType		in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkBankID			in_bankID,						///< ID of the bank to Prepare/Unprepare.
		AkBankCallbackFunc	in_pfnBankCallback,				///< Callback function
		void* in_pCookie,						///< Callback cookie (reserved to user, passed to the callback function)
		AK::SoundEngine::AkBankContent	in_uFlags = AK::SoundEngine::AkBankContent_All,	///< Structures only (including events) or all content.
		AkBankType						in_bankType = AkBankType_User	///< Type of the bank to Prepare/Unprepare.
		) override;

	/// Clear all previously prepared events.\n
	/// \return
	/// - \c AK_Success if successful.
	/// - \c AK_Fail if the sound engine was not correctly initialized or if there is not enough memory to handle the command.
	/// \remarks
	/// The function \c ClearBanks() also clears all prepared events.
	/// \sa
	/// - \c <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - \c <tt>AK::SoundEngine::ClearBanks()</tt>
	virtual AKRESULT ClearPreparedEvents() override;

#ifdef AK_SUPPORT_WCHAR
	/// Prepares or unprepares Events synchronously (by Unicode string).\n
	/// The Events are identified by strings, and converted to IDs internally
	/// (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking \c PrepareEvent(), use \c LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however,\c  PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns when the request is completely processed.
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareEvent() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// Whenever at least one event fails to be resolved, the actions performed for all 
	/// other events are cancelled.
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		const wchar_t** in_ppszString,			///< Array of event names
		AkUInt32			in_uNumEvent			///< Number of events in the array
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Prepares or unprepares events synchronously.\n
	/// The Events are identified by strings and converted to IDs internally
	/// (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking PrepareEvent(), use LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however, PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns when the request is completely processed.
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareEvent() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// Whenever at least one event fails to be resolved, the actions performed for all 
	/// other events are cancelled.
	/// \aknote  The use of PrepareEvent is incompatible with LoadBank, using in-memory data.
	/// \endaknote
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		const char** in_ppszString,			///< Array of event names
		AkUInt32			in_uNumEvent			///< Number of events in the array
		) override;

	/// Prepares or unprepares events synchronously (by ID).
	/// The Events are identified by their ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking PrepareEvent(), use LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however, PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns when the request is completely processed.
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareEvent() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// Whenever at least one event fails to be resolved, the actions performed for all 
	/// other events are cancelled.
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkUniqueID* in_pEventID,			///< Array of event IDs
		AkUInt32			in_uNumEvent			///< Number of event IDs in the array
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Prepares or unprepares an event asynchronously (by Unicode string).
	/// The Events are identified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking PrepareEvent(), use LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however, \c PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// Whenever at least one Event fails to be resolved, the actions performed for all 
	/// other Events are cancelled.
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		const wchar_t** in_ppszString,			///< Array of event names
		AkUInt32			in_uNumEvent,			///< Number of events in the array
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Prepares or unprepares an event asynchronously.
	/// The Events are identified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking PrepareEvent(), use LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however, PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// Whenever at least one event fails to be resolved, the actions performed for all 
	/// other events are cancelled.
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		const char** in_ppszString,			///< Array of event names 
		AkUInt32			in_uNumEvent,			///< Number of events in the array
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;

	/// Prepares or unprepares events asynchronously (by ID).\n
	/// The Events are identified by their ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// Before invoking PrepareEvent(), use LoadBank() to explicitly load the SoundBank(s) 
	/// that contain the Events and structures. When a request is posted to the
	/// Bank Manager consumer thread, it will resolve all dependencies needed to 
	/// successfully post the specified Events and load the required loose media files. 
	/// \aknote Before version 2015.1, the required media files could be included
	/// in a separate media SoundBank. As described in \ref whatsnew_2015_1_migration,
	/// however, PrepareEvent() now only looks for loose media files.
	/// \endaknote
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// Whenever at least one event fails to be resolved, the actions performed for all 
	/// other events are cancelled.
	/// \sa
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::ClearPreparedEvents()</tt>
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareEvent(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkUniqueID* in_pEventID,			///< Array of event IDs
		AkUInt32			in_uNumEvent,			///< Number of event IDs in the array
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;

	/// Indicates the location of a specific Media ID in memory
	/// The sources are identified by their ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// \return AK_Success if operation was successful, AK_InvalidParameter if in_pSourceSettings is invalid or media sizes are 0.
	virtual AKRESULT SetMedia(
		AkSourceSettings* in_pSourceSettings,		///< Array of Source Settings
		AkUInt32			in_uNumSourceSettings	///< Number of Source Settings in the array
		) override;

	/// Removes the specified source from the list of loaded media, even if this media is already in use.
	/// The sources are identified by their ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// \aknote This function is unsafe and deprecated. Use TryUnsetMedia() in its place.
	/// Media that is still in use by the sound engine should not be unset by this function. 
	/// If the media is still in use, this function will attempt to forcibly kill all sounds and effects referencing this media,
	/// and then return AK_ResourceInUse. The client should NOT presume that the memory can be safely released at this point.
	/// The moment at which the memory can be safely released is unknown, and the only safe course of action is to keep the memory
	/// alive until the sound engine is terminated.
	/// \endaknote
	/// \return
	/// - \c AK_Success: Operation was successful, and the memory can be released on the client side.
	/// - \c AK_ResourceInUse: Specified media is still in use by the sound engine, the client should not release the memory.
	/// - \c AK_InvalidParameter: in_pSourceSettings is invalid
	virtual AKRESULT UnsetMedia(
		AkSourceSettings* in_pSourceSettings,		///< Array of Source Settings
		AkUInt32			in_uNumSourceSettings	///< Number of Source Settings in the array
		) override;

	/// Removes the specified source from the list of loaded media, only if this media is not already in use.
	/// The sources are identified by their ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// \aknote Media that is still in use by the sound engine should not be unset. It is marked for removal to prevent additional use.
	/// If this function returns AK_ResourceInUse, then the client must not release memory for this media.
	/// Instead, the client should retry the TryUnsetMedia operation later with the same parameters and check for AK_Success.
	/// \endaknote
	/// If out_pUnsetResults is not null, then it is assumed to point to an array of result codes of the same length as in_pSourceSettings.
	/// out_pUnsetResults will be filled with either AK_Success or AK_ResourceInUse, indicating which media was still in use and not unset.
	/// \return
	/// - \c AK_Success: Operation was successful, and the memory can be released on the client side.
	/// - \c AK_ResourceInUse: Specified media is still in use by the sound engine, and the media was not unset. Do not release memory, and try again later.
	/// - \c AK_InvalidParameter: in_pSourceSettings is invalid
	virtual AKRESULT TryUnsetMedia(
		AkSourceSettings* in_pSourceSettings,       ///< Array of Source Settings
		AkUInt32          in_uNumSourceSettings,    ///< Number of Source Settings in the array
		AKRESULT* out_pUnsetResults         ///< (optional, can be null) Array of result codes
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Prepares or unprepares game syncs synchronously (by Unicode string).\n
	/// The group and game syncs are specified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with LoadBank()).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns when the request has been completely processed. 
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareGameSyncs() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// You need to call PrepareGameSyncs() if the sound engine was initialized with AkInitSettings::bEnableGameSyncPreparation 
	/// set to true. When set to false, the sound engine automatically prepares all game syncs when preparing events,
	/// so you never need to call this function.
	/// \sa 
	/// - \c <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - \c <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - \c <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \c AkInitSettings
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType		in_eGameSyncType,			///< The type of game sync.
		const wchar_t* in_pszGroupName,			///< The State Group Name or the Switch Group Name.
		const wchar_t** in_ppszGameSyncName,		///< The specific ID of the state to either support or not support.
		AkUInt32		in_uNumGameSyncs			///< The number of game sync in the string array.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Prepares or unprepares game syncs synchronously.\n
	/// The group and game syncs are specified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with LoadBank()).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns when the request has been completely processed. 
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareGameSyncs() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// You need to call PrepareGameSyncs() if the sound engine was initialized with AkInitSettings::bEnableGameSyncPreparation 
	/// set to true. When set to false, the sound engine automatically prepares all game syncs when preparing events,
	/// so you never need to call this function.
	/// \sa 
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkInitSettings
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType		in_eGameSyncType,			///< The type of game sync.
		const char* in_pszGroupName,			///< The State Group Name or the Switch Group Name.
		const char** in_ppszGameSyncName,		///< The specific ID of the state to either support or not support.
		AkUInt32		in_uNumGameSyncs			///< The number of game sync in the string array.
		) override;

	/// Prepares or unprepares game syncs synchronously (by ID).\n
	/// The group and game syncs are specified by ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with LoadBank()).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns when the request has been completely processed. 
	/// \return 
	/// - \c AK_Success: Prepare/un-prepare successful.
	/// - \c AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareGameSyncs() does not exist.
	/// - \c AK_InsufficientMemory: Insufficient memory to store bank data.
	/// - \c AK_BankReadError: I/O error.
	/// - \c AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
	/// you used to generate the SoundBanks matches that of the SDK you are currently using.
	/// - \c AK_InvalidFile: File specified could not be opened.
	/// - \c AK_InvalidParameter: Invalid parameter, invalid memory alignment.		
	/// - \c AK_Fail: Load or unload failed for any other reason. (Most likely small allocation failure)
	/// \remarks
	/// You need to call \c PrepareGameSyncs() if the sound engine was initialized with \c AkInitSettings::bEnableGameSyncPreparation 
	/// set to \c true. When set to \c false, the sound engine automatically prepares all game syncs when preparing Events,
	/// so you never need to call this function.
	/// \sa 
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkInitSettings
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType	in_PreparationType,			///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType		in_eGameSyncType,			///< The type of game sync.
		AkUInt32		in_GroupID,					///< The State Group ID or the Switch Group ID.
		AkUInt32* in_paGameSyncID,			///< Array of ID of the game syncs to either support or not support.
		AkUInt32		in_uNumGameSyncs			///< The number of game sync ID in the array.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Prepares or unprepares game syncs asynchronously (by Unicode string).\n
	/// The group and game syncs are specified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with <tt>LoadBank()</tt>).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// You need to call \c PrepareGameSyncs() if the sound engine was initialized with \c AkInitSettings::bEnableGameSyncPreparation 
	/// set to \c true. When set to \c false, the sound engine automatically prepares all game syncs when preparing Events,
	/// so you never need to call this function.
	/// \sa 
	/// - \c <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - \c <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - \c <tt>AK::SoundEngine::LoadBank()</tt>
	/// - \c AkInitSettings
	/// - \c AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType			in_eGameSyncType,		///< The type of game sync.
		const wchar_t* in_pszGroupName,		///< The State Group Name or the Switch Group Name.
		const wchar_t** in_ppszGameSyncName,	///< The specific ID of the state to either support or not support.
		AkUInt32			in_uNumGameSyncs,		///< The number of game sync in the string array.
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Prepares or unprepares game syncs asynchronously.\n
	/// The group and game syncs are specified by string (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with LoadBank()).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// You need to call PrepareGameSyncs() if the sound engine was initialized with AkInitSettings::bEnableGameSyncPreparation 
	/// set to true. When set to false, the sound engine automatically prepares all game syncs when preparing events,
	/// so you never need to call this function.
	/// \sa 
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkInitSettings
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType			in_eGameSyncType,		///< The type of game sync.
		const char* in_pszGroupName,		///< The State Group Name or the Switch Group Name.
		const char** in_ppszGameSyncName,	///< The specific ID of the state to either support or not support.
		AkUInt32			in_uNumGameSyncs,		///< The number of game sync in the string array.
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;

	/// Prepares or un-prepare game syncs asynchronously (by ID).\n
	/// The group and game syncs are specified by ID (refer to \ref soundengine_banks_general for a discussion on using strings and IDs).
	/// The game syncs definitions must already exist in the sound engine by having
	/// explicitly loaded the bank(s) that contain them (with LoadBank()).
	/// A request is posted to the Bank Manager consumer thread. It will resolve all 
	/// dependencies needed to successfully set this game sync group to one of the
	/// game sync values specified, and load the required banks, if applicable. 
	/// The function returns immediately. Use a callback to be notified when the request has finished being processed.
	/// \return AK_Success if scheduling is was successful, AK_Fail otherwise.
	/// \remarks
	/// You need to call PrepareGameSyncs() if the sound engine was initialized with AkInitSettings::bEnableGameSyncPreparation 
	/// set to true. When set to false, the sound engine automatically prepares all Game Syncs when preparing Events,
	/// so you never need to call this function.
	/// \sa 
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::PrepareEvent()</tt>
	/// - <tt>AK::SoundEngine::LoadBank()</tt>
	/// - AkInitSettings
	/// - AkBankCallbackFunc
	/// - \ref soundengine_banks
	/// - \ref sdk_bank_training
	virtual AKRESULT PrepareGameSyncs(
		AK::SoundEngine::PreparationType		in_PreparationType,		///< Preparation type ( Preparation_Load or Preparation_Unload )
		AkGroupType			in_eGameSyncType,		///< The type of game sync.
		AkUInt32			in_GroupID,				///< The State Group ID or the Switch Group ID.
		AkUInt32* in_paGameSyncID,		///< Array of ID of the Game Syncs to either support or not support.
		AkUInt32			in_uNumGameSyncs,		///< The number of game sync ID in the array.
		AkBankCallbackFunc	in_pfnBankCallback,		///< Callback function
		void* in_pCookie				///< Callback cookie (reserved to user, passed to the callback function)
		) override;

	//@}


	////////////////////////////////////////////////////////////////////////
	/// @name Listeners
	//@{

	/// Sets a game object's associated listeners.
	/// All listeners that have previously been added via AddListener or set via SetListeners will be removed and replaced with the listeners in the array in_pListenerGameObjs.
	/// Calling this function will override the default set of listeners and in_emitterGameObj will now reference its own, unique set of listeners.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_CommandTooLarge if the number of positions is too large for the command queue. Reduce the number of positions.
	/// \sa 
	/// - <tt>AK::SoundEngine::AddListener</tt>
	/// - <tt>AK::SoundEngine::RemoveListener</tt>
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT SetListeners(
		AkGameObjectID in_emitterGameObj,			///< Emitter game object.  Must have been previously registered via RegisterGameObj.
		const AkGameObjectID* in_pListenerGameObjs,	///< Array of listener game object IDs that will be activated for in_emitterGameObj. 
		AkUInt32 in_uNumListeners					///< Length of array
		) override;

	/// Add a single listener to a game object's set of associated listeners.
	/// Any listeners that have previously been added or set via AddListener or SetListeners will remain as listeners and in_listenerGameObj will be added as an additional listener.
	/// Calling this function will override the default set of listeners and in_emitterGameObj will now reference its own, unique set of listeners.
	/// \sa 
	/// - <tt>AK::SoundEngine::SetListeners</tt>
	/// - <tt>AK::SoundEngine::RemoveListener</tt>
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT AddListener(
		AkGameObjectID in_emitterGameObj,			///< Emitter game object.  Must have been previously registered via RegisterGameObj.
		AkGameObjectID in_listenerGameObj			///< Listener game object IDs that will be activated for in_emitterGameObj. 
		) override;

	/// Remove a single listener from a game object's set of active listeners.
	/// Calling this function will override the default set of listeners and in_emitterGameObj will now reference its own, unique set of listeners.
	/// \sa 
	/// - <tt>AK::SoundEngine::SetListeners</tt>
	/// - <tt>AK::SoundEngine::AddListener</tt>
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT RemoveListener(
		AkGameObjectID in_emitterGameObj,			///< Emitter game object.
		AkGameObjectID in_listenerGameObj			///< Listener game object IDs that will be deactivated for in_emitterGameObj. Game objects must have been previously registered.
		) override;

	/// Sets the default set of associated listeners for game objects that have not explicitly overridden their listener sets.  Upon registration, all game objects reference the default listener set, until
	/// a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that game object.
	/// All default listeners that have previously been added via AddDefaultListener or set via SetDefaultListeners will be removed and replaced with the listeners in the array in_pListenerGameObjs.
	/// \return Always returns AK_Success
	/// \sa 
	/// - \ref soundengine_listeners
	virtual AKRESULT SetDefaultListeners(
		const AkGameObjectID* in_pListenerObjs,	///< Array of listener game object IDs that will be activated for subsequent registrations. Game objects must have been previously registered.
		AkUInt32 in_uNumListeners				///< Length of array
		) override;

	/// Add a single listener to the default set of listeners.  Upon registration, all game objects reference the default listener set, until
	/// a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that game object.
	/// \sa 
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - <tt>AK::SoundEngine::RemoveDefaultListener</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT AddDefaultListener(
		AkGameObjectID in_listenerGameObj			///< Listener game object IDs that will be added to the default set of listeners.
		) override;

	/// Remove a single listener from the default set of listeners.  Upon registration, all game objects reference the default listener set, until
	/// a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that game object.
	/// \sa 
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - <tt>AK::SoundEngine::AddDefaultListener</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT RemoveDefaultListener(
		AkGameObjectID in_listenerGameObj			///< Listener game object IDs that will be removed from the default set of listeners.
		) override;

	/// Resets the listener associations to the default listener(s), as set by <tt>SetDefaultListeners</tt>. This will also reset per-listener gains that have been set using <tt>SetGameObjectOutputBusVolume</tt>.
	/// \return Always returns AK_Success
	/// \sa 
	/// - <tt>AK::SoundEngine::SetListeners</tt>
	/// - <tt>AK::SoundEngine::SetDefaultListeners</tt>
	/// - <tt>AK::SoundEngine::SetGameObjectOutputBusVolume</tt>
	/// - \ref soundengine_listeners
	virtual AKRESULT ResetListenersToDefault(
		AkGameObjectID in_emitterGameObj	///< Emitter game object.
		) override;

	/// Sets a listener's spatialization parameters. This lets you define listener-specific 
	/// volume offsets for each audio channel.
	/// If \c in_bSpatialized is false, only \c in_pVolumeOffsets is used for this listener (3D positions 
	/// have no effect on the speaker distribution). Otherwise, \c in_pVolumeOffsets is added to the speaker
	/// distribution computed for this listener.
	/// Use helper functions of \c AK::SpeakerVolumes to manipulate the vector of volume offsets in_pVolumeOffsets.
	/// 
	/// \remarks
	/// - If a sound is mixed into a bus that has a different speaker configuration than in_channelConfig,
	/// standard up/downmix rules apply.
	/// - Sounds with 3D Spatialization set to None will not be affected by these parameters.
	/// \return 
	/// - \c AK_Success if message was successfully posted to sound engine queue
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	/// - \c AK_InsufficientMemory if there wasn't enough memory in the message queue		
	/// \sa 
	/// - \ref soundengine_listeners_spatial
	virtual AKRESULT SetListenerSpatialization(
		AkGameObjectID in_uListenerID,				///< Listener game object ID
		bool in_bSpatialized,						///< Spatialization toggle (True : enable spatialization, False : disable spatialization)
		AkChannelConfig in_channelConfig,			///< Channel configuration associated with volumes in_pVolumeOffsets. Ignored if in_pVolumeOffsets is NULL.
		AK::SpeakerVolumes::VectorPtr in_pVolumeOffsets = NULL	///< Per-speaker volume offset, in dB. See AkSpeakerVolumes.h for how to manipulate this vector.
		) override;

	//@}


	////////////////////////////////////////////////////////////////////////
	/// @name Game Syncs
	//@{

	/// Sets the value of a real-time parameter control (by ID).
	/// With this function, you may set a game parameter value with global scope or with game object scope. 
	/// Game object scope supersedes global scope. (Once a value is set for the game object scope, it will not be affected by changes to the global scope value.) Game parameter values set with a global scope are applied to all 
	/// game objects that not yet registered, or already registered but not overridden with a value with game object scope.
	/// To set a game parameter value with global scope, pass \c AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for \c in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call \c <tt><tt>SetRTPCValue()</tt></tt> with <tt>in_uValueChangeDuration = 0</tt> in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use \c in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success if the value was successfully set
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)
	/// - \c AK_InvalidID if in_rtpcID is AK_INVALID_UNIQUE_ID (0)
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetRTPCValue(
		AkRtpcID in_rtpcID, 									///< ID of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets the value of a real-time parameter control (by Unicode string name).
	/// With this function, you may set a game parameter value to global scope or to game object scope. 
	/// Game object scope supersedes global scope. (Once a value is set for the game object scope, it will not be affected by changes to the global scope value.) Game parameter values set with global scope are applied to all 
	/// game objects that not yet registered, or already registered but not overridden with a value with game object scope.
	/// To set a game parameter value with global scope, pass AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValue() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success if the value was successfully set
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)		
	/// - \c AK_InvalidID if in_pszRtpcName is NULL.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_rtpc
	virtual AKRESULT SetRTPCValue(
		const wchar_t* in_pszRtpcName,							///< Name of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets the value of a real-time parameter control.
	/// With this function, you may set a game parameter value with global scope or with game object scope. 
	/// Game object scope supersedes global scope. (Once a value is set for the game object scope, it will not be affected by changes to the global scope value.) Game parameter values set with global scope are applied to all 
	/// game objects that not yet registered, or already registered but not overridden with a value with game object scope.
	/// To set a game parameter value with global scope, pass AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for \c in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValue() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success if the value was successfully set
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)	
	/// - \c AK_InvalidID if in_pszRtpcName is NULL.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_rtpc
	virtual AKRESULT SetRTPCValue(
		const char* in_pszRtpcName,								///< Name of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;

	/// Sets the value of a real-time parameter control (by ID).
	/// With this function, you may set a game parameter value on playing id scope. 
	/// Playing id scope supersedes both game object scope and global scope. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValueByPlayingID() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// - \c AK_Success if successful
	/// - \c AK_PlayingIDNotFound if in_playingID is not found.
	/// - \c AK_InvalidID if in_pszRtpcName is NULL.
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetRTPCValueByPlayingID(
		AkRtpcID in_rtpcID, 									///< ID of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkPlayingID in_playingID,								///< Associated playing ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when, for example, loading a level and you don't want the values to interpolate.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets the value of a real-time parameter control (by Unicode string name).
	/// With this function, you may set a game parameter value on playing ID scope. 
	/// Playing id scope supersedes both game object scope and global scope. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValueByPlayingID() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// - \c AK_Success if successful
	/// - \c AK_PlayingIDNotFound if in_playingID is not found.
	/// - \c AK_InvalidID if in_pszRtpcName is NULL.
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetRTPCValueByPlayingID(
		const wchar_t* in_pszRtpcName,							///< Name of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkPlayingID in_playingID,								///< Associated playing ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when, for example, loading a level and you don't want the values to interpolate.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets the value of a real-time parameter control (by string name).
	/// With this function, you may set a game parameter value on playing id scope. 
	/// Playing id scope supersedes both game object scope and global scope. 
	/// With this function, you may also change the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValueByPlayingID() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. Thus, if you call this 
	/// function at every game frame, you should not use in_uValueChangeDuration, as it would have no effect and it is less efficient.
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// - \c AK_Success if successful
	/// - \c AK_PlayingIDNotFound if in_playingID is not found.
	/// - \c AK_InvalidID if in_pszRtpcName is NULL.
	/// - \c AK_InvalidFloatValue if the value specified was NaN, Inf or FLT_MAX (3.402823e+38)
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetRTPCValueByPlayingID(
		const char* in_pszRtpcName,								///< Name of the game parameter
		AkRtpcValue in_value, 									///< Value to set
		AkPlayingID in_playingID,								///< Associated playing ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards in_value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;

	/// Resets the value of the game parameter to its default value, as specified in the Wwise project.
	/// With this function, you may reset a game parameter to its default value with global scope or with game object scope. 
	/// Game object scope supersedes global scope. Game parameter values reset with global scope are applied to all 
	/// game objects that were not overridden with a value with game object scope.
	/// To reset a game parameter value with global scope, pass AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also reset the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValue() or ResetRTPCValue() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. 
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidID if in_rtpcID is AK_INVALID_UNIQUE_ID (0)
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// - <tt>AK::SoundEngine::SetRTPCValue()</tt>
	virtual AKRESULT ResetRTPCValue(
		AkRtpcID in_rtpcID, 									///< ID of the game parameter
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards its default value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Resets the value of the game parameter to its default value, as specified in the Wwise project.
	/// With this function, you may reset a game parameter to its default value with global scope or with game object scope. 
	/// Game object scope supersedes global scope. Game parameter values reset with global scope are applied to all 
	/// game objects that were not overridden with a value with game object scope.
	/// To reset a game parameter value with global scope, pass AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also reset the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValue() or ResetRTPCValue() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. 
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if in_pszParamName is NULL.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::SetRTPCValue()</tt>
	virtual AKRESULT ResetRTPCValue(
		const wchar_t* in_pszRtpcName,							///< Name of the game parameter
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards its default value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Resets the value of the game parameter to its default value, as specified in the Wwise project.
	/// With this function, you may reset a game parameter to its default value with global scope or with game object scope. 
	/// Game object scope supersedes global scope. Game parameter values reset with global scope are applied to all 
	/// game objects that were not overridden with a value with game object scope.
	/// To reset a game parameter value with global scope, pass AK_INVALID_GAME_OBJECT as the game object. 
	/// With this function, you may also reset the value of a game parameter over time. To do so, specify a non-zero 
	/// value for in_uValueChangeDuration. At each audio frame, the game parameter value will be updated internally 
	/// according to the interpolation curve. If you call SetRTPCValue() or ResetRTPCValue() with in_uValueChangeDuration = 0 in the 
	/// middle of an interpolation, the interpolation stops and the new value is set directly. 
	/// Refer to \ref soundengine_rtpc_pergameobject, \ref soundengine_rtpc_buses and 
	/// \ref soundengine_rtpc_effects for more details on RTPC scope.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if in_pszParamName is NULL.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_rtpc
	/// - <tt>AK::SoundEngine::SetRTPCValue()</tt>
	virtual AKRESULT ResetRTPCValue(
		const char* in_pszRtpcName,								///< Name of the game parameter
		AkGameObjectID in_gameObjectID = AK_INVALID_GAME_OBJECT,///< Associated game object ID
		AkTimeMs in_uValueChangeDuration = 0,					///< Duration during which the game parameter is interpolated towards its default value
		AkCurveInterpolation in_eFadeCurve = AkCurveInterpolation_Linear,	///< Curve type to be used for the game parameter interpolation
		bool in_bBypassInternalValueInterpolation = false		///< True if you want to bypass the internal "slew rate" or "over time filtering" specified by the sound designer. This is meant to be used when for example loading a level and you dont want the values to interpolate.
		) override;

	/// Sets the State of a Switch Group (by IDs).
	/// \return Always returns AK_Success
	/// \sa 
	/// - \ref soundengine_switch
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetSwitch(
		AkSwitchGroupID in_switchGroup, 			///< ID of the Switch Group
		AkSwitchStateID in_switchState, 			///< ID of the Switch
		AkGameObjectID in_gameObjectID				///< Associated game object ID
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets the State of a Switch Group (by Unicode string names).
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the switch or Switch Group name was not resolved to an existing ID\n
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_switch
	virtual AKRESULT SetSwitch(
		const wchar_t* in_pszSwitchGroup,			///< Name of the Switch Group
		const wchar_t* in_pszSwitchState, 			///< Name of the Switch
		AkGameObjectID in_gameObjectID				///< Associated game object ID
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets the state of a Switch Group.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the switch or Switch Group name was not resolved to an existing ID\n
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_switch
	virtual AKRESULT SetSwitch(
		const char* in_pszSwitchGroup,				///< Name of the Switch Group
		const char* in_pszSwitchState, 				///< Name of the Switch
		AkGameObjectID in_gameObjectID				///< Associated game object ID
		) override;

	/// Post the specified trigger (by IDs).
	/// \return Always returns AK_Success
	/// \sa 
	/// - \ref soundengine_triggers
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT PostTrigger(
		AkTriggerID 	in_triggerID, 				///< ID of the trigger
		AkGameObjectID 	in_gameObjectID				///< Associated game object ID
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Posts the specified trigger (by Unicode string name).
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the trigger name was null
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_triggers
	virtual AKRESULT PostTrigger(
		const wchar_t* in_pszTrigger,				///< Name of the trigger
		AkGameObjectID in_gameObjectID				///< Associated game object ID
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Posts the specified trigger.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the trigger name was null
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_triggers
	virtual AKRESULT PostTrigger(
		const char* in_pszTrigger,			 	    ///< Name of the trigger
		AkGameObjectID in_gameObjectID				///< Associated game object ID
		) override;

	/// Sets the state of a State Group (by IDs).
	/// \return Always returns AK_Success
	/// \sa 
	/// - \ref soundengine_states
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetState(
		AkStateGroupID in_stateGroup, 				///< ID of the State Group
		AkStateID in_state 							///< ID of the state
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets the state of a State Group (by Unicode string names).
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the state or State Group name was null
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_states
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetState(
		const wchar_t* in_pszStateGroup,				///< Name of the State Group
		const wchar_t* in_pszState 						///< Name of the state
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets the state of a State Group.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidID if the state or State Group name was null
	/// Make sure that the banks were generated with the "include string" option.
	/// \aknote Strings are case-insensitive. \endaknote
	/// \sa 
	/// - \ref soundengine_states
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	virtual AKRESULT SetState(
		const char* in_pszStateGroup,					///< Name of the State Group
		const char* in_pszState 						///< Name of the state
		) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Environments
	//@{

	/// Sets the Auxiliary Busses to route the specified game object
	/// To clear the game object's auxiliary sends, \c in_uNumSendValues must be 0.
	/// \sa 
	/// - \ref soundengine_environments
	/// - \ref soundengine_environments_dynamic_aux_bus_routing
	/// - \ref soundengine_environments_id_vs_string
	/// - <tt>AK::SoundEngine::GetIDFromString()</tt>
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InvalidParameter if a duplicated environment is found in the array
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	virtual AKRESULT SetGameObjectAuxSendValues(
		AkGameObjectID		in_gameObjectID,		///< Associated game object ID
		AkAuxSendValue* in_aAuxSendValues,		///< Variable-size array of AkAuxSendValue structures
													///< (it may be NULL if no environment must be set)
		AkUInt32			in_uNumSendValues		///< The number of auxiliary busses at the pointer's address
													///< (it must be 0 if no environment is set)
		) override;

	/// Registers a callback to allow the game to modify or override the volume to be applied at the output of an audio bus.
	/// The callback must be registered once per bus ID.
	/// Call with <tt>in_pfnCallback = NULL</tt> to unregister.
	/// \aknote The bus in_busID needs to be a mixing bus.\endaknote
	/// \aknote The callback function will not be called for the Master Audio Bus, since the output of this bus is not a bus, but is instead an Audio Device.\endaknote
	/// \sa 
	/// - \ref goingfurther_speakermatrixcallback
	/// - \ref soundengine_environments
	/// - AkSpeakerVolumeMatrixCallbackInfo
	/// - <tt>AK::IAkMixerInputContext</tt>
	/// - <tt>AK::IAkMixerPluginContext</tt>
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_IDNotFound if the bus is not found
	/// - \c AK_NotInitialized if the sound engine is not initialized
	/// - \c AK_InsufficientMemory if there is not enough memory to complete the operation
	virtual AKRESULT RegisterBusVolumeCallback(
		AkUniqueID in_busID,						///< Bus ID, as obtained by GetIDFromString( bus_name ).
		AkBusCallbackFunc in_pfnCallback,			///< Callback function.
		void* in_pCookie = NULL						///< User cookie.
		) override;

	/// Registers a callback to be called to allow the game to access metering data from any mixing bus. You may use this to monitor loudness at any point of the mixing hierarchy 
	/// by accessing the peak, RMS, True Peak and K-weighted power (according to loudness standard ITU BS.1770). See \ref goingfurther_speakermatrixcallback for an example.
	/// The callback must be registered once per bus ID.
	/// Call with in_pfnCallback = NULL to unregister.
	/// \aknote The bus in_busID needs to be a mixing bus.\endaknote
	/// \sa 
	/// - \ref goingfurther_speakermatrixcallback
	/// - AkBusMeteringCallbackFunc
	/// - <tt>AK::AkMetering</tt>
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_IDNotFound if the bus is not found
	/// - \c AK_NotInitialized if the sound engine is not initialized
	/// - \c AK_InsufficientMemory if there is not enough memory to complete the operation
	virtual AKRESULT RegisterBusMeteringCallback(
		AkUniqueID in_busID,						///< Bus ID, as obtained by GetIDFromString( bus_name ).
		AkBusMeteringCallbackFunc in_pfnCallback,	///< Callback function.
		AkMeteringFlags in_eMeteringFlags,			///< Metering flags.
		void* in_pCookie = NULL						///< User cookie.
		) override;

	/// Registers a callback to be called to allow the game to access metering data from any output device. You may use this to monitor loudness as sound leaves the Wwise sound engine 
	/// by accessing the peak, RMS, True Peak and K-weighted power (according to loudness standard ITU BS.1770). See \ref goingfurther_speakermatrixcallback for an example.
	/// The callback must be registered once per device ShareSet ID.
	/// Call with in_pfnCallback = NULL to unregister.
	/// \sa 
	/// - \ref goingfurther_speakermatrixcallback
	/// - AkOutputDeviceMeteringCallbackFunc
	/// - <tt>AK::AkMetering</tt>
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_DeviceNotFound if the device is not found
	/// - \c AK_NotInitialized if the sound engine is not initialized
	/// - \c AK_InsufficientMemory if there is not enough memory to complete the operation
	virtual AKRESULT RegisterOutputDeviceMeteringCallback(
		AkOutputDeviceID in_idOutput,				///< Output ID, as returned from AddOutput or GetOutputID.  You can pass 0 for the main (default) output
		AkOutputDeviceMeteringCallbackFunc in_pfnCallback,	///< Callback function.
		AkMeteringFlags in_eMeteringFlags,			///< Metering flags.
		void* in_pCookie = NULL						///< User cookie.
		) override;

	/// Sets the Output Bus Volume (direct) to be used for the specified game object.
	/// The control value is a number ranging from 0.0f to 1.0f.
	/// Output Bus Volumes are stored per listener association, so calling this function will override the default set of listeners. The game object in_emitterObjID will now reference its own set of listeners which will 
	/// be the same as the old set of listeners, but with the new associated gain.  Future changes to the default listener set will not be picked up by this game object unless ResetListenersToDefault() is called.
	/// \sa 
	/// - \ref AK::SoundEngine::ResetListenersToDefault
	/// - \ref soundengine_environments
	/// - \ref soundengine_environments_setting_dry_environment
	/// - \ref soundengine_environments_id_vs_string
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	virtual AKRESULT SetGameObjectOutputBusVolume(
		AkGameObjectID		in_emitterObjID,		///< Associated emitter game object ID
		AkGameObjectID		in_listenerObjID,		///< Associated listener game object ID. Pass AK_INVALID_GAME_OBJECT to set the Output Bus Volume for all connected listeners.
		AkReal32			in_fControlValue		///< A multiplier in the range [0.0f:16.0f] ( -inf dB to +24 dB).																			
													///< A value greater than 1.0f will amplify the sound. 
		) override;

	/// Sets an Effect ShareSet at the specified audio node and Effect slot index.
	/// \aknote
	/// Replacing effects is preferably done through a Set Effect Event Action.
	/// \endaknote
	/// The target node cannot be a Bus, to set effects on a bus, use SetBusEffect() instead.
	/// \aknote The option "Override Parent" in 
	/// the Effect section in Wwise must be enabled for this node, otherwise the parent's effect will 
	/// still be the one in use and the call to SetActorMixerEffect will have no impact.
	/// \endaknote
	/// \return Always returns AK_Success
	virtual AKRESULT SetActorMixerEffect(
		AkUniqueID in_audioNodeID,					///< Can be a member of the Actor-Mixer or Interactive Music Hierarchy (not a bus).
		AkUInt32 in_uFXIndex,						///< Effect slot index (0-3)
		AkUniqueID in_ShareSetID					///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to clear the effect slot
		) override;

	/// Sets an Effect ShareSet at the specified bus and Effect slot index.
	/// \aknote
	/// Replacing effects is preferably done through a Set Effect Event Action.
	/// \endaknote
	/// The Bus can either be an Audio Bus or an Auxiliary Bus.
	/// This adds a reference on the audio node to an existing ShareSet.
	/// \aknote This function has unspecified behavior when adding an Effect to a currently playing
	/// Bus which does not have any Effects, or removing the last Effect on a currently playing bus.
	/// \endaknote
	/// \aknote This function will replace existing Effects on the node. If the target node is not at 
	/// the top of the hierarchy and is in the actor-mixer hierarchy, the option "Override Parent" in 
	/// the Effect section in Wwise must be enabled for this node, otherwise the parent's Effect will 
	/// still be the one in use and the call to SetBusEffect will have no impact.
	/// \endaknote
	/// \return
	/// - \c AK_Success when successfully posted.
	/// - \c AK_IDNotFound if the Bus isn't found by in_audioNodeID
	/// - \c AK_InvalidParameter if in_uFXIndex isn't in range
	virtual AKRESULT SetBusEffect(
		AkUniqueID in_audioNodeID,					///< Bus Short ID.
		AkUInt32 in_uFXIndex,						///< Effect slot index (0-3)
		AkUniqueID in_ShareSetID					///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to clear the Effect slot
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets an Effect ShareSet at the specified Bus and Effect slot index.
	/// \aknote
	/// Replacing effects is preferably done through a Set Effect Event Action.
	/// \endaknote
	/// The Bus can either be an Audio Bus or an Auxiliary Bus.
	/// This adds a reference on the audio node to an existing ShareSet.
	/// \aknote This function has unspecified behavior when adding an Effect to a currently playing
	/// bus which does not have any Effects, or removing the last Effect on a currently playing Bus.
	/// \endaknote
	/// \aknote This function will replace existing Effects on the node. If the target node is not at 
	/// the top of the hierarchy and is in the Actor-Mixer Hierarchy, the option "Override Parent" in 
	/// the Effect section in Wwise must be enabled for this node, otherwise the parent's Effect will 
	/// still be the one in use and the call to \c SetBusEffect will have no impact.
	/// \endaknote
	/// \returns 
	/// - \c AK_Success when successfully posted.
	/// - \c AK_IDNotFound if the Bus name doesn't point to a valid bus.
	/// - \c AK_InvalidID if in_pszBusName is null
	/// - \c AK_InvalidParameter if in_uFXIndex isn't in range or in_pszBusName is null
	virtual AKRESULT SetBusEffect(
		const wchar_t* in_pszBusName,				///< Bus name
		AkUInt32 in_uFXIndex,						///< Effect slot index (0-3)
		AkUniqueID in_shareSetID					///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to clear the effect slot
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets an Effect ShareSet at the specified Bus and Effect slot index.
	/// \aknote
	/// Replacing effects is preferably done through a Set Effect Event Action.
	/// \endaknote
	/// The Bus can either be an Audio Bus or an Auxiliary Bus.
	/// This adds a reference on the audio node to an existing ShareSet.
	/// \aknote This function has unspecified behavior when adding an Effect to a currently playing
	/// Bus which does not have any effects, or removing the last Effect on a currently playing bus.
	/// \endaknote
	/// \aknote Make sure the new effect ShareSet is included in a soundbank, and that sound bank is loaded. Otherwise you will see errors in the Capture Log.\endaknote
	/// \aknote This function will replace existing Effects on the node. If the target node is not at 
	/// the top of the hierarchy and is in the Actor-Mixer Hierarchy, the option "Override Parent" in 
	/// the Effect section in Wwise must be enabled for this node, otherwise the parent's Effect will 
	/// still be the one in use and the call to SetBusEffect will have no impact.
	/// \endaknote
	/// \returns 
	/// - \c AK_Success when successfully posted.
	/// - \c AK_IDNotFound if the Bus name doesn't point to a valid bus.
	/// - \c AK_InvalidParameter if in_uFXIndex isn't in range
	/// - \c AK_InvalidID if in_pszBusName is null
	virtual AKRESULT SetBusEffect(
		const char* in_pszBusName,		///< Bus name
		AkUInt32 in_uFXIndex,			///< Effect slot index (0-3)
		AkUniqueID in_shareSetID		///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to clear the effect slot
		) override;

	/// Sets an audio device effect ShareSet on the specified output device and effect slot index.
	/// \aknote
	/// Replacing effects is preferably done through a Set Effect Event Action.
	/// \endaknote
	/// \aknote Make sure the new effect ShareSet is included in a soundbank, and that sound bank is loaded. Otherwise you will see errors in the Capture Log.\endaknote
	/// \aknote This function will replace existing effects of the audio device ShareSet. \endaknote
	/// \aknote Audio device effects support is limited to one ShareSet per plug-in type at any time. \endaknote
	/// \aknote Errors are reported in the Wwise Capture Log if the effect cannot be set on the output device. \endaknote

	/// \returns Always returns AK_Success
	virtual AKRESULT SetOutputDeviceEffect(
		AkOutputDeviceID in_outputDeviceID,				///< Output ID, as returned from AddOutput or GetOutputID. Most of the time this should be 0 to designate the main (default) output
		AkUInt32 in_uFXIndex,							///< Effect slot index (0-3)
		AkUniqueID in_FXShareSetID						///< Effect ShareSet ID
		) override;

	/// Sets a Mixer ShareSet at the specified bus.
	/// \aknote This function has unspecified behavior when adding a mixer to a currently playing
	/// Bus which does not have any Effects or mixer, or removing the last mixer on a currently playing Bus.
	/// \endaknote
	/// \aknote Make sure the new mixer ShareSet is included in a soundbank, and that sound bank is loaded. Otherwise you will see errors in the Capture Log.\endaknote
	/// \aknote This function will replace existing mixers on the node. 
	/// \endaknote
	/// \return Always returns AK_Success
	virtual AKRESULT SetMixer(
		AkUniqueID in_audioNodeID,					///< Bus Short ID.
		AkUniqueID in_shareSetID					///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to remove.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets a Mixer ShareSet at the specified bus.
	/// \aknote This function has unspecified behavior when adding a mixer to a currently playing
	/// bus which does not have any effects nor mixer, or removing the last mixer on a currently playing bus.
	/// \endaknote
	/// \aknote Make sure the new mixer ShareSet is included in a soundbank, and that sound bank is loaded. Otherwise you will see errors in the Capture Log.\endaknote
	/// \aknote This function will replace existing mixers on the node. 
	/// \endaknote
	/// \returns 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidID if in_pszBusName is null
	virtual AKRESULT SetMixer(
		const wchar_t* in_pszBusName,				///< Bus name
		AkUniqueID in_shareSetID					///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to remove.
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Sets a Mixer ShareSet at the specified bus.
	/// \aknote This function has unspecified behavior when adding a mixer to a currently playing
	/// bus which does not have any effects nor mixer, or removing the last mixer on a currently playing bus.
	/// \endaknote
	/// \aknote Make sure the new mixer ShareSet is included in a soundbank, and that sound bank is loaded. Otherwise you will see errors in the Capture Log.\endaknote
	/// \aknote This function will replace existing mixers on the node.		 
	/// \endaknote
	/// \returns 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidID if in_pszBusName is null
	virtual AKRESULT SetMixer(
		const char* in_pszBusName,		///< Bus name
		AkUniqueID in_shareSetID		///< ShareSets ID; pass AK_INVALID_UNIQUE_ID to remove.
		) override;

	/// Forces channel configuration for the specified bus.
	/// \aknote You cannot change the configuration of the master bus.\endaknote
	/// 
	/// \return Always returns AK_Success
	virtual AKRESULT SetBusConfig(
		AkUniqueID in_audioNodeID,					///< Bus Short ID.
		AkChannelConfig in_channelConfig			///< Desired channel configuration. An invalid configuration (from default constructor) means "as parent".
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Forces channel configuration for the specified bus.
	/// \aknote You cannot change the configuration of the master bus.\endaknote
	/// 
	/// \returns 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidID if in_pszBusName is null
	virtual AKRESULT SetBusConfig(
		const wchar_t* in_pszBusName,				///< Bus name
		AkChannelConfig in_channelConfig			///< Desired channel configuration. An invalid configuration (from default constructor) means "as parent".
		) override;
#endif //AK_SUPPORT_WCHAR

	/// Forces channel configuration for the specified bus.
	/// \aknote You cannot change the configuration of the master bus.\endaknote
	/// 
	/// \returns 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidID if in_pszBusName is null
	virtual AKRESULT SetBusConfig(
		const char* in_pszBusName,					///< Bus name
		AkChannelConfig in_channelConfig			///< Desired channel configuration. An invalid configuration (from default constructor) means "as parent".
		) override;

	/// Sets a game object's obstruction and occlusion levels. If SetMultiplePositions were used, values are set for all positions.
	/// This function is used to affect how an object should be heard by a specific listener.
	/// \sa 
	/// - \ref soundengine_obsocc
	/// - \ref soundengine_environments
	/// \return Always returns AK_Success
	virtual AKRESULT SetObjectObstructionAndOcclusion(
		AkGameObjectID in_EmitterID,		///< Emitter game object ID
		AkGameObjectID in_ListenerID,		///< Listener game object ID
		AkReal32 in_fObstructionLevel,		///< ObstructionLevel: [0.0f..1.0f]
		AkReal32 in_fOcclusionLevel			///< OcclusionLevel: [0.0f..1.0f]
		) override;

	/// Sets a game object's obstruction and occlusion level for each positions defined by SetMultiplePositions.
	/// This function differs from SetObjectObstructionAndOcclusion as a list of obstruction/occlusion pair is provided
	/// and each obstruction/occlusion pair will affect the corresponding position defined at the same index.
	/// \aknote In the case the number of obstruction/occlusion pairs is smaller than the number of positions, remaining positions' 
	/// obstruction/occlusion values are set to 0.0. \endaknote
	/// \return
	/// - \c AK_Success if successful
	/// - \c AK_CommandTooLarge if the number of obstruction values is too large for the command queue.
	/// - \c AK_InvalidParameter if one of the parameter is out of range (check the debug console)
	/// - \c AK_InvalidFloatValue if one of the occlusion/obstruction values is NaN or Inf.
	/// \sa
	/// - \ref soundengine_obsocc
	/// - \ref soundengine_environments
	/// \return AK_Success if occlusion/obstruction values are successfully stored for this emitter
	virtual AKRESULT SetMultipleObstructionAndOcclusion(
		AkGameObjectID in_EmitterID,										///< Emitter game object ID
		AkGameObjectID in_uListenerID,										///< Listener game object ID
		AkObstructionOcclusionValues* in_fObstructionOcclusionValues,		///< Array of obstruction/occlusion pairs to apply
																			///< ObstructionLevel: [0.0f..1.0f]
																			///< OcclusionLevel: [0.0f..1.0f]
		AkUInt32 in_uNumOcclusionObstruction								///< Number of obstruction/occlusion pairs specified in the provided array
		) override;

	/// Saves the playback history of container structures.
	/// This function will write history data for all currently loaded containers and instantiated game
	/// objects (for example, current position in Sequence Containers and previously played elements in
	/// Random Containers). 
	/// \remarks
	/// This function acquires the main audio lock, and may block the caller for several milliseconds.
	/// \return
	/// - \c AK_Success when successful
	/// - \c AK_Fail is in_pBytes could not be parsed (corruption or data is truncated)
	/// \sa 
	/// - <tt>AK::SoundEngine::SetContainerHistory()</tt>
	virtual AKRESULT GetContainerHistory(
		AK::IWriteBytes* in_pBytes			///< Pointer to IWriteBytes interface used to save the history.
		) override;

	/// Restores the playback history of container structures.
	/// This function will read history data from the passed-in stream reader interface, and apply it to all
	/// currently loaded containers and instantiated game objects. Game objects are matched by
	/// ID. History for unloaded structures and unknown game objects will be skipped.
	/// \remarks
	/// This function acquires the main audio lock, and may block the caller for several milliseconds.
	/// \return 
	/// - \c AK_Success if successful
	/// - \c AK_InsufficientMemory if not enough memory is available for IReadBytes operation
	/// \sa 
	/// - <tt>AK::SoundEngine::GetContainerHistory()</tt>
	virtual AKRESULT SetContainerHistory(
		AK::IReadBytes* in_pBytes 			///< Pointer to IReadBytes interface used to load the history.
		) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Capture
	//@{

	/// Starts recording the sound engine audio output. 
	/// StartOutputCapture outputs a wav file per current output device of the sound engine.
	/// If more than one device is active, the system will create multiple files in the same output 
	/// directory and will append numbers at the end of the provided filename.
	///
	/// If no device is running yet, the system will return success AK_Success despite doing nothing.
	/// Use RegisterAudioDeviceStatusCallback to get notified when devices are created/destructed.
	///
	/// \return AK_Success if successful, AK_Fail if there was a problem starting the output capture.
	/// \remark
	///		- The sound engine opens a stream for writing using <tt>AK::IAkStreamMgr::CreateStd()</tt>. If you are using the
	///			default implementation of the Stream Manager, file opening is executed in your implementation of 
	///			the Low-Level IO interface <tt>AK::StreamMgr::IAkLowLevelIOHook::BatchOpen()</tt>. The following 
	///			AkFileSystemFlags are passed: uCompanyID = AKCOMPANYID_AUDIOKINETIC and uCodecID = AKCODECID_PCM,
	///			and the AkOpenMode is AK_OpenModeWriteOvrwr. Refer to \ref streamingmanager_lowlevel_location for
	///			more details on managing the deployment of your Wwise generated data.
	/// \return
	/// - \c AK_Success when successful
	/// - \c AK_InvalidParameter if in_CaptureFileName is null.
	/// - \c AK_InsufficientMemory if not enough memory is available.
	/// \sa 
	/// - <tt>AK::SoundEngine::StopOutputCapture()</tt>
	/// - <tt>AK::StreamMgr::SetFileLocationResolver()</tt>
	/// - \ref streamingdevicemanager
	/// - \ref streamingmanager_lowlevel_location
	/// - RegisterAudioDeviceStatusCallback
	virtual AKRESULT StartOutputCapture(
		const AkOSChar* in_CaptureFileName				///< Name of the output capture file
		) override;

	/// Stops recording the sound engine audio output. 
	/// \return AK_Success if successful, AK_Fail if there was a problem stopping the output capture.
	/// \sa 
	/// - <tt>AK::SoundEngine::StartOutputCapture()</tt>
	virtual AKRESULT StopOutputCapture() override;

	/// Adds text marker in audio output file. 
	/// \return 
	/// - \c AK_Success when successful
	/// - \c AK_InvalidParameter if in_MarkerText is null.
	/// - \c AK_InsufficientMemory if not enough memory is available.
	/// \sa 
	/// - <tt>AK::SoundEngine::StartOutputCapture()</tt>
	virtual AKRESULT AddOutputCaptureMarker(
		const char* in_MarkerText					///< Text of the marker
		) override;

	/// Adds binary data to a marker in audio output file.
	/// \return
	/// - \c AK_Success when successful
	/// - \c AK_InvalidParameter if in_pMarkerData is null or in_uMarkerDataSize is zero.
	/// - \c AK_InsufficientMemory if not enough memory is available.
	/// \sa
	/// - <tt>AK::SoundEngine::StartOutputCapture()</tt>
	virtual AKRESULT AddOutputCaptureBinaryMarker(
		void* in_pMarkerData,								///< Marker data
		AkUInt32 in_uMarkerDataSize							///< Size of the marker data
		) override;

	/// Gets the system sample rate.
	/// \return The sample rate.
	virtual AkUInt32 GetSampleRate() override;

	/// Registers a callback used for retrieving audio samples.
	/// The callback will be called from the audio thread during real-time rendering and from the main thread during offline rendering.
	/// \return
	/// - \c AK_Success when successful
	/// - \c AK_DeviceNotFound if the audio device ID doesn't match to a device in use.
	/// - \c AK_InvalidParameter when in_pfnCallback is null
	/// - \c AK_NotInitialized if the sound engine is not initialized at this time
	/// \sa
	/// - <tt>AK::SoundEngine::AddOutput()</tt>
	/// - <tt>AK::SoundEngine::GetOutputID()</tt>
	/// - <tt>AK::SoundEngine::UnregisterCaptureCallback()</tt>
	virtual AKRESULT RegisterCaptureCallback(
		AkCaptureCallbackFunc in_pfnCallback,								///< Capture callback function to register.
		AkOutputDeviceID in_idOutput = AK_INVALID_OUTPUT_DEVICE_ID,			///< The audio device specific id, return by AK::SoundEngine::AddOutput or AK::SoundEngine::GetOutputID
		void* in_pCookie = NULL												///< Callback cookie that will be sent to the callback function along with additional information
		) override;

	/// Unregisters a callback used for retrieving audio samples.
	/// \return
	/// - \c AK_Success when successful
	/// - \c AK_DeviceNotFound if the audio device ID doesn't match to a device in use.
	/// - \c AK_InvalidParameter when in_pfnCallback is null
	/// - \c AK_NotInitialized if the sound engine is not initialized at this time
	/// \sa
	/// - <tt>AK::SoundEngine::AddOutput()</tt>
	/// - <tt>AK::SoundEngine::GetOutputID()</tt>
	/// - <tt>AK::SoundEngine::RegisterCaptureCallback()</tt>
	virtual AKRESULT UnregisterCaptureCallback(
		AkCaptureCallbackFunc in_pfnCallback,								///< Capture callback function to unregister.
		AkOutputDeviceID in_idOutput = AK_INVALID_OUTPUT_DEVICE_ID,			///< The audio device specific id, return by AK::SoundEngine::AddOutput or AK::SoundEngine::GetOutputID
		void* in_pCookie = NULL												///< Callback cookie that will be sent to the callback function along with additional information
		) override;

	/// Starts recording the sound engine profiling information into a file. This file can be read
	/// by Wwise Authoring. The file is created at the base path. If you have integrated Wwise I/O,
	/// you can use <tt>CAkDefaultIOHookDeferred::SetBasePath()</tt> (or <tt>CAkDefaultIOHookDeferred::AddBasePath()</tt>)
	/// to change the location where the file is saved. The profiling session records all data types possible.
	/// Note that this call captures peak metering for all the busses loaded and mixing
	/// while this call is invoked.
	/// \remark This function is provided as a utility tool only. It does nothing if it is 
	///			called in the release configuration and returns AK_NotCompatible.
	virtual AKRESULT StartProfilerCapture(
		const AkOSChar* in_CaptureFileName				///< Name of the output profiler file (.prof extension recommended)
		) override;

	/// Stops recording the sound engine profiling information. 
	/// \remark This function is provided as a utility tool only. It does nothing if it is 
	///			called in the release configuration and returns AK_NotCompatible.
	virtual AKRESULT StopProfilerCapture() override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Offline Rendering
	//@{

	/// Sets the offline rendering frame time in seconds.
	/// When offline rendering is enabled, every call to \ref RenderAudio() will generate sample data as if this much time has elapsed. If the frame time argument is less than or equal to zero, every call to RenderAudio() will generate one audio buffer.
	/// \return Always returns AK_Success
	virtual AKRESULT SetOfflineRenderingFrameTime(
		AkReal32 in_fFrameTimeInSeconds					///< frame time in seconds used during offline rendering
		) override;

	/// Enables/disables offline rendering. 
	/// \return Always returns AK_Success
	virtual AKRESULT SetOfflineRendering(
		bool in_bEnableOfflineRendering					///< enables/disables offline rendering
		) override;

	//@}

	////////////////////////////////////////////////////////////////////////
	/// @name Secondary Outputs
	//@{

	/// Adds an output to the sound engine. Use this to add controller-attached headphones, controller speakers, DVR output, etc.  
	/// The in_Settings parameter contains an Audio Device ShareSet to specify the output plugin to use and a device ID to specify the instance, if necessary (e.g. which game controller).
	/// 
	/// Like most functions of AK::SoundEngine, AddOutput is asynchronous. A successful return code merely indicates that the request is properly queued.
	/// Error codes returned by this function indicate various invalid parameters. To know if this function succeeds or not, and the failure code,
	/// register an AkDeviceStatusCallbackFunc callback with RegisterAudioDeviceStatusCallback.
	/// 
	/// \sa AkOutputSettings for more details.
	/// \sa \ref integrating_secondary_outputs
	/// \sa \ref default_audio_devices
	/// \sa AK::SoundEngine::RegisterAudioDeviceStatusCallback
	/// \sa AK::AkDeviceStatusCallbackFunc
	/// \return 
	/// The following codes are returned directly from the function, as opposed to the AkDeviceStatusCallback
	/// - \c AK_NotImplemented: Feature not supported, some platforms don't have other outputs.
	/// - \c AK_InvalidParameter: Out of range parameters or unsupported parameter combinations (see parameter list below).
	/// - \c AK_IDNotFound: The audioDeviceShareSet on in_settings doesn't exist.  Possibly, the Init bank isn't loaded yet or was not updated with latest changes.
	/// - \c AK_DeviceNotReady: The idDevice on in_settings doesn't match with a valid hardware device.  Either the device doesn't exist or is disabled.  Disconnected devices (headphones) are not considered "not ready" therefore won't cause this error.
	/// - \c AK_NotInitialized: If AK::SoundEngine::Init was not called or if the Init.bnk was not loaded before the call.
	/// - \c AK_Success: Parameters are valid.
	/// 
	/// The following codes are returned from the callback.
	/// - \c AK_InsufficientMemory : Not enough memory to complete the operation.
	/// - \c AK_IDNotFound: The audioDeviceShareSet on in_settings doesn't exist.  Possibly, the Init bank isn't loaded yet or was not updated with latest changes.
	/// - \c AK_PluginNotRegistered: The audioDeviceShareSet exists but the plug-in it refers to is not installed or statically linked with the game.
	/// - \c AK_NotCompatible: The hardware does not support this type of output. Wwise will try to use the System output instead, and a separate callback will fire when that completes.
	/// - \c AK_DeviceNotCompatible: The hardware does not support this type of output. Wwise will NOT fallback to any other type of output.
	/// - \c AK_Fail: Generic code for any non-permanent conditions (e.g. disconnection) that prevent the use of the output. Wwise has created the output and sounds will be routed to it, but this output is currently silent until the temporary condition resolves.
	/// - \c AK_NoDistinctListener: Outputs of the same type (same ShareSet, like controller speakers) must have distinct Listeners to make a proper routing. This doesn't happen if there is only one output of that type.
	virtual AKRESULT AddOutput(
		const AkOutputSettings& in_Settings,			///< Creation parameters for this output.  \ref AkOutputSettings							
		AkOutputDeviceID* out_pDeviceID = NULL,			///< (Optional) Output ID to use with all other Output management functions.  Leave to NULL if not required. \ref AK::SoundEngine::GetOutputID
		const AkGameObjectID* in_pListenerIDs = NULL,	///< Specific listener(s) to attach to this device. 
														///< If specified, only the sounds routed to game objects linked to those listeners will play in this device.  
														///< It is necessary to have separate listeners if multiple devices of the same type can coexist (e.g. controller speakers)
														///< If not specified, sound routing simply obey the associations between Master Busses and Audio Devices setup in the Wwise Project.
		AkUInt32 in_uNumListeners = 0					///< The number of elements in the in_pListenerIDs array.
		) override;

	/// Removes one output added through AK::SoundEngine::AddOutput
	/// If a listener was associated with the device, you should consider unregistering the listener prior to call RemoveOutput 
	/// so that Game Object/Listener routing is properly updated according to your game scenario.			
	/// \sa \ref integrating_secondary_outputs
	/// \sa AK::SoundEngine::AddOutput
	/// \return AK_Success: Parameters are valid.
	virtual AKRESULT RemoveOutput(
		AkOutputDeviceID in_idOutput	///< ID of the output to remove.  Use the returned ID from AddOutput, GetOutputID, or ReplaceOutput
		) override;

	/// Replaces an output device previously created during engine initialization or from AddOutput, with a new output device.
	/// In addition to simply removing one output device and adding a new one, the new output device will also be used on all of the master buses
	/// that the old output device was associated with, and preserve all listeners that were attached to the old output device.
	/// 
	/// Like most functions of AK::SoundEngine, AddOutput is asynchronous. A successful return code merely indicates that the request is properly queued.
	/// Error codes returned by this function indicate various invalid parameters. To know if this function succeeds or not, and the failure code,
	/// register an AkDeviceStatusCallbackFunc callback with RegisterAudioDeviceStatusCallback.
	/// 
	/// \sa AK::SoundEngine::AddOutput
	/// \sa AK::SoundEngine::RegisterAudioDeviceStatusCallback
	/// \sa AK::AkDeviceStatusCallbackFunc
	/// \return 
	/// - \c AK_InvalidID: The audioDeviceShareSet on in_settings was not valid.
	/// - \c AK_IDNotFound: The audioDeviceShareSet on in_settings doesn't exist.  Possibly, the Init bank isn't loaded yet or was not updated with latest changes.
	/// - \c AK_DeviceNotReady: The idDevice on in_settings doesn't match with a valid hardware device.  Either the device doesn't exist or is disabled.  Disconnected devices (headphones) are not considered "not ready" therefore won't cause this error.
	/// - \c AK_DeviceNotFound: The in_outputDeviceId provided does not match with any of the output devices that the sound engine is currently using.
	/// - \c AK_InvalidParameter: Out of range parameters or unsupported parameter combinations on in_settings
	/// - \c AK_Success: parameters were valid, and the remove and add will occur.
	virtual AKRESULT ReplaceOutput(
		const AkOutputSettings& in_Settings,				///< Creation parameters for this output.  \ref AkOutputSettings
		AkOutputDeviceID in_outputDeviceId,					///< AkOutputDeviceID of the output to replace. Use 0 to target the current main output, regardless of its id.  Otherwise, use the AkOutputDeviceID returned from AddOutput() or ReplaceOutput(), or generated by GetOutputID()
		AkOutputDeviceID* out_pOutputDeviceId = NULL		///< (Optional) Pointer into which the method writes the AkOutputDeviceID of the new output device. If the call fails, the value pointed to will not be modified.
		) override;

	/// Gets the compounded output ID from ShareSet and device id.
	/// Outputs are defined by their type (Audio Device ShareSet) and their specific system ID.  A system ID could be reused for other device types on some OS or platforms, hence the compounded ID.
	/// Use 0 for in_idShareSet & in_idDevice to get the Main Output ID (the one usually initialized during AK::SoundEngine::Init)
	/// \return The id of the output					
	virtual AkOutputDeviceID GetOutputID(
		AkUniqueID in_idShareSet,		///< Audio Device ShareSet ID, as defined in the Wwise Project.  If needed, use AK::SoundEngine::GetIDFromString() to convert from a string.  Set to AK_INVALID_UNIQUE_ID to use the default.
		AkUInt32 in_idDevice			///< Device specific identifier, when multiple devices of the same type are possible.  If only one device is possible, leave to 0.
										/// \sa \ref obtaining_device_id
		) override;

	virtual AkOutputDeviceID GetOutputID(
		const char* in_szShareSet,			///< Audio Device ShareSet Name, as defined in the Wwise Project.  If Null, will select the Default Output ShareSet (always available)
		AkUInt32 in_idDevice				///< Device specific identifier, when multiple devices of the same type are possible.  If only one device is possible, leave to 0.
											/// \sa \ref obtaining_device_id
		) override;

#ifdef AK_SUPPORT_WCHAR
	virtual AkOutputDeviceID GetOutputID(
		const wchar_t* in_szShareSet,			///< Audio Device ShareSet Name, as defined in the Wwise Project.  If Null, will select the Default Output ShareSet (always available)
		AkUInt32 in_idDevice			///< Device specific identifier, when multiple devices of the same type are possible.  If only one device is possible, leave to 0.
										/// \sa \ref obtaining_device_id
		) override;
#endif

	/// Sets the Audio Device to which a master bus outputs.  This overrides the setting in the Wwise project.	
	/// Can only be set on top-level busses. The Init bank should be successfully loaded prior to this call.
	/// \aknote This function is useful only if used before the creation of an output, at the beginning of the sound engine setup. 
	/// Once active outputs using this Bus have been created, it is imperative to use AK::SoundEngine:ReplaceOutput instead to change the type of output.
	/// \return 
	/// AK_IDNotFound when either the Bus ID or the Device ID are not present in the Init bank or the bank was not loaded
	/// AK_InvalidParameter when the specified bus is not a Master Bus.  This function can be called only on busses that have no parent bus.
	virtual AKRESULT SetBusDevice(
		AkUniqueID in_idBus,			///< Id of the master bus
		AkUniqueID in_idNewDevice		///< New device ShareSet to replace with.
		) override;

	/// Sets the Audio Device to which a master bus outputs.  This overrides the setting in the Wwise project.	
	/// Can only be set on top-level busses. The Init bank should be successfully loaded prior to this call.
	/// \aknote This function is useful only if used before the creation of an output, at the beginning of the sound engine setup. 
	/// Once active outputs using this Bus have been created, it is imperative to use AK::SoundEngine:ReplaceOutput instead to change the type of output.
	/// \return 
	/// AK_IDNotFound when either the Bus ID or the Device ID are not present in the Init bank or the bank was not loaded
	/// AK_InvalidParameter when the specified bus is not a Master Bus.  This function can be called only on busses that have no parent bus.
	virtual AKRESULT SetBusDevice(
		const char* in_BusName,		///< Name of the master bus
		const char* in_DeviceName		///< New device ShareSet to replace with.
		) override;

#ifdef AK_SUPPORT_WCHAR
	/// Sets the Audio Device to which a master bus outputs.  This overrides the setting in the Wwise project.	
	/// Can only be set on top-level busses. The Init bank should be successfully loaded prior to this call.
	/// SetBusDevice must be preceded by a call to AddOutput for the new device ShareSet to be registered as an output.
	/// \return 
	/// AK_IDNotFound when either the Bus ID or the Device ID are not present in the Init bank or the bank was not loaded
	/// AK_InvalidParameter when the specified bus is not a Master Bus.  This function can be called only on busses that have no parent bus.
	virtual AKRESULT SetBusDevice(
		const wchar_t* in_BusName,		///< Name of the master bus
		const wchar_t* in_DeviceName	///< New device ShareSet to replace with.
		) override;
#endif

	/// Returns a listing of the current devices for a given sink plug-in, including Device ID, friendly name, and state.
	/// This call is only valid for sink plug-ins that support device enumeration.
	/// Prerequisites: the plug-in must have been initialized by loading the init bank or by calling \ref AK::SoundEngine::RegisterPlugin.
	/// \return
	/// - \c AK_NotImplemented if the sink plug-in does not implement device enumeration
	/// - \c AK_PluginNotRegistered if the plug-in has not been registered yet either by loading the init bank or by calling RegisterPluginDLL.
	/// - \c AK_NotCompatible if no device of this type are supported on the current platform
	/// - \c AK_Fail in case of system device manager failure (OS related)
	/// 
	virtual AKRESULT GetDeviceList(
		AkUInt32 in_ulCompanyID,					///< Company identifier (as declared in the plug-in description XML file)
		AkUInt32 in_ulPluginID,						///< Plug-in identifier (as declared in the plug-in description XML file)
		AkUInt32& io_maxNumDevices,					///< In: The length of the out_deviceDescriptions array. Out: If out_deviceDescriptions is not-null, this will be set to the number of entries in out_deviceDescriptions that was populated. If out_deviceDescriptions is null, this will be set to the number of devices that may be available.
		AkDeviceDescription* out_deviceDescriptions ///< The output array of device descriptions.
		) override;

	/// Returns a listing of the current devices for a given sink plug-in, including Device ID, friendly name, and state.
	/// This call is only valid for sink plug-ins that support device enumeration.
	/// Prerequisites:
	///  * The plug-in must have been initialized by loading the init bank or by calling \ref AK::SoundEngine::RegisterPlugin.
	///  * The audio device ShareSet must have been loaded from a soundbank and a device with this ShareSet must exist in the pipeline.
	/// \return
	/// AK_NotImplemented if the sink plug-in does not implement device enumeration
	/// AK_PluginNotRegistered if the plug-in has not been registered yet either by loading the init bank or by calling RegisterPluginDLL.
	virtual AKRESULT GetDeviceList(
		AkUniqueID in_audioDeviceShareSetID,		///< In: The audio device ShareSet ID for which to list the sink plug-in devices.
		AkUInt32& io_maxNumDevices,					///< In: The length of the out_deviceDescriptions array. Out: If out_deviceDescriptions is not-null, this will be set to the number of entries in out_deviceDescriptions that was populated. If out_deviceDescriptions is null, this will be set to the number of devices that may be available.
		AkDeviceDescription* out_deviceDescriptions ///< The output array of device descriptions.
		) override;

	/// Sets the volume of a output device.		
	/// \return
	/// - \c AK_Success if successful		
	/// - \c AK_InvalidFloatValue if the value specified was NaN or Inf
	virtual AKRESULT SetOutputVolume(
		AkOutputDeviceID in_idOutput,	///< Output ID to set the volume on.  As returned from AddOutput or GetOutputID
		AkReal32 in_fVolume				///< Volume (0.0 = Muted, 1.0 = Volume max)
		) override;

	/// Returns whether or not the audio device matching the device ID provided supports spatial audio (i.e. the functionality is enabled, and more than 0 dynamic objects are supported).
	/// If Spatial Audio is supported, then you can call Init, AddOutput, or ReplaceOutput with an Audio Device ShareSet corresponding to the respective platform-specific plug-in that
	/// provides spatial audio, such as the Microsoft Spatial Sound Platform for Windows. Note that on Xbox One, you need to call EnableSpatialAudio() before the sound engine is
	/// initialized, or initialize the sound engine with AkPlatformInitSettings::bEnableSpatialAudio set to true if you want spatial audio support; otherwise this will always return AK_NotCompatible.
	/// \return
	/// AK_NotCompatible when the device ID provided does not support spatial audio, or the platform does not support spatial audio
	/// AK_Fail when there is some other miscellaneous failure, or the device ID provided does not match a device that the system knows about
	/// AK_Success when the device ID provided does support spatial audio
	virtual AKRESULT GetDeviceSpatialAudioSupport(
		AkUInt32 in_idDevice			///< Device specific identifier, when multiple devices of the same type are possible.  If only one device is possible, leave to 0.
										/// \sa \ref obtaining_device_id
		) override;


	//@}

	/// This function should be called to put the sound engine in background mode, where audio isn't processed anymore. This needs to be called if the console has a background mode or some suspended state.
	/// Call \c WakeupFromSuspend when your application receives the message from the OS that the process is back in foreground.
	/// When suspended, the sound engine will process API messages (like PostEvent and SetSwitch) only when \ref RenderAudio() is called. 
	/// It is recommended to match the <b>in_bRenderAnyway</b> parameter with the behavior of the rest of your game: 
	/// if your game still runs in background and you must keep some kind of coherent state between the audio engine and game, then allow rendering.
	/// If you want to minimize CPU when in background, then don't allow rendering and never call RenderAudio from the game.
	///
	/// Consult \ref workingwithsdks_system_calls to learn when it is appropriate to call this function for each platform.
	/// \sa WakeupFromSuspend
	/// \sa \ref workingwithsdks_system_calls
	virtual AKRESULT Suspend(
		bool in_bRenderAnyway = false, ///< If set to true, audio processing will still occur, but not outputted. When set to false, no audio will be processed at all, even upon reception of RenderAudio().
		bool in_bFadeOut = true        ///< Delay the suspend by one audio frame in order to fade-out. When false, the suspend takes effect immediately but audio may glitch.
		) override;

	/// This function should be called to wake up the sound engine and start processing audio again. This needs to be called if the console has a background mode or some suspended state.
	///
	/// Consult \ref workingwithsdks_system_calls to learn when it is appropriate to call this function for each platform.
	/// \sa Suspend
	/// \sa \ref workingwithsdks_system_calls
	virtual AKRESULT WakeupFromSuspend(
		AkUInt32 in_uDelayMs = 0      /// Delay (in milliseconds) before the wake up occurs. Rounded up to audio frame granularity. Adding a delay is useful if there is a possibility that another OS event may override the wake-up in the near future.
		) override;

	/// Obtains the current audio output buffer tick. This corresponds to the number of buffers produced by
	/// the sound engine since initialization. 
	/// \return Tick count.
	virtual AkUInt32 GetBufferTick() override;

	/// Obtains the current audio output sample tick. This corresponds to the number of samples produced by
	/// the sound engine since initialization. 
	/// \return Sample count.
	virtual AkUInt64 GetSampleTick() override;

	/// Query namespace
	/// \remarks The functions in this namespace are thread-safe, unless stated otherwise. We recommend that you use these functions in development builds only, because they can cause CPU spikes.
	///
	/// \akwarning
	/// The functions in this namespace might stall for several milliseconds before returning
	/// because they cannot execute while the main sound engine tick is running. 
	/// They should therefore not be called from any game-critical thread, such as the main game loop. 
	/// However, if the function definition states that it does not require the main audio lock, no delay should occur.
	///
	/// There might be a significant delay between a Sound Engine call, such as PostEvent, and
	/// the information being returned in a Query, such as GetIsGameObjectActive. 
	/// \endakwarning
	class WWISESOUNDENGINE_API FQuery : public IQuery
	{
	public:
		UE_NONCOPYABLE(FQuery);
		FQuery() = default;

		////////////////////////////////////////////////////////////////////////
		/// @name Game Objects
		//@{

		/// Get the position of a game object.
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered
		/// \sa 
		/// - \ref soundengine_3dpositions
		virtual AKRESULT GetPosition(
			AkGameObjectID in_GameObjectID,				///< Game object identifier
			AkSoundPosition& out_rPosition				///< Position to get
			) override;

		//@}

		////////////////////////////////////////////////////////////////////////
		/// @name Listeners
		//@{

		/// Get a game object's listeners.  
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered
		/// \sa 
		/// - \ref soundengine_listeners
		virtual AKRESULT GetListeners(
			AkGameObjectID in_GameObjectID,				///< Source game object identifier
			AkGameObjectID* out_ListenerObjectIDs,		///< Pointer to an array of AkGameObjectID's.  Will be populated with the IDs of the listeners of in_GameObjectID. Pass NULL to query the size required.
			AkUInt32& oi_uNumListeners					///< Pass in the the available number of elements in the array 'out_ListenerObjectIDs'. After return, the number of valid elements filled in the array.
			) override;

		/// Get a listener's position.
		/// \return AK_Success if succeeded, or AK_InvalidParameter if the index is out of range
		/// \sa 
		/// - \ref soundengine_listeners_settingpos
		virtual AKRESULT GetListenerPosition(
			AkGameObjectID in_uListenerID,				///< Listener game object ID. 
			AkListenerPosition& out_rPosition			///< Position set
			) override;

		/// Get a listener's spatialization parameters. 
		/// \return AK_Success if succeeded, or AK_InvalidParameter if the index is out of range
		/// \sa 
		/// - AK::SoundEngine::SetListenerSpatialization().
		/// - \ref soundengine_listeners_spatial
		virtual AKRESULT GetListenerSpatialization(
			AkGameObjectID in_uListenerID,				///< Listener game object ID. 
			bool& out_rbSpatialized,					///< Spatialization enabled
			AK::SpeakerVolumes::VectorPtr& out_pVolumeOffsets,	///< Per-speaker vector of volume offsets, in decibels. Use the functions of AK::SpeakerVolumes::Vector to interpret it.
			AkChannelConfig& out_channelConfig			///< Channel configuration associated with out_rpVolumeOffsets. 
			) override;

		//@}


		////////////////////////////////////////////////////////////////////////
		/// @name Game Syncs
		//@{

		/// Get the value of a real-time parameter control (by ID)
		/// An RTPC can have a any combination of a global value, a unique value for each game object, or a unique value for each playing ID.  
		/// The value requested is determined by RTPCValue_type, in_gameObjectID and in_playingID.  
		/// If a value at the requested scope (determined by RTPCValue_type) is not found, the value that is available at the the next broadest scope will be returned, and io_rValueType will be changed to indicate this.
		/// \note
		///		When looking up RTPC values via playing ID (ie. io_rValueType is RTPC_PlayingID), in_gameObjectID can be set to a specific game object (if it is available to the caller) to use as a fall back value.
		///		If the game object is unknown or unavailable, AK_INVALID_GAME_OBJECT can be passed in in_gameObjectID, and the game object will be looked up via in_playingID.  
		///		However in this case, it is not possible to retrieve a game object value as a fall back value if the playing id does not exist.  It is best to pass in the game object if possible.
		///		
		/// \return AK_Success if succeeded, AK_IDNotFound if the RTPC does not exist
		/// \sa 
		/// - \ref soundengine_rtpc
		/// - RTPCValue_type
		virtual AKRESULT GetRTPCValue(
			AkRtpcID in_rtpcID, 				///< ID of the RTPC
			AkGameObjectID in_gameObjectID,		///< Associated game object ID, ignored if io_rValueType is RTPCValue_Global.
			AkPlayingID	in_playingID,			///< Associated playing ID, ignored if io_rValueType is not RTPC_PlayingID.
			AkRtpcValue& out_rValue, 			///< Value returned
			AK::SoundEngine::Query::RTPCValue_type& io_rValueType		///< In/Out value, the user must specify the requested type. The function will return in this variable the type of the returned value.
			) override;

#ifdef AK_SUPPORT_WCHAR

		/// Get the value of a real-time parameter control (by ID)
		/// An RTPC can have a any combination of a global value, a unique value for each game object, or a unique value for each playing ID.  
		/// The value requested is determined by RTPCValue_type, in_gameObjectID and in_playingID.  
		/// If a value at the requested scope (determined by RTPCValue_type) is not found, the value that is available at the the next broadest scope will be returned, and io_rValueType will be changed to indicate this.
		/// \note
		///		When looking up RTPC values via playing ID (ie. io_rValueType is RTPC_PlayingID), in_gameObjectID can be set to a specific game object (if it is available to the caller) to use as a fall back value.
		///		If the game object is unknown or unavailable, AK_INVALID_GAME_OBJECT can be passed in in_gameObjectID, and the game object will be looked up via in_playingID.  
		///		However in this case, it is not possible to retrieve a game object value as a fall back value if the playing id does not exist.  It is best to pass in the game object if possible.
		///		
		/// \return AK_Success if succeeded, AK_IDNotFound if the RTPC does not exist
		/// \sa 
		/// - \ref soundengine_rtpc
		/// - RTPCValue_type
		virtual AKRESULT GetRTPCValue(
			const wchar_t* in_pszRtpcName,		///< String name of the RTPC
			AkGameObjectID in_gameObjectID,		///< Associated game object ID, ignored if io_rValueType is RTPCValue_Global.
			AkPlayingID	in_playingID,			///< Associated playing ID, ignored if io_rValueType is not RTPC_PlayingID.
			AkRtpcValue& out_rValue, 			///< Value returned
			AK::SoundEngine::Query::RTPCValue_type& io_rValueType		///< In/Out value, the user must specify the requested type. The function will return in this variable the type of the returned value.				) = 0;
			) override;

#endif //AK_SUPPORT_WCHAR

		/// Get the value of a real-time parameter control (by ID)
		/// An RTPC can have a any combination of a global value, a unique value for each game object, or a unique value for each playing ID.  
		/// The value requested is determined by RTPCValue_type, in_gameObjectID and in_playingID.  
		/// If a value at the requested scope (determined by RTPCValue_type) is not found, the value that is available at the the next broadest scope will be returned, and io_rValueType will be changed to indicate this.
		/// \note
		///		When looking up RTPC values via playing ID (ie. io_rValueType is RTPC_PlayingID), in_gameObjectID can be set to a specific game object (if it is available to the caller) to use as a fall back value.
		///		If the game object is unknown or unavailable, AK_INVALID_GAME_OBJECT can be passed in in_gameObjectID, and the game object will be looked up via in_playingID.  
		///		However in this case, it is not possible to retrieve a game object value as a fall back value if the playing id does not exist.  It is best to pass in the game object if possible.
		///		
		/// \return AK_Success if succeeded, AK_IDNotFound if the RTPC does not exist
		/// \sa 
		/// - \ref soundengine_rtpc
		/// - RTPCValue_type
		virtual AKRESULT GetRTPCValue(
			const char* in_pszRtpcName,			///< String name of the RTPC
			AkGameObjectID in_gameObjectID,		///< Associated game object ID, ignored if io_rValueType is RTPCValue_Global.
			AkPlayingID	in_playingID,			///< Associated playing ID, ignored if io_rValueType is not RTPC_PlayingID.
			AkRtpcValue& out_rValue, 			///< Value returned
			AK::SoundEngine::Query::RTPCValue_type& io_rValueType		///< In/Out value, the user must specify the requested type. The function will return in this variable the type of the returned value.				) = 0;
			) override;

		/// Get the state of a switch group (by IDs).
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered
		/// \sa 
		/// - \ref soundengine_switch
		virtual AKRESULT GetSwitch(
			AkSwitchGroupID in_switchGroup, 			///< ID of the switch group
			AkGameObjectID  in_gameObjectID,			///< Associated game object ID
			AkSwitchStateID& out_rSwitchState 			///< ID of the switch
			) override;

#ifdef AK_SUPPORT_WCHAR
		/// Get the state of a switch group.
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered or the switch group name can not be found
		/// \sa 
		/// - \ref soundengine_switch
		virtual AKRESULT GetSwitch(
			const wchar_t* in_pstrSwitchGroupName,			///< String name of the switch group
			AkGameObjectID in_GameObj,					///< Associated game object ID
			AkSwitchStateID& out_rSwitchState			///< ID of the switch
			) override;
#endif //AK_SUPPORT_WCHAR

		/// Get the state of a switch group.
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered or the switch group name can not be found
		/// \sa 
		/// - \ref soundengine_switch
		virtual AKRESULT GetSwitch(
			const char* in_pstrSwitchGroupName,			///< String name of the switch group
			AkGameObjectID in_GameObj,					///< Associated game object ID
			AkSwitchStateID& out_rSwitchState			///< ID of the switch
			) override;

		/// Get the state of a state group (by IDs).
		/// \return AK_Success if succeeded
		/// \sa 
		/// - \ref soundengine_states
		virtual AKRESULT GetState(
			AkStateGroupID in_stateGroup, 				///< ID of the state group
			AkStateID& out_rState 						///< ID of the state
			) override;

#ifdef AK_SUPPORT_WCHAR
		/// Get the state of a state group.
		/// \return AK_Success if succeeded, or AK_IDNotFound if the state group name can not be found
		/// \sa 
		/// - \ref soundengine_states
		virtual AKRESULT GetState(
			const wchar_t* in_pstrStateGroupName,			///< String name of the state group
			AkStateID& out_rState						///< ID of the state
			) override;
#endif //AK_SUPPORT_WCHAR

		/// Get the state of a state group.
		/// \return AK_Success if succeeded, or AK_IDNotFound if the state group name can not be found
		/// \sa 
		/// - \ref soundengine_states
		virtual AKRESULT GetState(
			const char* in_pstrStateGroupName,			    ///< String name of the state group
			AkStateID& out_rState						///< ID of the state
			) override;

		//@}

		////////////////////////////////////////////////////////////////////////
		/// @name Environments
		//@{

		/// Get the environmental ratios used by the specified game object.
		/// \sa 
		/// - \ref soundengine_environments
		/// - \ref soundengine_environments_dynamic_aux_bus_routing
		/// - \ref soundengine_environments_id_vs_string
		/// \return AK_Success if succeeded, or AK_InvalidParameter if io_ruNumSendValues is 0 or out_paEnvironmentValues is NULL, or AK_PartialSuccess if more environments exist than io_ruNumSendValues
		virtual AKRESULT GetGameObjectAuxSendValues(
			AkGameObjectID		in_gameObjectID,		///< Associated game object ID
			AkAuxSendValue* out_paAuxSendValues,	///< Variable-size array of AkAuxSendValue structures
															///< (it may be NULL if no aux send must be set)
			AkUInt32& io_ruNumSendValues		///< The number of Auxiliary busses at the pointer's address
														///< (it must be 0 if no aux bus is set)
			) override;

		/// Get the environmental dry level to be used for the specified game object
		/// The control value is a number ranging from 0.0f to 1.0f.
		/// 0.0f stands for 0% dry, while 1.0f stands for 100% dry.
		/// \aknote Reducing the dry level does not mean increasing the wet level. \endaknote
		/// \sa 
		/// - \ref soundengine_environments
		/// - \ref soundengine_environments_setting_dry_environment
		/// - \ref soundengine_environments_id_vs_string
		/// \return AK_Success if succeeded, or AK_IDNotFound if the game object was not registered
		virtual AKRESULT GetGameObjectDryLevelValue(
			AkGameObjectID		in_EmitterID,			///< Associated emitter game object ID
			AkGameObjectID		in_ListenerID,			///< Associated listener game object ID
			AkReal32& out_rfControlValue		///< Dry level control value, ranging from 0.0f to 1.0f
														///< (0.0f stands for 0% dry, while 1.0f stands for 100% dry)
			) override;

		/// Get a game object's obstruction and occlusion levels.
		/// \sa 
		/// - \ref soundengine_obsocc
		/// - \ref soundengine_environments
		/// \return AK_Success if succeeded, AK_IDNotFound if the game object was not registered
		virtual AKRESULT GetObjectObstructionAndOcclusion(
			AkGameObjectID in_EmitterID,			///< Associated game object ID
			AkGameObjectID in_ListenerID,			///< Listener object ID
			AkReal32& out_rfObstructionLevel,		///< ObstructionLevel: [0.0f..1.0f]
			AkReal32& out_rfOcclusionLevel			///< OcclusionLevel: [0.0f..1.0f]
			) override;

		//@}

		/// Get the list of audio object IDs associated to an event.
		/// \aknote It is possible to call QueryAudioObjectIDs with io_ruNumItems = 0 to get the total size of the
		/// structure that should be allocated for out_aObjectInfos. \endaknote
		/// \return AK_Success if succeeded, AK_IDNotFound if the eventID cannot be found, AK_InvalidParameter if out_aObjectInfos is NULL while io_ruNumItems > 0
		/// or AK_PartialSuccess if io_ruNumItems was set to 0 to query the number of available items.
		virtual AKRESULT QueryAudioObjectIDs(
			AkUniqueID in_eventID,				///< Event ID
			AkUInt32& io_ruNumItems,			///< Number of items in array provided / Number of items filled in array
			AkObjectInfo* out_aObjectInfos		///< Array of AkObjectInfo items to fill
			) override;

#ifdef AK_SUPPORT_WCHAR
		/// Get the list of audio object IDs associated to a event name.
		/// \aknote It is possible to call QueryAudioObjectIDs with io_ruNumItems = 0 to get the total size of the
		/// structure that should be allocated for out_aObjectInfos. \endaknote
		/// \return AK_Success if succeeded, AK_IDNotFound if the event name cannot be found, AK_InvalidParameter if out_aObjectInfos is NULL while io_ruNumItems > 0
		/// or AK_PartialSuccess if io_ruNumItems was set to 0 to query the number of available items.
		virtual AKRESULT QueryAudioObjectIDs(
			const wchar_t* in_pszEventName,		///< Event name
			AkUInt32& io_ruNumItems,			///< Number of items in array provided / Number of items filled in array
			AkObjectInfo* out_aObjectInfos		///< Array of AkObjectInfo items to fill
			) override;
#endif //AK_SUPPORT_WCHAR

		/// Get the list of audio object IDs associated to an event name.
		/// \aknote It is possible to call QueryAudioObjectIDs with io_ruNumItems = 0 to get the total size of the
		/// structure that should be allocated for out_aObjectInfos. \endaknote
		/// \return AK_Success if succeeded, AK_IDNotFound if the event name cannot be found, AK_InvalidParameter if out_aObjectInfos is NULL while io_ruNumItems > 0
		/// or AK_PartialSuccess if io_ruNumItems was set to 0 to query the number of available items.
		virtual AKRESULT QueryAudioObjectIDs(
			const char* in_pszEventName,		///< Event name
			AkUInt32& io_ruNumItems,			///< Number of items in array provided / Number of items filled in array
			AkObjectInfo* out_aObjectInfos		///< Array of AkObjectInfo items to fill
			) override;

		/// Get positioning information associated to an audio object.
		/// \return AK_Success if succeeded, AK_IDNotFound if the object ID cannot be found, AK_NotCompatible if the audio object cannot expose positioning
		virtual AKRESULT GetPositioningInfo(
			AkUniqueID in_ObjectID,						///< Audio object ID
			AkPositioningInfo& out_rPositioningInfo		///< Positioning information structure to be filled
			) override;

		/// Fill the provided list with all the game object IDs that are currently active in the sound engine.
		/// The function may be used to avoid updating game objects positions that are not required at the moment.
		/// After calling this function, the list will contain the list of all game objects that are currently active in the sound engine.
		/// Being active means that either a sound is playing or pending to be played using this game object.
		/// \sa 
		/// - AkGameObjectsList
		virtual AKRESULT GetActiveGameObjects(
			FAkGameObjectsList& io_GameObjectList	///< returned list of active game objects.
			) override;

		/// Query if the specified game object is currently active.
		/// Being active means that either a sound is playing or pending to be played using this game object.
		virtual bool GetIsGameObjectActive(
			AkGameObjectID in_GameObjId ///< Game object ID
			) override;

		/// Returns the maximum distance used in attenuations associated to all sounds currently playing.
		/// This may be used for example by the game to know if some processing need to be performed on the game side, that would not be required
		/// if the object is out of reach anyway.
		///
		/// Example usage:
		/// \code
		/// /*******************************************************/
		/// AkRadiusList RadLst; //creating the list( array ).
		/// // Do not reserve any size for the array, 
		/// // the system will reserve the correct size.
		///
		/// GetMaxRadius( RadLst ) = 0;
		/// // Use the content of the list
		/// (...)
		///
		/// RadLst.Term() = 0;// the user is responsible to free the memory allocated
		/// /*******************************************************/
		/// \endcode
		///
		/// \aknote The returned value is NOT the distance from a listener to an object but
		/// the maximum attenuation distance of all sounds playing on this object. This is
		/// not related in any way to the current 3D position of the object. \endaknote
		///
		/// \return 
		/// - AK_Success if succeeded
		/// - AK_InsufficientMemory if there was not enough memory
		///
		/// \aknote 
		/// The Scaling factor (if one was specified on the game object) is included in the return value.
		/// The Scaling factor is not updated once a sound starts playing since it 
		/// is computed only when the playback starts with the initial scaling factor of this game object. Scaling factor will 
		/// be re-computed for every playback instance, always using the scaling factor available at this time.
		/// \endaknote
		///
		/// \sa 
		/// - AkRadiusList
		virtual AKRESULT GetMaxRadius(
			FAkRadiusList& io_RadiusList	///< List that will be filled with AK::SoundEngine::Query::GameObjDst objects.
			) override;

		/// Returns the maximum distance used in attenuation associated to sounds playing using the specified game object.
		/// This may be used for example by the game to know if some processing need to be performed on the game side, that would not be required
		/// if the object is out of reach anyway.
		/// 
		/// \aknote The returned value is NOT the distance from a listener to an object but the maximum attenuation distance of all sounds playing on this object. \endaknote
		///
		/// \return
		/// - A negative number if the game object specified is not playing.
		/// - 0, if the game object was only associated to sounds playing using no distance attenuation.
		/// - A positive number represents the maximum of all the distance attenuation playing on this game object.
		///
		/// \aknote 
		/// The Scaling factor (if one was specified on the game object) is included in the return value.
		/// The Scaling factor is not updated once a sound starts playing since it 
		/// is computed only when the playback starts with the initial scaling factor of this game object. Scaling factor will 
		/// be re-computed for every playback instance, always using the scaling factor available at this time.
		/// \endaknote
		///
		/// \sa 
		/// - \ref AK::SoundEngine::SetScalingFactor
		virtual AkReal32 GetMaxRadius(
			AkGameObjectID in_GameObjId ///< Game object ID
			) override;

		/// Get the Event ID associated to the specified PlayingID.
		/// This function does not acquire the main audio lock.
		///
		/// \return AK_INVALID_UNIQUE_ID on failure.
		virtual AkUniqueID GetEventIDFromPlayingID(
			AkPlayingID in_playingID ///< Associated PlayingID
			) override;

		/// Get the ObjectID associated to the specified PlayingID.
		/// This function does not acquire the main audio lock.
		///
		/// \return AK_INVALID_GAME_OBJECT on failure.
		virtual AkGameObjectID GetGameObjectFromPlayingID(
			AkPlayingID in_playingID ///< Associated PlayingID
			) override;

		/// Get the list PlayingIDs associated with the given game object.
		/// This function does not acquire the main audio lock.
		///
		/// \aknote It is possible to call GetPlayingIDsFromGameObject with io_ruNumItems = 0 to get the total size of the
		/// structure that should be allocated for out_aPlayingIDs. \endaknote
		/// \return AK_Success if succeeded, AK_InvalidParameter if out_aPlayingIDs is NULL while io_ruNumItems > 0
		virtual AKRESULT GetPlayingIDsFromGameObject(
			AkGameObjectID in_GameObjId,		///< Game object ID
			AkUInt32& io_ruNumIDs,				///< Number of items in array provided / Number of items filled in array
			AkPlayingID* out_aPlayingIDs		///< Array of AkPlayingID items to fill
			) override;

		/// Get the value of a custom property of integer or boolean type.
		/// \return AK_PartialSuccess if the object was found but no matching custom property was found on this object. Note that it could mean this value is the default value. 
		virtual AKRESULT GetCustomPropertyValue(
			AkUniqueID in_ObjectID,			///< Object ID, this is the 32bit ShortID of the AudioFileSource or Sound object found in the .wwu XML file. At runtime it can only be retrieved by the AK_Duration callback when registered with PostEvent(), or by calling Query::QueryAudioObjectIDs() to get all the shortIDs associated with an event.
			AkUInt32 in_uPropID,			///< Property ID of your custom property found under the Custom Properties tab of the Wwise project settings.
			AkInt32& out_iValue				///< Property Value
			) override;

		/// Get the value of a custom property of real type.
		/// \return AK_PartialSuccess if the object was found but no matching custom property was found on this object. Note that it could mean this value is the default value.
		virtual AKRESULT GetCustomPropertyValue(
			AkUniqueID in_ObjectID,			///< Object ID, this is the 32bit ShortID of the AudioFileSource or Sound object found in the .wwu XML file. At runtime it can only be retrieved by the AK_Duration callback when registered with PostEvent(), or by calling Query::QueryAudioObjectIDs() to get all the shortIDs associated with an event.
			AkUInt32 in_uPropID,			///< Property ID of your custom property found under the Custom Properties tab of the Wwise project settings.
			AkReal32& out_fValue			///< Property Value
			) override;
	};

	class WWISESOUNDENGINE_API FAudioInputPlugin : public IAudioInputPlugin
	{
	public:
		UE_NONCOPYABLE(FAudioInputPlugin);
		FAudioInputPlugin() = default;

		void SetAudioInputCallbacks(
			AkAudioInputPluginExecuteCallbackFunc in_pfnExecCallback,
			AkAudioInputPluginGetFormatCallbackFunc in_pfnGetFormatCallback = nullptr, // Optional
			AkAudioInputPluginGetGainCallbackFunc in_pfnGetGainCallback = nullptr      // Optional
		) override;
	};

#if WITH_EDITORONLY_DATA
	class WWISESOUNDENGINE_API FErrorTranslator : public AkErrorMessageTranslator
	{
		FGetInfoErrorMessageTranslatorFunction GetInfoErrorMessageTranslatorFunction;

	public:

		FErrorTranslator(FGetInfoErrorMessageTranslatorFunction InMessageTranslatorFunction);
		virtual void Term() override {};
		virtual bool GetInfo(TagInformation* in_pTagList, AkUInt32 in_uCount, AkUInt32& out_uTranslated) override;
	};
#endif

	AkErrorMessageTranslator* NewErrorMessageTranslator(FGetInfoErrorMessageTranslatorFunction InMessageTranslatorFunction) override;
};
