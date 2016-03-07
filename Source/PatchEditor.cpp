/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "InstanceProcessor.h"
#include "PatchEditor.h"

// ==================================================================================== //
//                                      IMAGE BUTTON                                    //
// ==================================================================================== //

class PatchEditor::ImgButton : public Button
{
private:
    DrawableImage   m_image1;
    DrawableImage   m_image2;
    DrawableImage   m_image3;
    PatchEditor*    m_editor;
public:
    ImgButton(PatchEditor* editor, const String& name, int rid) : Button(name), m_editor(editor)
    {
        addListener(editor);
        setClickingTogglesState(false);
        setAlwaysOnTop(true);
        setRadioGroupId(rid);
        m_image1.setImage(ImageCache::getFromMemory(BinaryData::flower1_png, BinaryData::flower1_pngSize));
        m_image2.setImage(ImageCache::getFromMemory(BinaryData::flower1_png, BinaryData::flower1_pngSize));
        m_image3.setImage(ImageCache::getFromMemory(BinaryData::flower2_png, BinaryData::flower2_pngSize));
        m_image1.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image2.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image3.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image1.setOverlayColour(PatchEditor::getColorTxt());
        m_image2.setOverlayColour(PatchEditor::getColorTxt().interpolatedWith(PatchEditor::getColorBg(), 0.5f));
        addAndMakeVisible(m_image1, 0);
        addChildComponent(m_image2, 0);
        addAndMakeVisible(m_image3, -1);
        m_image3.setAlwaysOnTop(true);
        setBounds(3, 3, 15, 15);
    }
    
    void paintButton(Graphics& g, bool over, bool down) override {}
    
    void buttonStateChanged() override
    {
        if(isDown() || isOver())
        {
            m_image1.setVisible(false);
            m_image2.setVisible(true);
        }
        else
        {
            m_image1.setVisible(true);
            m_image2.setVisible(false);
        }
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImgButton)
};

// ==================================================================================== //
//                                          ABOUT                                       //
// ==================================================================================== //

class PatchEditor::About : public Component
{
private:
    DrawableImage    m_image1;
    juce::TextEditor m_text;
public:
    About()
    {
        m_image1.setImage(ImageCache::getFromMemory(BinaryData::flowerG_png, BinaryData::flowerG_pngSize));
        m_image1.setBoundingBox(RelativeParallelogram(Rectangle<float>(300.f - 128.f, 370.f - 128.f, 128.f, 128.f)));
        m_image1.setOpacity(0.5f);
        addAndMakeVisible(&m_image1);
        
        m_text.setMultiLine(true);
        m_text.setReadOnly(true);
        m_text.setScrollbarsShown(false);
        m_text.setCaretVisible(false);
        m_text.setPopupMenuEnabled(true);
        m_text.setColour(juce::TextEditor::backgroundColourId, PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::outlineColourId, PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::shadowColourId,PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::textColourId, PatchEditor::getColorTxt());
        m_text.setFont(PatchEditor::getFont());
        m_text.setText("Camomile is a dynamic plugin made with Juce that allows to load and control Pure Data patches inside a digital audio workstation.\n\n"
                       "Author :\n"+ String(JucePlugin_Manufacturer) + "\n\n"
                       "Organizations :\nCICM | Université Paris 8 | Labex Arts H2H\n\n"
                       "Contacts :\n"+String(JucePlugin_ManufacturerEmail)+"\n"+ String(JucePlugin_ManufacturerWebsite)+"\n\n"
                       "Credits :\nPure Data by Miller Puckette\nJuce by ROLI Ltd.");
        m_text.setBounds(0, 0, 300, 370);
        addAndMakeVisible(&m_text, -1);
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(About)
};

// ==================================================================================== //
//                                          CONSOLE                                     //
// ==================================================================================== //

class PatchEditor::Console : public Component, public juce::Timer, public juce::TextEditor::Listener
{
private:
    juce::TextEditor m_text;
public:
    Console()
    {
        m_text.setMultiLine(true);
        m_text.setReadOnly(false);
        m_text.setScrollbarsShown(true);
        m_text.setCaretVisible(false);
        m_text.setPopupMenuEnabled (true);
        m_text.setColour(juce::TextEditor::backgroundColourId, PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::outlineColourId, PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::shadowColourId,PatchEditor::getColorInv());
        m_text.setColour(juce::TextEditor::textColourId, PatchEditor::getColorTxt());
        m_text.setFont(PatchEditor::getFont());
        m_text.setText(pd::Pd::getConsole());
        m_text.setBounds(0, 0, 300, 370);
        addAndMakeVisible(&m_text, 1);
        startTimer(20);
    }
    
    void textEditorTextChanged(juce::TextEditor& ed) override
    {
        pd::Pd::setConsole(ed.getText().toStdString());
    }
    
