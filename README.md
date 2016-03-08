# ![](https://cloud.githubusercontent.com/assets/1409918/13610634/ed41896c-e55e-11e5-844d-d795e97cfada.png) Camomile

Camomile is a dynamic plugin made with [Juce](http://www.juce.com) that allows to load and control [Pure Data](http://msp.ucsd.edu/software.html) patches inside a digital audio workstation.

Feature :
 - Support for Multiple plugin instances
 - Automatic creation of GUIs (toggle, slider, radio and numbox)
 - Automatic recognition of parameters (name, label, range, etc.)
 - Up to 16 channels
 - Up to 32 parameters
 - Support for Midi

![](https://cloud.githubusercontent.com/assets/1409918/13610631/ebdacae8-e55e-11e5-903c-fb3ad342adb8.png)

> The Camomile plugin is only available for Mac.  
> The Windows version should come soon.  

How to use the Camomile plugin:

1. Download the [plugin](https://github.com/pierreguillot/Camomile/releases/download/v0.0.3-beta/Camomile_v0.0.3.zip) (*vst*, *vst3*) for Mac.
2. Put the binaries in the folders :  
 * /Users/"user name"/Library/Audio/Plug-Ins/VST for the vst.
 * /Users/"user name"/Library/Audio/Plug-Ins/VST3 for the vst3.
3. Open your DAW and load the plugin.
4. Click on the camomile flower. Open one of the Pure Data patches given with the binaries. Create your own Camomile patch is very easy, follow the next section.

How to create a patch for the Camomile plugin:

* Use the *adc~* and the *dac~* objects to receive and send the signal from the plugin.    
![](https://cloud.githubusercontent.com/assets/1409918/13610984/91081d12-e560-11e5-8abb-f924367ab6eb.png)    

* Add a label (the first word is the name of the parameter and the second word is the label of the parameter) and a receive symbol to a GUI object (only the *toggle*, the *slider*, the *radio* and the *numbox* are available for the moment) to create a parameter. The minimum and maximum values will be used for the range of the parameter.  
![](https://cloud.githubusercontent.com/assets/1409918/13611062/eb6348a4-e560-11e5-8c62-b41803183783.png)

* Add only a receive symbol to a GUI object if you want to use a second interfaces to control a parameter.  
![](https://cloud.githubusercontent.com/assets/1409918/13611063/eb74e08c-e560-11e5-8d34-4df8559bbb8d.png)

* Activate the  "Graph-On-Parent" option in your patch as and use the size and the offset to define the visible area of the patch.  
![](https://cloud.githubusercontent.com/assets/1409918/13611033/c48b69fa-e560-11e5-9ab8-3950adba08f0.png)

> You should always ensure that the send and receive symbols are specifics to the patch instance by using *$0*.

Author : Pierre Guillot  
Organizations : CICM | Université Paris 8 | Labex Arts H2H  
Credits : Pure Data by Miller Puckette | Juce by ROLI Ltd.
