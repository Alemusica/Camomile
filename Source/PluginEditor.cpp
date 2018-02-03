/*
 // Copyright (c) 2015-2018 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginEnvironment.h"
#include "Gui/LookAndFeel.hpp"
#include "Gui/Gui.hpp"
#include "Gui/GuiConsole.hpp"

#include "Pd/PdPatch.hpp"
#include <locale>


#define Camomile_Author_UTF8 "Camomile Author: " + String(JucePlugin_Manufacturer) + "\n\n"
#define Camomile_Organizations_UTF8 "Organizations: CICM | Université Paris 8 \n\n"
#define Camomile_Website_UTF8 "Website: " + String(JucePlugin_ManufacturerWebsite) + "\n\n"
#define Camomile_Credits_Pd_UTF8 "Pure Data by Miller Puckette and others\n"
#define Camomile_Credits_libpd_UTF8 "libpd by the Peter Brinkmann, Dan Wilcox and others"
#define Camomile_Credits_JUCE_UTF8 "JUCE by ROLI Ltd.\n"
#if defined(JucePlugin_Build_VST) || defined(JucePlugin_Build_VST3)
#define Camomile_Credits_Steinberg_UTF8 "VST PlugIn Technology by Steinberg Media Technologies"
#else
#define Camomile_Credits_Steinberg_UTF8 ""
#endif
#define Camomile_Credits_UTF8 "Credits:\n" + Camomile_Credits_Pd_UTF8 + Camomile_Credits_libpd_UTF8 + Camomile_Credits_JUCE_UTF8 + Camomile_Credits_Steinberg_UTF8

#define Camomile_About_UTF8 Camomile_Author_UTF8 + Camomile_Organizations_UTF8 + Camomile_Website_UTF8 + Camomile_Credits_UTF8

CamomileAudioProcessorEditor::CamomileAudioProcessorEditor(CamomileAudioProcessor& p) :
AudioProcessorEditor (&p), CamomileEditorInteractionManager(p), processor (p)
{
    static CamoLookAndFeel lookAndFeel;
    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
    setOpaque(true);
    setWantsKeyboardFocus(true);
    if(p.getPatch().isGraph())
    {
        auto bounds = p.getPatch().getBounds();
        setSize(std::max(bounds[2], 100), std::max(bounds[3], 100));
    }
    else
    {
        setSize(400, 300);
    }
    addAndMakeVisible(button);
    button.addListener(this);
    
    window.setUsingNativeTitleBar(true);
    window.setBounds(50, 50, 300, 370);
    window.setResizable(false, false);
    window.setDropShadowEnabled(true);
    window.setVisible(true);
    window.setBackgroundColour(Gui::getColorBg());
    setInterceptsMouseClicks(true, true);
    
    if(processor.getPatch().isGraph())
    {
        auto guis(p.getPatch().getGuis());
        for(auto& gui : guis)
        {
            addAndMakeVisible(objects.add(GuiObject::createTyped(*this, gui)));
        }
    }
   
    if(!CamomileEnvironment::getImageName().empty())
    {
        File f(CamomileEnvironment::getPatchPath() + File::getSeparatorString() + String(CamomileEnvironment::getImageName()));
        if(f.exists())
        {
            Image img = ImageFileFormat::loadFrom(f);
            if(img.isValid())
            {
                background.setImage(img);
                background.setTransformToFit(getBounds().toType<float>(), RectanglePlacement::stretchToFit);
                addAndMakeVisible(background, 0);
            }
            else
            {
                processor.add(CamomileAudioProcessor::ConsoleLevel::Error,
                              "background image " + CamomileEnvironment::getImageName() + " is invalid");
            }
        }
        else
        {
            processor.add(CamomileAudioProcessor::ConsoleLevel::Error,
                               "background image " + CamomileEnvironment::getImageName() + " doesn't exist");
        }
    }
    startTimer(25);
}

CamomileAudioProcessorEditor::~CamomileAudioProcessorEditor()
{
    ;
}

void CamomileAudioProcessorEditor::timerCallback()
{
    CamomileEditorPanelManager::processMessages();
    for(auto object : objects)
    {
        object->update();
    }
}


void CamomileAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::white);
    if(!CamomileEnvironment::isValid())
    {
        g.setColour(Colours::black);
        g.drawText("Plugin Not Valid", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
    else if(!processor.getPatch().isGraph())
    {
        g.setColour(Colours::black);
        g.drawText("No Graphical User Interface Available", 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
}

void CamomileAudioProcessorEditor::buttonClicked(Button* button)
{
    if(button)
    {
        juce::PopupMenu m;
        m.addItem(1, "About");
        m.addItem(2, "Console");
        const int result = m.showAt(button->getScreenBounds().translated(-2, 3));
        if(result == 1)
        {
            TextEditor* about = new TextEditor();
            if(about)
            {
                about->setMultiLine(true);
                about->setReadOnly(true);
                about->setScrollbarsShown(false);
                about->setCaretVisible(false);
                about->setPopupMenuEnabled(true);
                about->setFont(Gui::getFont());
                about->setText(Camomile_About_UTF8);
                about->setBounds(0, 0, 300, 370);
                
                window.setContentOwned(about, false);
                window.setName("About Camomile " + String(JucePlugin_VersionString));
                window.addToDesktop();
                window.toFront(false);
                window.grabKeyboardFocus();
                if(CamomileEnvironment::wantsKey())
                {
                    window.addKeyListener(this);
                }
            }
        }
        else if(result == 2)
        {
            window.setContentOwned(new GuiConsole(processor), false);
            window.setName("Camomile Console");
            window.addToDesktop();
            window.toFront(true);
            window.grabKeyboardFocus();
            if(CamomileEnvironment::wantsKey())
            {
                window.addKeyListener(this);
            }
        }
    }
}

CamomileAudioProcessorEditor::FlowerButton::FlowerButton() : Button("CamomileButton")
{
    setClickingTogglesState(false);
    setAlwaysOnTop(true);
    m_center.setImage(ImageCache::getFromMemory(BinaryData::flower_center_png, BinaryData::flower_center_pngSize));
    m_petals.setImage(ImageCache::getFromMemory(BinaryData::flower_petals_png, BinaryData::flower_petals_pngSize));
    m_center.setTransformToFit(juce::Rectangle<float>(0.f, 0.f, 18.f, 18.f), RectanglePlacement::stretchToFit);
    m_petals.setTransformToFit(juce::Rectangle<float>(0.f, 0.f, 18.f, 18.f), RectanglePlacement::stretchToFit);
    m_center.setAlwaysOnTop(true);
    addAndMakeVisible(m_center, -1);
    addAndMakeVisible(m_petals, 0);
    m_petals.setOverlayColour(Gui::getColorTxt());
    m_petals.setAlpha(0.5f);
    setBounds(3, 3, 18, 18);
}

void CamomileAudioProcessorEditor::FlowerButton::buttonStateChanged()
{
    m_petals.setAlpha((isDown() || isOver()) ? 1.f : 0.5f);
}

void CamomileAudioProcessorEditor::focusGained(FocusChangeType t)
{
    window.toFront(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool CamomileAudioProcessorEditor::keyPressed(const KeyPress& key)
{
    return CamomileEditorKeyManager::keyPressed(key);
}

bool CamomileAudioProcessorEditor::keyStateChanged(bool isKeyDown)
{
    return CamomileEditorKeyManager::keyStateChanged(isKeyDown);
}

void CamomileAudioProcessorEditor::modifierKeysChanged(const ModifierKeys& modifiers)
{
    CamomileEditorKeyManager::keyModifiersChanged(modifiers);
}

bool CamomileAudioProcessorEditor::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    return CamomileEditorKeyManager::keyPressed(key);
}

bool CamomileAudioProcessorEditor::keyStateChanged(bool isKeyDown, Component* originatingComponent)
{
    return CamomileEditorKeyManager::keyStateChanged(isKeyDown);
}



