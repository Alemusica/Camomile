v1.0.3
- Improve support for multiple audio buses
- Add warning when the plugin code or the plugin type is not defined

v1.0.2
- Fix DSP off message to Pd
- Prepare the DSP before opening the patch (#83)
- Initialize parameters and programs before opening the patch (#82)
- Use libpd_process_raw in the DSP perform method for optimization
- Add support for MIDI In SysEx, SysRealTime & Byte
- Add support for MIDI Out Byte
- Add full support for keys objects [key], [keyup] (#80)
- Add partial support for key objects[keyname] (perhaps some names are still missing names are missing)
- Improve the console (resizable)
- Improve prints for lists (remove line breaks)
- Floating Window always on top
- Floating Window with tabs (Console/Patch/Camomile)
- Display patch description
- Add support for compatibility versions (plugin's version of the patch <= plugin's version used)
- Add support for block size inferior to 64 samples (implies delay)
- Add support for non-real time processing
- Improve name of the UI Window
- Fix the FFT objects for multithreading support

v1.0.1
- Fix thread concurrency issue that occurred when selecting a program (#77).
- Fix stack overflow issue due to concurrent access to the Pd's stack counter (#69).
- Update documentation for VST2/VST3/AU generation on MacOS to display the name of the plugins in Ableton (#75).
- Improve the whole documentation (#72) and start "How to Create Patches" (#73).
- Add more warning when there are extra arguments in parameters' methods.
- Add support for "openpanel" and "savepanel" methods.
- Update examples Bulgroz, AlmondOrgan, Castafiore, MiniMock.
- Start/Add support for patch description in the text file (#74).
- Start/Add support for patch credits in the text file (#74).

v1.0.0
Only main changes are presented. Most of the changes will be presented during the JIM 2018 and perhaps the LAC 2018.
- Use libpd instead of my personal wrapper.
- Use TLS approach of Pd to manage thread concurrency issues.
- Use a text file to define the properties of the plugins.
- Generate plugins with the patches included.
- Separate the GUI and the parameters' definitions.

There is no ChangeLog for previous versions, sorry.
