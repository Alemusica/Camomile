/*
 // Copyright (c) 2015 - 2018 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef CAMOMILE_PROCESSOR_INCLUDE_HPP
#define CAMOMILE_PROCESSOR_INCLUDE_HPP

#include "../JuceLibraryCode/JuceHeader.h"


// ==================================================================================== //
//                                  CAMOMILE PROCESSOR                                  //
// ==================================================================================== //

class CamomileProcessor : public AudioProcessor
{
public:
    CamomileProcessor();
    ~CamomileProcessor();

    AudioProcessorEditor* createEditor() final;
    bool hasEditor() const final {return true;};
    const String getName() const final ;
    
    const String getInputChannelName(int index) const final {return String(index + 1);}
    const String getOutputChannelName(int index) const final {return String(index + 1);}
    bool isInputChannelStereoPair(int index) const final {return false;}
    bool isOutputChannelStereoPair(int index) const final {return false;}
    
    bool acceptsMidi() const final {return bool(JucePlugin_WantsMidiInput);}
    bool producesMidi() const final {return bool(JucePlugin_ProducesMidiOutput);}
    bool silenceInProducesSilenceOut() const final {return false;}
    double getTailLengthSeconds() const final {return 0.0;}
    
    
    // ==================================================================================== //
    //                                          PARAMETERS                                  //
    // ==================================================================================== //
    /*
    int getNumParameters() final;
    float getParameter(int index) final;
    void setParameter(int index, float newValue) final;
    float getParameterDefaultValue(int index) final;
    const String getParameterName(int index) final;
    const String getParameterText(int index) final;
    String getParameterText(int index, int size) final;
    String getParameterLabel (int index) const final;
    int getParameterNumSteps(int index) final;
     */

    // ==================================================================================== //
    //                                          PRESETS                                     //
    // ==================================================================================== //

    int getNumPrograms() final {return 1;}
    int getCurrentProgram() final {return 0;}
    void setCurrentProgram(int index) final {}
    const String getProgramName(int index) final {return String();}
    void changeProgramName(int index, const String& newName) final {}
    
    // ==================================================================================== //
    //                                          DSP & MIDI                                  //
    // ==================================================================================== //
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) final;
    void releaseResources() final;
    void processBlock(AudioSampleBuffer&, MidiBuffer&) final;
    //void receive(xpd::midi::event const& event) final;
    
    // ==================================================================================== //
    //                                          PATCH                                       //
    // ==================================================================================== //

    void load_patch(std::string const& name, std::string const& path);
    void close_patch();

    // ================================================================================ //
    //                                  STATE INFORMATION                               //
    // ================================================================================ //

    void getStateInformation(MemoryBlock& destData) final;
    void setStateInformation(const void* data, int sizeInBytes) final;
    
private:
    juce::String    m_name;
    juce::String    m_path;
    MidiBuffer      m_midi;
    AudioPlayHead::CurrentPositionInfo  m_playinfos;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CamomileProcessor)
};


#endif // CAMOMILE_PROCESSOR_INCLUDE_HPP
