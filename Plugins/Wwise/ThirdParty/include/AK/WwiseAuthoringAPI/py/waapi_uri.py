'''

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

'''
class WAAPI_URI:
    # Executes an action on all nodes that are referenced in the specified event in a Play action. See <tt>AK::SoundEngine::ExecuteActionOnEvent</tt>.
    ak_soundengine_executeactiononevent = u"ak.soundengine.executeActionOnEvent"
    # Gets the current state of a State Group. When using setState just prior to getState, allow a brief delay (no more than 10ms) for the information to update in the sound engine.
    ak_soundengine_getstate = u"ak.soundengine.getState"
    # Gets the current state of a Switch Group for a given Game Object.
    ak_soundengine_getswitch = u"ak.soundengine.getSwitch"
    # Load a SoundBank. See <tt>AK::SoundEngine::LoadBank</tt>.
    ak_soundengine_loadbank = u"ak.soundengine.loadBank"
    # Asynchronously post an Event to the sound engine (by event ID). See <tt>AK::SoundEngine::PostEvent</tt>.
    ak_soundengine_postevent = u"ak.soundengine.postEvent"
    # Display a message in the Profiler's Capture Log view.
    ak_soundengine_postmsgmonitor = u"ak.soundengine.postMsgMonitor"
    # Posts the specified Trigger. See <tt>AK::SoundEngine::PostTrigger</tt>.
    ak_soundengine_posttrigger = u"ak.soundengine.postTrigger"
    # Register a game object. Registering a game object twice does nothing. Unregistering it once unregisters it no matter how many times it has been registered. See <tt>AK::SoundEngine::RegisterGameObj</tt>.
    ak_soundengine_registergameobj = u"ak.soundengine.registerGameObj"
    # Resets the value of a real-time parameter control to its default value, as specified in the Wwise project. See <tt>AK::SoundEngine::ResetRTPCValue</tt>.
    ak_soundengine_resetrtpcvalue = u"ak.soundengine.resetRTPCValue"
    # Seeks inside all playing objects that are referenced in Play Actions of the specified Event. See <tt>AK::SoundEngine::SeekOnEvent</tt>.
    ak_soundengine_seekonevent = u"ak.soundengine.seekOnEvent"
    # Sets the default active listeners for all subsequent game objects that are registered. See <tt>AK::SoundEngine::SetDefaultListeners</tt>.
    ak_soundengine_setdefaultlisteners = u"ak.soundengine.setDefaultListeners"
    # Sets the Auxiliary Busses to route the specified game object. See <tt>AK::SoundEngine::SetGameObjectAuxSendValues</tt>.
    ak_soundengine_setgameobjectauxsendvalues = u"ak.soundengine.setGameObjectAuxSendValues"
    # Set the output bus volume (direct) to be used for the specified game object. See <tt>AK::SoundEngine::SetGameObjectOutputBusVolume</tt>.
    ak_soundengine_setgameobjectoutputbusvolume = u"ak.soundengine.setGameObjectOutputBusVolume"
    # Sets a listener's spatialization parameters. This lets you define listener-specific volume offsets for each audio channel. See <tt>AK::SoundEngine::SetListenerSpatialization</tt>.
    ak_soundengine_setlistenerspatialization = u"ak.soundengine.setListenerSpatialization"
    # Sets a single game object's active listeners. By default, all new game objects have no listeners active, but this behavior can be overridden with <tt>SetDefaultListeners()</tt>. Inactive listeners are not computed. See <tt>AK::SoundEngine::SetListeners</tt>.
    ak_soundengine_setlisteners = u"ak.soundengine.setListeners"
    # Sets multiple positions for a single game object. Setting multiple positions for a single game object is a way to simulate multiple emission sources while using the resources of only one voice. This can be used to simulate wall openings, area sounds, or multiple objects emitting the same sound in the same area. See <tt>AK::SoundEngine::SetMultiplePositions</tt>.
    ak_soundengine_setmultiplepositions = u"ak.soundengine.setMultiplePositions"
    # Set a game object's obstruction and occlusion levels. This function is used to affect how an object should be heard by a specific listener. See <tt>AK::SoundEngine::SetObjectObstructionAndOcclusion</tt>.
    ak_soundengine_setobjectobstructionandocclusion = u"ak.soundengine.setObjectObstructionAndOcclusion"
    # Sets the position of a game object. See <tt>AK::SoundEngine::SetPosition</tt>.
    ak_soundengine_setposition = u"ak.soundengine.setPosition"
    # Sets the value of a real-time parameter control. See <tt>AK::SoundEngine::SetRTPCValue</tt>.
    ak_soundengine_setrtpcvalue = u"ak.soundengine.setRTPCValue"
    # Sets the scaling factor of a game object. You can modify the attenuation computations on this game object to simulate sounds with a larger or smaller affected areas. See <tt>AK::SoundEngine::SetScalingFactor</tt>.
    ak_soundengine_setscalingfactor = u"ak.soundengine.setScalingFactor"
    # Sets the State of a State Group. See <tt>AK::SoundEngine::SetState</tt>.
    ak_soundengine_setstate = u"ak.soundengine.setState"
    # Sets the State of a Switch Group. See <tt>AK::SoundEngine::SetSwitch</tt>.
    ak_soundengine_setswitch = u"ak.soundengine.setSwitch"
    # Stop playing the current content associated to the specified game object ID. If no game object is specified, all sounds are stopped. See <tt>AK::SoundEngine::StopAll</tt>.
    ak_soundengine_stopall = u"ak.soundengine.stopAll"
    # Stops the current content, associated to the specified playing ID, from playing. See <tt>AK::SoundEngine::StopPlayingID</tt>.
    ak_soundengine_stopplayingid = u"ak.soundengine.stopPlayingID"
    # Unload a SoundBank. See <tt>AK::SoundEngine::UnloadBank</tt>.
    ak_soundengine_unloadbank = u"ak.soundengine.unloadBank"
    # Unregisters a game object. Registering a game object twice does nothing. Unregistering it once unregisters it no matter how many times it has been registered. Unregistering a game object while it is in use is allowed, but the control over the parameters of this game object is lost. For example, say a sound associated with this game object is a 3D moving sound. It stops moving when the game object is unregistered, and there is no way to regain control over the game object. See <tt>AK::SoundEngine::UnregisterGameObj</tt>.
    ak_soundengine_unregistergameobj = u"ak.soundengine.unregisterGameObj"
    # Adds a new platform to a project. The platform must not already exist.
    ak_wwise_cli_addnewplatform = u"ak.wwise.cli.addNewPlatform"
    # External sources conversion. Converts the external sources files for the specified project. Optionally, additional WSOURCES can be specified. External Sources are a special type of source that you can put in a sound object in Wwise. It indicates that the real sound data will be provided at runtime. While external source conversion can be triggered by SoundBank generation, this operation can be used to process sources not contained in the Wwise Project. Refer to \ref integrating_external_sources.
    ak_wwise_cli_convertexternalsource = u"ak.wwise.cli.convertExternalSource"
    # Creates a blank new project. The project must not already exist. If the directory does not exist, it is created.
    ak_wwise_cli_createnewproject = u"ak.wwise.cli.createNewProject"
    # Dump the objects model of a project as a JSON file.
    ak_wwise_cli_dumpobjects = u"ak.wwise.cli.dumpObjects"
    # Execute a Lua script. Optionally, specify additional Lua search paths, additional modules, and additional Lua scripts to load prior to the main script. The script can return a value. All arguments will be passed to the Lua script in the "wa_args" global variable.
    ak_wwise_cli_executeluascript = u"ak.wwise.cli.executeLuaScript"
    # SoundBank generation. SoundBank generation is performed according to the settings stored in the project. User SoundBanks Settings are normally ignored when SoundBank generation is launched from the command line. However, when using the Source Control for generated SoundBanks, the User Project Settings are loaded for the Source Control settings. Also, some of these settings can be overridden from the command line.
    ak_wwise_cli_generatesoundbank = u"ak.wwise.cli.generateSoundbank"
    # Migrates and saves the project.
    ak_wwise_cli_migrate = u"ak.wwise.cli.migrate"
    # Moves the project's media IDs from its work units (.wwu) to a single file, <project-name>.mediaid.  This command forces a save of all the project's work units.
    ak_wwise_cli_movemediaidstosinglefile = u"ak.wwise.cli.moveMediaIdsToSingleFile"
    # Moves the project's media IDs from a single xml file <project-name>.mediaid to its work units (.wwu).  This command forces a save of all the project's work units.
    ak_wwise_cli_movemediaidstoworkunits = u"ak.wwise.cli.moveMediaIdsToWorkUnits"
    # Imports a tab-delimited file to create and modify different object hierarchies. The project is automatically migrated (if required). It is also automatically saved following the import.
    ak_wwise_cli_tabdelimitedimport = u"ak.wwise.cli.tabDelimitedImport"
    # Loads the project and updates the contents of <project-name>.mediaid, if it exists.
    ak_wwise_cli_updatemediaidsinsinglefile = u"ak.wwise.cli.updateMediaIdsInSingleFile"
    # Loads the project and does nothing else. This is useful to see the log for verification purposes without actually migrating and saving.
    ak_wwise_cli_verify = u"ak.wwise.cli.verify"
    # Starts a command-line Wwise Authoring API server, to which client applications, using the Wwise Authoring API, can connect.
    ak_wwise_cli_waapiserver = u"ak.wwise.cli.waapiServer"
    # Closes the current project. This operation is synchronous.
    ak_wwise_console_project_close = u"ak.wwise.console.project.close"
    # Creates, saves and opens new empty project, specified by path and platform. The project has no factory setting WorkUnit. This operation is synchronous.
    ak_wwise_console_project_create = u"ak.wwise.console.project.create"
    # Opens a project, specified by path. This operation is synchronous.
    ak_wwise_console_project_open = u"ak.wwise.console.project.open"
    # Creates Wwise objects and imports audio files. This function does not return an error when something fails during the import process, please refer to the log for the result of each import command. This function uses the same importation processor available through the Tab Delimited import in the Audio File Importer. The function returns an array of all objects created, replaced or re-used. Use the options to specify how the objects are returned. For more information, refer to \ref waapi_import.
    ak_wwise_core_audio_import = u"ak.wwise.core.audio.import"
    # Scripted object creation and audio file import from a tab-delimited file.
    ak_wwise_core_audio_importtabdelimited = u"ak.wwise.core.audio.importTabDelimited"
    # Sent at the end of an import operation.
    ak_wwise_core_audio_imported = u"ak.wwise.core.audio.imported"
    # Mutes an object.
    ak_wwise_core_audio_mute = u"ak.wwise.core.audio.mute"
    # Unmute all muted objects.
    ak_wwise_core_audio_resetmute = u"ak.wwise.core.audio.resetMute"
    # Unsolo all soloed objects.
    ak_wwise_core_audio_resetsolo = u"ak.wwise.core.audio.resetSolo"
    # Solos an object.
    ak_wwise_core_audio_solo = u"ak.wwise.core.audio.solo"
    # Gets the min/max peak pairs, in the given region of an audio source, as a collection of binary strings (one per channel). The strings are base-64 encoded, 16-bit signed int arrays, with min and max values being interleaved. If getCrossChannelPeaks is true, only one binary string represents the peaks across all channels globally.
    ak_wwise_core_audiosourcepeaks_getminmaxpeaksinregion = u"ak.wwise.core.audioSourcePeaks.getMinMaxPeaksInRegion"
    # Gets the min/max peak pairs in the entire trimmed region of an audio source, for each channel, as an array of binary strings (one per channel). The strings are base-64 encoded, 16-bit signed int arrays, with min and max values being interleaved. If getCrossChannelPeaks is true, there is only one binary string representing peaks across all channels globally.
    ak_wwise_core_audiosourcepeaks_getminmaxpeaksintrimmedregion = u"ak.wwise.core.audioSourcePeaks.getMinMaxPeaksInTrimmedRegion"
    # Execute a Lua script. Optionally, specify additional Lua search paths, additional modules, and additional Lua scripts to load prior to the main script. The script can return a value. All arguments will be passed to the Lua script in the "wa_args" global variable.
    ak_wwise_core_executeluascript = u"ak.wwise.core.executeLuaScript"
    # Retrieve global Wwise information.
    ak_wwise_core_getinfo = u"ak.wwise.core.getInfo"
    # Retrieve information about the current project opened, including platforms, languages and project directories.
    ak_wwise_core_getprojectinfo = u"ak.wwise.core.getProjectInfo"
    # Adds a new item to the logs on the specified channel.
    ak_wwise_core_log_additem = u"ak.wwise.core.log.addItem"
    # Clears the logs on the specified channel.
    ak_wwise_core_log_clear = u"ak.wwise.core.log.clear"
    # Retrieves the latest log for a specific channel. Refer to \ref ak_wwise_core_log_itemadded to be notified when an item is added to the log. The log is empty when used in WwiseConsole.
    ak_wwise_core_log_get = u"ak.wwise.core.log.get"
    # Sent when an item is added to the log. This could be used to retrieve items added to the SoundBank generation log. To retrieve the complete log, refer to \ref ak_wwise_core_log_get.
    ak_wwise_core_log_itemadded = u"ak.wwise.core.log.itemAdded"
    # Sent when an attenuation curve is changed.
    ak_wwise_core_object_attenuationcurvechanged = u"ak.wwise.core.object.attenuationCurveChanged"
    # Sent when an attenuation curve's link/unlink is changed.
    ak_wwise_core_object_attenuationcurvelinkchanged = u"ak.wwise.core.object.attenuationCurveLinkChanged"
    # Sent when an object is added as a child to another object.
    ak_wwise_core_object_childadded = u"ak.wwise.core.object.childAdded"
    # Sent when an object is removed from the children of another object.
    ak_wwise_core_object_childremoved = u"ak.wwise.core.object.childRemoved"
    # Copies an object to the given parent. Note that if a Work Unit is copied, the operation cannot be undone and the project will be saved.
    ak_wwise_core_object_copy = u"ak.wwise.core.object.copy"
    # Creates an object of type 'type', as a child of 'parent'. Refer to \ref waapi_import for more information about creating objects. Also refer to \ref ak_wwise_core_audio_import to import audio files to Wwise. To create Effect or Source plug-ins, use \ref ak_wwise_core_object_set, and refer to \ref wobjects_index for the classId.
    ak_wwise_core_object_create = u"ak.wwise.core.object.create"
    # Sent when an object is created.
    ak_wwise_core_object_created = u"ak.wwise.core.object.created"
    # Sent when one or many curves are changed.
    ak_wwise_core_object_curvechanged = u"ak.wwise.core.object.curveChanged"
    # Deletes the specified object. Note that if a Work Unit is deleted, the operation cannot be undone and the project will be saved.
    ak_wwise_core_object_delete = u"ak.wwise.core.object.delete"
    # Compares properties and lists of the source object with those in the target object.
    ak_wwise_core_object_diff = u"ak.wwise.core.object.diff"
    # Performs a query and returns the data, as specified in the options, for each object in the query result. The query can specify either a 'waql' argument or a 'from' argument with an optional 'transform' argument. Refer to \ref waql_introduction or \ref waapi_query for more information. Refer to \ref waapi_query_return to learn about options.
    ak_wwise_core_object_get = u"ak.wwise.core.object.get"
    # Gets the specified attenuation curve for a given attenuation object.
    ak_wwise_core_object_getattenuationcurve = u"ak.wwise.core.object.getAttenuationCurve"
    # Retrieves the list of property and reference names for an object.
    ak_wwise_core_object_getpropertyandreferencenames = u"ak.wwise.core.object.getPropertyAndReferenceNames"
    # Retrieves information about an object property. Note that this function does not return the value of a property. To retrieve the value of a property, refer to \ref ak_wwise_core_object_get and \ref waapi_query_return.
    ak_wwise_core_object_getpropertyinfo = u"ak.wwise.core.object.getPropertyInfo"
    # Retrieves the list of property and reference names for an object.
    # \deprecated in favor of ak.wwise.core.object.getPropertyAndReferenceNames
    ak_wwise_core_object_getpropertynames = u"ak.wwise.core.object.getPropertyNames"
    # Retrieves the list of all object types registered in Wwise's object model. This function returns the equivalent of \ref wobjects_index .
    ak_wwise_core_object_gettypes = u"ak.wwise.core.object.getTypes"
    # Indicates whether a property, reference, or object list is bound to a particular platform or to all platforms.
    ak_wwise_core_object_islinked = u"ak.wwise.core.object.isLinked"
    # Returns true if a property is enabled based on the values of the properties it depends on.
    ak_wwise_core_object_ispropertyenabled = u"ak.wwise.core.object.isPropertyEnabled"
    # Moves an object to the given parent. Returns the moved object.
    ak_wwise_core_object_move = u"ak.wwise.core.object.move"
    # Sent when an object is renamed. Publishes the renamed object.
    ak_wwise_core_object_namechanged = u"ak.wwise.core.object.nameChanged"
    # Sent when the object's notes are changed.
    ak_wwise_core_object_noteschanged = u"ak.wwise.core.object.notesChanged"
    # Pastes properties, references and lists from one object to any number of target objects. Only those properties, references and lists which differ between source and target are pasted. Refer to \ref wobjects_index for more information on the properties, references and lists available on each object type.
    ak_wwise_core_object_pasteproperties = u"ak.wwise.core.object.pasteProperties"
    # Sent following an object's deletion.
    ak_wwise_core_object_postdeleted = u"ak.wwise.core.object.postDeleted"
    # Sent prior to an object's deletion.
    ak_wwise_core_object_predeleted = u"ak.wwise.core.object.preDeleted"
    # Sent when the watched property of an object changes.
    ak_wwise_core_object_propertychanged = u"ak.wwise.core.object.propertyChanged"
    # Sent when an object reference is changed.
    ak_wwise_core_object_referencechanged = u"ak.wwise.core.object.referenceChanged"
    # Allows for batch processing of the following operations: Object creation in a child hierarchy, Object creation in a list, Setting name, notes, properties and references. Refer to \ref waapi_import for more information about creating objects. Also refer to \ref ak_wwise_core_audio_import to import audio files to Wwise.
    ak_wwise_core_object_set = u"ak.wwise.core.object.set"
    # Sets the specified attenuation curve for a given attenuation object.
    ak_wwise_core_object_setattenuationcurve = u"ak.wwise.core.object.setAttenuationCurve"
    # Link or unlink a property/reference or object list to a particular platform.
    ak_wwise_core_object_setlinked = u"ak.wwise.core.object.setLinked"
    # Renames an object.
    ak_wwise_core_object_setname = u"ak.wwise.core.object.setName"
    # Sets the object's notes.
    ak_wwise_core_object_setnotes = u"ak.wwise.core.object.setNotes"
    # Sets a property value of an object for a specific platform. Refer to \ref wobjects_index for more information on the properties available on each object type. Refer to \ref ak_wwise_core_object_setreference to set a reference to an object. Refer to \ref ak_wwise_core_object_get to obtain the value of a property for an object.
    ak_wwise_core_object_setproperty = u"ak.wwise.core.object.setProperty"
    # Sets the randomizer values of a property of an object for a specific platform. Refer to \ref wobjects_index for more information on the properties available on each object type.
    ak_wwise_core_object_setrandomizer = u"ak.wwise.core.object.setRandomizer"
    # Sets an object's reference value. Refer to \ref wobjects_index for more information on the references available on each object type.
    ak_wwise_core_object_setreference = u"ak.wwise.core.object.setReference"
    # Sets the State Group objects associated with an object. Note, this will remove any previously associated State Group.
    ak_wwise_core_object_setstategroups = u"ak.wwise.core.object.setStateGroups"
    # Set the state properties of an object. Note, this will remove any previous state property, including the default ones.
    ak_wwise_core_object_setstateproperties = u"ak.wwise.core.object.setStateProperties"
    # Retrieves the list of all object types registered in Wwise's object model. This function returns the equivalent of \ref wobjects_index .
    # \deprecated in favor of ak.wwise.core.object.getTypes
    ak_wwise_core_plugin_getlist = u"ak.wwise.core.plugin.getList"
    # Retrieves the list of property and reference names for an object.
    # \deprecated in favor of ak.wwise.core.object.getPropertyAndReferenceNames
    ak_wwise_core_plugin_getproperties = u"ak.wwise.core.plugin.getProperties"
    # Retrieves information about an object property. Note that this function does not return the value of a property. To retrieve the value of a property, refer to \ref ak_wwise_core_object_get and \ref waapi_query_return.
    # \deprecated in favor of ak.wwise.core.object.getPropertyInfo
    ak_wwise_core_plugin_getproperty = u"ak.wwise.core.plugin.getProperty"
    # Sent when a new entry is added to the capture log. Note that all entries are being sent. No filtering is applied as opposed to the Capture Log view.
    ak_wwise_core_profiler_capturelog_itemadded = u"ak.wwise.core.profiler.captureLog.itemAdded"
    # Specifies the type of data you want to capture. Overrides the user's profiler settings.
    ak_wwise_core_profiler_enableprofilerdata = u"ak.wwise.core.profiler.enableProfilerData"
    # Sent when a game object has been registered.
    ak_wwise_core_profiler_gameobjectregistered = u"ak.wwise.core.profiler.gameObjectRegistered"
    # Sent when the game objects have been reset, such as closing a connection to a game while profiling.
    ak_wwise_core_profiler_gameobjectreset = u"ak.wwise.core.profiler.gameObjectReset"
    # Sent when a game object has been unregistered.
    ak_wwise_core_profiler_gameobjectunregistered = u"ak.wwise.core.profiler.gameObjectUnregistered"
    # Retrieves the Audio Objects at a specific profiler capture time.
    ak_wwise_core_profiler_getaudioobjects = u"ak.wwise.core.profiler.getAudioObjects"
    # Retrieves the busses at a specific profiler capture time.
    ak_wwise_core_profiler_getbusses = u"ak.wwise.core.profiler.getBusses"
    # Retrieves CPU usage statistics at a specific profiler capture time. This data can also be found in the Advanced Profiler, under the CPU tab. To ensure the CPU data is received, refer to \ref ak_wwise_core_profiler_enableprofilerdata. The returned data includes "Inclusive" and "Exclusive" values, where "Inclusive" refers to the time spent in the element plus the time spent in any called elements, and "Exclusive" values pertain to execution only within the element itself.
    ak_wwise_core_profiler_getcpuusage = u"ak.wwise.core.profiler.getCpuUsage"
    # Returns the current time of the specified profiler cursor, in milliseconds.
    ak_wwise_core_profiler_getcursortime = u"ak.wwise.core.profiler.getCursorTime"
    # Retrieves the game objects at a specific profiler capture time.
    ak_wwise_core_profiler_getgameobjects = u"ak.wwise.core.profiler.getGameObjects"
    # Retrieves the loaded media at a specific profiler capture time. This data can also be found in the Advanced Profiler, under the Loaded Media tab. To ensure the Loaded Media data is received, refer to \ref ak_wwise_core_profiler_enableprofilerdata.
    ak_wwise_core_profiler_getloadedmedia = u"ak.wwise.core.profiler.getLoadedMedia"
    # Retrieves the Performance Monitor statistics at a specific profiler capture time. Refer to \ref globalcountersids for the available counters.
    ak_wwise_core_profiler_getperformancemonitor = u"ak.wwise.core.profiler.getPerformanceMonitor"
    # Retrieves active RTPCs at a specific profiler capture time.
    ak_wwise_core_profiler_getrtpcs = u"ak.wwise.core.profiler.getRTPCs"
    # Retrieves the streaming media at a specific profiler capture time. This data can also be found in the Advanced Profiler, under the Streams tab. To ensure the Streams data is received, refer to \ref ak_wwise_core_profiler_enableprofilerdata.
    ak_wwise_core_profiler_getstreamedmedia = u"ak.wwise.core.profiler.getStreamedMedia"
    # Retrieves all parameters affecting voice volume, highpass and lowpass for a voice path, resolved from pipeline IDs.
    ak_wwise_core_profiler_getvoicecontributions = u"ak.wwise.core.profiler.getVoiceContributions"
    # Retrieves the voices at a specific profiler capture time.
    ak_wwise_core_profiler_getvoices = u"ak.wwise.core.profiler.getVoices"
    # Saves profiler as a .prof file according to the given file path.
    ak_wwise_core_profiler_savecapture = u"ak.wwise.core.profiler.saveCapture"
    # Starts the profiler capture and returns the time at the beginning of the capture, in milliseconds.
    ak_wwise_core_profiler_startcapture = u"ak.wwise.core.profiler.startCapture"
    # Sent when a state group state has been changed. This subscription does not require the profiler capture log to be started.
    ak_wwise_core_profiler_statechanged = u"ak.wwise.core.profiler.stateChanged"
    # Stops the profiler capture and returns the time at the end of the capture, in milliseconds.
    ak_wwise_core_profiler_stopcapture = u"ak.wwise.core.profiler.stopCapture"
    # Sent when a switch group state has been changed. This function does not require the profiler capture log to be started.
    ak_wwise_core_profiler_switchchanged = u"ak.wwise.core.profiler.switchChanged"
    # Sent when the project has been successfully loaded.
    ak_wwise_core_project_loaded = u"ak.wwise.core.project.loaded"
    # Sent when the after the project is completely closed.
    ak_wwise_core_project_postclosed = u"ak.wwise.core.project.postClosed"
    # Sent when the project begins closing.
    ak_wwise_core_project_preclosed = u"ak.wwise.core.project.preClosed"
    # Saves the current project.
    ak_wwise_core_project_save = u"ak.wwise.core.project.save"
    # Sent when the project has been saved.
    ak_wwise_core_project_saved = u"ak.wwise.core.project.saved"
    # Connects the Wwise Authoring application to a Wwise Sound Engine running executable or to a saved profile file. The host must be running code with communication enabled. If only "host" is provided, Wwise connects to the first Sound Engine instance found. To distinguish between different instances, you can also provide the name of the application to connect to.
    ak_wwise_core_remote_connect = u"ak.wwise.core.remote.connect"
    # Disconnects the Wwise Authoring application from a connected Wwise Sound Engine running executable.
    ak_wwise_core_remote_disconnect = u"ak.wwise.core.remote.disconnect"
    # Retrieves all consoles available for connecting Wwise Authoring to a Sound Engine instance.
    ak_wwise_core_remote_getavailableconsoles = u"ak.wwise.core.remote.getAvailableConsoles"
    # Retrieves the connection status.
    ak_wwise_core_remote_getconnectionstatus = u"ak.wwise.core.remote.getConnectionStatus"
    # Sets which version of the source is being used for the specified sound. Use \ref ak_wwise_core_object_get with the 'activeSource' return option to get the active source of a sound.
    ak_wwise_core_sound_setactivesource = u"ak.wwise.core.sound.setActiveSource"
    # Converts the external sources files for the project as detailed in the wsources file, and places them into either the default folder, or the folder specified by the output argument. External Sources are a special type of source that you can put in a Sound object in Wwise. It indicates that the real sound data will be provided at run time. While External Source conversion is also triggered by SoundBank generation, this operation can be used to process sources not contained in the Wwise Project. Please refer to Wwise SDK help page "Integrating External Sources".
    ak_wwise_core_soundbank_convertexternalsources = u"ak.wwise.core.soundbank.convertExternalSources"
    # Generate a list of SoundBanks with the import definition specified in the WAAPI request. If you do not write the SoundBanks to disk, subscribe to \ref ak_wwise_core_soundbank_generated to receive SoundBank structure info and the bank data as base64. Note: This is a synchronous operation.
    ak_wwise_core_soundbank_generate = u"ak.wwise.core.soundbank.generate"
    # Sent when a single SoundBank is generated. This could be sent multiple times during SoundBank generation, for every SoundBank generated and for every platform. To generate SoundBanks, refer to \ref ak_wwise_core_soundbank_generate or \ref ak_wwise_ui_commands_execute with one of the SoundBank generation commands. Refer to \ref globalcommandsids for the list of commands.
    ak_wwise_core_soundbank_generated = u"ak.wwise.core.soundbank.generated"
    # Sent when all SoundBanks are generated. Note: This notification is only sent when SoundBanks have been generated, it is not a reliable way to determine when \ref ak_wwise_core_soundbank_generate has completed.
    ak_wwise_core_soundbank_generationdone = u"ak.wwise.core.soundbank.generationDone"
    # Retrieves a SoundBank's inclusion list.
    ak_wwise_core_soundbank_getinclusions = u"ak.wwise.core.soundbank.getInclusions"
    # Imports SoundBank definitions from the specified file. Multiple files can be specified. See the WAAPI log for status messages.
    ak_wwise_core_soundbank_processdefinitionfiles = u"ak.wwise.core.soundbank.processDefinitionFiles"
    # Modifies a SoundBank's inclusion list. The 'operation' argument determines how the 'inclusions' argument modifies the SoundBank's inclusion list; 'inclusions' may be added to / removed from / replace the SoundBank's inclusion list.
    ak_wwise_core_soundbank_setinclusions = u"ak.wwise.core.soundbank.setInclusions"
    # Add files to source control. Equivalent to Mark for Add for Perforce.
    ak_wwise_core_sourcecontrol_add = u"ak.wwise.core.sourceControl.add"
    # Check out files from source control. Equivalent to Check Out for Perforce.
    ak_wwise_core_sourcecontrol_checkout = u"ak.wwise.core.sourceControl.checkOut"
    # Commit files to source control. Equivalent to Submit Changes for Perforce.
    ak_wwise_core_sourcecontrol_commit = u"ak.wwise.core.sourceControl.commit"
    # Delete files from source control. Equivalent to Mark for Delete for Perforce.
    ak_wwise_core_sourcecontrol_delete = u"ak.wwise.core.sourceControl.delete"
    # Retrieve all original files.
    ak_wwise_core_sourcecontrol_getsourcefiles = u"ak.wwise.core.sourceControl.getSourceFiles"
    # Get the source control status of the specified files.
    ak_wwise_core_sourcecontrol_getstatus = u"ak.wwise.core.sourceControl.getStatus"
    # Move or rename files in source control. Always pass the same number of elements in files and newFiles. Equivalent to Move for Perforce.
    ak_wwise_core_sourcecontrol_move = u"ak.wwise.core.sourceControl.move"
    # Revert changes to files in source control.
    ak_wwise_core_sourcecontrol_revert = u"ak.wwise.core.sourceControl.revert"
    # Change the source control provider and credentials. This is the same setting as the Source Control option in the Project Settings dialog in Wwise.
    ak_wwise_core_sourcecontrol_setprovider = u"ak.wwise.core.sourceControl.setProvider"
    # Assigns a Switch Container's child to a Switch. This is the equivalent of doing a drag&drop of the child to a state in the Assigned Objects view. The child is always added at the end for each state.
    ak_wwise_core_switchcontainer_addassignment = u"ak.wwise.core.switchContainer.addAssignment"
    # Sent when an assignment is added to a Switch Container.
    ak_wwise_core_switchcontainer_assignmentadded = u"ak.wwise.core.switchContainer.assignmentAdded"
    # Sent when an assignment is removed from a Switch Container.
    ak_wwise_core_switchcontainer_assignmentremoved = u"ak.wwise.core.switchContainer.assignmentRemoved"
    # Returns the list of assignments between a Switch Container's children and states.
    ak_wwise_core_switchcontainer_getassignments = u"ak.wwise.core.switchContainer.getAssignments"
    # Removes an assignment between a Switch Container's child and a State.
    ak_wwise_core_switchcontainer_removeassignment = u"ak.wwise.core.switchContainer.removeAssignment"
    # Creates a transport object for the given Wwise object. The return transport object can be used to play, stop, pause and resume the Wwise object via the other transport functions.
    ak_wwise_core_transport_create = u"ak.wwise.core.transport.create"
    # Destroys the given transport object.
    ak_wwise_core_transport_destroy = u"ak.wwise.core.transport.destroy"
    # Executes an action on the given transport object, or all transport objects if none is specified.
    ak_wwise_core_transport_executeaction = u"ak.wwise.core.transport.executeAction"
    # Returns the list of transport objects.
    ak_wwise_core_transport_getlist = u"ak.wwise.core.transport.getList"
    # Gets the state of the given transport object.
    ak_wwise_core_transport_getstate = u"ak.wwise.core.transport.getState"
    # Prepare the object and its dependencies for playback. Use this function before calling PostEventSync or PostMIDIOnEventSync from IAkGlobalPluginContext.
    ak_wwise_core_transport_prepare = u"ak.wwise.core.transport.prepare"
    # Sent when the transport's state has changed.
    ak_wwise_core_transport_statechanged = u"ak.wwise.core.transport.stateChanged"
    # Sets the Original/Converted transport toggle globally. This allows playing the original or the converted sound files.
    ak_wwise_core_transport_useoriginals = u"ak.wwise.core.transport.useOriginals"
    # Begins an undo group. Make sure to call \ref ak_wwise_core_undo_endgroup exactly once for every ak.wwise.core.beginUndoGroup call you make. Calls to ak.wwise.core.undo.beginGroup can be nested. When closing a WAMP session, a check is made to ensure that all undo groups are closed. If not, a cancelGroup is called for each of the groups still open.
    ak_wwise_core_undo_begingroup = u"ak.wwise.core.undo.beginGroup"
    # Cancels the last undo group.
    ak_wwise_core_undo_cancelgroup = u"ak.wwise.core.undo.cancelGroup"
    # Ends the last undo group.
    ak_wwise_core_undo_endgroup = u"ak.wwise.core.undo.endGroup"
    # Redoes the last operation in the Undo stack.
    ak_wwise_core_undo_redo = u"ak.wwise.core.undo.redo"
    # Undoes the last operation in the Undo stack.
    ak_wwise_core_undo_undo = u"ak.wwise.core.undo.undo"
    # Sent when an assert has failed. This is only available in Debug builds.
    ak_wwise_debug_assertfailed = u"ak.wwise.debug.assertFailed"
    # Enables debug assertions. Every call to enableAsserts with 'false' increments the ref count. Calling with true decrements the ref count. This is only available with Debug builds.
    ak_wwise_debug_enableasserts = u"ak.wwise.debug.enableAsserts"
    # Enables or disables the automation mode for Wwise. This reduces the potential interruptions caused by message boxes and dialogs. For instance, enabling the automation mode silently accepts: project migration, project load log, EULA acceptance, project licence display and generic message boxes.
    ak_wwise_debug_enableautomationmode = u"ak.wwise.debug.enableAutomationMode"
    # Generate a WAV file playing a tone with a simple envelope and save it to the specified location. This is provided as a utility to generate test WAV files.
    ak_wwise_debug_generatetonewav = u"ak.wwise.debug.generateToneWAV"
    # Retrieves the WAL tree, which describes the nodes that are synchronized in the Sound Engine. Private use only.
    ak_wwise_debug_getwaltree = u"ak.wwise.debug.getWalTree"
    # Restart WAAPI servers. For internal use only.
    ak_wwise_debug_restartwaapiservers = u"ak.wwise.debug.restartWaapiServers"
    # Private use only.
    ak_wwise_debug_testassert = u"ak.wwise.debug.testAssert"
    # Private use only.
    ak_wwise_debug_testcrash = u"ak.wwise.debug.testCrash"
    # Bring Wwise main window to foreground. Refer to SetForegroundWindow and AllowSetForegroundWindow on MSDN for more information on the restrictions. Refer to ak.wwise.core.getInfo to obtain the Wwise process ID for AllowSetForegroundWindow.
    ak_wwise_ui_bringtoforeground = u"ak.wwise.ui.bringToForeground"
    # Captures a part of the Wwise UI relative to a view.
    ak_wwise_ui_capturescreen = u"ak.wwise.ui.captureScreen"
    # Executes a command. Some commands can take a list of objects as parameters. Refer to \ref globalcommandsids for the available commands.
    ak_wwise_ui_commands_execute = u"ak.wwise.ui.commands.execute"
    # Sent when a command is executed. The objects for which the command is executed are sent in the publication.
    ak_wwise_ui_commands_executed = u"ak.wwise.ui.commands.executed"
    # Gets the list of commands.
    ak_wwise_ui_commands_getcommands = u"ak.wwise.ui.commands.getCommands"
    # Registers an array of add-on commands. Registered commands remain until the Wwise process is terminated. Refer to \ref defining_custom_commands for more information about registering commands. Also refer to \ref ak_wwise_ui_commands_executed.
    ak_wwise_ui_commands_register = u"ak.wwise.ui.commands.register"
    # Unregisters an array of add-on UI commands.
    ak_wwise_ui_commands_unregister = u"ak.wwise.ui.commands.unregister"
    # Retrieves the list of objects currently selected by the user in the active view.
    ak_wwise_ui_getselectedobjects = u"ak.wwise.ui.getSelectedObjects"
    # Closes the current project.
    ak_wwise_ui_project_close = u"ak.wwise.ui.project.close"
    # Creates, saves and opens new empty project, specified by path and platform. The project has no factory setting WorkUnit.  Please refer to \ref ak_wwise_core_project_loaded for further explanations on how to be notified when the operation has completed.
    ak_wwise_ui_project_create = u"ak.wwise.ui.project.create"
    # Opens a project, specified by path. Please refer to \ref ak_wwise_core_project_loaded for further explanations on how to be notified when the operation has completed.
    ak_wwise_ui_project_open = u"ak.wwise.ui.project.open"
    # Sent when the selection changes in the project.
    ak_wwise_ui_selectionchanged = u"ak.wwise.ui.selectionChanged"
    # Retrieves the list of functions.
    ak_wwise_waapi_getfunctions = u"ak.wwise.waapi.getFunctions"
    # Retrieves the JSON schema of a Waapi URI.
    ak_wwise_waapi_getschema = u"ak.wwise.waapi.getSchema"
    # Retrieves the list of topics to which a client can subscribe.
    ak_wwise_waapi_gettopics = u"ak.wwise.waapi.getTopics"
