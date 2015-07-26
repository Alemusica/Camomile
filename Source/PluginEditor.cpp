/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

CamomileAudioProcessorEditor::CamomileAudioProcessorEditor(CamomileAudioProcessor& p) :
AudioProcessorEditor(&p),
m_processor(p),
m_file_drop(false)
{
    setSize(400, 300);
}

CamomileAudioProcessorEditor::~CamomileAudioProcessorEditor()
{
}

void CamomileAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    if(m_file_drop)
    {
        g.fillAll(Colours::lightblue);
    }
    g.setColour(Colours::black);
    if(!m_processor.hasPatch())
    {
        g.setFont (15.0f);
        g.drawText(juce::String("Drag & Drop your patch..."), getBounds().withZeroOrigin(), juce::Justification::centred);
    }
    else
    {
        g.setFont (15.0f);
        g.drawText(juce::String("Patch loaded"), getBounds().withZeroOrigin(), juce::Justification::centred);
    }
}

bool CamomileAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            if(files[i].endsWith(juce::StringRef(".pd")))
            {
                return true;
            }
        }
    }
    return false;
}

void CamomileAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            juce::File file(files[i]);
            if(file.getFileExtension() == juce::String(".pd"))
            {
                m_processor.loadPatch(file);
                repaint();
            }
        }
    }
}

void CamomileAudioProcessorEditor::fileDragEnter(const StringArray& files, int x, int y)
{
    m_file_drop = true;
    repaint();
}

void CamomileAudioProcessorEditor::fileDragExit(const StringArray& files)
{
    m_file_drop = false;
    repaint();
}

void CamomileAudioProcessorEditor::resized()
{
    
}