    void timerCallback() override
    {
        m_text.setText(pd::Pd::getConsole());
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Console)
};

// ==================================================================================== //
//                                          WINDOW                                      //
// ==================================================================================== //

class PatchEditor::PatchWin : public DocumentWindow
{
public:
    PatchWin(String const& name) :
    DocumentWindow(name, Colours::lightgrey, DocumentWindow::closeButton, false)
    {
        setUsingNativeTitleBar(true);
        setBounds(50, 50, 300, 370);
        setResizable(false, false);
        setAlwaysOnTop(true);
        setDropShadowEnabled(true);
        setVisible(true);
        setBackgroundColour(Colours::lightgrey);
    }

    void closeButtonPressed() override
    {
        removeFromDesktop();
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchWin)
};

// ==================================================================================== //
//                                  PATCHER EDITOR                                  //
// ==================================================================================== //

PatchEditor::PatchEditor(InstanceProcessor& p) :
AudioProcessorEditor(&p), m_processor(p),
m_dropping(false), m_window(nullptr)
{
    Component::setWantsKeyboardFocus(true);
    m_processor.addListener(this);
    m_button = new ImgButton(this, "CamomileButton", 1);
    m_window = new PatchWin("");
    m_patcher= new GuiPatcher(p);
    addAndMakeVisible(m_patcher);
    addAndMakeVisible(m_button);
    setSize(600, 420);
    patchChanged();
}

PatchEditor::~PatchEditor()
{
    m_processor.removeListener(this);
}

void PatchEditor::paint(Graphics& g)
{
    String text;
    g.fillAll(PatchEditor::getColorBg());
    g.setColour(PatchEditor::getColorBd());
    g.drawRect(getBounds().withZeroOrigin(), PatchEditor::getBordersize());
    g.drawLine(0.f, 20.f, getWidth(), 20.f, PatchEditor::getBordersize());
    g.setFont(PatchEditor::getFont());
    g.setColour(PatchEditor::getColorTxt());
    const pd::Patch patch = m_processor.getPatch();
    if(patch.isValid())
    {
        g.drawText(String(patch.getName()).trimCharactersAtEnd(".pd"), 0, 0, getWidth(), 20, juce::Justification::centred);
    }
    else
    {
        g.drawText(String("No Patch"), 0, 0, getWidth(), 20, juce::Justification::centred);
    }
    if(m_dropping)
    {
        g.fillAll(Colours::white.withAlpha(0.2f));
    }
}

void PatchEditor::patchChanged()
{
    const pd::Patch patch = m_processor.getPatch();
    if(patch.isValid())
    {
        m_patcher->setPatch(patch);
        setSize(m_patcher->getWidth(), m_patcher->getHeight() + 20);
    }
}

void PatchEditor::buttonClicked(Button* button)
{
    if(button->getRadioGroupId() == 1)
    {
        juce::PopupMenu m;
        m.addItem(1, "About");
        m.addItem(2, "Open");
        m.addItem(3, "Close");
        m.addItem(4, "Reload");
        m.addItem(5, "Console");
        m.addItem(6, "Help");
        const int result = m.showAt(button->getScreenBounds().translated(-3, 1));
        if(result == 1)
        {
            m_window->setContentOwned(new About(), false);
            m_window->setName("About Camomile " + String(JucePlugin_VersionString));
            m_window->addToDesktop();
        }
        else if(result == 2)
        {
            if(m_window)
            {
                m_window = nullptr;
            }
            const pd::Patch patch = m_processor.getPatch();
            if(patch.isValid())
            {
                FileChooser fc("Open a patch...", File(patch.getPath()), "*.pd", true);
                if(fc.browseForFileToOpen())
                {
                    juce::File file(fc.getResult());
                    if(file.getFileExtension() == juce::String(".pd"))
                    {
                        m_processor.loadPatch(file);
                    }
                }
            }
            else
            {
                FileChooser fc("Open a patch...", File::getSpecialLocation(File::userDocumentsDirectory), "*.pd", true);
                if(fc.browseForFileToOpen())
                {
                    juce::File file(fc.getResult());
                    if(file.getFileExtension() == juce::String(".pd"))
                    {
                        m_processor.loadPatch(file);
                    }
                }
            }
            
        }
        else if(result == 3)
        {
            if(m_window)
            {
                m_window = nullptr;
            }
            m_processor.loadPatch(juce::File());
        }
        else if(result == 4)
        {
            const pd::Patch patch = m_processor.getPatch();
            if(patch.isValid())
            {
                File file(patch.getPath() + File::separatorString.toStdString() + patch.getName());
                if(file.exists())
                {
                    m_processor.loadPatch(file);
                }
            }
        }
        else if(result == 5)
        {
            m_window->setContentOwned(new Console(), false);
            m_window->setName("Camomile Console");
            m_window->addToDesktop();
        }
        else if(result == 6)
        {
            if(m_window)
            {
                m_window = nullptr;
            }
            juce::URL url("https://github.com/pierreguillot/Camomile");
            if(url.isWellFormed())
            {
                url.launchInDefaultBrowser();
            }
        }
    }
}

