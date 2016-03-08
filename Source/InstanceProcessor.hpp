/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __CAMOMILE_INTANCE_PROCESSOR__
#define __CAMOMILE_INTANCE_PROCESSOR__

// TODO VST3 don't change the name and label of parameters
// TODO VST the number of ioputs seems to be 14 (VST3) ??
// Adds infos about bpm and position

#include "Pd.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

class InstanceProcessor : public AudioProcessor, public pd::Instance
{
public:
    class Listener;
    InstanceProcessor();
    ~InstanceProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {return true;};
    const String getName() const override {return String(JucePlugin_Name);};
    
    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    float getParameterDefaultValue(int index) override;
    const String getParameterName(int index) override;
    const String getParameterText(int index) override;
    String getParameterText(int index, int size) override;
    String getParameterLabel (int index) const override;
    int getParameterNumSteps(int index) override;
    bool isParameterAutomatable(int index) const override;
    bool isParameterOrientationInverted (int index) const override;
    bool isMetaParameter(int index) const override;
    float getParameterNonNormalized(int index) const;
    void setParameterNonNormalized(int index, float newValue);
    void setParameterNonNormalizedNotifyingHost(int index, float newValue);
    
    const String getInputChannelName(int index) const override {return String(index + 1);}
    const String getOutputChannelName(int index) const override {return String(index + 1);}
    bool isInputChannelStereoPair(int index) const override {return true;}
    bool isOutputChannelStereoPair(int index) const override {return true;}

    bool acceptsMidi() const override {return bool(JucePlugin_WantsMidiInput);}
    bool producesMidi() const override {return bool(JucePlugin_ProducesMidiOutput);}
    bool silenceInProducesSilenceOut() const override {return false;}
    double getTailLengthSeconds() const override {return 0.0;}

    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override {return String();}
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    void loadPatch(const juce::File& file);
    inline const pd::Patch getPatch() const noexcept {return m_patch;}
    inline pd::Patch getPatch() noexcept {return m_patch;}
    int getParameterIndex(pd::BindingName const& name);
    int getParameterIndex(String const& name);
    
    void addListener(Listener* listener);
    void removeListener(Listener* listener);
    class Listener
    {
    public:
        inline constexpr Listener() {}
        inline virtual ~Listener() {}
        virtual void patchChanged() = 0;
    };

private:
    
    class Parameter : public AudioProcessorParameter
    {
    public:
        Parameter();
        Parameter(Parameter const& other);
        Parameter(pd::Gui const& gui);
        ~Parameter();
        Parameter& operator=(Parameter const& other);
        Parameter& operator=(Parameter&& other);
        bool isValid() const noexcept;
        float getValue() const final;
        float getValueNonNormalized() const;
        void setValue(float newValue) final;
        void setValueNonNormalized(float newValue);
        float getValueNormalized(float newValue);
        float getDefaultValue() const final;
        String getName(int maximumStringLength) const final;
        String getLabel() const final;
        String getText (float value, int size) const final;
        float getValueForText (const String& text) const final;
        bool isOrientationInverted() const final;
        int getNumSteps() const final;
        inline pd::BindingName const& getBindingName() const noexcept {return m_bname;}
        
    private:
        bool   m_valid;
        float  m_value;
        float  m_min;
        float  m_max;
        String m_name;
        String m_label;
        pd::BindingName m_bname;
        int    m_nsteps;
    };
    
    pd::Patch              m_patch;
    std::set<Listener*>    m_listeners;
    std::vector<Parameter> m_parameters;
    mutable std::mutex     m_mutex;
    
    void parametersChanged();
    std::vector<Listener*> getListeners() const noexcept;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstanceProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
